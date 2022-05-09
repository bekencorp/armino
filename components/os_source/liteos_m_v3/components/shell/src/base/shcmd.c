/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "shcmd.h"
#include "show.h"
#include "shmsg.h"
#include "stdlib.h"
#include "unistd.h"
#include "dirent.h"
#include "securec.h"
#include "los_mux.h"
#include "los_memory.h"

#define SHELL_INIT_MAGIC_FLAG 0xABABABAB

STATIC CmdModInfo cmdInfo;
ShellCB *g_shellCB = NULL;
CmdItem g_shellcmdAll[] = {
    {CMD_TYPE_STD, "date", XARGS, (CmdCallBackFunc)OsShellCmdDate},
    {CMD_TYPE_EX, "task", 1, (CmdCallBackFunc)OsShellCmdDumpTask},
    {CMD_TYPE_EX, "free", XARGS, (CmdCallBackFunc)OsShellCmdFree},
#ifdef LWIP_SHELLCMD_ENABLE
    {CMD_TYPE_EX, "ifconfig", XARGS, (CmdCallBackFunc)lwip_ifconfig},
    {CMD_TYPE_EX, "ping", XARGS, (CmdCallBackFunc)OsShellPing},
#endif
    {CMD_TYPE_EX, "touch", XARGS, (CmdCallBackFunc)OsShellCmdTouch},
    {CMD_TYPE_EX, "ls", XARGS, (CmdCallBackFunc)OsShellCmdLs},
    {CMD_TYPE_EX, "pwd", XARGS, (CmdCallBackFunc)OsShellCmdPwd},
    {CMD_TYPE_EX, "cd", XARGS, (CmdCallBackFunc)OsShellCmdCd},
    {CMD_TYPE_EX, "cat", XARGS, (CmdCallBackFunc)OsShellCmdCat},
    {CMD_TYPE_EX, "rm", XARGS, (CmdCallBackFunc)OsShellCmdRm},
    {CMD_TYPE_EX, "rmdir", XARGS, (CmdCallBackFunc)OsShellCmdRmdir},
    {CMD_TYPE_EX, "mkdir", XARGS, (CmdCallBackFunc)OsShellCmdMkdir},
    {CMD_TYPE_EX, "cp", XARGS, (CmdCallBackFunc)OsShellCmdCp},
    {CMD_TYPE_EX, "help", 0, (CmdCallBackFunc)OsShellCmdHelp},
};

CmdModInfo *OsCmdInfoGet(VOID)
{
    return &cmdInfo;
}

/*
 * Description: Pass in the string and clear useless space ,which inlcude:
 *                1) The overmatch space which is not be marked by Quote's area
 *                   Squeeze the overmatch space into one space
 *                2) Clear all space before first vaild charatctor
 * Input:       cmdKey : Pass in the buff string, which is ready to be operated
 *              cmdOut : Pass out the buffer string ,which has already been operated
 *              size : cmdKey length
 */
LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdKeyShift(const CHAR *cmdKey, CHAR *cmdOut, UINT32 size)
{
    CHAR *output = NULL;
    CHAR *outputBak = NULL;
    UINT32 len;
    INT32 ret;
    BOOL quotes = FALSE;

    if ((cmdKey == NULL) || (cmdOut == NULL)) {
        return (UINT32)OS_ERROR;
    }

    len = strlen(cmdKey);
    if (len >= size) {
        return (UINT32)OS_ERROR;
    }
    output = (CHAR*)LOS_MemAlloc(m_aucSysMem0, len + 1);
    if (output == NULL) {
        PRINTK("malloc failure in %s[%d]", __FUNCTION__, __LINE__);
        return (UINT32)OS_ERROR;
    }
    /* Backup the 'output' start address */
    outputBak = output;
    /* Scan each charactor in 'cmdKey',and squeeze the overmuch space and ignore invaild charactor */
    for (; *cmdKey != '\0'; cmdKey++) {
        /* Detected a Double Quotes, switch the matching status */
        if (*(cmdKey) == '\"') {
            SWITCH_QUOTES_STATUS(quotes);
        }
        /* Ignore the current charactor in following situation */
        /* 1) Quotes matching status is FALSE (which said that the space is not been marked by double quotes) */
        /* 2) Current charactor is a space */
        /* 3) Next charactor is a space too, or the string is been seeked to the end already(\0) */
        /* 4) Invaild charactor, such as single quotes */
        if ((*cmdKey == ' ') && ((*(cmdKey + 1) == ' ') || (*(cmdKey + 1) == '\0')) && QUOTES_STATUS_CLOSE(quotes)) {
            continue;
        }
        if (*cmdKey == '\'') {
            continue;
        }
        *output = *cmdKey;
        output++;
    }
    *output = '\0';
    /* Restore the 'output' start address */
    output = outputBak;
    len = strlen(output);
    /* Clear the space which is located at the first charactor in buffer */
    if (*outputBak == ' ') {
        output++;
        len--;
    }
    /* Copy out the buffer which is been operated already */
    ret = strncpy_s(cmdOut, size, output, len);
    if (ret != EOK) {
        PRINT_ERR("%s,%d strncpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
        (VOID)LOS_MemFree(m_aucSysMem0, output);
        return OS_ERROR;
    }
    cmdOut[len] = '\0';

    (VOID)LOS_MemFree(m_aucSysMem0, output);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR BOOL OsCmdKeyCheck(const CHAR *cmdKey)
{
    const CHAR *temp = cmdKey;
    enum Stat {
        STAT_NONE,
        STAT_DIGIT,
        STAT_OTHER
    } state = STAT_NONE;

    if (strlen(cmdKey) >= CMD_KEY_LEN) {
        return FALSE;
    }

    while (*temp != '\0') {
        if (!((*temp <= '9') && (*temp >= '0')) &&
            !((*temp <= 'z') && (*temp >= 'a')) &&
            !((*temp <= 'Z') && (*temp >= 'A')) &&
            (*temp != '_') && (*temp != '-')) {
            return FALSE;
        }

        if ((*temp >= '0') && (*temp <= '9')) {
            if (state == STAT_NONE) {
                state = STAT_DIGIT;
            }
        } else {
            state = STAT_OTHER;
        }

        temp++;
    }

    if (state == STAT_DIGIT) {
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR VOID OsCmdAscendingInsert(CmdItemNode *cmd)
{
    CmdItemNode *cmdItem = NULL;
    CmdItemNode *cmdNext = NULL;

    if (cmd == NULL) {
        return;
    }

    for (cmdItem = LOS_DL_LIST_ENTRY((&cmdInfo.cmdList.list)->pstPrev, CmdItemNode, list);
        &cmdItem->list != &(cmdInfo.cmdList.list); ) {
        cmdNext = LOS_DL_LIST_ENTRY(cmdItem->list.pstPrev, CmdItemNode, list);
        if (&cmdNext->list != &(cmdInfo.cmdList.list)) {
            if ((strncmp(cmdItem->cmd->cmdKey, cmd->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) >= 0) &&
                (strncmp(cmdNext->cmd->cmdKey, cmd->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) < 0)) {
                LOS_ListTailInsert(&(cmdItem->list), &(cmd->list));
                return;
            }
            cmdItem = cmdNext;
        } else {
            if (strncmp(cmd->cmd->cmdKey, cmdItem->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) > 0) {
                cmdItem = cmdNext;
            }
            break;
        }
    }

    LOS_ListTailInsert(&(cmdItem->list), &(cmd->list));
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellKeyInit(ShellCB *shellCB)
{
    CmdKeyLink *cmdKeyLink = NULL;
    CmdKeyLink *cmdHistoryLink = NULL;

    if (shellCB == NULL) {
        return OS_ERROR;
    }
    cmdKeyLink = (CmdKeyLink *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdKeyLink));
    if (cmdKeyLink == NULL) {
        PRINT_ERR("Shell CmdKeyLink memory alloc error!\n");
        return OS_ERROR;
    }
    cmdHistoryLink = (CmdKeyLink *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdKeyLink));
    if (cmdHistoryLink == NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, cmdKeyLink);
        PRINT_ERR("Shell CmdHistoryLink memory alloc error!\n");
        return OS_ERROR;
    }

    cmdKeyLink->count = 0;
    LOS_ListInit(&(cmdKeyLink->list));
    shellCB->cmdKeyLink = (VOID *)cmdKeyLink;

    cmdHistoryLink->count = 0;
    LOS_ListInit(&(cmdHistoryLink->list));
    shellCB->cmdHistoryKeyLink = (VOID *)cmdHistoryLink;
    shellCB->cmdMaskKeyLink = (VOID *)cmdHistoryLink;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellKeyDeInit(CmdKeyLink *cmdKeyLink)
{
    CmdKeyLink *cmdtmp = NULL;
    if (cmdKeyLink == NULL) {
        return;
    }

    while (!LOS_ListEmpty(&(cmdKeyLink->list))) {
        cmdtmp = LOS_DL_LIST_ENTRY(cmdKeyLink->list.pstNext, CmdKeyLink, list);
        LOS_ListDelete(&cmdtmp->list);
        (VOID)LOS_MemFree(m_aucSysMem0, cmdtmp);
    }

    cmdKeyLink->count = 0;
    (VOID)LOS_MemFree(m_aucSysMem0, cmdKeyLink);
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellSysCmdRegister(VOID)
{
    UINT32 i;
    UINT8 *cmdItemGroup = NULL;
    UINT32 index = sizeof(g_shellcmdAll) / sizeof(CmdItem);
    CmdItemNode *cmdItem = NULL;

    cmdItemGroup = (UINT8 *)LOS_MemAlloc(m_aucSysMem0, index * sizeof(CmdItemNode));
    if (cmdItemGroup == NULL) {
        PRINT_ERR("[%s]System memory allocation failure!\n", __FUNCTION__);
        return (UINT32)OS_ERROR;
    }

    for (i = 0; i < index; ++i) {
        cmdItem = (CmdItemNode *)(cmdItemGroup + i * sizeof(CmdItemNode));
        cmdItem->cmd = &g_shellcmdAll[i];
        OsCmdAscendingInsert(cmdItem);
    }
    cmdInfo.listNum += index;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdExec(CmdParsed *cmdParsed, CHAR *cmdStr)
{
    UINT32 ret;
    CmdCallBackFunc cmdHook = NULL;
    CmdItemNode *curCmdItem = NULL;
    UINT32 i;
    const CHAR *cmdKey = NULL;

    if ((cmdParsed == NULL) || (cmdStr == NULL) || (strlen(cmdStr) == 0)) {
        return (UINT32)OS_ERROR;
    }

    ret = OsCmdParse(cmdStr, cmdParsed);
    if (ret != LOS_OK) {
        goto OUT;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &(cmdInfo.cmdList.list), CmdItemNode, list) {
        cmdKey = curCmdItem->cmd->cmdKey;
        if ((cmdParsed->cmdType == curCmdItem->cmd->cmdType) &&
            (strlen(cmdKey) == strlen(cmdParsed->cmdKeyword)) &&
            (strncmp(cmdKey, (CHAR *)(cmdParsed->cmdKeyword), strlen(cmdKey)) == 0)) {
            cmdHook = curCmdItem->cmd->cmdHook;
            break;
        }
    }

    ret = OS_ERROR;
    if (cmdHook != NULL) {
        ret = (cmdHook)(cmdParsed->paramCnt, (const CHAR **)cmdParsed->paramArray);
    }

OUT:
    for (i = 0; i < cmdParsed->paramCnt; i++) {
        if (cmdParsed->paramArray[i] != NULL) {
            (VOID)LOS_MemFree(m_aucSysMem0, cmdParsed->paramArray[i]);
            cmdParsed->paramArray[i] = NULL;
        }
    }

    return (UINT32)ret;
}

ShellCB *OsGetShellCb(VOID)
{
    return g_shellCB;
}

char *OsShellGetWorkingDirtectory(VOID)
{
    return OsGetShellCb()->shellWorkingDirectory;
}

VOID OsShellCBInit(VOID)
{
    int ret;
    ShellCB *shellCB = NULL;

    shellCB = (ShellCB *)malloc(sizeof(ShellCB));
    if (shellCB == NULL) {
        goto ERR_OUT1;
    }
    ret = memset_s(shellCB, sizeof(ShellCB), 0, sizeof(ShellCB));
    if (ret != SH_OK) {
        goto ERR_OUT1;
    }

    ret = (int)OsShellKeyInit(shellCB);
    if (ret != SH_OK) {
        goto ERR_OUT1;
    }
    (void)strncpy_s(shellCB->shellWorkingDirectory, PATH_MAX, "/", 2); /* 2:space for "/" */

    g_shellCB = shellCB;
    return;

ERR_OUT1:
    (void)free(shellCB);
    return;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdInit(VOID)
{
    UINT32 ret;
    LOS_ListInit(&(cmdInfo.cmdList.list));
    cmdInfo.listNum = 0;
    cmdInfo.initMagicFlag = SHELL_INIT_MAGIC_FLAG;
    ret = LOS_MuxCreate(&cmdInfo.muxLock);
    if (ret != LOS_OK) {
        PRINT_ERR("Create mutex for shell cmd info failed\n");
        return OS_ERROR;
    }
    OsShellCBInit();

    return LOS_OK;
}

STATIC UINT32 OsCmdItemCreate(CmdType cmdType, const CHAR *cmdKey, UINT32 paraNum, CmdCallBackFunc cmdProc)
{
    CmdItem *cmdItem = NULL;
    CmdItemNode *cmdItemNode = NULL;

    cmdItem = (CmdItem *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdItem));
    if (cmdItem == NULL) {
        return OS_ERRNO_SHELL_CMDREG_MEMALLOC_ERROR;
    }
    (VOID)memset_s(cmdItem, sizeof(CmdItem), '\0', sizeof(CmdItem));

    cmdItemNode = (CmdItemNode *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdItemNode));
    if (cmdItemNode == NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, cmdItem);
        return OS_ERRNO_SHELL_CMDREG_MEMALLOC_ERROR;
    }
    (VOID)memset_s(cmdItemNode, sizeof(CmdItemNode), '\0', sizeof(CmdItemNode));
    cmdItemNode->cmd = cmdItem;
    cmdItemNode->cmd->cmdHook = cmdProc;
    cmdItemNode->cmd->paraNum = paraNum;
    cmdItemNode->cmd->cmdType = cmdType;
    cmdItemNode->cmd->cmdKey = cmdKey;

    (VOID)LOS_MuxPend(cmdInfo.muxLock, LOS_WAIT_FOREVER);
    OsCmdAscendingInsert(cmdItemNode);
    cmdInfo.listNum++;
    (VOID)LOS_MuxPost(cmdInfo.muxLock);

    return LOS_OK;
}

/* open API */
LITE_OS_SEC_TEXT_MINOR UINT32 osCmdReg(CmdType cmdType, const CHAR *cmdKey, UINT32 paraNum, CmdCallBackFunc cmdProc)
{
    CmdItemNode *cmdItemNode = NULL;

    (VOID)LOS_MuxPend(cmdInfo.muxLock, LOS_WAIT_FOREVER);
    if (cmdInfo.initMagicFlag != SHELL_INIT_MAGIC_FLAG) {
        (VOID)LOS_MuxPost(cmdInfo.muxLock);
        PRINT_ERR("[%s] shell is not yet initialized!\n", __FUNCTION__);
        return OS_ERRNO_SHELL_NOT_INIT;
    }
    (VOID)LOS_MuxPost(cmdInfo.muxLock);

    if ((cmdProc == NULL) || (cmdKey == NULL) ||
        (cmdType >= CMD_TYPE_BUTT) || (strlen(cmdKey) >= CMD_KEY_LEN) || !strlen(cmdKey)) {
        return OS_ERRNO_SHELL_CMDREG_PARA_ERROR;
    }

    if (paraNum > CMD_MAX_PARAS) {
        if (paraNum != XARGS) {
            return OS_ERRNO_SHELL_CMDREG_PARA_ERROR;
        }
    }

    if (OsCmdKeyCheck(cmdKey) != TRUE) {
        return OS_ERRNO_SHELL_CMDREG_CMD_ERROR;
    }

    (VOID)LOS_MuxPend(cmdInfo.muxLock, LOS_WAIT_FOREVER);
    LOS_DL_LIST_FOR_EACH_ENTRY(cmdItemNode, &(cmdInfo.cmdList.list), CmdItemNode, list) {
        if ((cmdType == cmdItemNode->cmd->cmdType) &&
            ((strlen(cmdKey) == strlen(cmdItemNode->cmd->cmdKey)) &&
            (strncmp((CHAR *)(cmdItemNode->cmd->cmdKey), cmdKey, strlen(cmdKey)) == 0))) {
            (VOID)LOS_MuxPost(cmdInfo.muxLock);
            return OS_ERRNO_SHELL_CMDREG_CMD_EXIST;
        }
    }
    (VOID)LOS_MuxPost(cmdInfo.muxLock);

    return OsCmdItemCreate(cmdType, cmdKey, paraNum, cmdProc);
}

