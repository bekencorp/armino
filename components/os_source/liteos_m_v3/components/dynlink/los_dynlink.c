/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
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
#include "los_dynlink.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "limits.h"
#include "sys/stat.h"
#include "securec.h"
#include "arch_elf.h"
#include "los_task.h"
#include "los_debug.h"
#include "los_mux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_DYNLINK == 1)

#if defined(__ICCARM__) || defined(__CC_ARM)
/**
 * Place instructions below in .icf linker file:
 *
 * keep {section .TABLE.START};
 * keep {section .sym.*};
 * keep {section .table.end};
 * define block SYMBOL_TABLE with fixed order
 * {
 *     section .TABLE.START,
 *     section .sym.*,
 *     section .table.end
 * };
 * place in ROM_region {readonly, block SYMBOL_TABLE};
 */
const SymInfo symTableStart __attribute__((section(".TABLE.START"))) = {
    .name = "start",
    .addr = 0
};

const SymInfo symTableEnd __attribute__((section(".table.end"))) = {
    .name = "end",
    .addr = 0
};
#pragma section=".TABLE.START"
#pragma section=".table.end"
#elif defined(__CLANG_ARM) || defined(__GNUC__)
/**
 * Place instructions below in rodata segment of .ld linker file:
 *
 *  __sym_table_start = .;
 *  KEEP(*( SORT (.sym.*)));
 *  __sym_table_end = .;
 */
extern char __sym_table_start[];
extern char __sym_table_end[];
#else
#error Unknown compiler.
#endif

STATIC LOS_DL_LIST g_dynSharedObjLink;
STATIC UINT32 g_dynlinkMux;

STATIC DynSharedObj *OsIsPreLoaded(const CHAR *fileName)
{
    DynSharedObj *dso = NULL;

    LOS_DL_LIST_FOR_EACH_ENTRY(dso, &g_dynSharedObjLink, DynSharedObj, dsoNode) {
        if (!strcmp(fileName, dso->fileName)) {
            ++dso->ref;
            return dso;
        }
    }

    return NULL;
}

STATIC INT32 OsVerifyEhdr(const LD_ELF_EHDR *ehdr, UINT32 fileLen)
{
    if (memcmp(ehdr->e_ident, LD_ELFMAG, LD_SELFMAG) != LOS_OK) {
        PRINT_ERR("The file is not elf format\n");
        return LOS_NOK;
    }
    if (ehdr->e_type != ET_DYN) {
        PRINT_ERR("The file is not shared library\n");
        return LOS_NOK;
    }
    if (!check_arch(ehdr)) {
        PRINT_ERR("The file can not load in current platform\n");
        return LOS_NOK;
    }
    if (ehdr->e_phnum > PHDR_NUM_MAX) {
        PRINT_ERR("The num of program header is out of limit\n");
        return LOS_NOK;
    }
    if (ehdr->e_phoff > fileLen) {
        PRINT_ERR("The offset of program header is invalid, elf file is bad\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

STATIC INT32 OsReadELFInfo(INT32 fd, UINT8 *buffer, size_t readSize, off_t offset)
{
    ssize_t byteNum;
    off_t returnPos;

    if (readSize > 0) {
        returnPos = lseek(fd, offset, SEEK_SET);
        if (returnPos != offset) {
            PRINT_ERR("Failed to seek the position!, offset: %#x\n", offset);
            return LOS_NOK;
        }

        byteNum = read(fd, buffer, readSize);
        if (byteNum <= 0) {
            PRINT_ERR("Failed to read from offset: %#x!\n", offset);
            return LOS_NOK;
        }
    }
    return LOS_OK;
}

STATIC INT32 OsGetFileLength(UINT32 *fileLen, const CHAR *fileName)
{
    struct stat buf;
    INT32 ret;

    ret = stat(fileName, &buf);
    if (ret < 0) {
        PRINT_ERR("Failed to stat file: %s, errno: %d\n", fileName, errno);
        return LOS_NOK;
    }

    if (buf.st_size > FILE_LENGTH_MAX) {
        PRINT_ERR("The file: %s length is out of limit!\n", fileName);
        return LOS_NOK;
    }

    *fileLen = (UINT32)buf.st_size;
    return LOS_OK;
}

STATIC INT32 OsReadEhdr(INT32 fd, UINT32 fileLen, DynLinkInfo *dlInfo)
{
    INT32 ret;

    ret = OsReadELFInfo(fd, (UINT8 *)&dlInfo->elfEhdr, sizeof(LD_ELF_EHDR), 0);
    if (ret != LOS_OK) {
        return -EIO;
    }

    ret = OsVerifyEhdr(&dlInfo->elfEhdr, fileLen);
    if (ret != LOS_OK) {
        return -ELIBBAD;
    }

    return LOS_OK;
}

STATIC INT32 OsReadPhdrs(INT32 fd, UINT32 fileLen, DynLinkInfo *dlInfo)
{
    INT32 ret;
    UINT32 size;
    LD_ELF_EHDR *ehdr = &dlInfo->elfEhdr;

    if ((ehdr->e_phnum == 0) || (ehdr->e_phentsize != sizeof(LD_ELF_PHDR))) {
        goto ERR;
    }

    size = sizeof(LD_ELF_PHDR) * ehdr->e_phnum;
    if ((ehdr->e_phoff + size) > fileLen) {
        goto ERR;
    }

    dlInfo->elfPhdr = (LD_ELF_PHDR *)LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
    if (dlInfo->elfPhdr == NULL) {
        return -ENOMEM;
    }

    ret = OsReadELFInfo(fd, (UINT8 *)dlInfo->elfPhdr, size, ehdr->e_phoff);
    if (ret != LOS_OK) {
        LOS_MemFree(OS_SYS_MEM_ADDR, dlInfo->elfPhdr);
        return -EIO;
    }

    return LOS_OK;

ERR:
    PRINT_ERR("the file is bad\n");
    return -ELIBBAD;
}

STATIC DynSharedObj *OsLoadInit(const CHAR *fileName, VOID *pool)
{
    DynSharedObj *dso = NULL;
    UINT32 allocSize, nameLen;
    UINT32 fileLen = 0;
    INT32 ret;

    nameLen = strlen(fileName);
    if (nameLen > PATH_MAX) {
        PRINT_ERR("file name length is too long\n");
        errno = ENAMETOOLONG;
        return NULL;
    }
    allocSize = sizeof(DynSharedObj) + nameLen + 1;

    dso = (DynSharedObj *)LOS_MemAlloc(OS_SYS_MEM_ADDR, allocSize);
    if (dso == NULL) {
        PRINT_ERR("failed to alloc for dso\n");
        errno = ENOMEM;
        return NULL;
    }
    (VOID)memset_s(dso, allocSize, 0, allocSize);

    dso->dlInfo = (DynLinkInfo *)LOS_MemAlloc(OS_SYS_MEM_ADDR, sizeof(DynLinkInfo));
    if (dso->dlInfo == NULL) {
        LOS_MemFree(OS_SYS_MEM_ADDR, dso);
        PRINT_ERR("failed to alloc for loadInfo\n");
        errno = ENOMEM;
        return NULL;
    }
    (VOID)memset_s(dso->dlInfo, sizeof(DynLinkInfo), 0, sizeof(DynLinkInfo));

    ret = OsGetFileLength(&fileLen, fileName);
    if (ret != LOS_OK) {
        errno = ENOENT;
        goto ERR1;
    }

    dso->fd = open(fileName, O_RDONLY);
    if (dso->fd < 0) {
        PRINT_ERR("Failed to open ELF file: %s!\n", fileName);
        goto ERR1;
    }

    ret = OsReadEhdr(dso->fd, fileLen, dso->dlInfo);
    if (ret != LOS_OK) {
        errno = -ret;
        goto ERR2;
    }

    ret = OsReadPhdrs(dso->fd, fileLen, dso->dlInfo);
    if (ret != LOS_OK) {
        errno = -ret;
        goto ERR2;
    }

    strcpy(dso->buf, fileName);
    dso->fileName = dso->buf;
    dso->ref = 1;
    dso->pool = (pool ? pool : OS_SYS_MEM_ADDR);
    LOS_ListInit(&dso->dsoNode);

    return dso;

ERR2:
    close(dso->fd);
ERR1:
    LOS_MemFree(OS_SYS_MEM_ADDR, dso->dlInfo);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso);
    return NULL;
}

STATIC INT32 OsReserveSpace(const DynLinkInfo *dlInfo, UINT32 *boundary)
{
    const LD_ELF_PHDR *elfPhdrTemp = dlInfo->elfPhdr;
    INT32 phdrNum = dlInfo->elfEhdr.e_phnum;
    UINTPTR addrMin = SIZE_MAX;
    UINTPTR addrMax = 0;
    UINT32 offStart = 0;
    UINT64 size;
    INT32 i;

    for (i = 0; i < phdrNum; ++i, ++elfPhdrTemp) {
        if (elfPhdrTemp->p_type == PT_TLS) {
            PRINT_ERR("unsupport tls\n");
            return 0;
        }
        if (elfPhdrTemp->p_type != PT_LOAD) {
            continue;
        }

        if (*boundary == 0) {
            *boundary = elfPhdrTemp->p_align;
        }

        if (elfPhdrTemp->p_vaddr < addrMin) {
            addrMin = elfPhdrTemp->p_vaddr;
            offStart = elfPhdrTemp->p_offset;
        }
        if ((elfPhdrTemp->p_vaddr + elfPhdrTemp->p_memsz) > addrMax) {
            addrMax = elfPhdrTemp->p_vaddr + elfPhdrTemp->p_memsz;
        }
    }

    if ((addrMin == addrMax) || (addrMax < addrMin)) {
        return 0;
    }
    size = ELF_ALIGN_UP(addrMax, *boundary) - ELF_ALIGN_DOWN(addrMin, *boundary) + ELF_ALIGN_DOWN(offStart, *boundary);

    return  (size > UINT_MAX) ? 0 : (UINT32)size;
}

STATIC UINTPTR OsDoLoadFile(INT32 fd, UINTPTR addr, const LD_ELF_PHDR *elfPhdr, UINT32 boundary)
{
    INT32 ret;
    UINT32 offset = elfPhdr->p_offset - ELF_ALIGN_OFFSET(elfPhdr->p_vaddr, boundary);
    UINT32 size = elfPhdr->p_filesz + ELF_ALIGN_OFFSET(elfPhdr->p_vaddr, boundary);
    if (size == 0) {
        return 0;
    }

    addr = ELF_ALIGN_DOWN(addr, boundary);
    ret = OsReadELFInfo(fd, (UINT8 *)addr, size, offset);
    if (ret != LOS_OK) {
        return 0;
    }

    return addr;
}

STATIC INT32 OsLoadELFFile(DynSharedObj *dso, UINT32 boundary)
{
    DynLinkInfo *dlInfo = dso->dlInfo;
    const LD_ELF_PHDR *elfPhdrTemp = dlInfo->elfPhdr;
    const LD_ELF_EHDR *elfEhdr = &dlInfo->elfEhdr;
    UINTPTR loadBase = dso->loadBase;
    UINTPTR vAddr, loadAddr, bssStart, bssEnd;
    INT32 i;

    for (i = 0; i < elfEhdr->e_phnum; ++i, ++elfPhdrTemp) {
        if (elfPhdrTemp->p_type != PT_LOAD) {
            continue;
        }

        if ((elfPhdrTemp->p_flags & PF_R) == 0) {
            return -ENOEXEC;
        }
        vAddr = elfPhdrTemp->p_vaddr;

        loadAddr = OsDoLoadFile(dso->fd, (vAddr + loadBase), elfPhdrTemp, boundary);
        if (loadAddr == 0) {
            return -EFAULT;
        }

        if ((elfPhdrTemp->p_memsz > elfPhdrTemp->p_filesz) && (elfPhdrTemp->p_flags & PF_W)) {
            bssStart = loadAddr + ELF_ALIGN_OFFSET(vAddr, boundary) + elfPhdrTemp->p_filesz;
            bssEnd = loadAddr + ELF_ALIGN_OFFSET(vAddr, boundary) + elfPhdrTemp->p_memsz;
            (VOID)memset_s((VOID *)bssStart, bssEnd - bssStart, 0, bssEnd - bssStart);
        }
    }

    return LOS_OK;
}

STATIC INT32 OsLoadLibrary(DynSharedObj *dso)
{
    UINT32 loadSize;
    UINT32 boundary = 0;
    INT32 ret;

    loadSize = OsReserveSpace(dso->dlInfo, &boundary);
    if (loadSize == 0) {
        PRINT_ERR("failed to reserve space!\n");
        return -EINVAL;
    }

    dso->loadBase = (UINTPTR)LOS_MemAllocAlign(dso->pool, loadSize, boundary);
    if (dso->loadBase == 0) {
        PRINT_ERR("failed to alloc memory for loading shared library\n");
        return -ENOMEM;
    }

    ret = OsLoadELFFile(dso, boundary);
    if (ret != LOS_OK) {
        LOS_MemFree(dso->pool, (VOID *)dso->loadBase);
        PRINT_ERR("failed to load shared library\n");
        return ret;
    }

    return LOS_OK;
}

STATIC INT32 OsGetDynBase(DynSharedObj *dso)
{
    DynLinkInfo *dlInfo = dso->dlInfo;
    const LD_ELF_PHDR *elfPhdrTemp = dlInfo->elfPhdr;
    INT32 phdrNum = dlInfo->elfEhdr.e_phnum;
    INT32 i;

    for (i = 0; i < phdrNum; ++i, ++elfPhdrTemp) {
        if (elfPhdrTemp->p_type != PT_DYNAMIC) {
            continue;
        }
        dlInfo->dynBase = dso->loadBase + elfPhdrTemp->p_vaddr;
        return LOS_OK;
    }

    return LOS_NOK;
}

STATIC INT32 OsParseDynamic(DynSharedObj *dso)
{
    LD_ELF_DYN *dyn = NULL;
    DynLinkInfo *dlInfo = dso->dlInfo;
    RelocInfoTab *relInfoTab = &dlInfo->relInfoTab;

    for (dyn = (LD_ELF_DYN *)dlInfo->dynBase; dyn->d_tag != DT_NULL; ++dyn) {
        switch (dyn->d_tag) {
            case DT_NEEDED:
                PRINT_ERR("shared library should not depend on others\n");
                return -ENOTSUP;
            case DT_TEXTREL:
                PRINT_ERR("you should recompile shared library with -fPIC\n");
                return -EFAULT;
            case DT_HASH:
                dlInfo->hashTab = (UINT32 *)(dso->loadBase + dyn->d_un.d_ptr);
                break;
            case DT_SYMTAB:
                dlInfo->symTab = (LD_ELF_SYM *)(dso->loadBase + dyn->d_un.d_ptr);
                break;
            case DT_STRTAB:
                dlInfo->symStrings = (CHAR *)(dso->loadBase + dyn->d_un.d_ptr);
                break;
            case DT_REL:
                relInfoTab->rel.relTab = dso->loadBase + dyn->d_un.d_ptr;
                relInfoTab->rel.relEntSize = sizeof(LD_ELF_REL);
                break;
            case DT_RELSZ:
                relInfoTab->rel.relTabSize = dyn->d_un.d_val;
                break;
            case DT_RELA:
                relInfoTab->rela.relTab = dso->loadBase + dyn->d_un.d_ptr;
                relInfoTab->rela.relEntSize = sizeof(LD_ELF_RELA);
                break;
            case DT_RELASZ:
                relInfoTab->rela.relTabSize = dyn->d_un.d_val;
                break;
            case DT_JMPREL:
                relInfoTab->jmpRel.relTab = dso->loadBase + dyn->d_un.d_ptr;
                break;
            case DT_PLTRELSZ:
                relInfoTab->jmpRel.relTabSize = dyn->d_un.d_val;
                break;
            case DT_PLTREL:
                relInfoTab->jmpRel.relEntSize = (dyn->d_un.d_val == DT_REL) ? sizeof(LD_ELF_REL) : sizeof(LD_ELF_RELA);
            default:
                break;
        }
    }

    return LOS_OK;
}

STATIC UINT32 OsGetHashVal(const CHAR *name)
{
    UINT32 hashVal = 0;
    UINT32 tmp;
    const UINT8 *str = (const UINT8 *)name;

    while (*str) {
        hashVal = (*str) + (hashVal << WORD_SHIFT);
        tmp = hashVal & HASH_MASK;
        if (tmp != 0) {
            hashVal ^= tmp >> HASH_SHIFT;
        }

        hashVal &= ~tmp;
        ++str;
    }

    return hashVal;
}

STATIC LD_ELF_SYM *OsFindSymInDso(const DynLinkInfo *dlInfo, const CHAR *name)
{
    LD_ELF_SYM *symTab = dlInfo->symTab;
    LD_ELF_SYM *sym = NULL;
    CHAR *symStr = dlInfo->symStrings;
    UINT32 *hashTab = dlInfo->hashTab;
    UINT32 bucketNum = hashTab[0];
    UINT32 *bucket = &hashTab[BUCKET_IDX];
    UINT32 *chain = &bucket[bucketNum];
    UINT32 hashVal = OsGetHashVal(name);
    UINT32 symIdx;

    for (symIdx = bucket[hashVal % bucketNum]; symIdx; symIdx = chain[symIdx]) {
        if (strcmp(name, symStr + symTab[symIdx].st_name) != 0) {
            continue;
        }

        sym = symTab + symIdx;
        if ((sym->st_value == 0) || (sym->st_shndx == 0)) {
            return NULL;
        }
        return symTab + symIdx;
    }

    return NULL;
}

STATIC SymInfo *OsFindSymInTable(const CHAR *name)
{
#if defined(__ICCARM__) || defined(__CC_ARM)
    SymInfo *symTab = (SymInfo *)__section_end(".TABLE.START");
    UINT32 symTableSize = ((UINTPTR)__section_begin(".table.end") -
                          (UINTPTR)__section_end(".TABLE.START")) / sizeof(SymInfo);
#elif defined(__CLANG_ARM) || defined(__GNUC__)
    SymInfo *symTab = (SymInfo *)__sym_table_start;
    UINT32 symTableSize = (__sym_table_end - __sym_table_start) / sizeof(SymInfo);
#endif
    INT32 startIdx = 0;
    INT32 endIdx = symTableSize - 1;
    INT32 ret, midIdx;

    while (startIdx <= endIdx) {
        midIdx = startIdx + ((UINT32)(endIdx - startIdx) >> 1);
        ret = strcmp(symTab[midIdx].name, name);
        if (ret > 0) {
            endIdx = midIdx - 1;
        } else if (ret < 0) {
            startIdx = midIdx + 1;
        } else {
            return symTab + midIdx;
        }
    }

    return NULL;
}

STATIC UINTPTR OsFindSym(const DynSharedObj *dso, INT32 symIdx)
{
    DynLinkInfo *dlInfo = dso->dlInfo;
    CHAR *symStrings = dlInfo->symStrings;
    CHAR *symStr = NULL;
    LD_ELF_SYM *symTab = dlInfo->symTab;
    LD_ELF_SYM *sym = NULL;
    LD_ELF_SYM *symInDso = NULL;
    SymInfo *symInTab = NULL;

    sym = symTab + symIdx;
    symStr = symStrings + sym->st_name;
    if ((symInDso = OsFindSymInDso(dlInfo, symStr)) != NULL) {
        return dso->loadBase + symInDso->st_value;
    } else if ((symInTab = OsFindSymInTable(symStr)) != NULL) {
        return symInTab->addr;
    } else {
        PRINT_ERR("failed to relocate %s, symbol: %s not found\n", dso->fileName, symStr);
        return 0;
    }
}

STATIC INT32 OsDoReloc(const DynSharedObj *dso, INT32 type, UINTPTR relocAddr, UINT32 addend, UINTPTR symAddr)
{
    switch (type) {
        case R_ARCH_NONE:
            break;
        case R_ARCH_GLOB_DAT:
        case R_ARCH_JUMP_SLOT:
            *(UINTPTR *)relocAddr = symAddr + addend;
            break;
        case R_ARCH_ABS32:
            *(UINTPTR *)relocAddr = symAddr + ((addend != 0) ? addend : *(UINTPTR *)relocAddr);
            break;
        case R_ARCH_RELATIVE:
            *(UINTPTR *)relocAddr = dso->loadBase + ((addend != 0) ?  addend : *(UINTPTR *)relocAddr);
            break;
        default:
            PRINT_ERR("failed to relocate %s, unsupported reloc type: %d\n", dso->fileName, type);
            return -ENOTSUP;
    }

    return LOS_OK;
}

STATIC INT32 OsDoRelocSyms(DynSharedObj *dso, RelocInfo *relInfo)
{
    UINT32 relStride = relInfo->relEntSize / sizeof(UINT32);
    UINT32 *relTab = (UINT32 *)relInfo->relTab;
    UINT32 addend;
    UINTPTR relocAddr, symAddr;
    INT32 i, symIdx, ret, type;

    for (i = 0; i < relInfo->relTabSize; i += relInfo->relEntSize, relTab += relStride) {
        type = REL_TYPE(relTab[1]);
        if (type == R_ARCH_NONE) {
            continue;
        }

        symIdx = REL_SYM(relTab[1]);
        if (symIdx == 0) {
            symAddr = 0;
        } else {
            symAddr = OsFindSym(dso, symIdx);
            if (symAddr == 0) {
                return -EFAULT;
            }
        }

        relocAddr = dso->loadBase + relTab[0];
        addend = (relInfo->relEntSize == sizeof(LD_ELF_REL)) ? 0 : relTab[relStride - 1];
        ret = OsDoReloc(dso, type, relocAddr, addend, symAddr);
        if (ret != LOS_OK) {
            return ret;
        }
    }

    return LOS_OK;
}

STATIC INT32 OsRelocSyms(DynSharedObj *dso)
{
    DynLinkInfo *dlInfo = dso->dlInfo;
    RelocInfo *relInfo = (RelocInfo *)&dlInfo->relInfoTab;
    INT32 relTypes = sizeof(RelocInfoTab) / sizeof(RelocInfo);
    INT32 i, ret;

    for (i = 0; i < relTypes; ++i, ++relInfo) {
        if (relInfo->relTab == 0) {
            continue;
        }

        ret = OsDoRelocSyms(dso, relInfo);
        if (ret != LOS_OK) {
            return ret;
        }
    }

    return LOS_OK;
}

STATIC INT32 OsDoDynLink(DynSharedObj *dso)
{
    INT32 ret;

    ret = OsGetDynBase(dso);
    if (ret != LOS_OK) {
        PRINT_ERR("there are no dynamic segments in elf file\n");
        return -ELIBBAD;
    }

    ret = OsParseDynamic(dso);
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsRelocSyms(dso);
    if (ret != LOS_OK) {
        return ret;
    }

    return LOS_OK;
}

STATIC VOID OsDeLoadInit(DynSharedObj *dso)
{
    LOS_MemFree(OS_SYS_MEM_ADDR, dso->dlInfo->elfPhdr);
    dso->dlInfo->elfPhdr = NULL;
    close(dso->fd);
}

STATIC VOID OsGetInitFini(DynSharedObj *dso)
{
    LD_ELF_DYN *dyn = NULL;
    DynLinkInfo *dlInfo = dso->dlInfo;
    InitFiniTab *initFiniTab = &dso->initFiniTab;

    for (dyn = (LD_ELF_DYN *)dlInfo->dynBase; dyn->d_tag != DT_NULL; ++dyn) {
        switch (dyn->d_tag) {
            case DT_INIT:
                initFiniTab->init.func = dyn->d_un.d_ptr;
                break;
            case DT_INIT_ARRAY:
                initFiniTab->init.array = dyn->d_un.d_ptr;
                break;
            case DT_INIT_ARRAYSZ:
                initFiniTab->init.arraySz = dyn->d_un.d_val;
                break;
            case DT_FINI:
                initFiniTab->fini.func = dyn->d_un.d_ptr;
                break;
            case DT_FINI_ARRAY:
                initFiniTab->fini.array = dyn->d_un.d_ptr;
                break;
            case DT_FINI_ARRAYSZ:
                initFiniTab->fini.arraySz = dyn->d_un.d_val;
                break;
            default:
                break;
        }
    }
}

STATIC VOID OsDoInit(DynSharedObj *dso)
{
    InitFiniTab *initFiniTab = &dso->initFiniTab;
    INIT_FINI_FUNC initFunc = NULL;
    UINTPTR *func = NULL;
    UINT32 funcNum;

    OsGetInitFini(dso);
    if (initFiniTab->init.func != 0) {
        initFunc = (INIT_FINI_FUNC)(dso->loadBase + initFiniTab->init.func);
        initFunc();
    }

    if (initFiniTab->init.array == 0) {
        return;
    }

    funcNum = initFiniTab->init.arraySz / sizeof(UINTPTR);
    func = (UINTPTR *)(dso->loadBase + initFiniTab->init.array);
    while (funcNum--) {
        initFunc = (INIT_FINI_FUNC)(*func);
        initFunc();
        ++func;
    }
}

VOID *LOS_SoLoad(const CHAR *fileName, VOID *pool)
{
    INT32 ret;
    DynSharedObj *dso = NULL;

    if (fileName == NULL) {
        PRINT_ERR("invalid file name\n");
        errno = EINVAL;
        return NULL;
    }

    (VOID)LOS_MuxPend(g_dynlinkMux, LOS_WAIT_FOREVER);
    dso = OsIsPreLoaded(fileName);
    if (dso != NULL) {
        (VOID)LOS_MuxPost(g_dynlinkMux);
        return dso;
    }

    dso = OsLoadInit(fileName, pool);
    if (dso == NULL) {
        (VOID)LOS_MuxPost(g_dynlinkMux);
        return NULL;
    }

    ret = OsLoadLibrary(dso);
    if (ret != LOS_OK) {
        errno = -ret;
        goto ERR1;
    }

    ret = OsDoDynLink(dso);
    if (ret != LOS_OK) {
        errno = -ret;
        goto ERR2;
    }

    OsDoInit(dso);

    LOS_ListAdd(&g_dynSharedObjLink, &dso->dsoNode);
    (VOID)LOS_MuxPost(g_dynlinkMux);
    OsDeLoadInit(dso);

    return dso;

ERR2:
    LOS_MemFree(dso->pool, (VOID *)dso->loadBase);
ERR1:
    close(dso->fd);
    (VOID)LOS_MuxPost(g_dynlinkMux);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso->dlInfo->elfPhdr);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso->dlInfo);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso);
    return NULL;
}

STATIC DynSharedObj *OsCheckHandle(VOID *handle)
{
    DynSharedObj *dso = NULL;

    LOS_DL_LIST_FOR_EACH_ENTRY(dso, &g_dynSharedObjLink, DynSharedObj, dsoNode) {
        if (handle == dso) {
            return dso;
        }
    }

    return NULL;
}

VOID *LOS_FindSym(VOID *handle, const CHAR *name)
{
    LD_ELF_SYM *sym = NULL;
    DynSharedObj *dso = NULL;
    VOID *symAddr = NULL;

    if ((handle == NULL) || (name == NULL)) {
        goto ERR;
    }

    (VOID)LOS_MuxPend(g_dynlinkMux, LOS_WAIT_FOREVER);
    dso = OsCheckHandle(handle);
    if (dso == NULL) {
        (VOID)LOS_MuxPost(g_dynlinkMux);
        goto ERR;
    }

    sym = OsFindSymInDso(dso->dlInfo, name);
    if (sym == NULL) {
        (VOID)LOS_MuxPost(g_dynlinkMux);
        PRINT_ERR("failed to find symbol: %s\n", name);
        errno = EFAULT;
        return NULL;
    }
    symAddr = (VOID *)(dso->loadBase + sym->st_value);
    (VOID)LOS_MuxPost(g_dynlinkMux);
    return symAddr;

ERR:
    PRINT_ERR("invalid input param\n");
    errno = EINVAL;
    return NULL;
}

STATIC VOID OsDoFini(DynSharedObj *dso)
{
    InitFiniTab *initFiniTab = &dso->initFiniTab;
    INIT_FINI_FUNC finiFunc = NULL;
    UINTPTR *func = NULL;
    UINT32 funcNum;

    if (initFiniTab->fini.array != 0) {
        funcNum = initFiniTab->fini.arraySz / sizeof(UINTPTR);
        func = (UINTPTR *)(dso->loadBase + initFiniTab->fini.array) + funcNum;
        while (funcNum--) {
            --func;
            finiFunc = (INIT_FINI_FUNC)(*func);
            finiFunc();
       }
    }

    if (initFiniTab->fini.func != 0) {
        finiFunc = (INIT_FINI_FUNC)(dso->loadBase + initFiniTab->fini.func);
        finiFunc();
    }
}

INT32 LOS_SoUnload(VOID *handle)
{
    DynSharedObj *dso = NULL;

    if (handle == NULL) {
        goto ERR;
    }

    (VOID)LOS_MuxPend(g_dynlinkMux, LOS_WAIT_FOREVER);
    dso = OsCheckHandle(handle);
    if (dso == NULL) {
        (VOID)LOS_MuxPost(g_dynlinkMux);
        goto ERR;
    }

    if (dso->ref > 1) {
        --dso->ref;
        (VOID)LOS_MuxPost(g_dynlinkMux);
        return LOS_OK;
    }

    OsDoFini(dso);

    LOS_ListDelete(&dso->dsoNode);
    (VOID)LOS_MuxPost(g_dynlinkMux);

    LOS_MemFree(dso->pool, (VOID *)dso->loadBase);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso->dlInfo);
    LOS_MemFree(OS_SYS_MEM_ADDR, dso);

    return LOS_OK;
ERR:
    PRINT_ERR("invalid handle\n");
    errno = EINVAL;
    return LOS_NOK;
}

INT32 LOS_DynlinkInit(VOID)
{
    UINT32 ret;

    LOS_ListInit(&g_dynSharedObjLink);
    ret = LOS_MuxCreate(&g_dynlinkMux);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    return LOS_OK;
}

#endif /* LOSCFG_DYNLINK */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

