/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwm_p.h"
#include "eFlexPwmTimer.h"

namespace eFlex {

  #ifndef DOXYGEN
  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  extern SubModule *SmList[NofTimers][NofSubmodules];

  //-----------------------------------------------------------------------------
  // protected
  inline PWM_Type *SubModule::ptr() {

    return m_ptr;
  }

  //-----------------------------------------------------------------------------
  // protected
  inline const PWM_Type *SubModule::ptr() const {

    return m_ptr;
  }

  //-----------------------------------------------------------------------------
  // protected
  inline pwm_signal_param_t *SubModule::kPwmSignals() {

    return m_signal;
  }

  //-----------------------------------------------------------------------------
  // protected
  inline const pwm_signal_param_t *SubModule::kPwmSignals() const {

    return m_signal;
  }

  //-----------------------------------------------------------------------------
  // protected
  inline void SubModule::setSignalToDefault (pwm_signal_param_t &signal) {

    signal.pwmchannelenable = true;
    signal.level = kPWM_HighTrue;
    signal.dutyCyclePercent = 50;
    signal.deadtimeValue = 0;
    signal.faultState = kPWM_PwmFaultState0;
    signal.pwmChannel = kPWM_PwmX;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDutyCyclePercent (Channel channel, uint8_t dutyCyclePercent) {

    m_signal[channel].dutyCyclePercent = dutyCyclePercent;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setLevel (Channel channel, pwm_level_select_t level) {

    m_signal[channel].level = level;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDeadtime (Channel channel, uint16_t deadtimeValue) {

    m_signal[channel].deadtimeValue = deadtimeValue;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setEnable (Channel channel, bool activate) {

    m_signal[channel].pwmchannelenable = activate;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setFaultState (Channel channel, pwm_fault_state_t faultState) {

    m_signal[channel].faultState = faultState;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDutyCyclePercent (uint8_t dutyCyclePercent) {

    for (uint8_t i = 0; i < NofPins; i++) {
      m_signal[i].dutyCyclePercent = dutyCyclePercent;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setLevel (pwm_level_select_t level) {

    for (uint8_t i = 0; i < NofPins; i++) {

      m_signal[i].level = level;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDeadtime (uint16_t deadtimeValue) {

    for (uint8_t i = 0; i < NofPins; i++) {

      m_signal[i].deadtimeValue = deadtimeValue;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setEnable (bool activate) {

    for (uint8_t i = 0; i < NofPins; i++) {

      m_signal[i].pwmchannelenable = activate;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setFaultState (pwm_fault_state_t faultState) {

    for (uint8_t i = 0; i < NofPins; i++) {

      m_signal[i].faultState = faultState;
    }
  }

  // ----------------------------------------------------------------------------
  inline uint8_t SubModule::index() const {

    return m_smidx;
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::updateDutyCyclePercent (uint8_t dutyCyclePercent, Channel channel) {

    PWM_UpdatePwmDutycycle (ptr(), SM[m_smidx], kPwmChan (channel), m_config.mode(), dutyCyclePercent);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::updateDutyCycle (uint16_t dutyCycle, Channel channel) {

    PWM_UpdatePwmDutycycleHighAccuracy (ptr(), SM[m_smidx], kPwmChan (channel), m_config.mode(), dutyCycle);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setupInputCapture (Channel channel, const pwm_input_capture_param_t *inputCaptureParams) {
    PWM_SetupInputCapture (ptr(), SM[m_smidx], kPwmChan (channel), inputCaptureParams);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setupForceSignal (Channel channel, pwm_force_signal_t mode) {
    PWM_SetupForceSignal (ptr(), SM[m_smidx], kPwmChan (channel), mode);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableInterrupts (uint32_t mask) {
    PWM_EnableInterrupts (ptr(), SM[m_smidx], mask);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::disableInterrupts (uint32_t mask) {
    PWM_DisableInterrupts (ptr(), SM[m_smidx], mask);
  }

  // ----------------------------------------------------------------------------
  inline uint32_t SubModule::enabledInterrupts () {
    return PWM_GetEnabledInterrupts (ptr(), SM[m_smidx]);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDMAWatermarkControl (pwm_watermark_control_t pwm_watermark_control) {
    PWM_DMAFIFOWatermarkControl (ptr(), SM[m_smidx], pwm_watermark_control);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDMACaptureSourceSelect (pwm_dma_source_select_t pwm_dma_source_select) {
    PWM_DMACaptureSourceSelect (ptr(), SM[m_smidx], pwm_dma_source_select);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableDMACapture (uint16_t mask, bool activate) {
    PWM_EnableDMACapture (ptr(), SM[m_smidx], mask, activate);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableDMAWrite (bool activate) {
    PWM_EnableDMAWrite (ptr(), SM[m_smidx], activate);
  }

  // ----------------------------------------------------------------------------
  inline uint32_t SubModule::statusFlags () {
    return PWM_GetStatusFlags (ptr(), SM[m_smidx]);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::clearStatusFlags (uint32_t mask) {
    PWM_ClearStatusFlags (ptr(), SM[m_smidx], mask);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setVALxValue (pwm_value_register_t valueRegister, uint16_t value) {
    PWM_SetVALxValue (ptr(), SM[m_smidx], valueRegister,  value);
  }

  // ----------------------------------------------------------------------------
  inline uint16_t SubModule::VALxValue (pwm_value_register_t valueRegister) {
    return PWM_GetVALxValue (ptr(), SM[m_smidx], valueRegister);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableOutputTrigger (pwm_value_register_t valueRegister, bool activate) {
    PWM_OutputTriggerEnable (ptr(), SM[m_smidx], valueRegister, activate);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableOutputTrigger (uint16_t valueRegisterMask) {
    PWM_ActivateOutputTrigger (ptr(), SM[m_smidx], valueRegisterMask);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::disableOutputTrigger (uint16_t valueRegisterMask) {
    PWM_DeactivateOutputTrigger (ptr(), SM[m_smidx], valueRegisterMask);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setupSwCtrlOut (Channel channel, bool value) {
    PWM_SetupSwCtrlOut (ptr(), SM[m_smidx], kPwmChan (channel), value);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setPwmFaultState (Channel channel, pwm_fault_state_t faultState) {
    PWM_SetPwmFaultState (ptr(), SM[m_smidx], kPwmChan (channel),  faultState);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setupFaultDisableMap (Channel channel, pwm_fault_channels_t pwm_fault_channels, uint16_t value) {
    PWM_SetupFaultDisableMap (ptr(), SM[m_smidx], kPwmChan (channel),  pwm_fault_channels,  value);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::enableOutput (Channel channel) {
    PWM_OutputEnable (ptr(), kPwmChan (channel), SM[m_smidx]);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::disableOutput (Channel channel) {
    PWM_OutputDisable (ptr(), kPwmChan (channel), SM[m_smidx]);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t SubModule::dutyCycle (Channel channel) {
    return PWM_GetPwmChannelState (ptr(), SM[m_smidx], kPwmChan (channel));
  }

  // ----------------------------------------------------------------------------
  inline bool SubModule::setOutputToIdle (Channel channel, bool idleStatus) {
    return (PWM_SetOutputToIdle (ptr(), kPwmChan (channel), SM[m_smidx], idleStatus) == kStatus_Success);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setClockMode (pwm_clock_prescale_t prescaler) {
    PWM_SetClockMode (ptr(), SM[m_smidx], prescaler);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setPwmForceOutputToZero (Channel channel, bool forcetozero) {
    PWM_SetPwmForceOutputToZero (ptr(), SM[m_smidx], kPwmChan (channel), forcetozero);
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setChannelOutput (Channel channel, pwm_output_state_t outputstate) {
    PWM_SetChannelOutput (ptr(), SM[m_smidx], kPwmChan (channel),  outputstate);
  }
  #endif
}
