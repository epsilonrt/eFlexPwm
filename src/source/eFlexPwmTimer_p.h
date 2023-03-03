/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwm_p.h"
#include "component/eFlexPwmTimer.h"

namespace eFlex {

  #ifndef DOXYGEN
  extern Timer TM[NofTimers];

  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  extern SubModule *SmList[NofTimers][NofSubmodules];

  // ----------------------------------------------------------------------------
  inline Timer &SubModule::timer() {

    return TM[m_tmidx];
  }

  //-----------------------------------------------------------------------------
  inline void SubModule::start() {

    timer().start (1 << index());
  }

  //-----------------------------------------------------------------------------
  inline void SubModule::stop() {

    timer().stop (1 << index());
  }

  //-----------------------------------------------------------------------------
  inline void SubModule::setPwmLdok (bool value) {

    timer().setPwmLdok (1 << index(), value);
  }

  // ----------------------------------------------------------------------------
  inline bool SubModule::setupPwmPhaseShift (Channel channel, uint32_t pwmFreq_Hz, uint8_t shiftvalue, bool doSync) {
    return (PWM_SetupPwmPhaseShift (ptr(), SM[m_smidx], kPwmChan (channel), pwmFreq_Hz, timer().srcClockHz(), shiftvalue, doSync) == kStatus_Success);
  }

  //-----------------------------------------------------------------------------
  //                                Timer class
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // protected
  inline PWM_Type *Timer::ptr() {

    return m_ptr;
  }

  //-----------------------------------------------------------------------------
  // protected
  inline const PWM_Type *Timer::ptr() const {

    return m_ptr;
  }

  inline uint8_t Timer::index() const {
    return m_tmidx;
  }

  // ----------------------------------------------------------------------------
  inline uint32_t Timer::srcClockHz() const {
    return F_BUS_ACTUAL;
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setLevel (pwm_level_select_t level) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setLevel (level);
      }
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setDeadtime (uint16_t deadtimeValue) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setDeadtime (deadtimeValue);
      }
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setEnable (bool activate) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setEnable (activate);
      }
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setFaultState (pwm_fault_state_t faultState) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setFaultState (faultState);
      }
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::start (uint8_t subModulesToStart) {
    PWM_StartTimer (ptr(), subModulesToStart);
  }

  inline void Timer::start() {
    start (SmMask[m_tmidx]);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::stop (uint8_t subModulesToStop) {
    PWM_StopTimer (ptr(), subModulesToStop);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::stop() {
    stop (SmMask[m_tmidx]);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setPwmLdok (uint8_t subModulesToUpdate, bool value) {
    PWM_SetPwmLdok (ptr(), subModulesToUpdate, value);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setPwmLdok (bool value) {
    setPwmLdok (SmMask[m_tmidx], value);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setupFaultInputFilter (const pwm_fault_input_filter_param_t *faultInputFilterParams) {
    PWM_SetupFaultInputFilter (ptr(), faultInputFilterParams);
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setupFaults (pwm_fault_input_t faultNum, const pwm_fault_param_t *faultParams) {
    PWM_SetupFaults (ptr(), faultNum, faultParams);
  }
  #endif
}
