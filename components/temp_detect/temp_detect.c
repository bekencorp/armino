#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "bk_misc.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "bk_saradc.h"
#include "bk_uart.h"

#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "bk_fake_clock.h"

#include "bk_phy.h"
#include "temp_detect_pub.h"
#include "temp_detect.h"
#include "volt_detect.h"
#include "bk_ps.h"
#include "bk_wifi_private.h"
#include "bk_pm_control.h"
#include <components/log.h>
#if CONFIG_SDMADC_TEMP
#include <driver/sdmadc.h>
#else
#include <driver/adc.h>
#endif
#include "drv_model.h"
#include "sys_driver.h"
#if CONFIG_FLASH_ORIGIN_API
#include "flash.h"
#endif
#include <modules/pm.h>

#define CFG_USE_TEMPERATURE_DETECT                 1
#define CFG_SUPPORT_SARADC                         1
#define CFG_SDMADC_OFFSET                          32768
//TODO - more optimization for temp detect
//1. Keep temperature detection related work here, such as detection task, config etc.
//2. Move temp sensor and adc related work to driver, better implenented temp sensor
//3. Define public temperature API, such as bk_tempsensor_get_temperature()
//   and temp detect module depends on tempsensor!

extern void manual_cal_tmp_pwr_init(uint16_t init_temp, uint16_t init_threshold,
		uint16_t init_dist);
saradc_desc_t tmp_detect_desc;
temp_detect_config_t g_temp_detect_config;

#if CONFIG_SDMADC_TEMP
static int16_t *s_raw_temperature_data = NULL;
#else
static uint16_t *s_raw_temperature_data = NULL;
#endif

//TODO
//Both temp detect and get_temperature api share same raw buffer,
//but we don't protect!
static int tempd_init_temperature_raw_data(void)
{
	if (!s_raw_temperature_data) {
		#if CONFIG_SDMADC_TEMP
		s_raw_temperature_data = (int16_t *)os_zalloc(ADC_TEMP_BUFFER_SIZE *
					sizeof(s_raw_temperature_data[0]));
		#else
		s_raw_temperature_data = (uint16_t *)os_zalloc(ADC_TEMP_BUFFER_SIZE *
					sizeof(s_raw_temperature_data[0]));
		#endif
	}

	if (!s_raw_temperature_data) {
		TEMPD_LOGE("oom\r\n");
		return BK_ERR_NO_MEM;
	}

	return BK_OK;
}

//TODO better to put to driver
static void temp_sensor_enable(void)
{

#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236)
    sys_drv_en_tempdet(1);
#else
    uint32_t param;

    param = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
#endif
    
}

static void temp_sensor_disable(void)
{
#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236)
    sys_drv_en_tempdet(0);
#else

    uint32_t param;
    param = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_DISABLE, &param);
#endif
}

#if TEMPD_DISPLAY_RAW_DATA
static void tempd_show_raw_temperature_data(void)
{
	TEMPD_LOGI("raw data ");
	for (int i = 0; i < ADC_TEMP_BUFFER_SIZE; i++) {
		BK_LOG_RAW("%04x ", s_raw_temperature_data[i]);
	}
	BK_LOG_RAW("\n");
}
#else
#define tempd_show_raw_temperature_data()
#endif

static uint16_t tempd_calculate_temperature(void)
{
	tempd_show_raw_temperature_data();

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7236)
	uint32_t sum = 0, index, count = 0;

	for (index = 5; index < ADC_TEMP_BUFFER_SIZE; index++) {
		/* 0 is invalid, but saradc may return 0 in power save mode */
		if ((0 != s_raw_temperature_data[index]) && (2048 != s_raw_temperature_data[index])) {
			#if CONFIG_SDMADC_TEMP
			sum += s_raw_temperature_data[index] + CFG_SDMADC_OFFSET; //offset half of 2^16 to be positive
			#else
			sum += s_raw_temperature_data[index];
			#endif
			count++;
		}
	}

	if (count == 0)
		s_raw_temperature_data[0] = 0;
	else {
		sum = sum / count;
		sum = sum / 4;
		s_raw_temperature_data[0] = sum;
	}
#elif (CONFIG_SOC_BK7256XX)
	uint32_t sum = 0, sum1, sum2;

	sum1 = s_raw_temperature_data[1] + s_raw_temperature_data[2];
	sum2 = s_raw_temperature_data[3] + s_raw_temperature_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	sum = sum / 16;
	s_raw_temperature_data[0] = sum;

#else
	uint32_t sum = 0, sum1, sum2;

	sum1 = s_raw_temperature_data[1] + s_raw_temperature_data[2];
	sum2 = s_raw_temperature_data[3] + s_raw_temperature_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	sum = sum / 4;
	s_raw_temperature_data[0] = sum;
#endif

	return s_raw_temperature_data[0];
}

#if CONFIG_SDMADC_TEMP
static bk_err_t tempd_adc_get_raw_data(adc_chan_t adc_chan)
{
	sdmadc_config_t config = {0};

	temp_sensor_enable();

	int err = BK_OK;
	err = bk_sdmadc_driver_init();
	if (BK_OK != err)
		goto _release_adc;

	err = bk_sdmadc_init();
	if (BK_OK != err)
		goto _release_adc;

	config.samp_mode = 0x0;
	config.samp_numb = 0x0;
	config.samp_chan = adc_chan;
	config.comp_bpss = 0x1;
	config.cic2_bpss = 0x1;
	config.cic2_gain = 0x2d;
	config.int_enable = 0x8;
	config.cali_offset = 0x0;
	config.cali_gains  = 0x1000;

	err = bk_sdmadc_set_cfg(&config);
	if (BK_OK != err)
		goto _release_adc;

	err = bk_sdmadc_read_raw(s_raw_temperature_data, ADC_TEMP_BUFFER_SIZE);

_release_adc:
	temp_sensor_disable();
	bk_sdmadc_deinit();
	bk_sdmadc_driver_deinit();
	return err;
}
#else
static int tempd_adc_get_raw_data(adc_chan_t adc_chan)
{
	adc_config_t config = {0};
	int err = BK_OK;

	BK_RETURN_ON_ERR(bk_adc_acquire());

	temp_sensor_enable();

	err = bk_adc_init(adc_chan);
	if (BK_OK != err)
		goto _release_adc;

	config.chan = adc_chan;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.clk = TEMP_DETEC_ADC_CLK;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.saturate_mode = ADC_TEMP_SATURATE_MODE;
	config.sample_rate = TEMP_DETEC_ADC_SAMPLE_RATE;
	config.steady_ctrl= TEMP_DETEC_ADC_STEADY_CTRL;
	config.adc_filter = 0;

	err = bk_adc_set_config(&config);
	if (BK_OK != err)
		goto _release_adc;

	err = bk_adc_enable_bypass_clalibration();
	if (BK_OK != err)
		goto _release_adc;

	err = bk_adc_start();
	if (BK_OK != err)
		goto _release_adc;

	int length = bk_adc_read_raw(s_raw_temperature_data, ADC_TEMP_BUFFER_SIZE,
				ADC_READ_SEMAPHORE_WAIT_TIME);
	if (length == 0) {
		err = BK_ERR_TEMPD_SAMPLE_NO_DATA;
		goto _release_adc;
	}

_release_adc:
	//TODO check it, do we need to always enable temperature sensor?
	temp_sensor_disable();
    bk_adc_stop();
	bk_adc_deinit(adc_chan);
	bk_adc_release();
	return err;
}
#endif

static int tempd_adc_get_temperature(uint16_t *temperature)
{
	int err;

	err = tempd_adc_get_raw_data(ADC_TEMP_SENSOR_CHANNEL);
	if (BK_OK != err)
		return err;

	*temperature = tempd_calculate_temperature();
	return BK_OK;
}

#if CONFIG_TEMP_DETECT
static temp_detect_config_t s_tempd = {0};
static beken_thread_t  s_tempd_task_handle = NULL;
static beken_queue_t s_tempd_msg_queue = NULL;

static void tempd_main(beken_thread_arg_t data);
extern void rwnx_cal_do_temp_detect(uint16_t cur_val, uint16_t threshold, uint16_t *last);

int temp_detect_send_msg(uint32_t msg_type)
{
	bk_err_t ret = BK_FAIL;
	tempd_msg_t msg;

	TEMPD_LOGD("send msg(%d)\n", msg_type);
	if (s_tempd_msg_queue) {
		msg.temp_msg = msg_type;

		ret = rtos_push_to_queue(&s_tempd_msg_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			TEMPD_LOGE("send msg failed ret:%x,m_t:%x\r\n", ret, msg_type);
	}

	return (int)ret;
}

int temp_detect_init(uint32_t init_temperature)
{
	int ret;

	TEMPD_LOGI("init temperature %d\r\n", init_temperature);
	BK_RETURN_ON_ERR(tempd_init_temperature_raw_data());

	if ((!s_tempd_task_handle) && (!s_tempd_msg_queue)) {

		ret = rtos_init_queue(&s_tempd_msg_queue,
				"tempd_q",
				sizeof(tempd_msg_t),
				TEMPD_QUEUE_LEN);
		if (BK_OK != ret) {
			TEMPD_LOGE("ceate Q failed(%d)\r\n", ret);
			return ret;
		}

		ret = rtos_create_thread(&s_tempd_task_handle,
			TEMPD_TASK_PRIO,
			"tempd",
			(beken_thread_function_t)tempd_main,
			TEMPD_TASK_STACK_SIZE,
			(beken_thread_arg_t)init_temperature);
		if (BK_OK != ret) {
			rtos_deinit_queue(&s_tempd_msg_queue);
			s_tempd_msg_queue = NULL;
			TEMPD_LOGE("create task failed(%d)\r\n", ret);
			return ret;
		}
	}

	return BK_OK;
}

//TODO by frank
int temp_detect_deinit(void)
{
	if ((s_tempd_task_handle) && (s_tempd_msg_queue)) {
		//temp_detect_send_msg(TMPD_DEINIT);

		// wait untill task exit
		//while(s_tempd_task_handle)
		//    rtos_delay_milliseconds(100);

		// set reg mod & pa to initial value, this must be happened in
		// txevm or rxsens to calibration txpwr or rxsens
		manual_cal_temp_pwr_unint();
	}

	return 0;
}

int temp_detect_stop(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
    if (s_tempd.detect_oneshot_timer.function
        && rtos_is_oneshot_timer_running(&s_tempd.detect_oneshot_timer))
        temp_detect_send_msg(TMPD_PAUSE_TIMER);
#else
    if (s_tempd.detect_timer.function
		&& rtos_is_timer_running(&s_tempd.detect_timer))
		temp_detect_send_msg(TMPD_PAUSE_TIMER);
#endif
	return BK_OK;
}

int temp_detect_start(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
	if (s_tempd.detect_oneshot_timer.function &&
		!rtos_is_oneshot_timer_running(&s_tempd.detect_oneshot_timer))
		temp_detect_send_msg(TMPD_RESTART_TIMER);
#else
	if (s_tempd.detect_timer.function &&
		!rtos_is_timer_running(&s_tempd.detect_timer))
		temp_detect_send_msg(TMPD_RESTART_TIMER);
#endif
	return BK_OK;
}

bool temp_detect_is_init(void)
{
	return !!((s_tempd_task_handle) && (s_tempd_msg_queue));
}

#if TEMP_DETECT_ONESHOT_TIMER
static void temp_detect_oneshot_timer_handler(void *data1, void *data2)
{
    temp_detect_send_msg(TMPD_TIMER_EXPIRED);
	temp_detect_send_msg(TMPD_RESTART_TIMER);
}
#else
static void temp_detect_timer_handler(void *data)
{
	temp_detect_send_msg(TMPD_TIMER_EXPIRED);
}
#endif

static void tempd_notify_temperature_to_calibration(uint16_t temperature)
{
	uint16_t threshold = s_tempd.detect_threshold;

#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236) || (!CONFIG_STA_PS)
	rwnx_cal_do_temp_detect(temperature, threshold, &s_tempd.last_detect_val);
#else
	ps_switch(PS_UNALLOW, PS_EVENT_TEMP, PM_RF_BIT);
	rwnx_cal_do_temp_detect(temperature, threshold, &s_tempd.last_detect_val);
	ps_switch(PS_ALLOW, PS_EVENT_TEMP, PM_RF_BIT);
#endif
}

static void tempd_stop(void)
{
	int err;
    
#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_stop_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_stop_timer(&s_tempd.detect_timer);
#endif

	BK_ASSERT(kNoErr == err);
	TEMPD_LOGD("stop\n");
}

static void tempd_restart(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_oneshot_reload_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_reload_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
	TEMPD_LOGD("restart\n");
}

static void tempd_change_config(void)
{
	temp_detect_change_configuration(ADC_TMEP_DETECT_INTERVAL,
		s_tempd.detect_threshold, ADC_TMEP_DIST_INTIAL_VAL);
}

static bool tempd_need_change_detect_interval(uint32_t detect_cnt)
{
	uint32_t detect_change_cnt = ADC_TMEP_DETECT_INTERVAL_CHANGE/
					ADC_TMEP_DETECT_INTERVAL_INIT;

	return (detect_cnt == detect_change_cnt);
}

static void tempd_detect_temperature(void)
{
	uint16_t temperature = 0;
	int err = BK_OK;
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 0,0);
	s_tempd.detect_cnt++;
	tempd_stop();

	err = tempd_adc_get_temperature(&temperature);
	if (BK_OK != err) {
		TEMPD_LOGW("detect failed(%d), retry\n", err);
		tempd_restart();
        bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);
		return; //TODO is that correct?
	}

	TEMPD_LOGD("cnt=%d, interval=%d, last=%d, cur=%d, thr=%d\r\n",
		s_tempd.detect_cnt, s_tempd.detect_interval, s_tempd.last_detect_val,
		temperature, s_tempd.detect_threshold);

	tempd_notify_temperature_to_calibration(temperature);

	if (tempd_need_change_detect_interval(s_tempd.detect_cnt))
		temp_detect_send_msg(TMPD_CHANGE_PARAM);
	else
		tempd_restart();

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);
}

static void tempd_init(uint32_t init_temperature)
{
	int err;

	s_tempd.last_detect_val = (uint32_t)(init_temperature);
	s_tempd.inital_data = (uint32_t)(init_temperature) + ADC_TMEP_DIST_INTIAL_VAL;
	s_tempd.detect_threshold = ADC_TMEP_LSB_PER_10DEGREE * ADC_TMEP_10DEGREE_PER_DBPWR;
	s_tempd.detect_interval = ADC_TMEP_DETECT_INTERVAL_INIT;
	s_tempd.detect_cnt = 0;
	s_tempd.dist_inital = ADC_TMEP_DIST_INTIAL_VAL;

	s_tempd.last_xtal_val = (uint32_t)(init_temperature);
	s_tempd.xtal_threshold_val = ADC_XTAL_DIST_INTIAL_VAL;

    #if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236)
    s_tempd.xtal_init_val = sys_drv_analog_get_xtalh_ctune();// to do,need remove old interface after all adaption is finished
    #else
    s_tempd.xtal_init_val = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_GET_XTALH_CTUNE, NULL);
    #endif

	TEMPD_LOGI("xtal inital:%d, %d, %d\r\n", s_tempd.last_xtal_val,
			  s_tempd.xtal_threshold_val, s_tempd.xtal_init_val);

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_init_oneshot_timer(&s_tempd.detect_oneshot_timer,
						  s_tempd.detect_interval * 1000,
						  temp_detect_oneshot_timer_handler,
						  (void *)0,
						  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
	err = rtos_init_timer(&s_tempd.detect_timer,
					  s_tempd.detect_interval * 1000,
					  temp_detect_timer_handler,
					  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
}

static void tempd_deinit(void)
{
	int err;
	
#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_deinit_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_deinit_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);

	rtos_deinit_queue(&s_tempd_msg_queue);
	s_tempd_msg_queue = NULL;
	s_tempd_task_handle = NULL;
	rtos_delete_thread(NULL);
}

static void tempd_main(beken_thread_arg_t data)
{
	int err;

	tempd_init((uint32_t)data);
#if CONFIG_VOLT_DETECT
	volt_daemon_init();
#endif

	while (1) {
		tempd_msg_t msg;

		err = rtos_pop_from_queue(&s_tempd_msg_queue, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.temp_msg) {
			case TMPD_PAUSE_TIMER:
				tempd_stop();
				break;

			case TMPD_RESTART_TIMER:
				tempd_restart();
				break;

			case TMPD_CHANGE_PARAM:
				tempd_change_config();
				break;

			case TMPD_TIMER_EXPIRED:
				tempd_detect_temperature();
				break;
#if CONFIG_VOLT_DETECT
			case VOLT_PAUSE_TIMER:
				volt_daemon_stop();
				break;

			case VOLT_RESTART_TIMER:
				volt_daemon_restart();
				break;

			case VOLT_TIMER_EXPIRED:
				volt_daemon_polling_handler();
				break;
#endif
			case TMPD_DEINIT:
				goto tempd_exit;

			default:
				break;
			}
		}
	}

tempd_exit:
	tempd_deinit();
#if CONFIG_VOLT_DETECT
	volt_daemon_deinit();
#endif
}

void temp_detect_change_configuration(uint32_t interval, uint32_t threshold, uint32_t dist)
{
	bk_err_t err;

	if (interval == 0)
		interval = ADC_TMEP_DETECT_INTERVAL;

	if (threshold == 0)
		threshold = ADC_TMEP_LSB_PER_10DEGREE * ADC_TMEP_10DEGREE_PER_DBPWR;

	if (dist == 0)
		dist = ADC_TMEP_DIST_INTIAL_VAL;

	TEMPD_LOGI("change config, interval=%d, threshold=%d, dist=%d\r\n",
		interval, threshold, dist);

	if ((s_tempd.detect_threshold != threshold) || (s_tempd.dist_inital != dist)) {
		s_tempd.detect_threshold = threshold;
		s_tempd.dist_inital = dist;

		manual_cal_tmp_pwr_init(s_tempd.inital_data, s_tempd.detect_threshold,
						s_tempd.dist_inital);
	}

	if (s_tempd.detect_interval != interval) {
		s_tempd.detect_interval = interval;
#if TEMP_DETECT_ONESHOT_TIMER
        if (s_tempd.detect_oneshot_timer.function) {
    		err = rtos_deinit_oneshot_timer(&s_tempd.detect_oneshot_timer);
    		BK_ASSERT(kNoErr == err);
    	}
        err = rtos_init_oneshot_timer(&s_tempd.detect_oneshot_timer,
                                  s_tempd.detect_interval * 1000,
                                  temp_detect_oneshot_timer_handler,
                                  (void *)0,
                                  (void *)0);
        BK_ASSERT(kNoErr == err);
        err = rtos_start_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
		if (s_tempd.detect_timer.function) {
			err = rtos_deinit_timer(&s_tempd.detect_timer);
			BK_ASSERT(kNoErr == err);
		}

		err = rtos_init_timer(&s_tempd.detect_timer,
							  s_tempd.detect_interval * 1000,
							  temp_detect_timer_handler,
							  (void *)0);
		BK_ASSERT(kNoErr == err);

		err = rtos_start_timer(&s_tempd.detect_timer);
#endif
		BK_ASSERT(kNoErr == err);
	}
}

#endif  // CONFIG_TEMP_DETECT

//TODO why only check the temperature range here?
//why NOT in temp_detect? Frank check it!!!
static bool tempd_is_temperature_valid(uint16_t temperature)
{
	return ((temperature > ADC_TEMP_VAL_MIN) && (temperature < ADC_TEMP_VAL_MAX));
}

int temp_detect_get_temperature(uint32_t *temperature)
{
	uint16_t cur_temperature = 0;
	int err = BK_OK;

	if (!temperature)
		return BK_ERR_NULL_PARAM;

	BK_RETURN_ON_ERR(tempd_init_temperature_raw_data());

	for (int i = 0; i < TEMPD_MAX_RETRY_NUM; i++) {
		err = tempd_adc_get_temperature(&cur_temperature);
		if (BK_OK != err) {
			TEMPD_LOGW("get temperature failed(%d), retry\n", err);
			continue;
		}

		if (tempd_is_temperature_valid(cur_temperature))
			break;
		err = BK_ERR_TRY_AGAIN;
	}

	*temperature = cur_temperature;
	TEMPD_LOGI("get temperature %d\n", cur_temperature);
	return err;
}
