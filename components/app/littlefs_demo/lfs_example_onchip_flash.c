#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lfs.h"
#include <driver/flash_types.h>
#include <driver/flash.h>
#include "cli.h"


#define LFS_TEST_LOG  os_printf

#define LITTLEFS_FLASH_ADDR		0x3DA000
int user_provided_block_device_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size)
{
	bk_flash_read_bytes(c->block_size*block+off+LITTLEFS_FLASH_ADDR,buffer,size);

	return 0;
}

// Program a region in a block. The block must have previously
// been erased. Negative error codes are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int user_provided_block_device_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size)
{
	flash_protect_type_t type = bk_flash_get_protect_type();

	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	bk_flash_write_bytes(c->block_size*block+off+LITTLEFS_FLASH_ADDR,buffer,size);

	bk_flash_set_protect_type(type);

	return 0;
}   

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int user_provided_block_device_erase(const struct lfs_config *c, lfs_block_t block)
{
	flash_protect_type_t type = bk_flash_get_protect_type();

	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	bk_flash_erase_sector(c->block_size*block+LITTLEFS_FLASH_ADDR);

	bk_flash_set_protect_type(type);

	return 0;
}	

// Sync the state of the underlying block device. Negative error codes
// are propogated to the user.
int user_provided_block_device_sync(const struct lfs_config *c)
{
	return 0;
}	

// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = user_provided_block_device_read,
    .prog  = user_provided_block_device_prog,
    .erase = user_provided_block_device_erase,
    .sync  = user_provided_block_device_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 16,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};

static lfs_t lfs = {0};
static lfs_file_t file = {0};
static void cli_lfs_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);
	if(err)
	{
		LFS_TEST_LOG("lfs_mount fail ret:%d\n",err);
	}

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) 
	{
        lfs_format(&lfs, &cfg);
		LFS_TEST_LOG("lfs format\r\n");
        err = lfs_mount(&lfs, &cfg);
    }
	if(!err)
	{
		LFS_TEST_LOG("lfs mount OK!\n");
	}
	else
	{
		LFS_TEST_LOG("lfs mount fail!\n");
		goto exit;
	}
	
	lfs_file_open(&lfs, &file, "test.txt", LFS_O_RDWR | LFS_O_CREAT);

	char cbuffer[256] = {0};
	lfs_file_read(&lfs, &file, cbuffer, sizeof(cbuffer)-1);
	LFS_TEST_LOG("%s\n", cbuffer);

	strcpy(cbuffer," w lfs test!");
	lfs_file_write(&lfs, &file, cbuffer, strlen(cbuffer));
	lfs_file_sync(&lfs, &file);

	memset(cbuffer,0,sizeof(cbuffer));
	lfs_file_rewind(&lfs, &file);
	lfs_file_read(&lfs, &file, cbuffer, sizeof(cbuffer)-1);
	LFS_TEST_LOG("\r\n%s\n", cbuffer);

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // release any resources we were using
    lfs_unmount(&lfs);

exit:
	return;
}

#define FLASH_CMD_CNT (sizeof(s_lfs_commands) / sizeof(struct cli_command))
static const struct cli_command s_lfs_commands[] = {
	{"lfs", "", cli_lfs_cmd},
};

int cli_lfs_init(void)
{
	return cli_register_commands(s_lfs_commands, FLASH_CMD_CNT);
}

