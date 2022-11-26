// Copyright 2020-2023 Beken
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

#include <sdkconfig.h>

/* TODO:
 * The weak doesn't work, define CONFIG_DEFAULT_CMSIS_START to overwrite the
 * default _start. Need to find out why the weak not work!!!
 */
#ifdef CONFIG_DEFAULT_CMSIS_START
__attribute__((weak)) void _start(void)
{
        //entry_main();
}
#endif
