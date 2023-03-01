/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

namespace eFlex {

  #ifndef DOXYGEN

  //-----------------------------------------------------------------------------
  //                            Config class
  //-----------------------------------------------------------------------------

  inline bool Config::enableDebugMode() const {

    return m_config.enableDebugMode;
  }

  // ----------------------------------------------------------------------------
  inline bool Config::enableWait() const {

    return m_config.enableWait;
  }

  // ----------------------------------------------------------------------------
  inline pwm_reload_source_select_t Config::reloadSelect() const {

    return m_config.reloadSelect;
  }

  // ----------------------------------------------------------------------------
  inline pwm_clock_source_t Config::clockSource() const {

    return m_config.clockSource;
  }

  // ----------------------------------------------------------------------------
  inline pwm_clock_prescale_t Config::prescale() const {

    return m_config.prescale;
  }

  // ----------------------------------------------------------------------------
  inline pwm_init_source_t Config::initializationControl() const {

    return m_config.initializationControl;
  }

  // ----------------------------------------------------------------------------
  inline pwm_force_output_trigger_t Config::forceTrigger() const {

    return m_config.forceTrigger;
  }

  // ----------------------------------------------------------------------------
  inline pwm_load_frequency_t Config::reloadFrequency() const {

    return m_config.reloadFrequency;
  }

  // ----------------------------------------------------------------------------
  inline pwm_register_reload_t Config::reloadLogic() const {

    return m_config.reloadLogic;
  }

  // ----------------------------------------------------------------------------
  inline pwm_chnl_pair_operation_t Config::pairOperation() const {

    return m_config.pairOperation;
  }

  // ----------------------------------------------------------------------------
  inline uint32_t Config::pwmFreqHz() const {

    return m_pwmfreq;
  }

  // ----------------------------------------------------------------------------
  inline pwm_mode_t Config::mode() const {

    return m_mode;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setEnableDebugMode (bool enableDebugMode) {
    m_config.enableDebugMode = enableDebugMode;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setEnableWait (bool enableWait) {

    m_config.enableWait = enableWait;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setReloadSelect (pwm_reload_source_select_t reloadSelect) {

    m_config.reloadSelect = reloadSelect;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setClockSource (pwm_clock_source_t clockSource) {

    m_config.clockSource = clockSource;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setPrescale (pwm_clock_prescale_t prescale) {

    m_config.prescale = prescale;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setInitializationControl (pwm_init_source_t initializationControl) {

    m_config.initializationControl = initializationControl;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setForceTrigger (pwm_force_output_trigger_t forceTrigger) {

    m_config.forceTrigger = forceTrigger;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setReloadFrequency (pwm_load_frequency_t reloadFrequency) {

    m_config.reloadFrequency = reloadFrequency;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setReloadLogic (pwm_register_reload_t reloadLogic) {

    m_config.reloadLogic = reloadLogic;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setPairOperation (pwm_chnl_pair_operation_t pairOperation) {

    m_config.pairOperation = pairOperation;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setPwmFreqHz (uint32_t pwmFreqHz) {

    m_pwmfreq = pwmFreqHz;
  }

  // ----------------------------------------------------------------------------
  inline void Config::setMode (pwm_mode_t mode) {

    m_mode = mode;
  }
  #endif
}
