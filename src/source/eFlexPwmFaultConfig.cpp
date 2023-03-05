/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmFaultConfig.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                FaultConfig class
  //-----------------------------------------------------------------------------
  FaultConfig::FaultConfig()  {

    PWM_FaultDefaultConfig (& m_config);
  }

  // ----------------------------------------------------------------------------
  // protected
  pwm_fault_param_t *FaultConfig::kPwmConfig() {

    return &m_config;
  }

  // ----------------------------------------------------------------------------
  // protected
  const pwm_fault_param_t *FaultConfig::kPwmConfig() const {

    return &m_config;
  }
}