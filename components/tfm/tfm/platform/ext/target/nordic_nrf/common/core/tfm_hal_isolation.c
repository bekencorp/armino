/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include "cmsis.h"
#include "mpu_armv8m_drv.h"
#include "region.h"
#include "spu.h"
#include "target_cfg.h"
#include "tfm_hal_isolation.h"
#include "tfm_spm_hal.h"
#include "mmio_defs.h"
#include "array.h"
#ifdef TFM_PSA_API
#include "load/spm_load_api.h"
#endif /* TFM_PSA_API */

REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);

#ifdef CONFIG_TFM_PARTITION_META
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
#endif /* CONFIG_TFM_PARTITION_META */

/* Get address of memory regions to configure MPU */
extern const struct memory_region_limits memory_regions;

struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

// We assume we are the only consumer of MPU regions and we use this
// variable to keep track of what the next available region is.
static uint32_t n_configured_regions = 0;
enum tfm_hal_status_t mpu_init_cfg(void);

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
    /* Set up isolation boundaries between SPE and NSPE */
    sau_and_idau_cfg();

    if (spu_init_cfg() != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    if (spu_periph_init_cfg() != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    if (mpu_init_cfg() != TFM_HAL_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    return TFM_HAL_SUCCESS;
}

#ifdef TFM_PSA_API
enum tfm_hal_status_t
tfm_hal_bind_boundaries(const struct partition_load_info_t *p_ldinf,
                        void **pp_boundaries)
{
    if (!p_ldinf || !pp_boundaries) {
        return TFM_HAL_ERROR_GENERIC;
    }

    bool privileged;

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    *pp_boundaries = (void *)(((uint32_t)privileged) & HANDLE_ATTR_PRIV_MASK);

    for (uint32_t i = 0; i < p_ldinf->nassets; i++) {
        const struct asset_desc_t *p_asset =
                (const struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);

        if (!(p_asset[i].attr & ASSET_ATTR_NAMED_MMIO)) {
            // Skip numbered MMIO. NB: Need to add validation if it
            // becomes supported. Should we return an error instead?
            continue;
        }

        bool found = false;
        for (uint32_t j = 0; j < ARRAY_SIZE(partition_named_mmio_list); j++) {
            if (partition_named_mmio_list[j] == p_asset[i].dev.dev_ref) {
                found = true;
                break;
            }
        }

        if (!found) {
            /* The MMIO asset is not in the allowed list of platform. */
            return TFM_HAL_ERROR_GENERIC;
        }

        /* Assume PPC & MPC settings are required even under level 1 */
        struct platform_data_t *plat_data_ptr =
                (struct platform_data_t *)p_asset[i].dev.dev_ref;

        if (plat_data_ptr->periph_start == 0) {
            // Should we return an error instead?
            continue;
        }

        spu_periph_configure_to_secure(plat_data_ptr->periph_start);

        /*
         * Static boundaries are set. Set up MPU region for MMIO.
         * Setup regions for unprivileged assets only.
         */
        if (!privileged) {
            struct mpu_armv8m_region_cfg_t localcfg;

            localcfg.region_nr = n_configured_regions++;

            localcfg.region_base = plat_data_ptr->periph_start;
            localcfg.region_limit = plat_data_ptr->periph_limit;
            localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
            localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
            localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
            localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;

            enum mpu_armv8m_error_t err =
                    mpu_armv8m_region_enable(&dev_mpu_s, &localcfg);

            if (err != MPU_ARMV8M_OK) {
                return TFM_HAL_ERROR_GENERIC;
            }
        }
    }

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t
tfm_hal_update_boundaries(const struct partition_load_info_t *p_ldinf,
                          void *p_boundaries)
{
    /* Privileged level is required to be set always */
    CONTROL_Type ctrl;
    ctrl.w = __get_CONTROL();

    ctrl.b.nPRIV = ((uint32_t)p_boundaries & HANDLE_ATTR_PRIV_MASK) ? 0 : 1;

    __set_CONTROL(ctrl.w);

    return TFM_HAL_SUCCESS;
}
#endif /* TFM_PSA_API */

#if !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0)
static bool accessible_to_region(const void *p, size_t s, int flags)
{
    cmse_address_info_t tt_base = cmse_TT((void *)p);
    cmse_address_info_t tt_last = cmse_TT((void *)((uint32_t)p + s - 1));

    uint32_t base_spu_id = tt_base.flags.idau_region;
    uint32_t last_spu_id = tt_last.flags.idau_region;

    size_t size;
    uint32_t p_start = (uint32_t)p;
    int i;

    if ((base_spu_id >= spu_regions_flash_get_start_id()) &&
        (last_spu_id <= spu_regions_flash_get_last_id())) {

        size = spu_regions_flash_get_last_address_in_region(base_spu_id) + 1
                                                                      - p_start;

        if (cmse_check_address_range((void *)p_start, size, flags) == 0) {
            return false;
        }

        for (i = base_spu_id + 1; i < last_spu_id; i++) {
            p_start = spu_regions_flash_get_base_address_in_region(i);
            if (cmse_check_address_range((void *)p_start,
                spu_regions_flash_get_region_size(), flags) == 0) {
                return false;
            }
        }

        p_start = spu_regions_flash_get_base_address_in_region(last_spu_id);
        size = (uint32_t)p + s - p_start;
        if (cmse_check_address_range((void *)p_start, size, flags) == 0) {
            return false;
        }


    } else if ((base_spu_id >= spu_regions_sram_get_start_id()) &&
        (last_spu_id <= spu_regions_sram_get_last_id())) {

        size = spu_regions_sram_get_last_address_in_region(base_spu_id) + 1
                                                                      - p_start;
        if (cmse_check_address_range((void *)p_start, size, flags) == 0) {
            return false;
        }

        for (i = base_spu_id + 1; i < last_spu_id; i++) {
            p_start = spu_regions_sram_get_base_address_in_region(i);
            if (cmse_check_address_range((void *)p_start,
                spu_regions_sram_get_region_size(), flags) == 0) {
                return false;
            }
        }

        p_start = spu_regions_sram_get_base_address_in_region(last_spu_id);
        size = (uint32_t)p + s - p_start;
        if (cmse_check_address_range((void *)p_start, size, flags) == 0) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}
#endif /* !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0) */

enum tfm_hal_status_t tfm_hal_memory_has_access(uintptr_t base, size_t size,
                                                uint32_t attr)
{
    int flags = 0;
    int32_t range_access_allowed_by_mpu;

    if (attr & TFM_HAL_ACCESS_NS) {
        CONTROL_Type ctrl;
        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            attr |= TFM_HAL_ACCESS_UNPRIVILEGED;
        } else {
            attr &= ~TFM_HAL_ACCESS_UNPRIVILEGED;
        }
        flags |= CMSE_NONSECURE;
    }

    if (attr & TFM_HAL_ACCESS_UNPRIVILEGED) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if ((attr & TFM_HAL_ACCESS_READABLE) && (attr & TFM_HAL_ACCESS_WRITABLE)) {
        flags |= CMSE_MPU_READWRITE;
    } else if (attr & TFM_HAL_ACCESS_READABLE) {
        flags |= CMSE_MPU_READ;
    } else {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    /* Use the TT instruction to check access to the partition's regions*/
    range_access_allowed_by_mpu =
            cmse_check_address_range((void *)base, size, flags) != NULL;

#if !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0)
    if (!range_access_allowed_by_mpu) {
        /*
         * Verification failure may be due to address range crossing
         * one or multiple IDAU boundaries. In this case request a
         * platform-specific check for access permissions.
         */
        cmse_address_info_t addr_info_base = cmse_TT((void *)base);
        cmse_address_info_t addr_info_last =
                cmse_TT((void *)((uint32_t)base + size - 1));

        if ((addr_info_base.flags.idau_region_valid != 0) &&
            (addr_info_last.flags.idau_region_valid != 0) &&
            (addr_info_base.flags.idau_region !=
             addr_info_last.flags.idau_region)) {
            range_access_allowed_by_mpu =
                                accessible_to_region((void *)base, size, flags);
        }
    }
#endif

    if (range_access_allowed_by_mpu) {
        return TFM_HAL_SUCCESS;
    } else {
        return TFM_HAL_ERROR_MEM_FAULT;
    }
}

enum tfm_hal_status_t mpu_init_cfg(void)
{
    struct mpu_armv8m_region_cfg_t region_cfg;
    enum mpu_armv8m_error_t err;

    mpu_armv8m_clean(&dev_mpu_s);

    /* Veneer region */
    region_cfg.region_nr = n_configured_regions++;

    region_cfg.region_base = memory_regions.veneer_base;
    region_cfg.region_limit = memory_regions.veneer_limit;
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;

    err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

    if (err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    /* TFM Core unprivileged code region */
    region_cfg.region_nr = n_configured_regions++;

    region_cfg.region_base =
            (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
    region_cfg.region_limit =
            (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;

    err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

    if (err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    /* RO region */
    region_cfg.region_nr = n_configured_regions++;

    region_cfg.region_base =
            (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base);
    region_cfg.region_limit =
            (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;

    err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

    if (err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    /* RW, ZI and stack as one region */
    region_cfg.region_nr = n_configured_regions++;

    region_cfg.region_base =
            (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base);
    region_cfg.region_limit =
            (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;

    err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

    if (err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata poniter region */
    region_cfg.region_nr = n_configured_regions++;

    region_cfg.region_base =
            (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
    region_cfg.region_limit =
            (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;

    err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

    if (err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }
#endif /* CONFIG_TFM_PARTITION_META */

#ifdef NULL_POINTER_EXCEPTION_DETECTION
	if(MPU_ARMV8M_NUM_REGIONS - n_configured_regions < 2) {
		// We have enabled null pointer detection, but we don't have
		// enough regions for it.
		//
	    // NB: Enabling null-pointer detection can also
		// cause tfm_hal_bind_boundaries to return an error due to
		// insufficient memory regions
		return TFM_HAL_ERROR_GENERIC;
	}

	// The armv8m MPU can not be configured to protect a memory region
	// from priviliged reads. However, it is invalid to have two
	// overlapping memory regions and when a memory access is made to
	// such an overlapping area we will get a MemFault. We exploit
	// this undefined behaviour to block priviliged reads to the first
	// 256 bytes. The first 350 bytes on nRF platforms are used for
	// the vector table but luckily the armv8m MPU does not affect
	// exception vector fetches so these two regions we configure will
	// not accidentally disturb any valid memory access.
	for(int i = 0; i < 2; i++) {
		region_cfg.region_nr = n_configured_regions++;

		region_cfg.region_base = 0;
		region_cfg.region_limit = 256 - 32; // The last protected address is limit + 31

		// The region_attridx, attr_access, attr_sh and attr_exec
		// have no effect when memory regions overlap as any
		// access will trigger a MemFault so we just use the
		// previously configured attributes.

		err = mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg);

		if (err != MPU_ARMV8M_OK) {
			return TFM_HAL_ERROR_GENERIC;
		}
	}
#endif /* NULL_POINTER_EXCEPTION_DETECTION */

    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                      HARDFAULT_NMI_ENABLE);

    return TFM_HAL_SUCCESS;
}
