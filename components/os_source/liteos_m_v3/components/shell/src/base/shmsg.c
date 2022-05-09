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

#include "shmsg.h"
#include "securec.h"
#include "shcmd.h"
#include "show.h"
#if (LOSCFG_USE_SHELL == 1)
#endif
#include "los_event.h"
#include "los_task.h"

EVENT_CB_S g_shellInputEvent;
#define SHELL_CMD_MAX_SIZE 64

UINT32 ShellMsgTypeGet(CmdParsed *cmdParsed, const CHAR *cmdType)
{
    CmdItemNode *curCmdItem = (CmdItemNode *)NULL;
    UINT32 len;
    UINT32 minLen;
    CmdModInfo *cmdInfo = OsCmdInfoGet();

    if ((cmdParsed == NULL) || (cmdType == NULL)) {
        return OS_INVALID;
    }

    len = strlen(cmdType);
    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &(cmdInfo->cmdList.list), CmdItemNode, list) {
        if ((len == strlen(curCmdItem->cmd->cmdKey)) &&
            (strncmp((CHAR *)(curCmdItem->cmd->cmdKey), cmdType, len) == 0)) {
            minLen = (len < CMD_KEY_LEN) ? len : CMD_KEY_LEN;
            (VOID)memcpy_s((CHAR *)(cmdParsed->cmdKeyword), CMD_KEY_LEN, cmdType, minLen);
            cmdParsed->cmdType = curCmdItem->cmd->cmdType;
            return LOS_OK;
        }
    }

    return OS_INVALID;
}

CHAR *GetCmdName(const CHAR *cmdline, UINT32 len)
{
    UINT32 loop;
    const CHAR *tmpStr = NULL;
    BOOL quotes = FALSE;
    CHAR *cmdName = NULL;
    if (cmdline == NULL) {
        return NULL;
    }

    cmdName = (CHAR *)malloc(len + 1);
    if (cmdName == NULL) {
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    /* Scan the 'cmdline' string for command */
    /* Notice: Command string must not have any special name */
    for (tmpStr = cmdline, loop = 0; (*tmpStr != '\0') && (loop < len); ) {
        /* If reach a double quotes, switch the quotes matching status */
        if (*tmpStr == '\"') {
            SWITCH_QUOTES_STATUS(quotes);
            /* Ignore the double quote charactor itself */
            tmpStr++;
            continue;
        }
        /* If detected a space which the quotes matching status is false */
        /* which said has detected the first space for seperator, finish this scan operation */
        if ((*tmpStr == ' ') && (QUOTES_STATUS_CLOSE(quotes))) {
            break;
        }
        cmdName[loop] = *tmpStr++;
        loop++;
    }
    cmdName[loop] = '\0';

    return cmdName;
}

INT32 ShellCmdExec(const CHAR *msgName, const CHAR *cmdString)
{
    UINT32 uintRet;
    errno_t err;
    CmdParsed cmdParsed;

    if (msgName == NULL || cmdString == NULL) {
        return -EFAULT;
    }
    err = memset_s(&cmdParsed, sizeof(CmdParsed), 0, sizeof(CmdParsed));
    if (err != EOK) {
        return -EFAULT;
    }

    uintRet = ShellMsgTypeGet(&cmdParsed, msgName);
    if (uintRet != LOS_OK) {
        PRINTK("%s:command not found\n", msgName);
        return -EFAULT;
    } else {
        (VOID)OsCmdExec(&cmdParsed, (CHAR *)cmdString);
    }
    return 0;
}

UINT32 PreHandleCmdline(const CHAR *input, CHAR **output, UINT32 *outputlen)
{
    UINT32 shiftLen;
    UINT32 ret;
    const CHAR *cmdBuf = input;
    UINT32 cmdBufLen = strlen(cmdBuf);
    CHAR *shiftStr = (CHAR *)malloc(cmdBufLen + 1);

    if (shiftStr == NULL) {
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        return SH_NOK;
    }
    (VOID)memset_s(shiftStr, cmdBufLen + 1, 0, cmdBufLen + 1);

    /* Call function 'OsCmdKeyShift' to squeeze and clear useless or overmuch space if string buffer */
    ret = OsCmdKeyShift(cmdBuf, shiftStr, cmdBufLen + 1);
    shiftLen = strlen(shiftStr);
    if ((ret != SH_OK) || (shiftLen == 0)) {
        ret = SH_NOK;
        goto END_FREE_SHIFTSTR;
    }
    *output = shiftStr;
    *outputlen = shiftLen;

    ret = SH_OK;
    goto END;

END_FREE_SHIFTSTR:
    free(shiftStr);
END:
    return ret;
}

static VOID ParseAndExecCmdline(CmdParsed *cmdParsed, const CHAR *cmdline, UINT32 len)
{
    INT32 i;
    UINT32 ret;
    CHAR *cmdlineOrigin = NULL;
    CHAR *cmdName = NULL;

    cmdlineOrigin = strdup(cmdline);
    if (cmdlineOrigin == NULL) {
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        return;
    }

    cmdName = GetCmdName(cmdline, len);
    if (cmdName == NULL) {
        free(cmdlineOrigin);
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        return;
    }

    ret = OsCmdParse((CHAR *)cmdline, cmdParsed);
    if (ret != SH_OK) {
        PRINTK("cmd parse failure in %s[%d]\n", __FUNCTION__, __LINE__);
        goto OUT;
    }

    (VOID)ShellCmdExec(cmdName, cmdlineOrigin);

OUT:
    for (i = 0; i < cmdParsed->paramCnt; i++) {
        if (cmdParsed->paramArray[i] != NULL) {
            free(cmdParsed->paramArray[i]);
            cmdParsed->paramArray[i] = NULL;
        }
    }
    free(cmdName);
    free(cmdlineOrigin);
}

LITE_OS_SEC_TEXT_MINOR VOID ExecCmdline(const CHAR *cmdline)
{
    UINT32 ret;
    CHAR *output = NULL;
    UINT32 outputlen;
    CmdParsed cmdParsed;

    if (cmdline == NULL) {
        return;
    }

    OsShellInit(0);

    /* strip out unnecessary characters */
    ret = PreHandleCmdline(cmdline, &output, &outputlen);
    if (ret == SH_NOK) {
        return;
    }

    (VOID)memset_s(&cmdParsed, sizeof(CmdParsed), 0, sizeof(CmdParsed));
    ParseAndExecCmdline(&cmdParsed, output, outputlen);
    free(output);
}

#if (LOSCFG_USE_SHELL == 1)
VOID ShellTaskEntry(VOID)
{
    CHAR buf[SHELL_CMD_MAX_SIZE] = {0};
    CHAR *ptr = buf;
    PRINTK("OHOS # ");
    while(1) {
        (VOID)LOS_EventRead(&g_shellInputEvent, 0x1, LOS_WAITMODE_AND | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        while((*ptr = (UINT8)UartGetc()) != 0 && *ptr != 13) {
            PRINTK("%c", *ptr);
            if ((ptr - buf) == (sizeof(buf) - 1)) {
                break;
            }
            ptr++;
        }
        if (ptr != buf) {
            if (*ptr == 13 || ((ptr - buf) == (sizeof(buf) - 1))) {
                PRINTK("%c", *ptr);
                *ptr = '\0';
                ptr = buf;
                PRINTK("\n\r", buf);
                ExecCmdline(buf);
                PRINTK("OHOS # ");
            }
        } else {
            PRINTK("\n\rOHOS # ");
        }
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 LosShellInit(VOID)
{
    UINT32 ret;
    UINT32 taskID1, taskID2;
    TSK_INIT_PARAM_S task1 = { 0 };

    ret = LOS_EventInit(&g_shellInputEvent);
    if (ret != LOS_OK) {
        PRINTK("Init shellInputEvent failed! ERROR: 0x%x\n", ret);
        return ret;
    }

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)ShellTaskEntry;
    task1.uwStackSize  = 0x1000;
    task1.pcName       = "ShellTaskEntry";
    task1.usTaskPrio   = LOSCFG_SHELL_PRIO;
    ret = LOS_TaskCreate(&taskID1, &task1);
    if (ret != LOS_OK) {
        PRINTK("Create Shell Task failed! ERROR: 0x%x\n", ret);
        return ret;
    }

    return ret;
}
#endif
