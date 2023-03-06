/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include <nxp/drivers/fsl_pwm.h>

#define EFLEXPWM_DUMPREG_ENABLED 1


/// @brief Library namespace
namespace eFlex {

  /**
   * @brief PWM Channel
   */
  enum Channel {
    ChanA = 0,
    ChanB = 1
  };
  
#ifndef DOXYGEN
  //   Constants
  // ----------------------------------------------------------------------------
  const uint8_t NofTimers = 4; ///< Number of timers in the MCU
  const uint8_t NofSubmodules = FSL_FEATURE_PWM_SUBMODULE_COUNT; ///< number of submodules per timer
  const uint8_t NofPins = PWM_SUBMODULE_CHANNEL; ///< max number of pins per sub-module (A/B)

  enum TimerIndex {
    Pwm1 = 0,
    Pwm2,
    Pwm3,
    Pwm4
  };

  enum TimerFlag {
    Pwm1Flag = (1 << Pwm1),
    Pwm2Flag = (1 << Pwm2),
    Pwm3Flag = (1 << Pwm3),
    Pwm4Flag = (1 << Pwm4),
    AllTimersFlag = (Pwm1Flag | Pwm2Flag | Pwm3Flag | Pwm4Flag)
  };

  enum SubmoduleIndex {
    Submodule1 = 0,
    Submodule2,
    Submodule3,
    Submodule4
  };

  enum SubmoduleFlag {
    Submodule1Flag = (1 << Submodule1),
    Submodule2Flag = (1 << Submodule2),
    Submodule3Flag = (1 << Submodule3),
    Submodule4Flag = (1 << Submodule4),
    AllSubmodulesFlag = (Submodule1Flag | Submodule2Flag | Submodule3Flag | Submodule4Flag)
  };

  const pwm_submodule_t SM[NofSubmodules] = {
    kPWM_Module_0,
    kPWM_Module_1,
    kPWM_Module_2,
    kPWM_Module_3
  };

  //   Variables
  // ----------------------------------------------------------------------------
  extern PWM_Type *PWM[NofTimers];
  extern uint8_t SmMask[NofTimers];

  //   Code
  // ----------------------------------------------------------------------------

  // ----------------------------------------------------------------------------
  inline pwm_channels_t kPwmChan (Channel c) {

    return (c == ChanA ? kPWM_PwmA : kPWM_PwmB);
  }
#endif
}
