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

  enum Channel {
    ChanA = 0,
    ChanB = 1
  };

}
