/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmTimer.h"

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

  // Masks of submodules recorded for each timer.
  uint8_t SmMask[NofTimers];
}