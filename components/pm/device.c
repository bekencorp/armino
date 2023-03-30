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

#include <components/log.h>
#include <pm/device.h>
#include <pm/pm.h>
#include <pm/policy.h>
#include "pm.h"

//TODO put it into a special section
const device_t __deviceobj_null;

static bool pm_device_is_null(const device_t *device)
{
	return (device == DEVICE_ID2PTR(null));
}

int pm_device_wakesource_enable(const device_t *device)
{
	const pm_device_t *pm = device->pm;
	PM_LOGD("enable wakesource %s\r\n", device_get_name(device));
	if (pm && pm->action_cb) {
		return pm->action_cb(device, PM_DEVICE_ACTION_WAKEUP_ENABLE);
	}
	return BK_OK;
}

int pm_device_wakesource_disable(const device_t *device)
{
	const pm_device_t *pm = device->pm;
	PM_LOGD("disable wakeup source %s\r\n", device_get_name(device));
	if (pm && pm->action_cb) {
		return pm->action_cb(device, PM_DEVICE_ACTION_WAKEUP_DISABLE);
	}
	return BK_OK;
}


int pm_device_update_freq(const device_t *device)
{
	const pm_device_t *pm = device->pm;
	PM_LOGD("%s update freq\r\n", device_get_name(device));
	if (pm && pm->action_cb) {
		return pm->action_cb(device, PM_DEVICE_ACTION_UPDATE_FREQ);
	}
	return BK_OK;
}

// Caller make sure device is not NULL
static inline int pm_device_do_resume(const device_t *device)
{
	pm_device_t *pm = device->pm;

	PM_LOGV("resume %s\r\n", device_get_name(device));
	if (pm) {
 		if (pm->action_cb)
			pm->action_cb(device, PM_DEVICE_ACTION_RESUME);
	}
	return BK_OK;
}

// suspend one device node
static inline int pm_device_do_suspend(const device_t *device)
{
	const pm_device_t *pm = device->pm;

	PM_LOGV("suspend device %s\r\n", device_get_name(device));
	if (pm && pm->action_cb) {
		return pm->action_cb(device, PM_DEVICE_ACTION_SUSPEND);
	}
	return BK_OK;
}

//resume always from leaf device to root device
int pm_device_resume(const device_t *device)
{
	pm_device_t *pm = device->pm;
	int ret;

	if (PM_DEVICE_IS_ACTIVE(pm)) {
		PM_LOGD("device %s already resumed\r\n", device_get_name(device));
		return BK_OK;
	}

	ret = pm_device_do_resume(device);
	if (ret != BK_OK) {
		PM_LOGE("resume device %s failed\r\n", device_get_name(device));
		return ret;
	}
	PM_DEVICE_SET_FLAG(pm, PM_DEVICE_FLAG_ACTIVE);
	PM_LOGV("resume %s, flags=%x\r\n", device_get_name(device), pm->flags);

	if (!PM_DEVICE_IS_PASSIVE_DEEP_SLEEP(pm)) {
		pm_policy_device_lock_state(device, PM_STATE_DEEP_SLEEP);
	}

	if (!PM_DEVICE_IS_PASSIVE_LOW_VOLTAGE(pm)) {
		pm_policy_device_lock_state(device, PM_STATE_LOW_VOLTAGE);
	}

	const device_t *pd = pm_device_get_pd(device);
	if (pd && pd->pm) {
		pd->pm->resume_children_num ++;
		pm_device_resume(pd);
	}

	return ret;
}

//suspend always from leaf device to root device
int pm_device_suspend(const device_t *device)
{
	pm_device_t *pm = device->pm;

	if (PM_DEVICE_IS_SUSPENDED(pm)) {
		PM_LOGD("device %s already suspended\r\n", device_get_name(device));
		return BK_OK;
	}

	pm_device_do_suspend(device);
	PM_DEVICE_CLR_FLAG(pm, PM_DEVICE_FLAG_ACTIVE);
	PM_LOGV("suspend %s, flags=%x\r\n", device_get_name(device), pm->flags);

	if (!PM_DEVICE_IS_PASSIVE_DEEP_SLEEP(pm)) {
		pm_policy_device_unlock_state(device, PM_STATE_DEEP_SLEEP);
	}

	if (!PM_DEVICE_IS_PASSIVE_LOW_VOLTAGE(pm)) {
		pm_policy_device_unlock_state(device, PM_STATE_LOW_VOLTAGE);
	}

	const device_t *pd = pm_device_get_pd(device);
	if (pd && pd->pm) {
		pd->pm->resume_children_num --;
		if (PM_DEVICE_IS_ALL_CHILDREN_SUSPEND(pd)) {
			pm_device_suspend(pd);
		}
	}

	return BK_OK;
}

bool pm_device_is_suspend_allowed(const device_t *device, pm_state_t state)
{
	const pm_device_t *pm = device->pm;

	switch (state) {
	case PM_STATE_LOW_VOLTAGE:
		return PM_DEVICE_IS_LOW_VOLTAGE_ALLOWED(pm);
	case PM_STATE_DEEP_SLEEP:
		return PM_DEVICE_IS_DEEP_SLEEP_ALLOWED(pm);
	default:
		return true;
	}
}

int pm_device_poweron(const device_t *device)
{
	if (!device || !device->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	pm_device_t *pm = device->pm;

	if (PM_DEVICE_IS_POWER_ON(pm)) {
		PM_LOGD("device %s already powered on\r\n", device_get_name(device));
		return BK_OK;
	}

 	if (pm->action_cb)
		pm->action_cb(device, PM_DEVICE_ACTION_POWER_ON);
	PM_DEVICE_SET_FLAG(pm, PM_DEVICE_FLAG_POWER_ON);
	PM_LOGV("power on %s, flags=%x\r\n", device_get_name(device), pm->flags);

	const device_t *pd = pm_device_get_pd(device);
	if (pd && pd->pm) {
		pd->pm->on_children_num ++;
		pm_device_poweron(pd);
	}

	return BK_OK;
}

int pm_device_poweroff(const device_t *device)
{
	if (!device || !device->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	pm_device_t *pm = device->pm;

	if (!PM_DEVICE_IS_POWER_ON(pm)) {
		PM_LOGD("device %s already powered off\r\n", device_get_name(device));
		return BK_OK;
	}

 	if (pm->action_cb)
		pm->action_cb(device, PM_DEVICE_ACTION_POWER_OFF);
	PM_DEVICE_CLR_FLAG(pm, PM_DEVICE_FLAG_POWER_ON);
	PM_LOGV("power off %s, flags=%x\r\n", device_get_name(device), pm->flags);

	const device_t *pd = pm_device_get_pd(device);
	if (pd && pd->pm) {
		pd->pm->on_children_num --;
		if (PM_DEVICE_IS_ALL_CHILDREN_OFF(pd)) {
			pm_device_poweroff(pd);
		}

	}

	return BK_OK;
}

static int pm_device_add_depend(const device_t *d1, const device_t *d2, pm_depend_type_t type, uint8_t value)
{
#if CONFIG_PM_DEPEND
	if (!d1 || !d2 || !d1->pm || !d2->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}
	BK_LOG_ON_ERR(pm_depend_remove(d1, d2, type));
	BK_LOG_ON_ERR(pm_depend_add(d1, d2, type, value));
#endif
	return BK_OK;
}

static int pm_device_remove_depend(const device_t *d1, const device_t *d2, pm_depend_type_t type)
{
	if (!d1 || !d2 || !d1->pm || !d2->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}
	BK_LOG_ON_ERR(pm_depend_remove(d1, d2, type));

	return BK_OK;
}

int pm_device_vote_freq(const device_t *d1, const device_t *d2, pm_device_freq_t freq)
{
	int ret = pm_device_add_depend(d1, d2, PM_DEPEND_TYPE_VOTE_FREQ, freq);

	if (ret == BK_OK) {
		ret = pm_device_update_freq(d2);
	}
	return BK_OK;
}

int pm_device_vote_poweron(const device_t *d1, const device_t *d2)
{
	int ret = pm_device_add_depend(d1, d2, PM_DEPEND_TYPE_VOTE_ON, 0);

	if (ret == BK_OK) {
		d2->pm->on_children_num ++;
		ret = pm_device_poweron(d2);
	}

	return ret;
}

int pm_device_vote_poweroff(const device_t *d1, const device_t *d2)
{
	int ret = pm_device_remove_depend(d1, d2, PM_DEPEND_TYPE_VOTE_ON);

	if (ret == BK_OK) {
		d2->pm->on_children_num --;

		if (PM_DEVICE_IS_ALL_CHILDREN_OFF(d2)) {
			ret = pm_device_poweroff(d2);
		}
	}

	return ret;
}

int pm_device_vote_resume(const device_t *d1, const device_t *d2)
{
	int ret = pm_device_add_depend(d1, d2, PM_DEPEND_TYPE_VOTE_RESUME, 0);

	if (ret == BK_OK) {
		d2->pm->resume_children_num ++;
		ret = pm_device_resume(d2);
	}

	return ret;
}

int pm_device_vote_suspend(const device_t *d1, const device_t *d2)
{
	int ret = pm_device_remove_depend(d1, d2, PM_DEPEND_TYPE_VOTE_RESUME);

	if (ret == BK_OK) {
		d2->pm->resume_children_num --;
 		if (PM_DEVICE_IS_ALL_CHILDREN_SUSPEND(d2)) {
			ret = pm_device_suspend(d2);
		}
	}

	return ret;
}

extern init_entry_t __a_init_start[];
extern init_entry_t __a_init_end[];

int pm_device_init(void)
{
	const init_entry_t *init_entry;
	const device_t *device;
	const device_t *pd;

	//TODO don't use magic number
	if (device_get_count() > 64) {
		PM_LOGE("Only support 64 devices\r\n");
		//TODO assert it
	}

	pm_depend_init();
	for (init_entry = __a_init_start; init_entry != __a_init_end; init_entry++) {
		device = init_entry->device;
		pd = init_entry->pd;

#if CONFIG_PM_DEPEND
		if (device->pm && pd && pd->pm) {
			device->pm->pd = DEVICE_PTR2HANDLE(pd);
			pm_depend_add_entry_to_list(&pd->pm->children, DEVICE_PTR2HANDLE(device), PM_DEPEND_TYPE_PD, 0);
		}
#endif
		PM_LOGD("init %s, pd=%s\r\n", device_get_name(device), device_get_name(pd));
	}

	return BK_OK;
}

static int a_pm_device_dump_detail_info(const device_t *device, int depth)
{
#define A_MAX_SPACE_PREFIX_LEN 64
	char prefix_str[A_MAX_SPACE_PREFIX_LEN];

	if (!device) {
		return BK_OK;
	}

	int space_prefix_len = depth * 9;
	if (space_prefix_len >= A_MAX_SPACE_PREFIX_LEN) {
		space_prefix_len = A_MAX_SPACE_PREFIX_LEN -1;
	}
	for (int i = 0; i < space_prefix_len; i++) {
		prefix_str[i] = ' ';
	}
	prefix_str[space_prefix_len] = 0;

	PM_LOGI("%s|- device obj=%p\r\n", prefix_str, device);
	PM_LOGI("%s   |- name: %s\r\n", prefix_str, device->name);
	pm_device_t *pm = device->pm;
	if (pm) {
		PM_LOGI("%s   |- pm:\r\n", prefix_str);
		PM_LOGI("%s      |- flags(%x): %s, %s%s%s%s%s\r\n", prefix_str, pm->flags,
			PM_DEVICE_IS_POWER_ON(pm) ? "ON" : "OFF",
			PM_DEVICE_IS_ACTIVE(pm) ? "active" : "suspended",
			PM_DEVICE_IS_PASSIVE_LOW_VOLTAGE(pm) ? ", passive_lv" : "",
			PM_DEVICE_IS_PASSIVE_DEEP_SLEEP(pm) ? ", passive_deep" : "",
			PM_DEVICE_IS_WAKEUP_CAPABLE(pm) ? ", wakeup_capable" : "",
			PM_DEVICE_IS_WAKEUP_ENABLED(pm) ? ", wakeup_enabled" : "");

		PM_LOGI("%s      |- on_children_num: %d\r\n", prefix_str, pm->on_children_num);
		PM_LOGI("%s      |- resume_children_num: %d\r\n", prefix_str, pm->resume_children_num);
		const device_t *pd_device = DEVICE_HANDLE2PTR(pm->pd);
		PM_LOGI("%s      |- pd: %s\r\n", prefix_str, device_get_name(pd_device));

#if CONFIG_PM_DEPEND
		PM_DEPEND_FOREACH(pm->children, entry_ptr) {
			const device_t* child_device = DEVICE_HANDLE2PTR(entry_ptr->handle);
			PM_LOGI("%s      |- child: %s/%s\r\n", prefix_str, device_get_name(child_device),
					pm_depend_get_entry_type_str(entry_ptr->type));
		}
#endif
	}
	return BK_OK;
}

int pm_device_dump_detail_info(const device_t *device)
{
	return a_pm_device_dump_detail_info(device, 0);
}

int pm_device_dump_simple_info(const device_t *device)
{
	if (!device) {
		return BK_OK;
	}

	pm_device_t *pm = device->pm;
	if (pm) {
		const device_t *pd_device = DEVICE_HANDLE2PTR(pm->pd);
		PM_LOGI("%-16s  %-6d  %-10p  %-16s  %s\r\n", device->name, DEVICE_PTR2HANDLE(device), device,
			device_get_name(pd_device), PM_DEVICE_IS_ACTIVE(pm) ? "active" : "suspended");
	} else {
		PM_LOGI("%-16s  %-6d  %-10p  %-16s  %s\r\n", device->name, DEVICE_PTR2HANDLE(device), device, "-",
			PM_DEVICE_IS_ACTIVE(pm) ? "active" : "suspended");
	}
	return BK_OK;
}

static int pm_device_dump_tree_recursively(const device_t *device, int depth)
{
	if (!device) {
		return BK_OK;
	}

	a_pm_device_dump_detail_info(device, depth);

#if CONFIG_PM_DEPEND
	pm_device_t *pm = device->pm;

	if (pm) {
		const device_t *child_device;
		PM_DEPEND_FOREACH(pm->children, entry_ptr) {
			child_device = DEVICE_HANDLE2PTR(entry_ptr->handle);

			if (entry_ptr->type == PM_DEPEND_TYPE_PD) {
				pm_device_dump_tree_recursively(child_device, depth + 1);
			}
		}
	}
#endif

	return BK_OK;
}

int pm_device_dump_tree(const device_t *device)
{
	return pm_device_dump_tree_recursively(device, 0);
}

int pm_device_dump_all_tree(void)
{
	DEVICE_FOREACH(device) {
		const device_t *pd = pm_device_get_pd(device);
		if (!pd) {
			pm_device_dump_tree(device);
		}
	}
	return BK_OK;
}

int pm_device_dump_all(void)
{
	PM_LOGI("%-16s  %6s  %-10s  %-16s  %s\r\n", "name", "handle", "ptr", "pd", "state");
	DEVICE_FOREACH(device) {
		pm_device_dump_simple_info(device);
	}

	return BK_OK;
}
