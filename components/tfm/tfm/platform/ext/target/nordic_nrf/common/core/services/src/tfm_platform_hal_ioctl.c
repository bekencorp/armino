#include <platform/include/tfm_platform_system.h>
#include <cmsis.h>
#include <stdio.h>
#include <tfm_ioctl_core_api.h>
#include <string.h>
#include <arm_cmse.h>
#include <array.h>
#include <tfm_hal_isolation.h>
#include <tfm_memory_utils.h>

/* This contains the user provided allowed ranges */
#include <tfm_read_ranges.h>

#include <hal/nrf_gpio.h>

enum tfm_platform_err_t
tfm_platform_hal_read_service(const psa_invec  *in_vec,
			      const psa_outvec *out_vec)
{
	struct tfm_read_service_args_t *args;
	struct tfm_read_service_out_t *out;
	enum tfm_hal_status_t status;
	enum tfm_platform_err_t err;
	uint32_t attr = TFM_HAL_ACCESS_WRITABLE |
			TFM_HAL_ACCESS_READABLE |
			TFM_HAL_ACCESS_NS;

	if (in_vec->len != sizeof(struct tfm_read_service_args_t) ||
	    out_vec->len != sizeof(struct tfm_read_service_out_t)) {
		return TFM_PLATFORM_ERR_INVALID_PARAM;
	}

	args = (struct tfm_read_service_args_t *)in_vec->base;
	out = (struct tfm_read_service_out_t *)out_vec->base;

	/* Assume failure, unless valid region is hit in the loop */
	out->result = -1;
	err = TFM_PLATFORM_ERR_INVALID_PARAM;

	if (args->destination == NULL || args->len <= 0) {
		return TFM_PLATFORM_ERR_INVALID_PARAM;
	}

	status = tfm_hal_memory_has_access((uintptr_t)args->destination,
					    args->len,
					    attr);
	if (status != TFM_HAL_SUCCESS) {
		return TFM_PLATFORM_ERR_INVALID_PARAM;
	}

	for (size_t i = 0; i < ARRAY_SIZE(ranges); i++) {
		uint32_t start = ranges[i].start;
		uint32_t size = ranges[i].size;

		if (args->addr >= start &&
		    args->addr + args->len <= start + size) {
			tfm_memcpy(args->destination,
				   (const void *)args->addr,
				   args->len);
			out->result = 0;
			err = TFM_PLATFORM_ERR_SUCCESS;
			break;
		}
	}

	return err;
}

#if defined(GPIO_PIN_CNF_MCUSEL_Msk)
static bool valid_mcu_select(uint32_t mcu)
{
	switch (mcu) {
	case NRF_GPIO_PIN_MCUSEL_APP:
	case NRF_GPIO_PIN_MCUSEL_NETWORK:
	case NRF_GPIO_PIN_MCUSEL_PERIPHERAL:
	case NRF_GPIO_PIN_MCUSEL_TND:
		return true;
	default:
		return false;
	}
}

static uint32_t gpio_service_mcu_select(struct tfm_gpio_service_args * args)
{
	if (nrf_gpio_pin_present_check(args->mcu_select.pin_number) &&
	    valid_mcu_select(args->mcu_select.mcu)) {
		nrf_gpio_pin_mcu_select(args->mcu_select.pin_number, args->mcu_select.mcu);
		return 0;
	} else {
		return -1;
	}
}

enum tfm_platform_err_t
tfm_platform_hal_gpio_service(const psa_invec  *in_vec, const psa_outvec *out_vec)
{
	struct tfm_gpio_service_args *args;
	struct tfm_gpio_service_out *out;

	if (in_vec->len != sizeof(struct tfm_gpio_service_args) ||
	    out_vec->len != sizeof(struct tfm_gpio_service_out)) {
		return TFM_PLATFORM_ERR_INVALID_PARAM;
	}

	args = (struct tfm_gpio_service_args *)in_vec->base;
	out = (struct tfm_gpio_service_out *)out_vec->base;
	out->result = -1;

	switch(args->type)
	{
	case TFM_GPIO_SERVICE_TYPE_PIN_MCU_SELECT:
		out->result = gpio_service_mcu_select(args);
		break;
	default:
		return TFM_PLATFORM_ERR_NOT_SUPPORTED;
	}


	return TFM_PLATFORM_ERR_SUCCESS;
}
#endif /* defined(GPIO_PIN_CNF_MCUSEL_Msk) */

