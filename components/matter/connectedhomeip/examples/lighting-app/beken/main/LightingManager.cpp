/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "matter_pal.h"
#include "LightingManager.h"

#include <lib/support/logging/CHIPLogging.h>

LightingManager LightingManager::sLight;

static void lighting_gpio_init()
{
#ifdef BEKEN_SOC_BK7231
    BkGpioInitialize(GPIO6, OUTPUT_NORMAL);
    BkGpioInitialize(GPIO7, OUTPUT_NORMAL);
    BkGpioInitialize(GPIO8, OUTPUT_NORMAL);
    BkGpioOutputHigh(GPIO6);
    BkGpioOutputHigh(GPIO7);
    BkGpioOutputHigh(GPIO8);
#endif

#ifdef BEKEN_SOC_BK7235
    bk_gpio_disable_input(GPIO_8);
    bk_gpio_enable_output(GPIO_8);
    bk_gpio_disable_pull(GPIO_8);
    bk_gpio_set_output_high(GPIO_8);
#endif
}
static void lighting_gpio_set(LightingManager::Action_t aAction)
{
#ifdef BEKEN_SOC_BK7231
    if(aAction == LightingManager::ON_ACTION)
        BkGpioOutputLow(GPIO6);
    if(aAction == LightingManager::OFF_ACTION)
        BkGpioOutputHigh(GPIO6);
#endif

#ifdef BEKEN_SOC_BK7235
    if(aAction == LightingManager::ON_ACTION)
        bk_gpio_set_output_low(GPIO_8);
    if(aAction == LightingManager::OFF_ACTION)
        bk_gpio_set_output_high(GPIO_8);
#endif
}

CHIP_ERROR LightingManager::Init()
{
    mState = kState_Off;
    lighting_gpio_init();
    mActionInitiated_CB = lighting_gpio_set;
    return CHIP_NO_ERROR;
}

bool LightingManager::IsTurnedOn()
{
    return mState == kState_On;
}

void LightingManager::SetCallbacks(LightingCallback_fn aActionInitiated_CB, LightingCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool LightingManager::InitiateAction(Action_t aAction)
{
    // TODO: this function is called InitiateAction because we want to implement some features such as ramping up here.
    bool action_initiated = false;
    State_t new_state;

    switch (aAction)
    {
    case ON_ACTION:
        ChipLogProgress(AppServer, "LightingManager::InitiateAction(ON_ACTION)");
        break;
    case OFF_ACTION:
        ChipLogProgress(AppServer, "LightingManager::InitiateAction(OFF_ACTION)");
        break;
    default:
        ChipLogProgress(AppServer, "LightingManager::InitiateAction(unknown)");
        break;
    }

    // Initiate On/Off Action only when the previous one is complete.
    if (mState == kState_Off && aAction == ON_ACTION)
    {
        action_initiated = true;
        new_state        = kState_On;
    }
    else if (mState == kState_On && aAction == OFF_ACTION)
    {
        action_initiated = true;
        new_state        = kState_Off;
    }

    if (action_initiated)
    {
        if (mActionInitiated_CB)
        {
            mActionInitiated_CB(aAction);
        }

        Set(new_state == kState_On);

        if (mActionCompleted_CB)
        {
            mActionCompleted_CB(aAction);
        }
    }

    return action_initiated;
}

void LightingManager::Set(bool aOn)
{
    if (aOn)
    {
        mState = kState_On;
    }
    else
    {
        mState = kState_Off;
    }
}
