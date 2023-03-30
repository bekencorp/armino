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

#include <pm/device.h>
#include <pm/pm.h>
#include "sdkconfig.h"
#include "pm.h"

#if CONFIG_PM_DEPEND
static pm_depend_entry_t s_depend_entry[CONFIG_PM_DEPEND_HANDLE_MAX] = {0};
static pm_depend_handle_t s_free_entry_list = 0;

pm_depend_entry_t* pm_depend_get_entry(pm_depend_handle_t depend_handle)
{
	if (depend_handle >= CONFIG_PM_DEPEND_HANDLE_MAX) {
		return NULL;
	}
	return &s_depend_entry[depend_handle];
}

const char* pm_depend_get_entry_type_str(pm_depend_type_t type)
{
	switch (type) {
	case PM_DEPEND_TYPE_PD:
		return "PD";
	case PM_DEPEND_TYPE_VOTE_FREQ:
		return "VOTE_FREQ";
	case PM_DEPEND_TYPE_VOTE_ON:
		return "VOTE_POWER_ON";
	case PM_DEPEND_TYPE_VOTE_OFF:
		return "VOTE_POWER_OFF";
	case PM_DEPEND_TYPE_VOTE_RESUME:
		return "VOTE_RESUME";
	case PM_DEPEND_TYPE_VOTE_SUSPEND:
		return "VOTE_SUSPEND";
	default:
		return "UNKNOWN";
	}
}

int pm_depend_init(void)
{
	if (CONFIG_PM_DEPEND_HANDLE_MAX > 255) {
		PM_LOGE("invalid dependency num %d, should <= 255\r\n", CONFIG_PM_DEPEND_HANDLE_MAX);
	}

	s_free_entry_list = 0;
	for (int i = 0; i < (CONFIG_PM_DEPEND_HANDLE_MAX - 1); i++) {
		s_depend_entry[i].handle = DEVICE_INVALID_HANDLE;
		s_depend_entry[i].next = i + 1;
	}
	s_depend_entry[CONFIG_PM_DEPEND_HANDLE_MAX - 1].handle = DEVICE_INVALID_HANDLE;
	s_depend_entry[CONFIG_PM_DEPEND_HANDLE_MAX - 1].next = PM_DEPEND_ENTRY_END;

	return BK_OK;
}

pm_depend_handle_t pm_depend_alloc_entry(void)
{
	pm_depend_handle_t alloc_entry = s_free_entry_list;

	if (s_free_entry_list != PM_DEPEND_ENTRY_END) {
		s_free_entry_list = s_depend_entry[s_free_entry_list].next;
	}

	PM_LOGV("alloc depend entry=%d, free_entry=%d\r\n", alloc_entry, s_free_entry_list);
	return alloc_entry;
}

void pm_depend_free_entry(pm_depend_handle_t depend_handle)
{
	if (depend_handle >= CONFIG_PM_DEPEND_HANDLE_MAX) {
		PM_LOGE("invalid entry index=%d\r\n", depend_handle);
		//TODO assert
	}

	s_depend_entry[depend_handle].next = s_free_entry_list;
	s_free_entry_list = depend_handle;
	PM_LOGV("free depend entry=%d\r\n", depend_handle);
}

static bool pm_depend_is_in_list(pm_depend_handle_t list, device_handle_t handle, pm_depend_type_t type)
{
	PM_DEPEND_FOREACH(list, entry_ptr) {
		if ((entry_ptr->handle == handle) && (entry_ptr->type == type)) {
			return true;
		}
	}

	return false;
}

static int pm_depend_remove_entry_from_list(pm_depend_handle_t *list, device_handle_t handle,
		pm_depend_type_t type, pm_depend_handle_t *remove_depend_handle)
{
	*remove_depend_handle = PM_DEPEND_ENTRY_END;
	pm_depend_handle_t *pre = list;

	PM_DEPEND_FOREACH(*list, entry_ptr) {
		if ((entry_ptr->handle == handle) && (entry_ptr->type == type)) {
			*remove_depend_handle = *pre;
			*pre = entry_ptr->next;
			return BK_OK;
		}

		pre = &entry_ptr->next;
	}

	return BK_OK;
}


int pm_depend_add_entry_to_list(pm_depend_handle_t *list, device_handle_t handle,
		pm_depend_type_t type, uint8_t value)
{
	pm_depend_handle_t new_depend_handle = pm_depend_alloc_entry();
	if (new_depend_handle == PM_DEPEND_ENTRY_END) {
		PM_LOGE("depend entry list full\r\n");
		return BK_ERR_PM_DEPEND_FULL;
	}

	s_depend_entry[new_depend_handle].handle = handle;
	s_depend_entry[new_depend_handle].type = type;
	s_depend_entry[new_depend_handle].value = value;
	s_depend_entry[new_depend_handle].next = *list;
	*list = new_depend_handle;
	PM_LOGV("add entry to list, index=%d next=%d\r\n", new_depend_handle, s_depend_entry[new_depend_handle].next);
	return BK_OK;
}

int pm_depend_add(const device_t *d1, const device_t *d2, pm_depend_type_t type, uint8_t value)
{
	if (!d1 || !d2 || !d1->pm || !d2->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	PM_LOGV("depend add+: %s depends on %s, type=%s, value=%d\r\n", device_get_name(d1), device_get_name(d2), pm_depend_get_entry_type_str(type), value);
	pm_device_t *d1_pm = d1->pm;
	pm_device_t *d2_pm = d2->pm;
	device_handle_t d1_handle = DEVICE_PTR2HANDLE(d1);

	if (pm_depend_is_in_list(d1_pm->children, d1_handle, type)) {
		PM_LOGD("depend entry already exists, remove it first!\r\n");
		return BK_ERR_PM_DEPEND_EXIST;
	}

	int ret = pm_depend_add_entry_to_list(&d2_pm->children, d1_handle, type, value);
	if (ret != BK_OK) {
		//TODO rollback
		return ret;
	}
	PM_LOGV("depend add-\r\n");

	return BK_OK;
}

int pm_depend_remove(const device_t *d1, const device_t *d2, pm_depend_type_t type)
{
	if (!d1 || !d2 || !d1->pm || !d2->pm) {
		return BK_ERR_PM_DEVICE_NULL_DEV;
	}

	PM_LOGV("depend remove+: %s depends on %s, type=%s\r\n", device_get_name(d1), device_get_name(d2), pm_depend_get_entry_type_str(type));

	pm_device_t *d2_pm = d2->pm;
	device_handle_t d1_handle = DEVICE_PTR2HANDLE(d1);
	pm_depend_handle_t depend_handle = PM_DEPEND_ENTRY_END;

	if (pm_depend_is_in_list(d2_pm->children, d1_handle, type) == false) {
		PM_LOGD("depend entry already removed\r\n");
		return BK_OK;
	}

	int ret = pm_depend_remove_entry_from_list(&d2_pm->children, d1_handle, type, &depend_handle);
	if (ret != BK_OK) {
		//TODO rollback
		return ret;
	}

	if (depend_handle != PM_DEPEND_ENTRY_END) {
		pm_depend_free_entry(depend_handle);
	}

	return BK_OK;
}

void pm_depend_dump_entry_list(uint8_t entry_list)
{
	if (entry_list >= CONFIG_PM_DEPEND_HANDLE_MAX) {
		PM_LOGE("invalid entry index=%d\r\n", entry_list);
		return;
	}

	pm_depend_handle_t depend_handle = entry_list;
	pm_depend_entry_t *entry;
	const device_t *device;
	PM_LOGI("%-5s  %-8s  %-12s  %-5s  %-5s\r\n", "index", "deviceice", "type", "value", "next");
	while (depend_handle != PM_DEPEND_ENTRY_END) {
		entry = &s_depend_entry[depend_handle];
		device = DEVICE_HANDLE2PTR(entry->handle);
		PM_LOGI("%-5d  %-8s  %-12s  %-5d  %-5d\r\n", depend_handle, device_get_name(device),
			pm_depend_get_entry_type_str(entry->type), entry->value, entry->next);
		depend_handle = s_depend_entry[depend_handle].next;
	}
}

void pm_depend_dump_free_list(void)
{
	pm_depend_dump_entry_list(s_free_entry_list);
}
#endif
