/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwm.h"

namespace eFlexPwm {

  #ifndef DOXYGEN
  //   Variables
  // ----------------------------------------------------------------------------
  extern PWM_Type *PWM[4];
  extern Timer TM[4];
  extern const pwm_submodule_t SM[4];

  //   Code
  // ----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------
  inline bool Pin::isValid() const {

    return (m_number != -1) && (pwmChannel != kPWM_PwmX);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::timerIndex () const {
    return ( (m_module >> 4) & 3);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::submoduleIndex () const {
    return (m_module & 3);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::muxval () const {
    return m_muxval;
  }

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

  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------

  inline Pin &SubModule::pin (uint8_t i) {

    return m_pin[i & 1];
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDutyCyclePercent (uint8_t dutyCyclePercent) {

    for (auto &p : m_pin) {

      p.dutyCyclePercent = dutyCyclePercent;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setLevel (pwm_level_select_t level) {

    for (auto &p : m_pin) {

      p.level = level;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setDeadtime (uint16_t deadtimeValue) {

    for (auto &p : m_pin) {

      p.deadtimeValue = deadtimeValue;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setEnable (bool activate) {

    for (auto &p : m_pin) {

      p.pwmchannelenable = activate;
    }
  }

  // ----------------------------------------------------------------------------
  inline void SubModule::setFaultState (pwm_fault_state_t faultState) {

    for (auto &p : m_pin) {

      p.faultState = faultState;
    }
  }

  // ----------------------------------------------------------------------------
  inline uint8_t SubModule::index() const {

    return m_smidx;
  }

  // ----------------------------------------------------------------------------
  inline Timer &SubModule::timer() {

    return TM[m_tmidx];
  }


  /*
     @brief Updates the PWM signal's dutycycle.

     The function updates the PWM dutycyle to the new value that is passed in.
     If the dead time insertion logic is enabled then the pulse period is reduced by the
     dead time period specified by the user.


     @param pwmSignal         Signal (PWM A or PWM B) to update
     @param dutyCyclePercent  New PWM pulse width, value should be between 0 to 100
                              0=inactive signal(0% duty cycle)...
                              100=active signal (100% duty cycle)
  */
  inline void SubModule::write (uint8_t dutyCyclePercent, pwm_channels_t pwmSignal) {

    PWM_UpdatePwmDutycycle (PWM[m_tmidx], SM[m_smidx], pwmSignal, m_mode, dutyCyclePercent);
  }

  /*
     @brief Updates the PWM signal's dutycycle with 16-bit accuracy.

     The function updates the PWM dutycyle to the new value that is passed in.
     If the dead time insertion logic is enabled then the pulse period is reduced by the
     dead time period specified by the user.


     @param pwmSignal         Signal (PWM A or PWM B) to update
     @param dutyCycle         New PWM pulse width, value should be between 0 to 65535
                              0=inactive signal(0% duty cycle)...
                              65535=active signal (100% duty cycle)
  */
  inline void SubModule::writeHighAccuracy (uint16_t dutyCycle, pwm_channels_t pwmSignal) {
    PWM_UpdatePwmDutycycleHighAccuracy (PWM[m_tmidx], SM[m_smidx], pwmSignal, m_mode, dutyCycle);
  }

  /*
     @brief Set PWM phase shift for PWM channel running on channel PWM_A, PWM_B which with 50% duty cycle..

     @param pwmChannel  PWM channel to configure
     @param pwmFreq_Hz  PWM signal frequency in Hz
     @param shiftvalue  Phase shift value
     @param doSync      true: Set LDOK bit for the submodule list;
                        false: LDOK bit don't set, need to call setPwmLdok to sync update.

     @return Returns false if there was error setting up the signal; true otherwise
  */
  inline bool SubModule::setupPwmPhaseShift (pwm_channels_t pwmChannel, uint32_t pwmFreq_Hz, uint8_t shiftvalue, bool doSync) {
    return (PWM_SetupPwmPhaseShift (PWM[m_tmidx], SM[m_smidx], pwmChannel, pwmFreq_Hz, timer().srcClockHz(), shiftvalue, doSync) == kStatus_Success);
  }

  /*
     @brief Sets up the PWM input capture

     Each PWM submodule has 3 pins that can be configured for use as input capture pins. This function
     sets up the capture parameters for each pin and enables the pin for input capture operation.

     @param pwmChannel         Channel in the submodule to setup
     @param inputCaptureParams Parameters passed in to set up the input pin
  */
  inline void SubModule::setupInputCapture (pwm_channels_t pwmChannel, const pwm_input_capture_param_t *inputCaptureParams) {
    PWM_SetupInputCapture (PWM[m_tmidx], SM[m_smidx], pwmChannel, inputCaptureParams);
  }

  /*
     @brief Selects the signal to output on a PWM pin when a FORCE_OUT signal is asserted.

     The user specifies which channel to configure by supplying the submodule number and whether
     to modify PWM A or PWM B within that submodule.

     @param pwmChannel Channel to configure
     @param mode       Signal to output when a FORCE_OUT is triggered
  */
  inline void SubModule::setupForceSignal (pwm_channels_t pwmChannel, pwm_force_signal_t mode) {
    PWM_SetupForceSignal (PWM[m_tmidx], SM[m_smidx], pwmChannel, mode);
  }

  /*
     @brief Enables the selected PWM interrupts

     @param mask      The interrupts to enable. This is a logical OR of members of the
                      enumeration ::pwm_interrupt_enable_t
  */
  inline void SubModule::enableInterrupts (uint32_t mask) {
    PWM_EnableInterrupts (PWM[m_tmidx], SM[m_smidx], mask);
  }

  /*
     @brief Disables the selected PWM interrupts

     @param mask      The interrupts to enable. This is a logical OR of members of the
                      enumeration ::pwm_interrupt_enable_t
  */
  inline void SubModule::disableInterrupts (uint32_t mask) {
    PWM_DisableInterrupts (PWM[m_tmidx], SM[m_smidx], mask);
  }

  /*
     @brief Gets the enabled PWM interrupts

     @return The enabled interrupts. This is the logical OR of members of the
             enumeration ::pwm_interrupt_enable_t
  */
  inline uint32_t SubModule::enabledInterrupts () {
    return PWM_GetEnabledInterrupts (PWM[m_tmidx], SM[m_smidx]);
  }

  /*
     @brief Capture DMA Enable Source Select.

     @param pwm_watermark_control PWM FIFO watermark and control
  */
  inline void SubModule::setDMAWatermarkControl (pwm_watermark_control_t pwm_watermark_control) {
    PWM_DMAFIFOWatermarkControl (PWM[m_tmidx], SM[m_smidx], pwm_watermark_control);
  }

  /*
     @brief Capture DMA Enable Source Select.

     @param pwm_dma_source_select PWM capture DMA enable source select
  */
  inline void SubModule::setDMACaptureSourceSelect (pwm_dma_source_select_t pwm_dma_source_select) {
    PWM_DMACaptureSourceSelect (PWM[m_tmidx], SM[m_smidx], pwm_dma_source_select);
  }

  /*
     @brief Enables or disables the selected PWM DMA Capture read request.

     @param mask      The DMA to enable or disable. This is a logical OR of members of the
                      enumeration ::pwm_dma_enable_t
     @param activate  true: Enable DMA read request; false: Disable DMA read request
  */
  inline void SubModule::enableDMACapture (uint16_t mask, bool activate) {
    PWM_EnableDMACapture (PWM[m_tmidx], SM[m_smidx], mask, activate);
  }

  /*
     @brief Enables or disables the PWM DMA write request.

     @param activate  true: Enable DMA write request; false: Disable DMA write request
  */
  inline void SubModule::enableDMAWrite (bool activate) {
    PWM_EnableDMAWrite (PWM[m_tmidx], SM[m_smidx], activate);
  }

  /*
     @brief Gets the PWM status flags

     @return The status flags. This is the logical OR of members of the
             enumeration ::pwm_status_flags_t
  */
  inline uint32_t SubModule::statusFlags () {
    return PWM_GetStatusFlags (PWM[m_tmidx], SM[m_smidx]);
  }

  /*
     @brief Clears the PWM status flags

     @param mask      The status flags to clear. This is a logical OR of members of the
                      enumeration ::pwm_status_flags_t
  */
  inline void SubModule::clearStatusFlags (uint32_t mask) {
    PWM_ClearStatusFlags (PWM[m_tmidx], SM[m_smidx], mask);
  }

  /*
     @brief Set the PWM VALx registers.

     This function allows the user to write value into VAL registers directly. And it will destroying the PWM clock period
     set by the setupPwm()/PWM_SetupPwmPhaseShift() functions.
     Due to VALx registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
     reached.

     @param valueRegister VALx register that will be writen new value
     @param value         Value that will been write into VALx register
  */
  inline void SubModule::setVALxValue (pwm_value_register_t valueRegister, uint16_t value) {
    PWM_SetVALxValue (PWM[m_tmidx], SM[m_smidx], valueRegister,  value);
  }

  /*
     @brief Get the PWM VALx registers.

     @param valueRegister VALx register that will be read value
     @return The VALx register value
  */
  inline uint16_t SubModule::VALxValue (pwm_value_register_t valueRegister) {
    return PWM_GetVALxValue (PWM[m_tmidx], SM[m_smidx], valueRegister);
  }

  /*
     @brief Enables or disables the PWM output trigger.

     This function allows the user to enable or disable the PWM trigger. The PWM has 2 triggers. Trigger 0
     is activated when the counter matches VAL 0, VAL 2, or VAL 4 register. Trigger 1 is activated
     when the counter matches VAL 1, VAL 3, or VAL 5 register.

     @param valueRegister Value register that will activate the trigger
     @param activate      true: Enable the trigger; false: Disable the trigger
  */
  inline void SubModule::enableOutputTrigger (pwm_value_register_t valueRegister, bool activate) {
    PWM_OutputTriggerEnable (PWM[m_tmidx], SM[m_smidx], valueRegister, activate);
  }

  /*
     @brief Enables the PWM output trigger.

     This function allows the user to enable one or more (VAL0-5) PWM trigger.

     @param valueRegisterMask Value register mask that will activate one or more (VAL0-5) trigger
                              enumeration ::_pwm_value_register_mask
  */
  inline void SubModule::enableOutputTrigger (uint16_t valueRegisterMask) {
    PWM_ActivateOutputTrigger (PWM[m_tmidx], SM[m_smidx], valueRegisterMask);
  }

  /*
     @brief Disables the PWM output trigger.

     This function allows the user to disables one or more (VAL0-5) PWM trigger.

     @param valueRegisterMask Value register mask that will Deactivate one or more (VAL0-5) trigger
                              enumeration ::_pwm_value_register_mask
  */
  inline void SubModule::disableOutputTrigger (uint16_t valueRegisterMask) {
    PWM_DeactivateOutputTrigger (PWM[m_tmidx], SM[m_smidx], valueRegisterMask);
  }

  /*
     @brief Sets the software control output for a pin to high or low.

     The user specifies which channel to modify by supplying the submodule number and whether
     to modify PWM A or PWM B within that submodule.

     @param pwmChannel Channel to configure
     @param value      true: Supply a logic 1, false: Supply a logic 0.
  */
  inline void SubModule::setupSwCtrlOut (pwm_channels_t pwmChannel, bool value) {
    PWM_SetupSwCtrlOut (PWM[m_tmidx], SM[m_smidx], pwmChannel, value);
  }


  /*
     @brief Set PWM output fault status

     These bits determine the fault state for the PWM_A output in fault conditions
     and STOP mode. It may also define the output state in WAIT and DEBUG modes
     depending on the settings of CTRL2[WAITEN] and CTRL2[DBGEN].
     This function can update PWM output fault status.

     @param pwmChannel Channel to configure
     @param faultState PWM output fault status
  */
  inline void SubModule::setPwmFaultState (pwm_channels_t pwmChannel, pwm_fault_state_t faultState) {
    PWM_SetPwmFaultState (PWM[m_tmidx], SM[m_smidx], pwmChannel,  faultState);
  }

  /*
     @brief Set PWM fault disable mapping

     Each of the four bits of this read/write field is one-to-one associated
     with the four FAULTx inputs of fault channel 0/1. The PWM output will be turned
     off if there is a logic 1 on an FAULTx input and a 1 in the corresponding
     bit of this field. A reset sets all bits in this field.

     @param pwmChannel         PWM channel to configure
     @param pwm_fault_channels PWM fault channel to configure
     @param value              Fault disable mapping mask value
                               enumeration ::pwm_fault_disable_t
  */
  inline void SubModule::setupFaultDisableMap (pwm_channels_t pwmChannel, pwm_fault_channels_t pwm_fault_channels, uint16_t value) {
    PWM_SetupFaultDisableMap (PWM[m_tmidx], SM[m_smidx], pwmChannel,  pwm_fault_channels,  value);
  }

  /*
     @brief Set PWM output enable

     This feature allows the user to enable the PWM Output.

     @param pwmChannel         PWM channel to configure
  */
  inline void SubModule::enableOutput (pwm_channels_t pwmChannel) {
    PWM_OutputEnable (PWM[m_tmidx], pwmChannel, SM[m_smidx]);
  }

  /*
     @brief Set PWM output disable

    This feature allows the user to disable the PWM output.

     @param pwmChannel         PWM channel to configure
  */
  inline void SubModule::disableOutput (pwm_channels_t pwmChannel) {
    PWM_OutputDisable (PWM[m_tmidx], pwmChannel, SM[m_smidx]);
  }

  /*
     @brief Get the dutycycle value.

     @param pwmChannel  PWM channel to configure

     @return Current channel dutycycle value.
  */
  inline uint8_t SubModule::dutyCycle (pwm_channels_t pwmChannel) {
    return PWM_GetPwmChannelState (PWM[m_tmidx], SM[m_smidx], pwmChannel);
  }

  /*
     @brief Set PWM output in idle status (high or low).

     @note This API should call after setupPwm() APIs, and PWMX submodule is not supported.

     @param pwmChannel         PWM channel to configure
     @param idleStatus         true: PWM output is high in idle status; false: PWM output is low in idle status.

     @return false if there was error setting up the signal; true if set output idle success
  */
  inline bool SubModule::setOutputToIdle (pwm_channels_t pwmChannel, bool idleStatus) {
    return (PWM_SetOutputToIdle (PWM[m_tmidx], pwmChannel, SM[m_smidx], idleStatus) == kStatus_Success);
  }

  /*
     @brief Set the pwm submodule prescaler.

     @param prescaler          Set prescaler value
  */
  inline void SubModule::setClockMode (pwm_clock_prescale_t prescaler) {
    PWM_SetClockMode (PWM[m_tmidx], SM[m_smidx], prescaler);
  }

  /*
     @brief This function enables-disables the forcing of the output of a given eFlexPwm channel to logic 0.

     @param pwmChannel         PWM channel to configure
     @param forcetozero        true: Enable the pwm force output to zero; false: Disable the pwm output resumes normal
                               function.
  */
  inline void SubModule::setPwmForceOutputToZero (pwm_channels_t pwmChannel, bool forcetozero) {
    PWM_SetPwmForceOutputToZero (PWM[m_tmidx], SM[m_smidx], pwmChannel, forcetozero);
  }

  /*
     @brief This function set the output state of the PWM pin as requested for the current cycle.

     @param pwmChannel         PWM channel to configure
     @param outputstate        Set pwm output state, see @ref pwm_output_state_t.
  */
  inline void SubModule::setChannelOutput (pwm_channels_t pwmChannel, pwm_output_state_t outputstate) {
    PWM_SetChannelOutput (PWM[m_tmidx], SM[m_smidx], pwmChannel,  outputstate);
  }

  //-----------------------------------------------------------------------------
  //                                Timer class
  //-----------------------------------------------------------------------------

  inline uint8_t Timer::index() const {
    return m_tmidx;
  }

  /*
     @brief PWM main counter clock in Hz.
  */
  inline uint32_t Timer::srcClockHz() const {
    return F_BUS_ACTUAL;
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setDutyCyclePercent (uint8_t dutyCyclePercent) {

    for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

      it->second->setDutyCyclePercent (dutyCyclePercent);
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setLevel (pwm_level_select_t level) {

    for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

      it->second->setLevel (level);
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setDeadtime (uint16_t deadtimeValue) {

    for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

      it->second->setDeadtime (deadtimeValue);
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setEnable (bool activate) {

    for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

      it->second->setEnable (activate);
    }
  }

  // ----------------------------------------------------------------------------
  inline void Timer::setFaultState (pwm_fault_state_t faultState) {

    for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

      it->second->setFaultState (faultState);
    }
  }

  /*
     @brief Starts the PWM counter for a single or multiple submodules.

     Sets the Run bit which enables the clocks to the PWM submodule. This function can start multiple
     submodules at the same time.

     @param subModulesToStart PWM submodules to start. This is a logical OR of members of the
                              enumeration ::pwm_module_control_t
  */
  inline void Timer::start (uint8_t subModulesToStart) {
    PWM_StartTimer (PWM[m_tmidx], subModulesToStart);
  }

  inline void Timer::start() {
    start (m_smmask);
  }

  /*
     @brief Stops the PWM counter for a single or multiple submodules.

     Clears the Run bit which resets the submodule's counter. This function can stop multiple
     submodules at the same time.

     @param subModulesToStop PWM submodules to stop. This is a logical OR of members of the
                             enumeration ::pwm_module_control_t
  */
  inline void Timer::stop (uint8_t subModulesToStop) {
    PWM_StopTimer (PWM[m_tmidx], subModulesToStop);
  }

  inline void Timer::stop() {
    stop (m_smmask);
  }

  /*
     @brief Sets or clears the PWM LDOK bit on a single or multiple submodules

     Set LDOK bit to load buffered values into CTRL[PRSC] and the INIT, FRACVAL and VAL registers. The
     values are loaded immediately if kPWM_ReloadImmediate option was choosen during config. Else the
     values are loaded at the next PWM reload point.
     This function can issue the load command to multiple submodules at the same time.

     @param subModulesToUpdate PWM submodules to update with buffered values. This is a logical OR of
                               members of the enumeration ::pwm_module_control_t
     @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
  */
  inline void Timer::setPwmLdok (uint8_t subModulesToUpdate, bool value) {
    PWM_SetPwmLdok (PWM[m_tmidx], subModulesToUpdate, value);
  }

  inline void Timer::setPwmLdok (bool value) {
    setPwmLdok (m_smmask, value);
  }

  /*
     @brief Sets up the PWM fault input filter.

     @param faultInputFilterParams Parameters passed in to set up the fault input filter.
  */
  inline void Timer::setupFaultInputFilter (const pwm_fault_input_filter_param_t *faultInputFilterParams) {
    PWM_SetupFaultInputFilter (PWM[m_tmidx], faultInputFilterParams);
  }

  /*
     @brief Sets up the PWM fault protection.

     PWM has 4 fault inputs.

     @param faultNum    PWM fault to configure.
     @param faultParams Pointer to the PWM fault config structure
  */
  inline void Timer::setupFaults (pwm_fault_input_t faultNum, const pwm_fault_param_t *faultParams) {
    PWM_SetupFaults (PWM[m_tmidx], faultNum, faultParams);
  }
  #endif
}
