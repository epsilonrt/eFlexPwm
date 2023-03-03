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
  //   Constants
  // ----------------------------------------------------------------------------
  const uint8_t NofTimers = 4; ///< Number of timers in the MCU
  const uint8_t NofSubmodules = 4; ///< number of submodules per timer
  const uint8_t NofPins = 2; ///< max number of pins per sub-module (A/B)

  enum TimerIndex {
    Pwm1 = 0,
    Pwm2,
    Pwm3,
    Pwm4
  };

  enum Channel {
    ChanA = 0,
    ChanB = 1
  };

}
