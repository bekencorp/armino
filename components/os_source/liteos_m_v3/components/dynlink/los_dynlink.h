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
#ifndef _LOS_DYNLINK_H
#define _LOS_DYNLINK_H

#include "elf.h"
#include "los_compiler.h"
#include "los_list.h"
#include "los_memory.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_AARCH64
typedef Elf64_Ehdr      LD_ELF_EHDR;
typedef Elf64_Phdr      LD_ELF_PHDR;
typedef Elf64_Sym       LD_ELF_SYM;
typedef Elf64_Dyn       LD_ELF_DYN;
typedef Elf64_Rel       LD_ELF_REL;
typedef Elf64_Rela      LD_ELF_RELA;
#define REL_TYPE(x)     ELF64_R_TYPE(x)
#define REL_SYM(x)      ELF64_R_SYM(x)
#define REL_INFO        ELF64_R_INFO
#else
typedef Elf32_Ehdr      LD_ELF_EHDR;
typedef Elf32_Phdr      LD_ELF_PHDR;
typedef Elf32_Sym       LD_ELF_SYM;
typedef Elf32_Dyn       LD_ELF_DYN;
typedef Elf32_Rel       LD_ELF_REL;
typedef Elf32_Rela      LD_ELF_RELA;
#define REL_TYPE(x)     ELF32_R_TYPE(x)
#define REL_SYM(x)      ELF32_R_SYM(x)
#define REL_INFO        ELF32_R_INFO
#endif

#define LD_ELFMAG       "\177ELF"
#define LD_SELFMAG      4
#define PHDR_NUM_MAX    128
#define DYNAMIC_CNT     32
#define HASH_MASK       0xf0000000
#define WORD_SHIFT      4
#define HASH_SHIFT      24
#define BUCKET_IDX      2

#define ELF_ALIGN_UP(a, b)          (((a) + ((b) - 1)) & ~((b) - 1))
#define ELF_ALIGN_DOWN(a, b)        ((a) & ~((b) - 1))
#define ELF_ALIGN_OFFSET(a, b)      ((a) & ((b) - 1))
#ifndef PATH_MAX
#define PATH_MAX 256
#endif
#ifndef FILE_LENGTH_MAX
#define FILE_LENGTH_MAX  0x40000
#endif
typedef VOID (*INIT_FINI_FUNC)(VOID);

typedef struct {
    UINTPTR relTab;
    UINT32  relTabSize;
    UINT32  relEntSize;
} RelocInfo;

typedef struct {
    RelocInfo rel;
    RelocInfo rela;
    RelocInfo jmpRel;
} RelocInfoTab;

typedef struct {
    UINTPTR func;
    UINTPTR array;
    UINT32  arraySz;
} InitFiniInfo;

typedef struct {
    InitFiniInfo init;
    InitFiniInfo fini;
} InitFiniTab;

typedef struct {
    LD_ELF_EHDR  elfEhdr;
    LD_ELF_PHDR  *elfPhdr;
    UINTPTR      dynBase;
    UINT32       *hashTab;
    LD_ELF_SYM   *symTab;
    CHAR         *symStrings;
    RelocInfoTab relInfoTab;
} DynLinkInfo;

typedef struct {
    CHAR         *fileName;
    INT32        fd;
    DynLinkInfo  *dlInfo;
    UINTPTR      loadBase;
    InitFiniTab  initFiniTab;
    LOS_DL_LIST  dsoNode;
    UINT32       ref;
    VOID         *pool;
    CHAR         buf[];
} DynSharedObj;

typedef struct {
  CHAR    *name;
  UINTPTR addr;
} SymInfo;

#define SYM_EXPORT(func)                                            \
const SymInfo sym_##func __attribute__((section(".sym."#func))) = { \
    .name = #func,                                                  \
    .addr = (UINTPTR)func                                           \
};

/*
 * @brief Load the shared library file named by the NULL-terminated string filename and
 * return a valid handle for the loaded library.
 *
 * @param fileName The name pointer of shared library.
 * @param pool The heap for shared library to load. If the parameter, pool, is NULL, then
 * the dynamic loader & linker module will use the default heap and the pool is not NULL,
 * then the shared library will be loaded to the heap by pool.
 *
 * @note When the heap, pool, is not NULL, you should call LOS_MemInit() to initialize the
 * pool before calling LOS_SoLoad(). By the way, the system will comsume a certain amount
 * of memory to initialize the pool. LOS_SoLoad must not be called in interrupt callback.
 *
 * @return Return NULL if error. Return non-NULL if success.
 */
VOID *LOS_SoLoad(const CHAR *fileName, VOID *pool);

/*
 * @brief Get the address of symbol named by the parameter, name, from the parameter, handle.
 *
 * @param handle The handle for the loaded shared library.
 * @param name   The name of symbol to search.
 *
 * @note LOS_FindSym must not be called in interrupt callback.
 *
 * @return Return NULL if error. Return non-NULL if success.
 */
VOID *LOS_FindSym(VOID *handle, const CHAR *name);

/*
 * @brief Decrement the reference count on the loaded shared library refered to by handle.
 * If the reference count drops to zero, then the library is unloaded.
 *
 * This function validates that the handle is valid.
 *
 * @param handle The handle for the loaded shared library by LOS_SoLoad() interface.
 *
 * @note LOS_SoUnload must not be called in interrupt callback.
 *
 * @return Return 1 if error. Return 0 if success.
 */
INT32 LOS_SoUnload(VOID *handle);

/*
 * @brief Initialization for dynamic loader & linker module.
 *
 * @param VOID.
 *
 * @return Return LOS_NOK if error. Return LOS_OK if success.
 */
INT32 LOS_DynlinkInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_DYNLINK_H */

