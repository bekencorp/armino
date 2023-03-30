#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#if (!CONFIG_SLAVE_CORE)
#include <components/shell_task.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
#include <modules/pm.h>
#include "test_fatfs.h"
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif

static char cFileName[50];

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))


#if (CONFIG_SDCARD)
static void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size)
{
	uint32_t i = 0;

	for (i = 0; i < size; i++) {
		write_data((dst+i), get_addr_data(src+i));

	}
}

static void bk_lcd_qspi_read_files(uint32_t *psram_addr)
{
	uint32_t once_read_len = 1024 * 64;
	uint32 uiTemp = 0;
	FIL file;
	FRESULT fr;

	uint8_t *sram_addr = NULL;
	uint32_t *paddr  = NULL;
	paddr = psram_addr;

	sram_addr = os_malloc(once_read_len);
	if (sram_addr == NULL) {
		os_printf("sram mem malloc failed!\r\n");
		return;
	}
	char *ucRdTemp = (char *)sram_addr;

	fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail. fr = %d\r\n", cFileName, fr);
		return;
	}

	while(1) {
		fr = f_read(&file, ucRdTemp, once_read_len, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}

		if (uiTemp == 0) {
//				os_printf("read file complete.\r\n");
			break;
		}

		if(once_read_len != uiTemp) {
			if (uiTemp % 4) {
				uiTemp = (uiTemp / 4 + 1) * 4;
			}
			memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp / 4);
		} else {
			memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len / 4);
			paddr += (once_read_len / 4);
		}
	}
	os_free(sram_addr);

	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", cFileName);
		return;
	}
}
#endif

void user_app_main(void){
	qspi_config_t lcd_qspi_config = {0};
	BK_LOG_ON_ERR(bk_qspi_driver_init());

	lcd_qspi_config.src_clk = 5;
	lcd_qspi_config.src_clk_div = 4;
	lcd_qspi_config.clk_div = 1;
	BK_LOG_ON_ERR(bk_qspi_init(&lcd_qspi_config));
	rtos_delay_milliseconds(5);

	test_mount(DISK_NUMBER_SDIO_SD);
	rtos_delay_milliseconds(5);

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);

	uint32_t psram_addr = 0x60000000;

	lcd_qspi_device_t *device_config = NULL;
	device_config = bk_lcd_qspi_get_device_by_name("sh8601a");
	bk_lcd_qspi_init(device_config);

	while (1) {
		sprintf(cFileName, "%d:/%s.rgb", DISK_NUMBER_SDIO_SD, "beken_logo");
		bk_lcd_qspi_read_files((uint32_t *)psram_addr);
		bk_lcd_qspi_send_data(device_config, (uint32_t *)psram_addr, device_config->frame_len);
		rtos_delay_milliseconds(2000);

		for (uint8_t k = 1; k < 14; k++) {
			sprintf(cFileName, "%d:/%d.rgb", DISK_NUMBER_SDIO_SD, k);
			bk_lcd_qspi_read_files((uint32_t *)(psram_addr + 0x97000 * (k - 1)));
		}

		for (uint8_t i = 0; i < 30; i++) {
			for (uint8_t j = 1; j < 14; j++) {
				bk_lcd_qspi_send_data(device_config, (uint32_t *)(psram_addr + 0x97000 * (j - 1)), device_config->frame_len);
			}
		}
		rtos_delay_milliseconds(2);
	}
}
#endif

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();

	return 0;
}