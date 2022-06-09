#include <stdlib.h>
#include "cli.h"
#include "BkDriverFlash.h"
#include "sys_driver.h"
#include "flash_bypass.h"
#include "flash.h"
#include <driver/psram.h>

/*
format:  FLASH  E/R/W  0xABCD
example: FLASH  R  0x00100
*/

extern bk_err_t test_flash_write(volatile uint32_t start_addr, uint32_t len);
extern bk_err_t test_flash_erase(volatile uint32_t start_addr, uint32_t len);
extern bk_err_t test_flash_read(volatile uint32_t start_addr, uint32_t len);
extern bk_err_t test_flash_read_time(volatile uint32_t start_addr, uint32_t len);
extern void flash_set_line_mode(UINT8 mode);
extern bk_err_t test_flash_read_without_print(volatile uint32_t start_addr, uint32_t len);
beken_thread_t idle_read_flash_handle = NULL;
beken_thread_t idle_read_psram_handle = NULL;
extern void delay_ms(UINT32 ms_count);
extern UINT16 flash_read_sr(UINT8 sr_width);

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

	bk_psram_init(0x00054043);

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
		flash_set_line_mode(flash_line_mode);
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

		flash_set_line_mode(2);

		for(int i = 0; i< 20; i++) {
			flash_bypass_quad_test(quad_enable, delay_cycle1, delay_cycle2);
			while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
			param = flash_read_sr(2);
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

	if (argc == 4) {
		cmd = argv[1][0];
		addr = atoi(argv[2]);
		len = atoi(argv[3]);

		switch (cmd) {
		case 'E':
			bk_flash_enable_security(FLASH_PROTECT_NONE);
			test_flash_erase(addr, len);
			bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);
			break;

		case 'R':
			test_flash_read(addr, len);
			break;
		case 'W':
			bk_flash_enable_security(FLASH_PROTECT_NONE);
			test_flash_write(addr, len);
			bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);
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
		partition = bk_flash_get_info(i);
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
