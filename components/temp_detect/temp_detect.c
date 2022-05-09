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
#include "bk_ps.h"
#include "bk_wifi_private.h"
#include "bk_pm_control.h"
#include <components/log.h>
#include <driver/adc.h>
#include "drv_model.h"
#include "sys_driver.h"
#include "flash.h"

#define CFG_USE_TEMPERATURE_DETECT                 1
#define CFG_SUPPORT_SARADC                         1
#define CFG_USE_VOLTAGE_DETECT                     0
//TODO - more optimization for temp detect
//1. Keep temperature detection related work here, such as detection task, config etc.
//2. Move temp sensor and adc related work to driver, better implenented temp sensor
//3. Define public temperature API, such as bk_tempsensor_get_temperature()
//   and temp detect module depends on tempsensor!

extern void manual_cal_tmp_pwr_init(uint16_t init_temp, uint16_t init_threshold,
		uint16_t init_dist);
volatile DD_HANDLE tmp_detect_hdl = DD_HANDLE_UNVALID;
volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;
saradc_desc_t tmp_single_desc;
saradc_desc_t tmp_detect_desc;
temp_detect_config_t g_temp_detect_config;
UINT16 tmp_single_buff[ADC_TEMP_BUFFER_SIZE];
beken_semaphore_t tmp_single_semaphore = NULL;

static uint16_t *s_raw_temperature_data = NULL;

//TODO
//Both temp detect and get_temperature api share same raw buffer,
//but we don't protect!
static int tempd_init_temperature_raw_data(void)
{
	if (!s_raw_temperature_data) {
		s_raw_temperature_data = (uint16_t *)os_zalloc(ADC_TEMP_BUFFER_SIZE *
					sizeof(s_raw_temperature_data[0]));
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

#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
    sys_drv_en_tempdet(1);
#else
    uint32_t param;

    param = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
#endif
    
}

static void temp_sensor_disable(void)
{
#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
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

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236)
	uint32_t sum = 0, index, count = 0;

	for (index = 5; index < ADC_TEMP_BUFFER_SIZE; index++) {
		/* 0 is invalid, but saradc may return 0 in power save mode */
		if ((0 != s_raw_temperature_data[index]) && (2048 != s_raw_temperature_data[index])) {
			sum += s_raw_temperature_data[index];
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
	sum = sum / 6;
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

static int tempd_adc_get_raw_data(void)
{
	adc_config_t config = {0};
	int err = BK_OK;

	BK_RETURN_ON_ERR(bk_adc_acquire());

	temp_sensor_enable();

	err = bk_adc_init(ADC_TEMP_SENSOR_CHANNEL);
	if (BK_OK != err)
		goto _release_adc;

	config.chan = ADC_TEMP_SENSOR_CHANNEL;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.clk = TEMP_DETEC_ADC_CLK;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.saturate_mode = ADC_TEMP_SATURATE_MODE;
	config.sample_rate = TEMP_DETEC_ADC_SAMPLE_RATE;
	config.steady_ctrl= TEMP_DETEC_ADC_STEADY_CTRL;
	config.adc_filter = 0;

#if (CONFIG_SOC_BK7256XX)
	sys_drv_set_ana_scal_en(0);
	sys_drv_set_ana_gadc_buf_ictrl(0xF);
	sys_drv_set_ana_gadc_cmp_ictrl(0x5);
	sys_drv_set_ana_pwd_gadc_buf(0);
	sys_drv_set_ana_vref_sel(0);
	bk_adc_enable_bypass_clalibration();
#endif

	
	err = bk_adc_set_config(&config);
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
	bk_adc_release();
	return err;
}

static int tempd_adc_get_temperature(uint16_t *temperature)
{
	int err;

	err = tempd_adc_get_raw_data();
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

void temp_detect_send_msg(uint32_t msg_type)
{
	bk_err_t ret;
	tempd_msg_t msg;

	TEMPD_LOGD("send msg(%d)\n", msg_type);
	if (s_tempd_msg_queue) {
		msg.temp_msg = msg_type;

		ret = rtos_push_to_queue(&s_tempd_msg_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			TEMPD_LOGE("send msg failed(%x)\r\n", ret);
	}
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
	if (s_tempd.detect_timer.function
		&& rtos_is_timer_running(&s_tempd.detect_timer))
		temp_detect_send_msg(TMPD_PAUSE_TIMER);

	return BK_OK;
}

int temp_detect_start(void)
{
	if (s_tempd.detect_timer.function &&
		!rtos_is_timer_running(&s_tempd.detect_timer))
		temp_detect_send_msg(TMPD_RESTART_TIMER);

	return BK_OK;
}

bool temp_detect_is_init(void)
{
	return !!((s_tempd_task_handle) && (s_tempd_msg_queue));
}

static void temp_detect_timer_handler(void *data)
{
	temp_detect_send_msg(TMPD_TIMER_EXPIRED);
}

static void tempd_notify_temperature_to_calibration(uint16_t temperature)
{
#if CONFIG_STA_PS
	uint16_t threshold = s_tempd.detect_threshold;
	ps_switch(PS_UNALLOW, PS_EVENT_TEMP, PM_RF_BIT);
	rwnx_cal_do_temp_detect(temperature, threshold, &s_tempd.last_detect_val);
	ps_switch(PS_ALLOW, PS_EVENT_TEMP, PM_RF_BIT);
#endif
}

static void tempd_stop(void)
{
	int err;
	err = rtos_stop_timer(&s_tempd.detect_timer);
	BK_ASSERT(kNoErr == err);
	TEMPD_LOGD("stop\n");
}

static void tempd_restart(void)
{
	int err;
	err = rtos_reload_timer(&s_tempd.detect_timer);
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

	s_tempd.detect_cnt++;
	tempd_stop();

	err = tempd_adc_get_temperature(&temperature);
	if (BK_OK != err) {
		TEMPD_LOGW("detect failed(%d), retry\n", err);
		tempd_restart();
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

    #if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
    s_tempd.xtal_init_val = sys_drv_analog_get_xtalh_ctune();// to do,need remove old interface after all adaption is finished
    #else
    s_tempd.xtal_init_val = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_GET_XTALH_CTUNE, NULL);
    #endif

	TEMPD_LOGI("xtal inital:%d, %d, %d\r\n", s_tempd.last_xtal_val,
			  s_tempd.xtal_threshold_val, s_tempd.xtal_init_val);

	err = rtos_init_timer(&s_tempd.detect_timer,
						  s_tempd.detect_interval * 1000,
						  temp_detect_timer_handler,
						  (void *)0);
	BK_ASSERT(kNoErr == err);
	err = rtos_start_timer(&s_tempd.detect_timer);
	BK_ASSERT(kNoErr == err);
}

static void tempd_deinit(void)
{
	int err;

	err = rtos_deinit_timer(&s_tempd.detect_timer);
	BK_ASSERT(kNoErr == err);

	rtos_deinit_queue(&s_tempd_msg_queue);
	s_tempd_msg_queue = NULL;
	s_tempd_task_handle = NULL;
	rtos_delete_thread(NULL);
}

#if CFG_USE_VOLTAGE_DETECT
static void volt_detect_polling_handler(void)
{
    int err;
    UINT16 cur_val;

    #if (!CONFIG_SOC_BK7231)
    cur_val = tmp_detect_desc.pData[0] * 4;
    #else
    cur_val = tmp_detect_desc.pData[ADC_TEMP_BUFFER_SIZE-1];
    #endif // (CFG_SOC_NAME != SOC_BK7231)

    ps_switch(PS_UNALLOW, PS_EVENT_TEMP, PM_RF_BIT);

    #if CFG_USE_STA_PS
    bk_wlan_dtim_rf_ps_mode_do_wakeup();
    #endif

    rwnx_cal_do_volt_detect(cur_val);
    ps_switch(PS_ALLOW, PS_EVENT_TEMP, PM_RF_BIT);

    err = rtos_reload_timer(&g_temp_detect_config.detect_timer);
    if (kNoErr != err)
    {
        TMP_DETECT_FATAL("volt_detect_polling_handler, restart detect timer failed\r\n");
    }
}
#endif

static void tempd_main(beken_thread_arg_t data)
{
	int err;

	tempd_init((uint32_t)data);

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
#if CFG_USE_VOLTAGE_DETECT
			case VOLT_INT_POLL:
				volt_detect_polling_handler();
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
		}

		if (tempd_is_temperature_valid(cur_temperature))
			break;
	}

	*temperature = cur_temperature;
	TEMPD_LOGI("get temperature %d\n", cur_temperature);
	return err;
}

#if (CONFIG_SOC_BK7256) ||(CONFIG_SOC_BK7236) ||(CONFIG_SOC_BK7231N)||(CONFIG_SOC_BK7235)||(CONFIG_SOC_BK7256_CP1)
static void temp_single_get_disable(void)
{
    UINT32 status = DRV_SUCCESS;

    status = ddev_close(tmp_single_hdl);
    if(DRV_FAILURE == status )
    {
        //TMP_DETECT_PRT("saradc disable failed\r\n");
        //return;
    }
    bk_adc_stop();
    tmp_single_hdl = DD_HANDLE_UNVALID;

    status = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_DISABLE, &status);

    TMP_DETECT_PRT("saradc_open is close \r\n");
}

static void temp_single_detect_handler(void)
{
    if(tmp_single_desc.current_sample_data_cnt >= tmp_single_desc.data_buff_size)
    {
#if (!CONFIG_SOC_BK7231)
        UINT32 sum = 0, sum1, sum2;
        //turnon_PA_in_temp_dect();
        temp_single_get_disable();
        TMP_DETECT_PRT("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) ||(CONFIG_SOC_BK7256)
        sum1 = tmp_single_desc.pData[6] + tmp_single_desc.pData[7];
        sum2 = tmp_single_desc.pData[8] + tmp_single_desc.pData[9];
        sum = sum1 / 2 + sum2 / 2;
#else
        sum1 = tmp_single_desc.pData[1] + tmp_single_desc.pData[2];
        sum2 = tmp_single_desc.pData[3] + tmp_single_desc.pData[4];
        sum = sum1 / 2 + sum2 / 2;
#endif
        sum = sum / 2;
        sum = sum / 4;

        tmp_single_desc.pData[0] = sum;
        #else
        turnon_PA_in_temp_dect();
        temp_single_get_disable();
        TMP_DETECT_PRT("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);
        #endif // (CFG_SOC_NAME != SOC_BK7231)

        rtos_set_semaphore(&tmp_single_semaphore);
    }
}

static void temp_single_get_desc_init(UINT8 channel)
{
    os_memset(&tmp_single_buff[0], 0, sizeof(UINT16)*ADC_TEMP_BUFFER_SIZE);

    saradc_config_param_init_for_temp(&tmp_single_desc);
    tmp_single_desc.channel = channel;
    tmp_single_desc.pData = &tmp_single_buff[0];
    tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
    tmp_single_desc.p_Int_Handler = temp_single_detect_handler;
}

static UINT32 temp_single_get_enable(UINT8 channel)
{
    UINT32 status;

#if CFG_USE_TEMPERATURE_DETECT
    while(tmp_detect_hdl != DD_HANDLE_UNVALID)
    {
        rtos_delay_milliseconds(10);
    }
#endif
    temp_single_get_desc_init(channel);

    status = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &status);

#if (CONFIG_SOC_BK7231)
    turnoff_PA_in_temp_dect();
#endif // (CFG_SOC_NAME == SOC_BK7231)

#if CFG_SUPPORT_SARADC
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    tmp_single_hdl = ddev_open(DD_DEV_TYPE_SARADC, &status, (UINT32)&tmp_single_desc);
    if(DD_HANDLE_UNVALID == tmp_single_hdl)
    {
        GLOBAL_INT_RESTORE();
        TMP_DETECT_FATAL("Can't open saradc, have you register this device?\r\n");
        return SARADC_FAILURE;
    }
    GLOBAL_INT_RESTORE();
#endif

    return SARADC_SUCCESS;
}

UINT32 volt_single_get_current_voltage(UINT32 *volt_value)
{
    UINT32 ret;
    int result;
    int retry_count = 3;

    *volt_value = 0;

    for (; retry_count > 0; retry_count--) {
        if(tmp_single_semaphore == NULL) {
            result = rtos_init_semaphore(&tmp_single_semaphore, 1);
            BK_ASSERT(kNoErr == result);
        }

        ret = temp_single_get_enable(ADC_VOLT_SENSER_CHANNEL);
        if (SARADC_SUCCESS != ret)
        {
            continue;
        }

        ret = 1000; // 1s
        result = rtos_get_semaphore(&tmp_single_semaphore, ret);
        if(result == kNoErr) {
            #if (!CONFIG_SOC_BK7231)
            *volt_value = tmp_single_desc.pData[0] * 4;
            #else
            *volt_value = tmp_single_desc.pData[4];
            #endif
            ret = 0;
        }else {
            TMP_DETECT_FATAL("volt_single timeout\r\n");
            ret = 1;
        }

        if ((ADC_TEMP_VAL_MIN < *volt_value) && (*volt_value < ADC_TEMP_VAL_MAX)) {
            break;
        }
    }

    return ret;
}
#endif