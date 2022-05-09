#include <common/bk_kernel_err.h>
#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "bk_gpio.h"
#include "bk_uart.h"
#include "bk_saradc.h"
#include "temp_detect_pub.h"
#include "temp_detect.h"
#include "sys_rtos.h"
#include <os/os.h>
#include "saradc_intf.h"
#include <os/mem.h>
#include "bk_fake_clock.h"
#include <components/system.h>
#include "bk_sys_ctrl.h"

#define TURING_ADC_TAG                      "saradc_intf"
#define TURING_ADC_TASK_PRIORITY            (18)
#define TURING_ADC_TASK_STACK_SIZE          (1024 * 1)
#define TURING_ADC_QITEM_COUNT              (3)

volatile uint8_t step_flag = 1;
volatile uint8_t adctest_flag = 1;
volatile uint8_t adc_accuracy = 0;

enum {
	TADC_TIMER_POLL          = 0,
	TADC_EXIT,
};

typedef struct tadc_message {
	UINT32 data;
} TADC_MSG_T;

typedef struct _tadc_entity_ {
	beken_thread_t tadc_thread;
	beken_queue_t msg_que;
	beken_timer_t check_timer;
	beken_semaphore_t sema_wait_end;

	ADC_OBJ *adc_obj_list;
	beken_mutex_t obj_list_mutex;

	DD_HANDLE adc_handle;
	saradc_desc_t adc_cfg;
	UINT16 adc_data[ADC_TEMP_BUFFER_SIZE];
} TADC_ENTITY_T;

void saradc_calculate_step1(void);
void saradc_calculate_step2(void);

static TADC_ENTITY_T *tadc_entity = NULL;

void adc_obj_init(ADC_OBJ *handle, adc_obj_callback cb, UINT32 channel, void *user_data)
{
	if (handle) {
		adc_obj_stop(handle);
		os_memset(handle, 0, sizeof(ADC_OBJ));
		handle->channel = channel;
		handle->user_data = user_data;
		handle->cb = cb;
	}
}

int adc_obj_start(ADC_OBJ *handle)
{
	ADC_OBJ *target;

	if (!tadc_entity || (!tadc_entity->obj_list_mutex)) {
		TADC_WARNING_PRINTF("adc obj start with no initial\r\n");
		return -1;
	}

	rtos_lock_mutex(&tadc_entity->obj_list_mutex);
	target = tadc_entity->adc_obj_list;
	while (target) {
		if (target == handle) {
			rtos_unlock_mutex(&tadc_entity->obj_list_mutex);
			return -1;	//already exist.
		}
		target = target->next;
	}
	handle->next = tadc_entity->adc_obj_list;
	tadc_entity->adc_obj_list = handle;
	rtos_unlock_mutex(&tadc_entity->obj_list_mutex);
	return 0;
}

void adc_obj_stop(ADC_OBJ *handle)
{
	ADC_OBJ **curr;

	rtos_lock_mutex(&tadc_entity->obj_list_mutex);
	for (curr = &tadc_entity->adc_obj_list; *curr;) {
		ADC_OBJ *entry = *curr;
		if (entry == handle)
			*curr = entry->next;
		else
			curr = &entry->next;
	}
	rtos_unlock_mutex(&tadc_entity->obj_list_mutex);
}

static void sadc_detect_handler(void)
{
	saradc_desc_t *p_ADC_drv_desc = &tadc_entity->adc_cfg;

	if (!p_ADC_drv_desc)
		return;

	if (p_ADC_drv_desc->current_sample_data_cnt >= p_ADC_drv_desc->data_buff_size) {
		UINT32 sum = 0, sum1, sum2;

		ddev_close(tadc_entity->adc_handle);
		tadc_entity->adc_handle = DD_HANDLE_UNVALID;
		saradc_ensure_close();

		sum1 = p_ADC_drv_desc->pData[1] + p_ADC_drv_desc->pData[2];
		sum2 = p_ADC_drv_desc->pData[3] + p_ADC_drv_desc->pData[4];
		sum = sum1 / 2 + sum2 / 2;
		sum = sum / 2;

		adc_accuracy = (uint8_t)saradc_check_accuracy();
		if (adc_accuracy != 0)
			sum = sum >> (adc_accuracy - 1);

		p_ADC_drv_desc->pData[0] = sum;

		rtos_set_semaphore(&tadc_entity->sema_wait_end);
	}
}

void tadc_obj_handler(ADC_OBJ *handle)
{
	saradc_desc_t *p_ADC_drv_desc = NULL;
	UINT32 status, ret;
	GLOBAL_INT_DECLARATION();

	p_ADC_drv_desc = &tadc_entity->adc_cfg;

	saradc_config_param_init(p_ADC_drv_desc);

	p_ADC_drv_desc->channel = handle->channel;
	p_ADC_drv_desc->data_buff_size          = ADC_TEMP_BUFFER_SIZE;
	p_ADC_drv_desc->current_read_data_cnt   = 0;
	p_ADC_drv_desc->current_sample_data_cnt = 0;
	p_ADC_drv_desc->has_data                = 0;
	p_ADC_drv_desc->pData                   = &tadc_entity->adc_data[0];
	os_memset(p_ADC_drv_desc->pData, 0x00, p_ADC_drv_desc->data_buff_size * sizeof(UINT16));

	p_ADC_drv_desc->p_Int_Handler           = sadc_detect_handler;

	ret = 0;
	do {
		GLOBAL_INT_DISABLE();
		if (saradc_check_busy() == 0) {
			tadc_entity->adc_handle = ddev_open(DD_DEV_TYPE_SARADC, &status, (UINT32)p_ADC_drv_desc);
			if (DD_HANDLE_UNVALID != tadc_entity->adc_handle) {
				GLOBAL_INT_RESTORE();
				break;
			}
		}
		GLOBAL_INT_RESTORE();
		rtos_delay_milliseconds(5);
		ret++;
	} while (ret < 5);

	if (ret == 5) {
		TADC_WARNING_PRINTF("adc timeout\r\n");
		return;
	}

	status = TURING_ADC_SCAN_INTERVALV;
	ret = rtos_get_semaphore(&tadc_entity->sema_wait_end, status * bk_get_ticks_per_second());
	if (step_flag == 0)
		saradc_calculate_step1();
	else
		saradc_calculate_step2();

	if (ret == kNoErr) {

		float voltage = 0.0;
		int mv;

		if (adctest_flag == 0)
			mv = p_ADC_drv_desc->pData[0];
		else {
			voltage = saradc_calculate(p_ADC_drv_desc->pData[0]);
			mv = voltage * 1000;
		}

		if (handle->cb)
			handle->cb(mv, handle->user_data);
	} else

		TADC_WARNING_PRINTF("sema_wait_end timeout:%d\r\n", status);
}

static void tadc_check_timer_poll_handle(void)
{
	ADC_OBJ *target;

	rtos_lock_mutex(&tadc_entity->obj_list_mutex);
	for (target = tadc_entity->adc_obj_list; target; target = target->next)
		tadc_obj_handler(target);
	rtos_unlock_mutex(&tadc_entity->obj_list_mutex);
}

void tadc_entity_deinit(TADC_ENTITY_T **adc_entity)
{
	TADC_WARNING_PRINTF("tadc_entity_deinit\r\n");
	if (*adc_entity) {
		if ((*adc_entity)->sema_wait_end) {
			rtos_deinit_semaphore(&((*adc_entity)->sema_wait_end));
			(*adc_entity)->sema_wait_end = NULL;
		}

		if ((*adc_entity)->obj_list_mutex) {
			rtos_deinit_mutex(&((*adc_entity)->obj_list_mutex));
			(*adc_entity)->obj_list_mutex = NULL;
		}

		os_free((*adc_entity));
		*adc_entity = NULL;
	}
}

TADC_ENTITY_T *tadc_entity_init(void)
{
	TADC_ENTITY_T *adc_entity = NULL;
	bk_err_t ret = 0;

	adc_entity = (TADC_ENTITY_T *) os_malloc(sizeof(TADC_ENTITY_T));
	if (adc_entity == NULL) {
		TADC_FATAL_PRINTF("malloc TADC_ENTITY_T failed\r\n");
		return  NULL;
	}

	os_memset(adc_entity, 0, sizeof(TADC_ENTITY_T));
	TADC_WARNING_PRINTF("tc_entity_init\r\n");

	ret = rtos_init_mutex(&adc_entity->obj_list_mutex);
	if (ret != 0) {
		TADC_FATAL_PRINTF("tc_entity->mutex error\r\n");
		tadc_entity_deinit(&adc_entity);
		return NULL;
	}

	ret = rtos_init_semaphore(&adc_entity->sema_wait_end, 1);
	if (ret != 0) {
		TADC_FATAL_PRINTF("tc_entity->sema_wait_end error\r\n");
		tadc_entity_deinit(&adc_entity);
		return NULL;
	}

	return adc_entity;
}

#if (!CONFIG_SOC_BK7231) && (!CONFIG_SOC_BK7231N) && (!CONFIG_SOC_BK7236) && (!CONFIG_SOC_BK7256XX)
/*
vol:	PSRAM_VDD_1_8V
		PSRAM_VDD_2_5V
		PSRAM_VDD_3_3V
*/
void saradc_config_vddram_voltage(UINT32 vol)
{
	UINT32 param;

	param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);

	param = vol;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_QSPI_VDDRAM_VOLTAGE, &param);
}

void saradc_disable_vddram_voltage(void)
{
	UINT32 param;

	param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_DISABLE, &param);

}
#endif

void tadc_send_msg(UINT32 new_msg)
{
	bk_err_t ret;
	TADC_MSG_T msg;

	if (tadc_entity->msg_que) {
		msg.data = new_msg;
		ret = rtos_push_to_queue(&tadc_entity->msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			TADC_PRINTF("tadc_send_msg failed\r\n");
	}
}

static void tadc_check_timer_callback(void *arg)
{
	UINT32 data = TADC_TIMER_POLL;
	tadc_send_msg(data);
}

static void tadc_thread_entry(beken_thread_arg_t data)
{
	bk_err_t err;

	TADC_WARNING_PRINTF("tadc_thread_entry\r\n");

	err = rtos_init_queue(&tadc_entity->msg_que,
						  "tadc_msg_queue",
						  sizeof(TADC_MSG_T),
						  TURING_ADC_QITEM_COUNT);
	if (kNoErr != err) {
		TADC_FATAL_PRINTF("tadc_entity->msg_que\r\n");
		goto  tadc_exit;
	}

	err = rtos_init_timer(&tadc_entity->check_timer,
						  TURING_ADC_SCAN_INTERVALV,
						  tadc_check_timer_callback,
						  (void *)0);
	BK_ASSERT(kNoErr == err);

	err = rtos_start_timer(&tadc_entity->check_timer);
	BK_ASSERT(kNoErr == err);

	while (1) {
		TADC_MSG_T msg;
		err = rtos_pop_from_queue(&tadc_entity->msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.data) {
			case TADC_TIMER_POLL: {
				tadc_check_timer_poll_handle();
				break;
			}
			case TADC_EXIT: {
				goto tadc_exit;
			}
			break;
			default:
				break;
			}
		}
	}

tadc_exit:
	if (tadc_entity->check_timer.handle) {
		err = rtos_deinit_timer(&tadc_entity->check_timer);
		BK_ASSERT(kNoErr == err);
	}

	if (tadc_entity->msg_que) {
		rtos_deinit_queue(&tadc_entity->msg_que);
		tadc_entity->msg_que = NULL;
	}

	tadc_entity_deinit(&tadc_entity);
	tadc_entity = NULL;
	rtos_delete_thread(NULL);
}

void saradc_work_create(void)
{
	bk_err_t result;

	TADC_WARNING_PRINTF("turing_adc_create\r\n");

	if (tadc_entity) {
		TADC_WARNING_PRINTF("aready turing_adc_create\r\n");
		return;
	}

	tadc_entity = tadc_entity_init();
	if (NULL == tadc_entity) {
		TADC_FATAL_PRINTF("turing_adc_create exceptional\r\n");
		goto create_exit;
	}

	result = rtos_create_thread(&tadc_entity->tadc_thread,
								TURING_ADC_TASK_PRIORITY,
								TURING_ADC_TAG,
								(beken_thread_function_t)tadc_thread_entry,
								TURING_ADC_TASK_STACK_SIZE,
								NULL);
	if (result != kNoErr) {
		TADC_FATAL_PRINTF("turing_adc_create exceptional\r\n");
		goto create_exit;
	}

	TADC_WARNING_PRINTF("turing_adc_create done\r\n");
	return;

create_exit:

	tadc_entity_deinit(&tadc_entity);
}

#if (CONFIG_SARADC_CALI && (CONFIG_RTT))
static void adc_check(int argc, char **argv)
{
	UINT32 status, ret;
	DD_HANDLE flash_handle;
	DD_HANDLE saradc_handle;
	saradc_cal_val_t p_ADC_cal;
	float voltage = 0.0;
	saradc_desc_t *p_ADC_drv_desc = NULL;
	GLOBAL_INT_DECLARATION();

	if (argc < 2)
		goto IDLE_CMD_ERR;

	if (0 == strcmp(argv[1], "read")) {
		status = manual_cal_load_adc_cali_flash();
		if (status != 0) {
			os_printf("Can't read cali value, use default!\r\n");
			os_printf("calibrate low value:[%x]\r\n", saradc_val.low);
			os_printf("calibrate high value:[%x]\r\n", saradc_val.high);
		}
	} else if (0 == strcmp(argv[1], "set")) {
		p_ADC_drv_desc = (saradc_desc_t *)malloc(sizeof(saradc_desc_t));
		if (p_ADC_drv_desc == NULL) {
			os_printf("malloc1 failed!\r\n");
			return;
		}

		os_memset(p_ADC_drv_desc, 0x00, sizeof(saradc_desc_t));
		saradc_config_param_init(p_ADC_drv_desc);

		p_ADC_drv_desc->channel = 1;
		p_ADC_drv_desc->data_buff_size = ADC_TEMP_BUFFER_SIZE;
		p_ADC_drv_desc->p_Int_Handler           = NULL;

		p_ADC_drv_desc->has_data                = 0;
		p_ADC_drv_desc->current_read_data_cnt   = 0;
		p_ADC_drv_desc->current_sample_data_cnt = 0;
		p_ADC_drv_desc->pData = (UINT16 *)malloc(p_ADC_drv_desc->data_buff_size * sizeof(UINT16));
		os_memset(p_ADC_drv_desc->pData, 0x00, p_ADC_drv_desc->data_buff_size * sizeof(UINT16));

		if (p_ADC_drv_desc->pData == NULL) {
			os_printf("malloc1 failed!\r\n");
			free(p_ADC_drv_desc);
			return;
		}

		ret = 0;
		do {
			GLOBAL_INT_DISABLE();
			if (saradc_check_busy() == 0) {
				saradc_handle = ddev_open(DD_DEV_TYPE_SARADC, &status, (UINT32)p_ADC_drv_desc);
				if (DD_HANDLE_UNVALID != saradc_handle) {
					GLOBAL_INT_RESTORE();
					break;
				}
			}
			GLOBAL_INT_RESTORE();

			rtos_delay_milliseconds(5);
			ret++;
		} while (ret < 5);

		if (ret == 5) {
			os_printf("saradc open failed!\r\n");
			free(p_ADC_drv_desc->pData);
			free(p_ADC_drv_desc);
			return;
		}

		while (1) {
			if (p_ADC_drv_desc->current_sample_data_cnt >=
				p_ADC_drv_desc->data_buff_size) {
				ddev_close(saradc_handle);
				saradc_ensure_close();
				break;
			}
		}

		{
			UINT32 sum = 0, sum1, sum2;
			UINT16 *pData = p_ADC_drv_desc->pData;
			sum1 = pData[1] + pData[2];
			sum2 = pData[3] + pData[4];
			sum = sum1 / 2  + sum2 / 2;
			sum = sum / 2;
			sum = sum / 4;
			p_ADC_drv_desc->pData[0] = sum;
		}

		if (0 == strcmp(argv[2], "low"))
			p_ADC_cal.mode = SARADC_CALIBRATE_LOW;
		else if (0 == strcmp(argv[2], "high"))
			p_ADC_cal.mode = SARADC_CALIBRATE_HIGH;
		else {
			os_printf("invalid parameter\r\n");
			return;
		}
		p_ADC_cal.val = p_ADC_drv_desc->pData[0];
		if (SARADC_FAILURE == ddev_control(saradc_handle, SARADC_CMD_SET_CAL_VAL, (VOID *)&p_ADC_cal)) {
			os_printf("set calibrate value failture\r\n");
			free(p_ADC_drv_desc->pData);
			free(p_ADC_drv_desc);
			return;
		}
		os_printf("set calibrate success\r\n");
		os_printf("type:[%s] value:[0x%x]\r\n", (p_ADC_cal.mode ? "high" : "low"), p_ADC_cal.val);
		free(p_ADC_drv_desc->pData);
		free(p_ADC_drv_desc);
	} else if (0 == strcmp(argv[1], "write")) {
		manual_cal_save_chipinfo_tab_to_flash();
		os_printf("calibrate low value:[%x]\r\n", saradc_val.low);
		os_printf("calibrate high value:[%x]\r\n", saradc_val.high);
	} else if (0 == strcmp(argv[1], "get")) {
		p_ADC_drv_desc = (saradc_desc_t *)malloc(sizeof(saradc_desc_t));
		if (p_ADC_drv_desc == NULL) {
			os_printf("malloc1 failed!\r\n");
			return;
		}

		os_memset(p_ADC_drv_desc, 0x00, sizeof(saradc_desc_t));
		saradc_config_param_init(p_ADC_drv_desc);
		p_ADC_drv_desc->p_Int_Handler           = NULL;

		p_ADC_drv_desc->channel = 1;
		if (argc > 2)
			p_ADC_drv_desc->channel = atoi(argv[2]);
		p_ADC_drv_desc->data_buff_size = ADC_TEMP_BUFFER_SIZE;
		p_ADC_drv_desc->has_data                = 0;
		p_ADC_drv_desc->current_read_data_cnt   = 0;
		p_ADC_drv_desc->current_sample_data_cnt = 0;
		p_ADC_drv_desc->pData = (UINT16 *)malloc(p_ADC_drv_desc->data_buff_size * sizeof(UINT16));
		os_memset(p_ADC_drv_desc->pData, 0x00, p_ADC_drv_desc->data_buff_size * sizeof(UINT16));

		if (p_ADC_drv_desc->pData == NULL) {
			os_printf("malloc1 failed!\r\n");
			free(p_ADC_drv_desc);
			return;
		}

		ret = 0;
		do {
			GLOBAL_INT_DISABLE();
			if (saradc_check_busy() == 0) {
				saradc_handle = ddev_open(DD_DEV_TYPE_SARADC, &status, (UINT32)p_ADC_drv_desc);
				if (DD_HANDLE_UNVALID != saradc_handle) {
					GLOBAL_INT_RESTORE();
					break;
				}
			}
			GLOBAL_INT_RESTORE();

			rtos_delay_milliseconds(5);
			ret++;
		} while (ret < 5);

		if (ret == 5) {
			os_printf("saradc open failed!\r\n");
			free(p_ADC_drv_desc->pData);
			free(p_ADC_drv_desc);
			return;
		}

		while (1) {
			if (p_ADC_drv_desc->current_sample_data_cnt >= p_ADC_drv_desc->data_buff_size) {
				ddev_close(saradc_handle);
				saradc_ensure_close();
				break;
			}
		}

		{
			UINT32 sum = 0, sum1, sum2;
			UINT16 *pData = p_ADC_drv_desc->pData;
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
			sum1 = pData[6] + pData[7];
			sum2 = pData[8] + pData[9];
#else
			sum1 = pData[1] + pData[2];
			sum2 = pData[3] + pData[4];
#endif
			sum = sum1 / 2 + sum2 / 2;
			sum = sum / 2;
			sum = sum / 4;
			p_ADC_drv_desc->pData[0] = sum;
		}

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
		os_printf("saradc[ch%d]=%d\r\n", (UINT32)p_ADC_drv_desc->channel, (UINT32)p_ADC_drv_desc->pData[0]);
#else
		voltage = saradc_calculate(p_ADC_drv_desc->pData[0]);
		os_printf("voltage is [%d] mv\r\n", (UINT32)(voltage * 1000));
#endif
		free(p_ADC_drv_desc->pData);
		free(p_ADC_drv_desc);
		return;
	} else
		goto IDLE_CMD_ERR;

	return;
IDLE_CMD_ERR:
	os_printf("Usage:ps [func] [param]\r\n");
}

MSH_CMD_EXPORT(adc_check, adc check);
#endif // (CONFIG_SARADC_CALI && (CONFIG_RTT))

static ADC_OBJ adc_test = {0};

void adc_detect_callback(int new_mv, void *user_data)
{
	ADC_OBJ *cfg = (ADC_OBJ *)user_data;

	TADC_WARNING_PRINTF("new:%d, %d\r\n", new_mv, cfg->channel);
}

void adc_detect_configuration(UINT32 channel)
{
	TADC_WARNING_PRINTF("adc_detect %d, %d\r\n", channel, adc_test.channel);

	if (channel == adc_test.channel)
		return;

	saradc_work_create();

	if (adc_test.cb)
		adc_obj_stop(&adc_test);

	adc_obj_init(&adc_test, adc_detect_callback, channel, &adc_test);
	adc_obj_start(&adc_test);
}

