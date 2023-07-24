#include <common/bk_include.h>
#include "bk_fake_clock.h"
#include "bk_icu.h"
#include "bk_drv_model.h"
#include "bk_uart.h"

#include "sys_rtos.h"
#include "bk_arm_arch.h"
#include "sys_ctrl.h"
#include "bk_sys_ctrl.h"

#include "reset_reason.h"
#include <components/log.h>

#define TAG "init"
#define DISPLAY_START_TYPE_STR 1



static uint32_t s_start_type;
static uint32_t s_misc_value_save;
static uint32_t s_mem_value_save;

uint32_t bk_misc_get_reset_reason(void)
{
	return s_start_type;
}

void persist_memory_init(void)
{
	*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)) = (uint32_t)CRASH_ILLEGAL_JUMP_VALUE;
}

uint32_t persist_memory_get(void)
{
	return (*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)));
}

bool persist_memory_is_lost(void)
{
	if ((uint32_t)CRASH_ILLEGAL_JUMP_VALUE == persist_memory_get())
		return false;
	else
		return true;
}

static char *misc_get_start_type_str(uint32_t start_type)
{
#if DISPLAY_START_TYPE_STR
	switch (start_type) {
	case RESET_SOURCE_POWERON:
		return "power on";

	case RESET_SOURCE_REBOOT:
		return "software reboot";

	case RESET_SOURCE_WATCHDOG:
		return "interrupt watchdog";

	case RESET_SOURCE_DEEPPS_GPIO:
		return "deep sleep gpio";

	case RESET_SOURCE_DEEPPS_RTC:
		return "deep sleep rtc";

	case RESET_SOURCE_DEEPPS_TOUCH:
		return "deep sleep touch";

	case RESET_SOURCE_CRASH_ILLEGAL_JUMP:
		return "illegal jump";

	case RESET_SOURCE_CRASH_UNDEFINED:
		return "undefined";

	case RESET_SOURCE_CRASH_PREFETCH_ABORT:
		return "prefetch abort";

	case RESET_SOURCE_CRASH_DATA_ABORT:
		return "data abort";

	case RESET_SOURCE_CRASH_UNUSED:
		return "unused";

	case RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION:
		return "illegal instruction";

	case RESET_SOURCE_CRASH_MISALIGNED:
		return "misaligned";

	case RESET_SOURCE_CRASH_ASSERT:
		return "assert";

	case RESET_SOURCE_DEEPPS_USB:
		return "deep sleep usb";

	case RESET_SOURCE_UNKNOWN:
	default:
		// Chip power on the value of start address may not always be 0
		// return "unknown";
		return "power on";
	}
#else
	return "";
#endif
}

void show_reset_reason(void)
{
	BK_LOGI(TAG, "reason - %s\r\n", misc_get_start_type_str(s_start_type));
	#if CONFIG_DEEP_PS
	if(RESET_SOURCE_DEEPPS_GPIO == s_start_type)
	{
		BK_LOGI(TAG, "by gpio - %d\r\n", bk_misc_wakeup_get_gpio_num());
	}
	#endif
	BK_LOGI(TAG, "regs - %x, %x, %x\r\n", s_start_type, s_misc_value_save, s_mem_value_save);
}

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A)
//only can be do once
uint32_t reset_reason_init(void)
{
	uint32_t misc_value;
	sctrl_ctrl(CMD_GET_SCTRL_RETETION, &misc_value);

	if ((s_start_type = sctrl_get_deep_sleep_wake_soure()) == 0) {
		if (0 == (misc_value & SW_RETENTION_WDT_FLAG)) {
			if (persist_memory_is_lost())
				s_start_type = RESET_SOURCE_POWERON;
			else
				s_start_type = RESET_SOURCE_CRASH_ILLEGAL_JUMP;
		} else {
			switch (misc_value & SW_RETENTION_VAL_MASK) {
			case (RESET_SOURCE_REBOOT & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_REBOOT;
				break;
			case (CRASH_UNDEFINED_VALUE & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_CRASH_UNDEFINED;
				break;
			case (CRASH_PREFETCH_ABORT_VALUE & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_CRASH_PREFETCH_ABORT;
				break;
			case (CRASH_DATA_ABORT_VALUE & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_CRASH_DATA_ABORT;
				break;
			case (CRASH_UNUSED_VALUE & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_CRASH_UNUSED;
				break;
			case (RESET_SOURCE_WATCHDOG & SW_RETENTION_VAL_MASK):
				s_start_type = RESET_SOURCE_WATCHDOG;
				break;
			default:
				s_start_type = RESET_SOURCE_UNKNOWN;
				break;
			}
		}
	}

	s_misc_value_save = misc_value;
	s_mem_value_save = persist_memory_get();
	persist_memory_init();

	//clear
	sctrl_ctrl(CMD_SET_SCTRL_RETETION, &misc_value);

	return s_start_type;
}

void bk_misc_set_reset_reason(uint32_t type)
{
	uint32_t misc_value = type & SW_RETENTION_VAL_MASK;
	sctrl_ctrl(CMD_SET_SCTRL_RETETION, &misc_value);
}

#elif (CONFIG_SOC_BK7256XX)

uint32_t reset_reason_init(void) {
#if (!CONFIG_SLAVE_CORE)
	uint32_t misc_value = REG_READ(START_TYPE_ADDR) >> 1;

	if (misc_value == (POWERON_INIT_MEM_TAG >> 1)) {
		s_start_type = RESET_SOURCE_POWERON;
	} else {
		s_start_type = misc_value;
	}

	s_misc_value_save = misc_value;
	s_mem_value_save = persist_memory_get();
	persist_memory_init();

#endif

	return s_start_type;
}

void bk_misc_set_reset_reason(uint32_t type)
{
#if (!CONFIG_SLAVE_CORE)
	uint32_t misc_value = (type << 1) | REG_GET_BIT(START_TYPE_ADDR, 0x1);
	REG_WRITE(START_TYPE_ADDR, misc_value);
#endif
}

#elif CONFIG_SOC_BK7236XX
uint32_t reset_reason_init(void) {
	//wangzhilei TODO
	return s_start_type;
}

void bk_misc_set_reset_reason(uint32_t type)
{
	//wangzhilei TODO
}

#else
//only can be do once
uint32_t reset_reason_init(void)
{
	uint32_t misc_value = *((volatile uint32_t *)(START_TYPE_ADDR));

#if (!CONFIG_SOC_BK7271)
	if ((s_start_type = sctrl_get_deep_sleep_wake_soure()) == 0)
#else
	BK_LOGI(TAG, "reset_reason_init TODO\r\n");
#endif
	{
		switch (misc_value) {
		case RESET_SOURCE_REBOOT:
			s_start_type = misc_value;
			break;
		case CRASH_UNDEFINED_VALUE:
			s_start_type = RESET_SOURCE_CRASH_UNDEFINED;
			break;
		case CRASH_PREFETCH_ABORT_VALUE:
			s_start_type = RESET_SOURCE_CRASH_PREFETCH_ABORT;
			break;
		case CRASH_DATA_ABORT_VALUE:
			s_start_type = RESET_SOURCE_CRASH_DATA_ABORT;
			break;
		case CRASH_UNUSED_VALUE:
			s_start_type = RESET_SOURCE_CRASH_UNUSED;
			break;
		case CRASH_ILLEGAL_JUMP_VALUE:
			s_start_type = RESET_SOURCE_CRASH_ILLEGAL_JUMP;
			break;
		case RESET_SOURCE_WATCHDOG:
			if (persist_memory_is_lost())
				s_start_type = RESET_SOURCE_WATCHDOG;
			else
				s_start_type = RESET_SOURCE_CRASH_ILLEGAL_JUMP;
			break;

		default:
			if (persist_memory_is_lost())
				s_start_type = RESET_SOURCE_POWERON;
			else
				s_start_type = RESET_SOURCE_CRASH_ILLEGAL_JUMP;
			break;
		}
	}

	s_misc_value_save = misc_value;
	s_mem_value_save = persist_memory_get();
	persist_memory_init();

	return s_start_type;
}

void bk_misc_set_reset_reason(uint32_t type)
{
	*((volatile uint32_t *)(START_TYPE_ADDR)) = (uint32_t)type;
}
#endif
