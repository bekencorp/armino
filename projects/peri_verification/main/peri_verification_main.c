// Copyright 2020-2021 Beken
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
#include <driver/timer.h>
#include "bk_api_cli.h"
#include "portmacro.h"

#define TAG "peri"

extern void arch_enable_int(void);

int main(void)
{
	BK_LOGI(TAG, "peri_verification main\r\n");
	portENABLE_INTERRUPTS();
	BK_LOG_ON_ERR(bk_cli_init());

	return 0;
}

