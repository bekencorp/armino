// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <soc/soc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <pm/pm.h>
#include <pm/dependency.h>
#include <common/bk_utils.h>
#include <components/init.h>
#include <driver/device.h>

struct device_t;
struct pm_state_info_t;
enum pm_state_t;
typedef struct pm_device_t pm_device_t;

//TODO optimize it
#define PM_DEVICE_DEFINE(device_id, pd_, flags_, data_, action_cb_) \
	DEVICE_EXPORT(device_id);\
	DEVICE_EXPORT(pd_);\
	static pm_device_t PM_DEVICE_ID2NAME(device_id) = {\
        .flags = (flags_),\
        .action_cb = action_cb_,\
        .pd = DEVICE_INVALID_HANDLE,\
        .on_children_num = 0,\
        .resume_children_num = 0,\
        .children = PM_DEPEND_ENTRY_END,\
        .data = (void*)(data_)\
	};\
	DEVICE_DEFINE(device_id, BK_STRINGIFY(device_id), NULL, &PM_DEVICE_ID2NAME(device_id), NULL, NULL, 0, 0, NULL, DEVICE_ID2PTR(pd_))

typedef enum {
	PM_DEVICE_STATE_ACTIVE,
	PM_DEVICE_STATE_SUSPENDED
} pm_device_state_t;

typedef enum {
	PM_DEVICE_FLAG_ACTIVE = BIT(0),
	PM_DEVICE_FLAG_POWER_ON = BIT(1),
	PM_DEVICE_FLAG_SUSPENDED_BY_SYS = BIT(2),
	PM_DEVICE_FLAG_PASSIVE_LOW_VOLTAGE = BIT(3),
	PM_DEVICE_FLAG_PASSIVE_DEEP_SLEEP = BIT(4),
	PM_DEVICE_FLAG_WAKEUP_CAPABLE = BIT(5),
	PM_DEVICE_FLAG_WAKEUP_ENABLED = BIT(6)
} pm_device_flag_t;

#define PM_DEVICE_SET_FLAG(pm, flag)           (pm)->flags |= (flag)
#define PM_DEVICE_CLR_FLAG(pm, flag)           (pm)->flags &= ~flag

#define PM_DEVICE_IS_ACTIVE(pm)                ((pm)->flags & PM_DEVICE_FLAG_ACTIVE)
#define PM_DEVICE_IS_POWER_ON(pm)              ((pm)->flags & PM_DEVICE_FLAG_POWER_ON)

#define PM_DEVICE_IS_SUSPENDED(pm)             !PM_DEVICE_IS_ACTIVE(pm)
#define PM_DEVICE_IS_SUSPENDED_BY_SYS(pm)      ((pm)->flags & PM_DEVICE_FLAG_SUSPENDED_BY_SYS)

#define PM_DEVICE_IS_LOW_VOLTAGE_ALLOWED(pm)   (PM_DEVICE_IS_PASSIVE_LOW_VOLTAGE(pm) || PM_DEVICE_IS_SUSPENDED(pm))
#define PM_DEVICE_IS_DEEP_SLEEP_ALLOWED(pm)    (PM_DEVICE_IS_PASSIVE_DEEP_SLEEP(pm) || PM_DEVICE_IS_SUSPENDED(pm))

#define PM_DEVICE_IS_PASSIVE_DEEP_SLEEP(pm)    ((pm)->flags & PM_DEVICE_FLAG_PASSIVE_DEEP_SLEEP)
#define PM_DEVICE_IS_PASSIVE_LOW_VOLTAGE(pm)   ((pm)->flags & PM_DEVICE_FLAG_PASSIVE_LOW_VOLTAGE)

#define PM_DEVICE_IS_WAKEUP_CAPABLE(pm)            !!((pm)->flags & PM_DEVICE_FLAG_WAKEUP_CAPABLE)
#define PM_DEVICE_IS_WAKEUP_ENABLED(pm)            !!((pm)->flags & PM_DEVICE_FLAG_WAKEUP_ENABLED)

#define PM_DEVICE_ID2NAME(device_id) BK_CONCAT(__pm_, device_id)

#define PM_DEVICE_IS_ALL_CHILDREN_OFF(dev)     ((dev)->pm->on_children_num == 0)
#define PM_DEVICE_IS_ALL_CHILDREN_SUSPEND(dev) ((dev)->pm->resume_children_num == 0)

enum {
	PM_DEVICE_ACTION_SUSPEND = 0,
	PM_DEVICE_ACTION_RESUME,
	PM_DEVICE_ACTION_POWER_ON,
	PM_DEVICE_ACTION_POWER_OFF,
	PM_DEVICE_ACTION_WAKEUP_ENABLE,
	PM_DEVICE_ACTION_WAKEUP_DISABLE,
	PM_DEVICE_ACTION_UPDATE_FREQ
};
typedef uint8_t pm_device_action_t;

#define PM_DEVICE_LIST_EMPTY (0)

typedef int (*pm_device_action_cb_t)(const device_t *device, pm_device_action_t action);

struct pm_device_t {
	uint32_t flags;
	device_handle_t pd; /**< Power domain deviceice handle */
	pm_depend_handle_t children; /**< Children depend entry list */
	uint8_t on_children_num;  /**< Number of device power on voter */
	uint8_t resume_children_num;  /**< Number of device resume voter */
	const pm_device_action_cb_t action_cb; /**< AVFS callback */
	void *data; /**< Device private data, such as wakeup parameters etc */

#if CONFIG_PM_DEVICE_STATS
	uint32_t resume_cnt;
	uint32_t suspend_cnt;
	uint32_t total_suspend_time;
	uint32_t max_suspend_time;
#endif
};

#if CONFIG_PM
static inline const device_t* pm_device_get_pd(const device_t *device)
{
	if (device && device->pm) {
		return DEVICE_HANDLE2PTR(device->pm->pd);
	}

	return NULL;
}
#endif

typedef enum {
	PM_DEVICE_FREQ_26M = 0,
	PM_DEVICE_FREQ_120M,
	PM_DEVICE_FREQ_240M,
	PM_DEVICE_FREQ_320M,
	PM_DEVICE_FREQ_COUNT
} pm_device_freq_t;

typedef uint64_t device_bits_t;

#define PM_DEVICE_BITS_SET(dev, bits) pm_device_bits_set(dev, bits)
#define PM_DEVICE_BITS_CLR(dev, bits) pm_device_bits_clr(dev, bits)

static inline void pm_device_bits_set(const device_t *dev, device_bits_t *bits)
{
	*bits |= BIT64(DEVICE_PTR2HANDLE(dev));
}

static inline void pm_device_bits_clr(const device_t *dev, device_bits_t *bits)
{
	*bits &= ~BIT64(DEVICE_PTR2HANDLE(dev));
}

bool pm_device_is_suspend_allowed(const device_t *device, enum pm_state_t state);
int pm_device_poweron(const device_t *device);
int pm_device_poweroff(const device_t *device);
int pm_device_resume(const device_t *device);
int pm_device_suspend(const device_t *device);
int pm_device_vote_freq(const device_t *d1, const device_t *d2, pm_device_freq_t freq);
int pm_device_vote_cpu_freq(const device_t *dev, pm_device_freq_t freq);
int pm_device_vote_poweron(const device_t *d1, const device_t *d2);
int pm_device_vote_poweroff(const device_t *d1, const device_t *d2);
int pm_device_vote_resume(const device_t *d1, const device_t *d2);
int pm_device_vote_suspend(const device_t *d1, const device_t *d2);
int pm_device_wakesource_enable(const device_t *device);
int pm_device_wakesource_disable(const device_t *device);
int pm_device_init(void);

//APIs for debug
int pm_device_dump_one(const device_t *device);
int pm_device_dump_tree(const device_t *device);
int pm_device_dump_all_tree(void);
int pm_device_dump_all(void);

//APIs for UT
bool pm_device_is_poweron(const device_t *device); //TODO
bool pm_device_is_suspended(const device_t *device); //TODO
int pm_device_get_poweron_ref(const device_t *device); //TODO
int pm_device_get_resume_ref(const device_t *device); //TODO
int pm_device_get_freq(const device_t *device); //TODO
int pm_device_get_voltage(const device_t *device); //TODO
