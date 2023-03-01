/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

namespace eFlex {

  #ifndef DOXYGEN
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
