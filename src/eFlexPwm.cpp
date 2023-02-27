/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "eFlexPwmTimer.h"

namespace eFlex {

  //   Namespace static variables
  // ----------------------------------------------------------------------------
  // Timer base addresses
  PWM_Type *PWM[NofTimers] = {
    PWM1,
    PWM2,
    PWM3,
    PWM4
  };

  // List of registered submodules
  // array initialized to zero, the sub-modules are saved in this array by their constructor.
  // [Timer][SubModule]
  SubModule *SmList[NofTimers][NofSubmodules];

  // Masks of submodules recorded for each timer.
  uint8_t SmMask[NofTimers];

  // All timers are instantiated globally
  Timer TM[NofTimers] = {
    Timer (Pwm1),
    Timer (Pwm2),
    Timer (Pwm3),
    Timer (Pwm4)
  };
}