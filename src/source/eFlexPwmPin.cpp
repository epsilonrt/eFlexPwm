/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmPin.h"

#include <imxrt.h>
#include <core_pins.h>

struct pwm_pin_info_struct {
  uint8_t type;    // 0=no pwm, 1=flexpwm, 2=quad
  uint8_t module;  // 0-3, 0-3
  uint8_t channel; // 0=X, 1=A, 2=B
  uint8_t muxval;  //
};

extern const struct pwm_pin_info_struct pwm_pin_info[];

namespace eFlex {

  //-----------------------------------------------------------------------------
  const struct pwm_pin_info_struct *pinInfo (uint8_t pin) {

    if (pin < CORE_NUM_DIGITAL) {
      const struct pwm_pin_info_struct *info = &pwm_pin_info[pin];

      if ( (info->type == 1) && (info->channel != 0)) {

        return info;
      }
    }
    return nullptr;
  }

  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------
  Pin::Pin (int number) {

    if (number != -1) {
      const struct pwm_pin_info_struct *info = pinInfo (number);

      if (info) {

        if (info->channel != 0) {

          m_channel = (info->channel == 1 ? ChanA : ChanB);
          m_number = number;
          m_module = info->module;
          m_muxval = info->muxval;
        }
      }
    }
  }

  //-----------------------------------------------------------------------------
  bool Pin::begin() {

    if (isValid()) {

      * (portConfigRegister (m_number)) = m_muxval;
    }
    return isValid();
  }
}