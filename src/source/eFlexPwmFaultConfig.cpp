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
    m_filter.faultGlitchStretch = false;
    m_filter.faultFilterCount = 0;
    m_filter.faultFilterPeriod = 0;
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

  // ----------------------------------------------------------------------------
  // protected
  pwm_fault_input_filter_param_t *FaultConfig::kPwmFilter() {

    return &m_filter;
  }

  // ----------------------------------------------------------------------------
  // protected
  const pwm_fault_input_filter_param_t *FaultConfig::kPwmFilter() const {

    return &m_filter;
  }
}