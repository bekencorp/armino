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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "limits.h"
#include "shell.h"
#include "show.h"

#define TEMP_PATH_MAX (PATH_MAX + SHOW_MAX_LEN)

static unsigned int VfsStrnlen(const char *str, size_t maxlen)
{
    const char *p = NULL;

    for (p = str; ((maxlen-- != 0) && (*p != '\0')); ++p) {}

    return p - str;
}

/* abandon the redundant '/' in the path, only keep one. */
static char *StrPath(char *path)
{
    char *dest = path;
    char *src = path;

    while (*src != '\0') {
        if (*src == '/') {
            *dest++ = *src++;
            while (*src == '/') {
                src++;
            }
            continue;
        }
        *dest++ = *src++;
    }
    *dest = '\0';
    return path;
}

static void StrRemovePathEndSlash(char *dest, const char *fullpath)
{
    if ((*dest == '.') && (*(dest - 1) == '/')) {
        *dest = '\0';
        dest--;
    }
    if ((dest != fullpath) && (*dest == '/')) {
        *dest = '\0';
    }
}

static char *StrNormalizePath(char *fullpath)
{
    char *dest = fullpath;
    char *src = fullpath;

    /* 2: The position of the path character: / and the end character /0 */
    while (*src != '\0') {
        if (*src == '.') {
            if (*(src + 1) == '/') {
                src += 2; /* 2, sizeof "./" */
                continue;
            } else if (*(src + 1) == '.') {
                if ((*(src + 2) == '/') || (*(src + 2) == '\0')) { /* 2, 2, offset to check */
                    src += 2; /* 2, sizeof offset */
                } else {
                    while ((*src != '\0') && (*src != '/')) {
                        *dest++ = *src++;
                    }
                    continue;
                }
            } else {
                *dest++ = *src++;
                continue;
            }
        } else {
            *dest++ = *src++;
            continue;
        }

        if ((dest - 1) != fullpath) {
            dest--;
        }

        while ((dest > fullpath) && (*(dest - 1) != '/')) {
            dest--;
        }

        if (*src == '/') {
            src++;
        }
    }

    *dest = '\0';

    /* remove '/' in the end of path if exist */

    dest--;

    StrRemovePathEndSlash(dest, fullpath);
    return dest;
}

static int VfsNormalizePathParameCheck(const char *filename, char **pathname)
{
    int namelen;
    char *name = NULL;

    if (pathname == NULL) {
        return -EINVAL;
    }

    /* check parameters */

    if (filename == NULL) {
        *pathname = NULL;
        return -EINVAL;
    }

    namelen = VfsStrnlen(filename, PATH_MAX);
    if (!namelen) {
        *pathname = NULL;
        return -EINVAL;
    } else if (namelen >= PATH_MAX) {
        *pathname = NULL;
        return -ENAMETOOLONG;
    }

    for (name = (char *)filename + namelen; ((name != filename) && (*name != '/')); name--) {
        if (strlen(name) > NAME_MAX) {
            *pathname = NULL;
            return -ENAMETOOLONG;
        }
    }

    return namelen;
}

static char *VfsNotAbsolutePath(const char *directory, const char *filename, char **pathname, int namelen)
{
    int ret;
    char *fullpath = NULL;

    /* 2: The position of the path character: / and the end character /0 */

    if ((namelen > 1) && (filename[0] == '.') && (filename[1] == '/')) {
        filename += 2; /* 2, sizeof "./" */
    }

    fullpath = (char *)malloc(strlen(directory) + namelen + 2);
    if (fullpath == NULL) {
        *pathname = NULL;
        SetErrno(ENOMEM);
        return (char *)NULL;
    }

    /* 2, sizeof "./", join path and file name */
    ret = snprintf_s(fullpath, strlen(directory) + namelen + 2, strlen(directory) + namelen + 1,
                     "%s/%s", directory, filename);
    if (ret < 0) {
        *pathname = NULL;
        free(fullpath);
        SetErrno(ENAMETOOLONG);
        return (char *)NULL;
    }

    return fullpath;
}

static char *VfsNormalizeFullpath(const char *directory, const char *filename, char **pathname, int namelen)
{
    char *fullpath = NULL;

    if (filename[0] != '/') {
        /* not a absolute path */

        fullpath = VfsNotAbsolutePath(directory, filename, pathname, namelen);
        if (fullpath == NULL) {
            return (char *)NULL;
        }
    } else {
        /* it's a absolute path, use it directly */

        fullpath = strdup(filename); /* copy string */
        if (fullpath == NULL) {
            *pathname = NULL;
            SetErrno(ENOMEM);
            return (char *)NULL;
        }
        if (filename[1] == '/') {
            *pathname = NULL;
            free(fullpath);
            SetErrno(EINVAL);
            return (char *)NULL;
        }
    }

    return fullpath;
}

int VfsNormalizePath(const char *directory, const char *filename, char **pathname)
{
    char *fullpath = NULL;
    int namelen;

    namelen = VfsNormalizePathParameCheck(filename, pathname);
    if (namelen < 0) {
        return namelen;
    }

    if ((directory == NULL) && (filename[0] != '/')) {
        PRINT_ERR("NO_WORKING_DIR\n");
        *pathname = NULL;
        return -EINVAL;
    }

    /* 2, The position of the path character: / and the end character /0 */
    if ((filename[0] != '/') && (strlen(directory) + namelen + 2 > TEMP_PATH_MAX)) {
        return -ENAMETOOLONG;
    }

    fullpath = VfsNormalizeFullpath(directory, filename, pathname, namelen);
    if (fullpath == NULL) {
        return -errno;
    }

    (void)StrPath(fullpath);
    (void)StrNormalizePath(fullpath);
    if (strlen(fullpath) >= PATH_MAX) {
        *pathname = NULL;
        free(fullpath);
        return -ENAMETOOLONG;
    }

    *pathname = fullpath;
    return 0;
}

