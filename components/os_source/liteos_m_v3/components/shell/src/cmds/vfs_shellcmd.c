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

#include "los_config.h"

#include "los_task.h"
#include "shell.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdio.h"
#include "pthread.h"

#include "shcmd.h"
#include "securec.h"
#include "show.h"

#include <dirent.h>
#include <ctype.h>

#define VFS_ERROR OS_ERROR
#define SHOW_MAX_LEN CMD_MAX_LEN
#define TEMP_PATH_MAX (PATH_MAX + SHOW_MAX_LEN)

typedef enum {
    RM_RECURSIVER,
    RM_FILE,
    RM_DIR,
    CP_FILE,
    CP_COUNT
} wildcard_type;

#define ERROR_OUT_IF(condition, message_function, handler) \
    do { \
        if (condition) { \
            message_function; \
            handler; \
        } \
    } while (0)

int OsShellCmdDoChdir(const char *path)
{
    char *fullpath = NULL;
    char *fullpathBak = NULL;
    DIR *dirent = NULL;
    int ret;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    if (path == NULL) {
        LOS_TaskLock();
        PRINTK("%s\n", shellWorkingDirectory);
        LOS_TaskUnlock();
        return 0;
    }

    ERROR_OUT_IF(strlen(path) > PATH_MAX, SetErr(ENOTDIR, "cd error"), return -1);

    ret = VfsNormalizePath(shellWorkingDirectory, path, &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "cd error"), return -1);

    fullpathBak = fullpath;
    dirent = opendir(fullpath);
    if (dirent == NULL) {
        free(fullpathBak);
        /* this is a not exist directory */
        PRINTK("no such file or directory\n");
        return -1;
    }

    /* close directory stream */
    (void)closedir(dirent);

    /* copy full path to working directory */
    LOS_TaskLock();
    ret = strncpy_s(shellWorkingDirectory, PATH_MAX, fullpath, strlen(fullpath));
    if (ret != EOK) {
        free(fullpathBak);
        LOS_TaskUnlock();
        return -1;
    }
    LOS_TaskUnlock();
    /* release normalize directory path name */

    free(fullpathBak);
    return 0;
}

static char *OsLsGetFullpath(const char *path, struct dirent *pdirent)
{
    char *fullpath = NULL;
    int ret;

    if (path[1] != '\0') {
        fullpath = (char *)malloc(strlen(path) + strlen(pdirent->d_name) + 2);
        if (fullpath == NULL) {
            goto exit_with_nomem;
        }

        ret = snprintf_s(fullpath, strlen(path) + strlen(pdirent->d_name) + 2,
                         strlen(path) + strlen(pdirent->d_name) + 1, "%s/%s", path, pdirent->d_name);
        if (ret < 0) {
            free(fullpath);
            return NULL;
        }
    } else {
        fullpath = (char *)malloc(strlen(pdirent->d_name) + 2);
        if (fullpath == NULL) {
            goto exit_with_nomem;
        }

        ret = snprintf_s(fullpath, strlen(pdirent->d_name) + 2, strlen(pdirent->d_name) + 1,
                         "/%s", pdirent->d_name);
        if (ret < 0) {
            free(fullpath);
            return NULL;
        }
    }
return fullpath;
exit_with_nomem:
    return (char *)NULL;
}

void OsLs(const char *pathname)
{
    struct dirent *pdirent = NULL;
    char *path = NULL;
    char *fullpath = NULL;
    char *fullpathBak = NULL;
    int ret;
    struct stat statInfo = { 0 };
    DIR *d = NULL;

    /* list all directory and file */
    if (pathname == NULL) {
        path = strdup("/");
        if (path == NULL) {
            return;
        }
    } else {
        ret = VfsNormalizePath(NULL, pathname, &path);
        if (ret < 0) {
            SetErrno(-ret);
            return;
        }
    }

    d = opendir(path);
    if (d == NULL) {
        PRINT_ERR("No such directory = %s\n", path);
    } else {
        PRINTK("Directory %s:\n", path);
        do {
            pdirent = readdir(d);
            if (pdirent != NULL) {
                (void)memset_s(&statInfo, sizeof(struct stat), 0, sizeof(struct stat));
                fullpath = OsLsGetFullpath(path, pdirent);
                if (fullpath == NULL) {
                    free(path);
                    (void)closedir(d);
                    return;
                }

                fullpathBak = fullpath;
                if (stat(fullpath, &statInfo) == 0) {
                    PRINTK("%-20s", pdirent->d_name);
                    if (S_ISDIR(statInfo.st_mode)) {
                        PRINTK(" %-25s\n", "<DIR>");
                    } else {
                        PRINTK(" %-25lu\n", statInfo.st_size);
                    }
                } else {
                    PRINTK("BAD file: %s\n", pdirent->d_name);
                }
                free(fullpathBak);
            }
        } while (pdirent != NULL);

        (void)closedir(d);
    }
}

int OsShellCmdLs(int argc, const char **argv)
{
    char *fullpath = NULL;
    const char *filename = NULL;
    int ret;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc > 1, PRINTK("ls or ls [DIRECTORY]\n"), return -1);

    if (argc == 0) {
        OsLs(shellWorkingDirectory);
        return 0;
    }

    filename = argv[0];
    ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "ls error"), return -1);

    OsLs(fullpath);
    free(fullpath);

    return 0;
}

int OsShellCmdCd(int argc, const char **argv)
{
    if (argc == 0) {
        (void)OsShellCmdDoChdir("/");
        return 0;
    }

    OsShellCmdDoChdir(argv[0]);
    return 0;
}

#define CAT_BUF_SIZE  512
#define CAT_TASK_PRIORITY  10
#define CAT_TASK_STACK_SIZE  0x3000
pthread_mutex_t g_mutex_cat = PTHREAD_MUTEX_INITIALIZER;

int OsShellCmdCat(int argc, const char **argv)
{
    char *fullpath = NULL;
    int ret;
    CHAR buf[CAT_BUF_SIZE];
    size_t size;

    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();

    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc != 1, PRINTK("cat [FILE]\n"), return -1);

    ret = VfsNormalizePath(shellWorkingDirectory, argv[0], &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "cat error"), return -1);

    int fd = open(fullpath, O_RDONLY, 0666);

    if (fd == -1) {
        ret = -1;
        free(fullpath);
        return ret;
    }

    do {
        (void)memset_s(buf, sizeof(buf), 0, CAT_BUF_SIZE);
        size = read(fd, buf, CAT_BUF_SIZE - 1);
        if ((int)size < 0) {
            free(fullpath);
            close(fd);
            return -1;
        }
        PRINTK("%s", buf);
        (void)LOS_TaskDelay(1);
    } while (size == CAT_BUF_SIZE);

    free(fullpath);
    close(fd);

    return ret;
}

int OsShellCmdMkdir(int argc, const char **argv)
{
    int ret;
    char *fullpath = NULL;
    const char *filename = NULL;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc != 1, PRINTK("mkdir [DIRECTORY]\n"), return 0);

    filename = argv[0];
    ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "mkdir error"), return -1);

    ret = mkdir(fullpath, S_IRWXU | S_IRWXG | S_IRWXO);
    if (ret == -1) {
        perror("mkdir error");
    }
    free(fullpath);
    return 0;
}

int OsShellCmdPwd(int argc, const char **argv)
{
    char buf[SHOW_MAX_LEN] = {0};
    DIR *dir = NULL;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc > 0, PRINTK("\nUsage: pwd\n"), return -1);

    LOS_TaskLock();
    if (strncpy_s(buf, SHOW_MAX_LEN, shellWorkingDirectory, SHOW_MAX_LEN - 1) != EOK) {
        LOS_TaskUnlock();
        PRINTK("pwd error: strncpy_s error!\n");
        return -1;
    }
    LOS_TaskUnlock();

    PRINTK("%s\n", buf);
    return 0;
}

int OsShellCmdTouch(int argc, const char **argv)
{
    int ret;
    int fd = -1;
    char *fullpath = NULL;
    const char *filename = NULL;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc != 1, PRINTK("touch [FILE]\n"), return -1);

    filename = argv[0];
    ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "touch error"), return -1);

    fd = open(fullpath, O_RDWR | O_CREAT, 0777);
    free(fullpath);
    if (fd == -1) {
        perror("touch error");
        return -1;
    }

    (void)close(fd);
    return 0;
}

#define CP_BUF_SIZE 4096
pthread_mutex_t g_mutexCp = PTHREAD_MUTEX_INITIALIZER;

static int OsShellCmdDoCp(const char *srcFilePath, const char *dstFileName)
{
    int  ret;
    char *srcFullPath = NULL;
    char *drcFullPath = NULL;
    const char *srcFileName = NULL;
    char *dstFilePath = NULL;
    char *buf = NULL;
    const char *filename = NULL;
    size_t rdSize, wrSize;
    int srcFd = -1;
    int dstFd = -1;
    struct stat statBuf;
    mode_t srcMode;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    buf = (char *)malloc(CP_BUF_SIZE);
    if (buf == NULL) {
        PRINTK("cp error: Out of memory!\n");
        return -1;
    }

    /* Get source fullpath. */
    ret = VfsNormalizePath(shellWorkingDirectory, srcFilePath, &srcFullPath);
    if (ret < 0) {
        SetErrno(-ret);
        PRINTK("cp error: %s\n", strerror(errno));
        free(buf);
        return -1;
    }

    /* Is source path exist? */
    ret = stat(srcFullPath, &statBuf);
    if (ret == -1) {
        PRINTK("cp %s error: %s\n", srcFullPath, strerror(errno));
        goto errout_with_srcpath;
    }
    srcMode = statBuf.st_mode;
    /* Is source path a directory? */
    if (S_ISDIR(statBuf.st_mode)) {
        PRINTK("cp %s error: Source file can't be a directory.\n", srcFullPath);
        goto errout_with_srcpath;
    }

    /* Get dest fullpath. */
    drcFullPath = strdup(dstFileName);
    if (drcFullPath == NULL) {
        PRINTK("cp error: Out of memory.\n");
        goto errout_with_srcpath;
    }
    /* Is dest path exist? */
    ret = stat(drcFullPath, &statBuf);
    if (ret == 0) {
        /* Is dest path a directory? */
        if (S_ISDIR(statBuf.st_mode)) {
            /* Get source file name without '/'. */
            srcFileName = srcFilePath;
            while (1) {
                filename = strchr(srcFileName, '/');
                if (filename == NULL) {
                    break;
                }
                srcFileName = filename + 1;
            }
            /* Add the source file after dest path. */
            ret = VfsNormalizePath(drcFullPath, srcFileName, &dstFilePath);
            if (ret < 0) {
                SetErrno(-ret);
                PRINTK("cp error. %s.\n", strerror(errno));
                goto errout_with_path;
            }
            free(drcFullPath);
            drcFullPath = dstFilePath;
        }
    }

    /* Is dest file same as source file? */
    if (strcmp(srcFullPath, drcFullPath) == 0) {
        PRINTK("cp error: '%s' and '%s' are the same file\n", srcFullPath, drcFullPath);
        goto errout_with_path;
    }

    /* Copy begins. */
    (void)pthread_mutex_lock(&g_mutexCp);
    srcFd = open(srcFullPath, O_RDONLY);
    if (srcFd < 0) {
        PRINTK("cp error: can't open %s. %s.\n", srcFullPath, strerror(errno));
        goto errout_with_mutex;
    }

    dstFd = open(drcFullPath, O_CREAT | O_WRONLY | O_TRUNC, srcMode);
    if (dstFd < 0) {
        PRINTK("cp error: can't create %s. %s.\n", drcFullPath, strerror(errno));
        goto errout_with_srcfd;
    }

    do {
        (void)memset_s(buf, CP_BUF_SIZE, 0, CP_BUF_SIZE);
        rdSize = read(srcFd, buf, CP_BUF_SIZE);
        if (rdSize < 0) {
            PRINTK("cp %s %s failed. %s.\n", srcFullPath, drcFullPath, strerror(errno));
            goto errout_with_fd;
        }
        wrSize = write(dstFd, buf, rdSize);
        if (wrSize != rdSize) {
            PRINTK("cp %s %s failed. %s.\n", srcFullPath, drcFullPath, strerror(errno));
            goto errout_with_fd;
        }
    } while (rdSize == CP_BUF_SIZE);

    /* Release resource. */
    free(buf);
    free(srcFullPath);
    free(drcFullPath);
    (void)close(srcFd);
    (void)close(dstFd);
    (void)pthread_mutex_unlock(&g_mutexCp);
    return LOS_OK;

errout_with_fd:
    (void)close(dstFd);
errout_with_srcfd:
    (void)close(srcFd);
errout_with_mutex:
    (void)pthread_mutex_unlock(&g_mutexCp);
errout_with_path:
    free(drcFullPath);
errout_with_srcpath:
    free(srcFullPath);
    free(buf);
    return -1;
}

/* The separator and EOF for a directory fullpath: '/'and '\0' */
#define SEPARATOR_EOF_LEN 2

static int OsShellCmdDoRmdir(const char *pathname)
{
    struct dirent *dirent = NULL;
    struct stat statInfo;
    DIR *d = NULL;
    char *fullpath = NULL;
    int ret;

    (void)memset_s(&statInfo, sizeof(statInfo), 0, sizeof(struct stat));
    if (stat(pathname, &statInfo) != 0) {
        return -1;
    }

    if (S_ISREG(statInfo.st_mode) || S_ISLNK(statInfo.st_mode)) {
        return remove(pathname);
    }
    d = opendir(pathname);
    if (d == NULL) {
        return -1;
    }
    while (1) {
        dirent = readdir(d);
        if (dirent == NULL) {
            break;
        }
        if (strcmp(dirent->d_name, "..") && strcmp(dirent->d_name, ".")) {
            size_t fullPathBufSize = strlen(pathname) + strlen(dirent->d_name) + SEPARATOR_EOF_LEN;
            if (fullPathBufSize <= 0) {
                PRINTK("buffer size is invalid!\n");
                (void)closedir(d);
                return -1;
            }
            fullpath = (char *)malloc(fullPathBufSize);
            if (fullpath == NULL) {
                PRINTK("malloc failure!\n");
                (void)closedir(d);
                return -1;
            }
            ret = snprintf_s(fullpath, fullPathBufSize, fullPathBufSize - 1, "%s/%s", pathname, dirent->d_name);
            if (ret < 0) {
                PRINTK("name is too long!\n");
                free(fullpath);
                (void)closedir(d);
                return -1;
            }
            (void)OsShellCmdDoRmdir(fullpath);
            free(fullpath);
        }
    }
    (void)closedir(d);
    return rmdir(pathname);
}

/*  Wildcard matching operations  */
static int OsWildcardMatch(const char *src, const char *filename)
{
    int ret;

    if (*src != '\0') {
        if (*filename == '*') {
            while ((*filename == '*') || (*filename == '?')) {
                filename++;
            }

            if (*filename == '\0') {
                return 0;
            }

            while (*src != '\0' && !(*src == *filename)) {
                src++;
            }

            if (*src == '\0') {
                return -1;
            }

            ret = OsWildcardMatch(src, filename);

            while ((ret != 0) && (*(++src) != '\0')) {
                if (*src == *filename) {
                    ret = OsWildcardMatch(src, filename);
                }
            }
            return ret;
        } else {
            if ((*src == *filename) || (*filename == '?')) {
                return OsWildcardMatch(++src, ++filename);
            }
            return -1;
        }
    }

    while (*filename != '\0') {
        if (*filename != '*') {
            return -1;
        }
        filename++;
    }
    return 0;
}

/*   To determine whether a wildcard character exists in a path   */
static int OsIsContainersWildcard(const char *filename)
{
    while (*filename != '\0') {
        if ((*filename == '*') || (*filename == '?')) {
            return 1;
        }
        filename++;
    }
    return 0;
}

/*  Delete a matching file or directory  */

static int OsWildcardDeleteFileOrDir(const char *fullpath, wildcard_type mark)
{
    int ret;

    switch (mark) {
        case RM_RECURSIVER:
            ret = OsShellCmdDoRmdir(fullpath);
            break;
        case RM_FILE:
            ret = unlink(fullpath);
            break;
        case RM_DIR:
            ret = rmdir(fullpath);
            break;
        default:
            return VFS_ERROR;
    }
    if (ret == -1) {
        PRINTK("%s  ", fullpath);
        perror("rm/rmdir error!");
        return ret;
    }

    PRINTK("%s match successful!delete!\n", fullpath);
    return 0;
}

/*  Split the path with wildcard characters  */

static char* OsWildcardSplitPath(char *fullpath, char **handle, char **wait)
{
    int n;
    int a = 0;
    int b = 0;
    int len  = strlen(fullpath);

    for (n = 0; n < len; n++) {
        if (fullpath[n] == '/') {
            if (b != 0) {
                fullpath[n] = '\0';
                *wait = fullpath + n + 1;
                break;
            }
            a = n;
        } else if (fullpath[n] == '*' || fullpath[n] == '?') {
            b = n;
            fullpath[a] = '\0';
            if (a == 0) {
                *handle = fullpath + a + 1;
                continue;
            }
            *handle = fullpath + a + 1;
        }
    }
    return fullpath;
}

/*  Handling entry of the path with wildcard characters  */

static int OsWildcardExtractDirectory(char *fullpath, void *dst, wildcard_type mark)
{
    char separator[] = "/";
    char src[PATH_MAX] = {0};
    struct dirent *dirent = NULL;
    char *f = NULL;
    char *s = NULL;
    char *t = NULL;
    int ret = 0;
    DIR *d = NULL;
    struct stat statBuf;
    int deleteFlag = 0;

    f = OsWildcardSplitPath(fullpath, &s, &t);

    if (s == NULL) {
        if (mark == CP_FILE) {
            ret = OsShellCmdDoCp(fullpath, dst);
        } else if (mark == CP_COUNT) {
            ret = stat(fullpath, &statBuf);
            if (ret == 0 && (S_ISREG(statBuf.st_mode) || S_ISLNK(statBuf.st_mode))) {
                (*(int *)dst)++;
            }
        } else {
            ret = OsWildcardDeleteFileOrDir(fullpath, mark);
        }
        return ret;
    }

    d = (*f == '\0') ? opendir("/") : opendir(f);

    if (d == NULL) {
        perror("opendir error");
        return VFS_ERROR;
    }

    while (1) {
        dirent = readdir(d);
        if (dirent == NULL) {
            break;
        }

        ret = strcpy_s(src, PATH_MAX, f);
        if (ret != EOK) {
            goto closedir_out;
        }

        ret = OsWildcardMatch(dirent->d_name, s);
        if (ret == 0) {
            ret = strcat_s(src, sizeof(src), separator);
            if (ret != EOK) {
                goto closedir_out;
            }
            ret = strcat_s(src, sizeof(src), dirent->d_name);
            if (ret != EOK) {
                goto closedir_out;
            }
            if (t == NULL) {
                if (mark == CP_FILE) {
                    ret = OsShellCmdDoCp(src, dst);
                } else if (mark == CP_COUNT) {
                    ret = stat(src, &statBuf);
                    if (ret == 0 && (S_ISREG(statBuf.st_mode) || S_ISLNK(statBuf.st_mode))) {
                        (*(int *)dst)++;
                        if ((*(int *)dst) > 1) {
                            break;
                        }
                    }
                } else {
                    ret = OsWildcardDeleteFileOrDir(src, mark);
                    if (ret == 0) {
                        deleteFlag = 1;
                    }
                }
            } else {
                ret = strcat_s(src, sizeof(src), separator);
                if (ret != EOK) {
                    goto closedir_out;
                }
                ret = strcat_s(src, sizeof(src), t);
                if (ret != EOK) {
                    goto closedir_out;
                }
                ret = OsWildcardExtractDirectory(src, dst, mark);
                if (mark == CP_COUNT && (*(int *)dst) > 1) {
                    break;
                }
            }
        }
    }
    (void)closedir(d);
    if (deleteFlag == 1) {
        ret = 0;
    }
    return ret;
closedir_out:
    (void)closedir(d);
    return VFS_ERROR;
}

int OsShellCmdCp(int argc, const char **argv)
{
    int  ret;
    const char *src = NULL;
    const char *dst = NULL;
    char *srcFullPath = NULL;
    char *drcFullPath = NULL;
    struct stat statBuf;
    int count = 0;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc < 2, PRINTK("cp [SOURCEFILE] [DESTFILE]\n"), return -1);

    src = argv[0];
    dst = argv[1];

    /* Get source fullpath. */

    ret = VfsNormalizePath(shellWorkingDirectory, src, &srcFullPath);
    if (ret < 0) {
        SetErrno(-ret);
        PRINTK("cp error:%s\n", strerror(errno));
        return -1;
    }

    if (src[strlen(src) - 1] == '/') {
        PRINTK("cp %s error: Source file can't be a directory.\n", src);
        goto errout_with_srcpath;
    }

    /* Get dest fullpath. */
    ret = VfsNormalizePath(shellWorkingDirectory, dst, &drcFullPath);
    if (ret < 0) {
        SetErrno(-ret);
        PRINTK("cp error: can't open %s. %s\n", dst, strerror(errno));
        goto errout_with_srcpath;
    }

    /* Is dest path exist? */

    ret = stat(drcFullPath, &statBuf);
    if (ret < 0) {
        /* Is dest path a directory? */
        if (dst[strlen(dst) - 1] == '/') {
            PRINTK("cp error: %s, %s.\n", drcFullPath, strerror(errno));
            goto errout_with_path;
        }
    } else {
        if ((S_ISREG(statBuf.st_mode) || S_ISLNK(statBuf.st_mode)) && dst[strlen(dst) - 1] == '/') {
            PRINTK("cp error: %s is not a directory.\n", drcFullPath);
            goto errout_with_path;
        }
    }

    if (OsIsContainersWildcard(srcFullPath)) {
        if (ret < 0 || S_ISREG(statBuf.st_mode) || S_ISLNK(statBuf.st_mode)) {
            char *srcCopy = strdup(srcFullPath);
            if (srcCopy == NULL) {
                PRINTK("cp error : Out of memory.\n");
                goto errout_with_path;
            }
            (void)OsWildcardExtractDirectory(srcCopy, &count, CP_COUNT);
            free(srcCopy);
            if (count > 1) {
                PRINTK("cp error : %s is not a directory.\n", drcFullPath);
                goto errout_with_path;
            }
        }
        ret = OsWildcardExtractDirectory(srcFullPath, drcFullPath, CP_FILE);
    } else {
        ret = OsShellCmdDoCp(srcFullPath, drcFullPath);
    }
    free(drcFullPath);
    free(srcFullPath);
    return ret;

errout_with_path:
    free(drcFullPath);
errout_with_srcpath:
    free(srcFullPath);
    return VFS_ERROR;
}

static inline void PrintRmUsage(void)
{
    PRINTK("rm [FILE] or rm [-r/-R] [FILE]\n");
}

int OsShellCmdRm(int argc, const char **argv)
{
    int  ret;
    char *fullpath = NULL;
    const char *filename = NULL;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();

    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc != 1 && argc != 2, PrintRmUsage(), return -1);

    if (argc == 2) {
        ERROR_OUT_IF(strcmp(argv[0], "-r") != 0 && strcmp(argv[0], "-R") != 0, PrintRmUsage(), return -1);

        filename = argv[1];
        ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
        ERROR_OUT_IF(ret < 0, SetErr(-ret, "rm error"), return -1);

        if (OsIsContainersWildcard(fullpath)) {
            ret = OsWildcardExtractDirectory(fullpath, NULL, RM_RECURSIVER);
        } else {
            ret = OsShellCmdDoRmdir(fullpath);
        }
    } else {
        filename = argv[0];
        ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
        ERROR_OUT_IF(ret < 0, SetErr(-ret, "rm error"), return -1);

        if (OsIsContainersWildcard(fullpath)) {
            ret = OsWildcardExtractDirectory(fullpath, NULL, RM_FILE);
        } else {
            ret = unlink(fullpath);
        }
    }
    if (ret == -1) {
        perror("rm error");
    }
    free(fullpath);
    return 0;
}

int OsShellCmdRmdir(int argc, const char **argv)
{
    int  ret;
    char *fullpath = NULL;
    const char *filename = NULL;
    char *shellWorkingDirectory = OsShellGetWorkingDirtectory();
    if (shellWorkingDirectory == NULL) {
        return -1;
    }

    ERROR_OUT_IF(argc == 0, PRINTK("rmdir [DIRECTORY]\n"), return -1);

    filename = argv[0];
    ret = VfsNormalizePath(shellWorkingDirectory, filename, &fullpath);
    ERROR_OUT_IF(ret < 0, SetErr(-ret, "rmdir error"), return -1);

    if (OsIsContainersWildcard(fullpath)) {
        ret = OsWildcardExtractDirectory(fullpath, NULL, RM_DIR);
    } else {
        ret = rmdir(fullpath);
    }
    if (ret == -1) {
        PRINTK("rmdir %s failed. Error: %s.\n", fullpath, strerror(errno));
    }
    free(fullpath);

    return 0;
}

