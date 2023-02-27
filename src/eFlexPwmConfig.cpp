/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "eFlexPwmConfig.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                Config class
  //-----------------------------------------------------------------------------
  Config::Config() :
    m_mode (kPWM_CenterAligned), m_pwmfreq (5000) {

    PWM_GetDefaultConfig (& m_config);
  }

  // ----------------------------------------------------------------------------
  // protected
  pwm_config_t *Config::kPwmConfig() {

    return &m_config;
  }

  // ----------------------------------------------------------------------------
  // protected
  const pwm_config_t *Config::kPwmConfig() const {

    return &m_config;
  }
}