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

struct device_t;
typedef uint8_t pm_depend_handle_t;
typedef uint8_t pm_depend_type_t;
typedef uint8_t device_handle_t;

enum {
	PM_DEPEND_TYPE_PD = 1,       /**< Device Power domain */
	PM_DEPEND_TYPE_VOTE_ON,      /**< Device power ON vote */
	PM_DEPEND_TYPE_VOTE_OFF,     /**< Device power OFF vote */
	PM_DEPEND_TYPE_VOTE_RESUME,  /**< Device resume vote */
	PM_DEPEND_TYPE_VOTE_SUSPEND, /**< Device suspend vote */
	PM_DEPEND_TYPE_VOTE_FREQ,    /**< Device frequency vote */
};

struct pm_depend_entry_t {
	device_handle_t handle; /**< deviceice handle */
	pm_depend_type_t type; /**< dependency type */
	uint8_t value; /**< dependency value */
	pm_depend_handle_t next; /**< next dependency entry */
};

#define PM_DEPEND_ENTRY_END 0xFF

typedef struct pm_depend_entry_t pm_depend_entry_t;

int pm_depend_init(void);
pm_depend_handle_t pm_depend_alloc_entry(void);
void pm_depend_free_entry(pm_depend_handle_t entry_index);
void pm_depend_dump_entry_list(pm_depend_handle_t entry_index);
void pm_depend_dump_free_list(void);
int pm_depend_add(const struct device_t *d1, const struct device_t *d2, uint8_t type, uint8_t value);
int pm_depend_remove(const struct device_t *d1, const struct device_t *d2, uint8_t type);
pm_depend_entry_t* pm_depend_get_entry(pm_depend_handle_t handle);
const char* pm_depend_get_entry_type_str(pm_depend_type_t type);
int pm_depend_add_entry_to_list(pm_depend_handle_t *list, device_handle_t handle,
                pm_depend_type_t type, uint8_t value);

#define PM_DEPEND_FOREACH(entry_index_, entry_ptr) \
	for (pm_depend_entry_t *entry_ptr = pm_depend_get_entry((entry_index_)); entry_ptr != NULL; entry_ptr = pm_depend_get_entry(entry_ptr->next))


