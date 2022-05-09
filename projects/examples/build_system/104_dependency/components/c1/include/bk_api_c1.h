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

#pragma once

//Don't include any headers in PRIV_INCLUDE_DIRS because they are only used
//in this component, but bk_api_c1.h is a public header!
//#include "c1_internal.h"

//Don't include any headers in bk_private because they are used by ARMINO-only,
//but the bk_api_c1.h is for customer only!
//#include "bk_private/bk_c1.h"

int c1_public_api(void);

typedef struct {
} c1_t;
