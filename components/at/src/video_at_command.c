#include "at_common.h"
#include <driver/jpeg_enc.h>
#include <driver/i2c.h>
#include <driver/dma.h>
#include <components/video_transfer.h>
#include <driver/psram.h>
#include <driver/dvp_camera.h>
#if CONFIG_MEDIA && CONFIG_LCD
#include "media_app.h"
#include "lcd_act.h"
#endif


#if CONFIG_PSRAM
int video_read_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_psram_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#endif

#if CONFIG_CAMERA
static beken_semaphore_t video_at_cmd_sema = NULL;
static uint8_t jpeg_isr_cnt = 20;

int video_set_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_close_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#endif //CONFIG_CAMERA

#if CONFIG_MEDIA && CONFIG_LCD

int video_lcd_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_mailbox_check_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#endif

const at_command_t video_at_cmd_table[] = {
#if CONFIG_PSRAM
	{0, "PSRAMREAD", 0, "psram write/read", video_read_psram_handler},
	{1, "PSRAMENABLE", 0, "init/deinit psram", video_psram_enable_handler},
#endif
#if CONFIG_CAMERA
	{3, "SETYUV", 0, "set jpeg/yuv mode and to psram", video_set_yuv_psram_handler},
	{4, "CLOSEYUV", 0, "close jpeg", video_close_yuv_psram_handler},
#endif

#if CONFIG_MEDIA && CONFIG_LCD
	{5, "LCDENABLE", 0, "enable/close:1/0", video_lcd_enable_handler},
	{5, "MAILBOX", 0, "NULL", video_mailbox_check_handler},
#endif
};

int video_at_cmd_cnt(void)
{
	return sizeof(video_at_cmd_table) / sizeof(video_at_cmd_table[0]);
}


#if CONFIG_PSRAM
int video_read_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint8_t i = 0;
	uint32_t value = 0;

	uint32_t psram = 0x60000000;
	if (argc != 0) {
		AT_LOGE("input param error\n");
		err = kParamErr;
		goto error;
	}

	err = bk_psram_init();
	if (err != kNoErr) {
		AT_LOGE("psram init error\n");
		err = kParamErr;
		goto error;
	}

	for (i = 0; i < 32; i++) {
		*((uint32 *)psram + i * 4) = i;
	}

	AT_LOGI("data:\n");
	for (i = 0; i < 32; i++) {
		value = *((uint32_t *)psram + i * 4);
		AT_LOGI("%d ", value);
		if (value != i)
		{
			AT_LOGE("psram compare error!\r\n");
			goto error;
		}
		
	}

	AT_LOGI("\n");

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}


int video_psram_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	if (argc != 1) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	if (os_strcmp(argv[0], "1") == 0)
	{
		err = bk_psram_init();
	}
	else if (os_strcmp(argv[0], "0") == 0)
	{
		err = bk_psram_deinit();
	}
	else
	{
		AT_LOGE("Input cmd param1 error!\r\n");
		goto error;
	}

	if (err != kNoErr) {
		AT_LOGE("psram fail\n");
		err = kParamErr;
		goto error;
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif // CONFIG_PSRAM

#if CONFIG_CAMERA
static void end_of_jpeg_frame(jpeg_unit_t id, void *param)
{
	if (jpeg_isr_cnt)
		jpeg_isr_cnt--;
	else
	{
		if (video_at_cmd_sema != NULL)
			rtos_set_semaphore(&video_at_cmd_sema);
	}
}

static void end_of_yuv_frame(jpeg_unit_t id, void *param)
{
	if (jpeg_isr_cnt)
		jpeg_isr_cnt--;
	else
	{
		if (video_at_cmd_sema != NULL)
			rtos_set_semaphore(&video_at_cmd_sema);
	}
}

int video_set_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint8_t yuv_mode = JPEG_ENC_MODE;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	const dvp_sensor_config_t *sensor = NULL;

	if (argc != 1) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	if (os_strcmp(argv[0], "1") == 0)
	{
		yuv_mode = JPEG_YUV_MODE;
	}
	else if (os_strcmp(argv[0], "0") == 0)
	{
		yuv_mode = JPEG_ENC_MODE;
	}
	else
	{
		AT_LOGE("Input cmd param1 error!\r\n");
		goto error;
	}

	if (yuv_mode == JPEG_YUV_MODE)
	{

#if (!CONFIG_PSRAM)
		AT_LOGE("NOT have psram, jpeg encode not support yuv mode");
		err = kParamErr;
		goto error;
#else
		err = bk_psram_init();
		if (err != kNoErr) {
			AT_LOGE("psram init error\n");
			err = kParamErr;
			goto error;
		}
#endif
	}

	bk_dvp_camera_power_enable(1);

#if CONFIG_SOC_BK7256XX
	bk_jpeg_enc_driver_init();
#endif

	// step 2: enable jpeg mclk for i2c communicate with dvp
	bk_jpeg_enc_mclk_enable();

	rtos_delay_milliseconds(5);

	// step 3: init i2c
	i2c_config.baud_rate = I2C_BAUD_RATE_100KHZ;
	i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
	bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);

	sensor = bk_dvp_get_sensor_auto_detect();
	if (sensor == NULL)
	{
		AT_LOGE("NOT find camera\r\n");
		err = kParamErr;
		return err;
	}

	jpeg_config.mode = yuv_mode;
	jpeg_config.x_pixel = ppi_to_pixel_x(sensor->def_ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(sensor->def_ppi) / 8;
	jpeg_config.vsync = sensor->vsync;
	jpeg_config.hsync = sensor->hsync;
	jpeg_config.clk = sensor->clk;


	err = bk_jpeg_enc_init(&jpeg_config);
	if (err != kNoErr) {
		AT_LOGE("jpeg init error\n");
		err = kParamErr;
		goto error;
	}

	bk_jpeg_enc_register_isr(JPEG_EOF, end_of_jpeg_frame, NULL);
	bk_jpeg_enc_register_isr(JPEG_EOY, end_of_yuv_frame, NULL);

	err = rtos_init_semaphore(&video_at_cmd_sema, 1);
	if(err != kNoErr){
		goto error;
	}

	sensor->init();
	sensor->set_ppi(sensor->def_ppi);
	sensor->set_fps(sensor->def_fps);

	bk_jpeg_enc_set_gpio(JPEG_ENABLE_DATA);

	err = rtos_get_semaphore(&video_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
	if(err != kNoErr){
		goto error;
	}

	rtos_deinit_semaphore(&video_at_cmd_sema);
	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

	return kNoErr;

error:
	if (video_at_cmd_sema)
		rtos_deinit_semaphore(&video_at_cmd_sema);
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int video_close_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	err = bk_jpeg_enc_deinit();
	if (err != kNoErr) {
		os_printf("jpeg deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("jpeg deinit ok!\n");

	err = bk_i2c_deinit(CONFIG_CAMERA_I2C_ID);
	if (err != kNoErr) {
		os_printf("i2c deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("I2c deinit ok!\n");

#if CONFIG_SYSTEM_CTRL
	err = bk_jpeg_enc_driver_deinit();
	if (err != kNoErr) {
		os_printf("video deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("video deinit ok!\n");
#endif

#if CONFIG_PSRAM
	err = bk_psram_deinit();
	if (err != kNoErr) {
		os_printf("psram deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("psram deinit ok!\n");
#endif

	bk_dvp_camera_power_enable(0);

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	jpeg_isr_cnt = 20;
	return kNoErr;

error:
	bk_dvp_camera_power_enable(0);
	jpeg_isr_cnt = 20;
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif // CONFIG_CAMERA


#if (CONFIG_MEDIA && CONFIG_LCD)

int video_lcd_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int ret = kNoErr;
	char *name = "NULL";

	if (argc != 1)
	{
		AT_LOGE("Input cmd param error!\r\n");
		goto error;
	}

	if (os_strcmp(argv[0], "1") == 0)
	{
		lcd_open_t lcd_open;
		lcd_open.device_ppi = PPI_480X272;
		lcd_open.device_name = name;
		ret = media_app_lcd_open(&lcd_open);
	}
	else if (os_strcmp(argv[0], "0") == 0)
	{
		ret = media_app_lcd_close();
	}
	else
	{
		AT_LOGE("Input cmd param1 error!\r\n");
		goto error;
	}

	if (ret != kNoErr)
	{
		AT_LOGE("cmd responed error!\r\n");
		goto error;
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return ret;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return ret;
}

int video_mailbox_check_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int ret = kNoErr;

	if (argc > 0)
	{
		AT_LOGE("Input cmd param error!\r\n");
		goto error;
	}

	ret = media_app_mailbox_test();

	if (ret != kNoErr)
	{
		AT_LOGE("cmd responed error!\r\n");
		goto error;
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return ret;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return ret;
}

#endif
