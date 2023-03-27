#include <stdlib.h>
#include "cli.h"
#include "driver/flash.h"
#include <driver/flash_partition.h>
#include "sys_driver.h"
#include "flash_bypass.h"
#include "flash.h"
#include <driver/psram.h>

#define  TICK_PER_US    26
beken_thread_t idle_read_flash_handle = NULL;
beken_thread_t idle_read_psram_handle = NULL;
extern void delay_ms(UINT32 ms_count);

static bk_err_t test_flash_write(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t i;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	uint32_t tmp = addr + length;

	for (i = 0; i < 256; i++)
		buf[i] = i;

	for (; addr < tmp; addr += 256) {
		os_printf("write addr(size:256):%d\r\n", addr);
		bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
	}

	return kNoErr;
}

static bk_err_t test_flash_erase(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t addr = start_addr;
	uint32_t length = len;
	uint32_t tmp = addr + length;

	for (; addr < tmp; addr += 0x1000) {
		os_printf("erase addr:%d\r\n", addr);
		bk_flash_erase_sector(addr);
	}
	return kNoErr;
}

static bk_err_t test_flash_read(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t i, j, tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	tmp = addr + length;

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
		os_printf("read addr:%x\r\n", addr);
		for (i = 0; i < 16; i++) {
			for (j = 0; j < 16; j++)
				os_printf("%02x ", buf[i * 16 + j]);
			os_printf("\r\n");
		}
	}

	return kNoErr;
}

static bk_err_t test_flash_read_without_print(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	tmp = addr + length;

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}

	return kNoErr;
}

static bk_err_t test_flash_read_time(volatile uint32_t start_addr, uint32_t len)
{
	UINT32 time_start, time_end;
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;

	tmp = addr + length;
	beken_time_get_time((beken_time_t *)&time_start);
	os_printf("read time start:%d\r\n", time_start);

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}
	beken_time_get_time((beken_time_t *)&time_end);
	os_printf("read time end:%d\r\n", time_end);
	os_printf("cost time:%d\r\n", time_end - time_start);

	return kNoErr;
}

#if (CONFIG_SYSTEM_CTRL)
static bk_err_t test_flash_count_time(volatile uint32_t start_addr, uint32_t len, uint32_t test_times)
{
	UINT32 time_start, time_end;
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	extern u64 riscv_get_mtimer(void);
	uint64_t start_tick, end_tick, tick_cnt = 0;

	tmp = addr + length;
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);

	beken_time_get_time((beken_time_t *)&time_start);
	os_printf("----- FLASH COUNT TIME TEST BEGIN: start time:%d ms  -----\r\n", time_start);
	os_printf("===============================\r\n");

	start_tick = riscv_get_mtimer();
	for (addr = start_addr; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[read 1 time] >>>>> cost tick time: %d us.\r\n", (uint32_t) (tick_cnt / TICK_PER_US));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (int i = 0; i < test_times; i++ ) {
		for (addr = start_addr; addr < tmp; addr += 256) {
			os_memset(buf, 0, 256);
			bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
		}
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[read %d time] >>>>> average tick time: %d us.\r\n", test_times, (uint32_t) (tick_cnt / TICK_PER_US / test_times));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (addr = start_addr; addr < tmp; addr += 0x1000) {
		os_printf("erase addr:%d\r\n", addr);
		bk_flash_erase_sector(addr);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[erase 1 time] >>>>> cost tick time: %d us.\r\n", (uint32_t) (tick_cnt / TICK_PER_US));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (int i = 0; i < test_times; i++ ) {
		for (addr = start_addr; addr < tmp; addr += 0x1000) {
			bk_flash_erase_sector(addr);
		}
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[erase %d time] >>>>> average tick time: %d us.\r\n", test_times, (uint32_t) (tick_cnt / TICK_PER_US / test_times));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (int i = 0; i < 256; i++)
		buf[i] = i;

	for (addr = start_addr; addr < tmp; addr += 256) {
		bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[write 1 time] >>>>> cost tick time: %d us.\r\n", (uint32_t) (tick_cnt / TICK_PER_US));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (int i = 0; i < test_times; i++ ) {
		for (addr = start_addr; addr < tmp; addr += 256) {
			bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
		}
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[write %d time] >>>>> average tick time: %d us.\r\n", test_times, (uint32_t) (tick_cnt / TICK_PER_US / test_times));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[enable security 1 time] >>>>> cost tick time: %d us.\r\n", (uint32_t)(tick_cnt / TICK_PER_US));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	for (int i = 0; i < test_times; i++ ) {
		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[enable security %d time] >>>>> average tick time: %d us.\r\n", test_times, (uint32_t)(tick_cnt / TICK_PER_US / test_times));
	os_printf("===============================\r\n");


	start_tick = riscv_get_mtimer();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	os_printf("[en/dis security 1 time] >>>>> cost tick time: %d us.\r\n", (uint32_t) ((tick_cnt / TICK_PER_US)));
	os_printf("===============================\r\n");


	beken_time_get_time((beken_time_t *)&time_end);
	os_printf("----- FLASH COUNT TIME TEST END: end time:%d ms  -----\r\n", time_end);
	os_printf("----- OVERALL TEST TIME:%d ms  -----\r\n", time_end - time_start);

	bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
	return kNoErr;
}
#endif

static void test_idle_read_flash(void *arg) {
	while (1) {
		test_flash_read_without_print(0x1000, 1000);
		test_flash_read_without_print(0x100000, 1000);
		test_flash_read_without_print(0x200000, 1000);
		test_flash_read_without_print(0x300000, 0x1000);
	}
	rtos_delete_thread(&idle_read_flash_handle);
}

#define write_data(addr,val)                 	*((volatile unsigned long *)(addr)) = val
#define read_data(addr,val)                  	val = *((volatile unsigned long *)(addr))
#define get_addr_data(addr)			*((volatile unsigned long *)(addr))

#if (CONFIG_SYSTEM_CTRL && CONFIG_PSRAM)
static void test_idle_read_psram(void *arg) {
	uint32_t i,val = 0;
	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	os_printf("enter test_idle_read_psram\r\n");

	bk_psram_init();

	while (1) {
		for(i=0;i<1024;i++){
			write_data((0x60000000+i*0x4),0x11+i);
			write_data((0x60001000+i*0x4),0x22+i);
			write_data((0x60002000+i*0x4),0x33+i);
			write_data((0x60003000+i*0x4),0x44+i);
		}
		for(i=0;i<1024;i++){
			write_data((0x60004000+i*0x4),0x55+i);
			write_data((0x60005000+i*0x4),0x66+i);
			write_data((0x60006000+i*0x4),0x77+i);
			write_data((0x60007000+i*0x4),0x88+i);
		}
		for(i=0;i<4*1024;i++){
			val = get_addr_data(0x60000000+i*0x4);
			s0 += val;
		}
		for(i=0;i<4*1024;i++){
			val = get_addr_data(0x60004000+i*0x4);
			s1 += val;
		}
		for(i=0;i<4*1024;i++){
			val = get_addr_data(0x60000000+i*0x4);
			s2 += val;
		}
		for(i=0;i<4*1024;i++){
			val = get_addr_data(0x60004000+i*0x4);
			s3 += val;
		}
	
	}
	rtos_delete_thread(&idle_read_psram_handle);
}
#endif
static void flash_command_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char cmd = 0;
	uint32_t len = 0;
	uint32_t addr = 0;
#if (CONFIG_SYSTEM_CTRL)
	if (os_strcmp(argv[1], "config") == 0) {
		uint32_t flash_src_clk = os_strtoul(argv[2], NULL, 10);
		uint32_t flash_div_clk = os_strtoul(argv[3], NULL, 10);
		uint32_t flash_line_mode = os_strtoul(argv[4], NULL, 10);

		if (FLASH_CLK_XTAL == flash_src_clk) {
			sys_drv_flash_cksel(flash_src_clk);
		}

		if((FLASH_CLK_XTAL != sys_drv_flash_get_clk_sel()) && (0 == flash_div_clk)) {
			os_printf("Config fail. Please set src clk as 26M, or set div larger than 0 firstly.\n");
			return;
		}

		sys_drv_flash_set_clk_div(flash_div_clk);
		sys_drv_flash_cksel(flash_src_clk);
		bk_flash_set_line_mode(flash_line_mode);
		os_printf("flash_src_clk = %u. [0 -> 26M; 1->98M; 2-> 120M]\n", flash_src_clk);
		os_printf("flash_div_clk = %u. \n", flash_div_clk);
		os_printf("flash_line_mode = %u.  \n", flash_line_mode);

		return;
	}
	if (os_strcmp(argv[1], "qe") == 0) {
		uint32_t param = 0;
		uint32_t quad_enable = os_strtoul(argv[2], NULL, 10);
		uint32_t delay_cycle1 = os_strtoul(argv[3], NULL, 10);
		uint32_t delay_cycle2 = os_strtoul(argv[4], NULL, 10);

		for(int i = 0; i< 20; i++) {
			bk_flash_set_line_mode(2);
			flash_bypass_quad_test(quad_enable, delay_cycle1, delay_cycle2);
			while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
			param = bk_flash_read_status_reg();
			if (quad_enable) {
				if(param & 0x200){
					break;
				} else {
					os_printf("retry quad test, i = %d, flash status: 0x%x.\n", i, param);
				}
			} else {
				if(param & 0x200){
					os_printf("retry quad test, i = %d, flash status: 0x%x.\n", i, param);
				} else {
					break;
				}
			}
		}

		if (quad_enable) {
			if(param & 0x200){
				os_printf("flash quad enable success, flash status: 0x%x.\n", param);
			} else {
				os_printf("flash quad enable fail, flash status: 0x%x.\n", param);
			}
		} else {
			if(param & 0x200){
				os_printf("flash quad disable fail, flash status: 0x%x.\n", param);
			} else {
				os_printf("flash quad disable success, flash status: 0x%x.\n", param);
			}
		}
		return;
	}
#endif
	if (os_strcmp(argv[1], "idle_read_start") == 0) {
		uint32_t task_prio = os_strtoul(argv[2], NULL, 10);
		os_printf("idle_read_flash task start: task_prio = %u.\n", task_prio);
		rtos_create_thread(&idle_read_flash_handle, task_prio,
			"idle_read_flash",
			(beken_thread_function_t) test_idle_read_flash,
			CONFIG_APP_MAIN_TASK_STACK_SIZE,
			(beken_thread_arg_t)0);

		return;
	} else if (os_strcmp(argv[1], "idle_read_stop") == 0) {
		if (idle_read_flash_handle) {
			rtos_delete_thread(&idle_read_flash_handle);
			idle_read_flash_handle = NULL;
			os_printf("idle_read_flash task stop\n");
		} 
		return;
	}

#if (CONFIG_SYSTEM_CTRL && CONFIG_PSRAM)
	if (os_strcmp(argv[1], "idle_read_psram_start") == 0) {
		uint32_t task_prio = os_strtoul(argv[2], NULL, 10);
		os_printf("idle_read_psram task start: task_prio = %u.\n", task_prio);
		rtos_create_thread(&idle_read_psram_handle, task_prio,
			"idle_read_psram",
			(beken_thread_function_t) test_idle_read_psram,
			CONFIG_APP_MAIN_TASK_STACK_SIZE,
			(beken_thread_arg_t)0);

		return;
	} else if (os_strcmp(argv[1], "idle_read_psram_stop") == 0) {
		if (idle_read_psram_handle) {
			rtos_delete_thread(&idle_read_psram_handle);
			idle_read_psram_handle = NULL;
			os_printf("idle_read_psram task stop\n");
		} 
		return;
	}
#endif

	if (os_strcmp(argv[1], "U") == 0) {
		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		return;
	} else if (os_strcmp(argv[1], "P") == 0) {
		bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
		return;
	} else if (os_strcmp(argv[1], "RSR") == 0) {
		uint16_t sts_val = bk_flash_read_status_reg();
		os_printf("read sts_val = 0x%x\n", sts_val);
		return;
	} else if (os_strcmp(argv[1], "WSR") == 0) {
		uint16_t sts_val = os_strtoul(argv[2], NULL, 16);
		bk_flash_write_status_reg(sts_val);
		return;
	} else if (os_strcmp(argv[1], "C") == 0) {
#if (CONFIG_SYSTEM_CTRL)
		addr = atoi(argv[2]);
		len = atoi(argv[3]);
		uint32_t test_times = os_strtoul(argv[4], NULL, 10);
		test_flash_count_time(addr, len, test_times);
#endif
		return;
	}

	if (argc == 4) {
		cmd = argv[1][0];
		addr = atoi(argv[2]);
		len = atoi(argv[3]);

		switch (cmd) {
		case 'E':
			bk_flash_set_protect_type(FLASH_PROTECT_NONE);
			test_flash_erase(addr, len);
			bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			break;

		case 'R':
			test_flash_read(addr, len);
			break;
		case 'W':
			bk_flash_set_protect_type(FLASH_PROTECT_NONE);
			test_flash_write(addr, len);
			bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			break;
		//to check whether protection mechanism can work
		case 'N':
			test_flash_erase(addr, len);
			break;
		case 'M':
			test_flash_write(addr, len);
			break;
		case 'T':
			test_flash_read_time(addr, len);
			break;
		default:
			break;
		}
	} else
		os_printf("FLASH <R/W/E/M/N/T> <start_addr> <len>\r\n");
}


static void partShow_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_partition_t i;
	bk_logic_partition_t *partition;

	for (i = BK_PARTITION_BOOTLOADER; i <= BK_PARTITION_MAX; i++) {
		partition = bk_flash_partition_get_info(i);
		if (partition == NULL)
			continue;

		os_printf("%4d | %11s |  Dev:%d  | 0x%08lx | 0x%08lx |\r\n", i,
				  partition->partition_description, partition->partition_owner,
				  partition->partition_start_addr, partition->partition_length);
	};

}

#define FLASH_CMD_CNT (sizeof(s_flash_commands) / sizeof(struct cli_command))
static const struct cli_command s_flash_commands[] = {
	{"fmap_test",    "flash_test memory map",      partShow_Command},
	{"flash_test",   "flash_test <cmd(R/W/E/N)>", flash_command_test},
};

int cli_flash_test_init(void)
{
	return cli_register_commands(s_flash_commands, FLASH_CMD_CNT);
}
