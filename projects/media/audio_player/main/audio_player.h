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

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t audio_player_start(const char *uri);

bk_err_t audio_player_stop(void);

bk_err_t audio_player_pause(void);

bk_err_t audio_player_play(void);

bk_err_t audio_player_set_volume(uint8_t vol_value);

bk_err_t audio_player_last(void);

bk_err_t audio_player_next(void);

bk_err_t audio_player_init(void);

bk_err_t audio_player_deinit(void);


#ifdef __cplusplus
}
#endif


