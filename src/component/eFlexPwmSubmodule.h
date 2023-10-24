/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmPin.h"
#include "eFlexPwmConfig.h"
#include "eFlexPwmTimer.h"

namespace eFlex {

  /**
    @brief Submodule

    This class is used to perform most operations.
  */
  class SubModule {
    public:
      /**
        @brief Construct a new SubModule object

         If the pin is valid (eFlexPwm type), its channel is configured with default values:
         pwmchannelenable = true;
         level = kPWM_HighTrue;
         dutyCycle = 32768;
         deadtimeValue = 0;
         faultState = kPWM_PwmFaultState0;

        @param pinChanA Arduino pin number of output A, this pin must correspond to an output A (23) of an eFlexPWM sub-module (refer to the datasheet)
        @param pinChanB Arduino pin number of output B, this pin must correspond to output B (45) of the same submodule as pinA (refer to datasheet)
      */
      SubModule (int pinChanA, int pinChanB = -1);

      /**
        @brief Checks the validity of the parameters passed to the constructor

        @warning If this function returns false, the call to functions \ref configure and \ref begin will fail.

        @return returns true if all of these conditions are true:
          - the pins passed to the constructor are of type eFlexPWM
          - pinA corresponds to output A of the submodule
          - pinB corresponds to output B of the submodule
          .
          false otherwise
      */
      bool isValid() const;

      /**
        @brief Configures the peripheral for basic operation.

         @note This API should be called at the beginning of the application using the PWM driver.

        @param config Reference to user's PWM config structure.
        @return true means success; if false is returned, \ref begin should not be called !
      */
      bool configure (const Config &config);

      /**
         @brief Current configuration
      */
      inline const Config &config() const {
        return m_config;
      }

      /**
        @brief Sets up the PWM signals for a PWM submodule.

          The function initializes the submodule according to the parameters passed in by the user. The function
          also sets up the value compare registers to match the PWM signal requirements.
          If the dead time insertion logic is enabled, the pulse period is reduced by the
          dead time period specified by the user.

        @param doStart starts signal generation
        @param doSync synchronizes the operation by surrounding it with setPwmLdok (false) / setPwmLdok (true)

        @return Returns false if there was error setting up the signal; true otherwise
      */
      bool begin (bool doStart = true, bool doSync = true);

      /**
        @brief Enable or disable the submodule

         This function allows you to enable/disable the output pins without
         changing anything in the configuration. When the submodule is
         disabled, its output pins are forced to zero.

        @param value true to enable, false otherwise
      */
      void enable (bool value = true);

      /**
        @brief Disable the submodule

         This function allows you to disable the output pins without
         changing anything in the configuration. When the submodule is
         disabled, its output pins are forced to zero.
      */
      inline void disable () {
        enable (false);
      }

      /**
        @brief Returns true if the outputsof  submodule were enabled
      */
      bool isEnabled() const;

      /**
        @brief Starts the PWM counter for this submodule

        @note This operation is not useful if \c begin() was called with doStart=true
      */
      inline void start (bool value = true) {
        timer().start (1 << index(), value);
      }


      /**
        @brief Stops the PWM counter for this submodule
      */
      inline void stop () {
        start (false);
      }

      /**
        @brief Sets or clears the PWM LDOK bit on all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doSync=true

        @param value true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value = true) {

        timer().setPwmLdok (1 << index(), value);
      }

      /**
        @brief Returns the parent PWM module
      */
      inline Timer &timer() {
        return *TM[m_tmidx];
      }

      /**
        @brief Returns the Submodule index (0 for Submodule0...)
      */
      inline uint8_t index() const {
        return m_smidx;
      }

      /**
        @brief Set the Pwm Frequency

        @param freq PWM signal frequency in Hz
        @param doSync true : Set LDOK bit for the submodule;
                      false: LDOK bit don't set, need to call setPwmLdok to sync update.
        @param adjustPrescaler false: no change prescaler, if freq is less than minPwmFrequency(), nothing is done and false is returned;
                            true: calls adjustPrescaler() to try to adjust the prescaler, the prescaler is adjusted if possible, otherwise, false is returned.
        @return false if there was error setting up the signal; true otherwise
      */
      bool setPwmFrequency (uint32_t freq, bool doSync = true, bool adjustPrescaler = false);

      /**
         @brief Current PWM signal frequency in Hz

         @return uint32_t
      */
      inline uint32_t pwmFrequency() const {
        return config().pwmFreqHz();
      }

      /**
        @brief Set the Pwm operation mode

        @param mode PWM operation mode
        @param doSync      true: Set LDOK bit for the submodule;
                            false: LDOK bit don't set, need to call setPwmLdok to sync update.

        @return Returns false if there was error setting up the signal; true otherwise
      */
      // TODO: Test setPwmMode()
      inline bool setPwmMode (pwm_mode_t  mode, bool doSync = true) {
        m_config.setMode (mode);
        return updateSetting (doSync);
      }

      /**
         @brief Current PWM signal frequency in Hz

         @return uint32_t
      */
      inline pwm_mode_t pwmMode() const {
        return config().mode();
      }

      /**
        @brief Setting output enable before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param channel PWM channel being setting; ChanA or ChanB
        @param activate true to enable the output
      */
      inline void setupOutputEnable (Channel channel, bool activate = true) {

        m_signal[channel].pwmchannelenable = activate;
      }

      /**
        @brief Setting output enable for all channels before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param activate true to enable the outputs
      */
      inline void setupOutputEnable (bool activate = true) {
        for (uint8_t i = ChanA; i <= ChanB ; i++) {
          setupOutputEnable (static_cast<Channel> (i), activate);
        }
      }

      /**
         @brief value of the parameter set by \ref setupOutputEnable (Channel, bool)

         @param channel PWM channel to read; ChanA or ChanB
      */
      bool outputEnableSetting (Channel channel) {
        return m_signal[channel].pwmchannelenable;
      }

      /**
        @brief Setting the duty cycle before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param channel PWM channel being setting; ChanA or ChanB
        @param dutyCycle duty cycle between 0-65535
      */
      inline void setupDutyCycle (Channel channel, uint16_t dutyCycle) {
        m_duty[static_cast<uint8_t> (channel)] = dutyCycle; // [0, 65535]
        m_signal[static_cast<uint8_t> (channel)].dutyCycle = dutyCycle;
      }

      /**
        @brief Setting the duty cycle for all channels before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param dutyCycle duty cycle between 0-65535
      */
      inline void setupDutyCycle (uint16_t dutyCycle) {
        for (uint8_t i = ChanA; i <= ChanB ; i++) {
          setupDutyCycle (static_cast<Channel> (i), dutyCycle);
        }
      }

      /**
         @brief value of the parameter set by \ref setupDutyCycle (Channel, uint16_t)

         @param channel PWM channel to read; ChanA or ChanB
      */
      uint16_t dutyCycleSetting (Channel channel) {
        return m_signal[channel].dutyCycle;
      }

      /**
        @brief Setting the output pulse mode before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param channel PWM channel being setting; ChanA or ChanB
        @param level output pulse mode
      */
      inline void setupLevel (Channel channel, pwm_level_select_t level) {
        m_signal[channel].level = level;
      }

      /**
        @brief Setting the output pulse mode for all channels before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param level output pulse mode
      */
      inline void setupLevel (pwm_level_select_t level) {
        for (uint8_t i = ChanA; i <= ChanB ; i++) {
          setupLevel (static_cast<Channel> (i), level);
        }
      }

      /**
         @brief value of the parameter set by \ref setupLevel (Channel, pwm_level_select_t)

         @param channel PWM channel to read; ChanA or ChanB
      */
      pwm_level_select_t levelSetting (Channel channel) {
        return m_signal[channel].level;
      }

      /**
        @brief Setting the deadtime before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param channel PWM channel being setting; ChanA or ChanB
        @param deadtimeValue The deadtime value in clock cycles; only used if channel pair is operating in complementary mode
        @param unit The unit of the deadtimeValue; 1: clock cycles; 1000: milliseconds; 1000000: microseconds; 1000000000: nanoseconds
      */
      inline void setupDeadtime (Channel channel, uint16_t deadtimeValue, uint32_t unit = 1) {
        if (unit == 1) {
          m_signal[channel].deadtimeValue = deadtimeValue;
        }
        else {
          m_signal[channel].deadtimeValue = ( (uint64_t) timer().srcClockHz() * deadtimeValue) / unit;
        }
      }

      /**
        @brief Setting the deadtime for all channels before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param deadtimeValue The deadtime value in clock cycles; only used if channel pair is operating in complementary mode
        @param unit The unit of the deadtimeValue; 1: clock cycles; 1000: milliseconds; 1000000: microseconds; 1000000000: nanoseconds
      */
      inline void setupDeadtime (uint16_t deadtimeValue, uint32_t unit = 1) {
        for (uint8_t i = ChanA; i <= ChanB ; i++) {
          setupDeadtime (static_cast<Channel> (i), deadtimeValue, unit);
        }
      }

      /**
         @brief value of the parameter set by \ref setupDeadtime (Channel, uint16_t)

         @param channel PWM channel to read; ChanA or ChanB
      */
      uint16_t deadtimeSetting (Channel channel) {
        return m_signal[channel].deadtimeValue;
      }

      /**
        @brief Setting the output fault status before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param channel PWM channel being setting; ChanA or ChanB
        @param faultState the output fault status
      */
      inline void setupFaultState (Channel channel, pwm_fault_state_t faultState) {
        m_signal[channel].faultState = faultState;
      }

      /**
        @brief Setting the output fault status for all channels before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param faultState the output fault status
      */
      inline void setupFaultState (pwm_fault_state_t faultState) {
        for (uint8_t i = ChanA; i <= ChanB ; i++) {
          setupFaultState (static_cast<Channel> (i), faultState);
        }
      }

      /**
         @brief value of the parameter set by \ref setupFaultState (Channel, pwm_fault_state_t)

         @param channel PWM channel to read; ChanA or ChanB
      */
      inline pwm_fault_state_t faultStateSetting (Channel channel) {
        return m_signal[channel].faultState;
      }

      /*!
        @brief Update PWM signals for a PWM submodule.

         The function initializes the submodule according to the parameters passed in by the user. The function
         also sets up the value compare registers to match the PWM signal requirements.
         If the dead time insertion logic is enabled, the pulse period is reduced by the
         dead time period specified by the user.

        @param doSync      true: Set LDOK bit for the submodule;
                            false: LDOK bit don't set, need to call setPwmLdok to sync update.

        @return Returns false if there was error setting up the signal; true otherwise
      */
      bool updateSetting (bool doSync = true);

      /**
        @brief Set the PWM INIT register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setInitValue (uint16_t value) {
        m_ptr->SM[m_smidx].INIT = value;
      }

      /**
        @brief Set the PWM VAL0 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal0Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL0 = value;
      }

      /**
        @brief Set the PWM VAL1 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal1Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL1 = value;
      }

      /**
        @brief Set the PWM VAL2 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal2Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL2 = value;
      }

      /**
        @brief Set the PWM VAL3 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal3Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL3 = value;
      }

      /**
        @brief Set the PWM VAL4 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal4Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL4 = value;
      }

      /**
        @brief Set the PWM VAL5 register.

         This function allows the user to write value into registers directly. And it will destroying the PWM clock period
         set by the begin()/updateSetting()/setupPwmPhaseShift() functions.
         Due to the registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param value Value that will been write into register
      */
      inline void setVal5Value (uint16_t value) {
        m_ptr->SM[m_smidx].VAL5 = value;
      }

      /**
        @brief Get the PWM INIT register.

        @return The register value
      */
      inline uint16_t initValue () {
        return m_ptr->SM[m_smidx].INIT;
      }

      /**
        @brief Get the PWM VAL1 register.

        @return The register value
      */
      inline uint16_t val0Value () {
        return m_ptr->SM[m_smidx].VAL0;
      }

      /**
        @brief Get the PWM VAL1 register.

        @return The register value
      */
      inline uint16_t val1Value () {
        return m_ptr->SM[m_smidx].VAL1;
      }

      /**
        @brief Get the PWM VAL2 register.

        @return The register value
      */
      inline uint16_t val2Value () {
        return m_ptr->SM[m_smidx].VAL2;
      }

      /**
        @brief Get the PWM VAL3 register.

        @return The register value
      */
      inline uint16_t val3Value () {
        return m_ptr->SM[m_smidx].VAL3;
      }

      /**
        @brief Get the PWM VAL4 register.

        @return The register value
      */
      inline uint16_t val4Value () {
        return m_ptr->SM[m_smidx].VAL4;
      }

      /**
        @brief Get the PWM VAL5 register.

        @return The register value
      */
      inline uint16_t val5Value () {
        return m_ptr->SM[m_smidx].VAL5;
      }

      /**
        @brief Print submodule registers to stream out

        @param out the output stream, Serial by default
      */
      void printRegs (Stream &out = Serial);

      //-----------------------------------------------------------------------
      //                            NXP SDK WRAPPER
      //-----------------------------------------------------------------------

      /**
        @brief Updates the PWM signal's dutycycle with 16-bit accuracy.

         The function updates the PWM dutycyle to the new value that is passed in.
         If the dead time insertion logic is enabled then the pulse period is reduced by the
         dead time period specified by the user.


        @param channel         Channel (PWM A or PWM B) to update
        @param dutyCycle         New PWM pulse width, value should be between 0 to 65535
                                  0=inactive signal(0% duty cycle)...
                                  65535=active signal (100% duty cycle)
      */
      inline void updateDutyCycle (uint16_t dutyCycle, Channel channel = ChanA) {

        m_duty[static_cast<uint8_t> (channel)] = dutyCycle; // [0, 65535]
        PWM_UpdatePwmDutycycle (ptr(), SM[m_smidx], kPwmChan (channel), m_config.m_mode, dutyCycle);
      }

      /**
        @brief Set PWM phase shift for PWM channel running on channel PWM_A, PWM_B which with 50% duty cycle..

        @param channel  PWM channel to configure
        @param shiftvalue  Phase shift value
        @param doSync      true: Set LDOK bit for the submodule list;
                            false: LDOK bit don't set, need to call setPwmLdok to sync update.

        @return Returns false if there was error setting up the signal; true otherwise
      */
      inline bool setupPwmPhaseShift (Channel channel, uint8_t shiftvalue, bool doSync = true) {

        return (PWM_SetupPwmPhaseShift (ptr(), SM[m_smidx], kPwmChan (channel), m_config.pwmFreqHz(), timer().srcClockHz(), shiftvalue, doSync) == kStatus_Success);
      }

      /**
        @brief This function set the phase delay from the master sync signal of submodule 0.

        @param channel  PWM channel to configure
        @param delayCycles  Phase delay value

        @return Returns false if there was error setting up the signal; true otherwise
      */
#if defined(FSL_FEATURE_PWM_HAS_PHASE_DELAY) && FSL_FEATURE_PWM_HAS_PHASE_DELAY
      inline bool setPhaseDelay(Channel channel, uint16_t delayCycles) {

        return (PWM_SetPhaseDelay (ptr(), SM[m_smidx], kPwmChan (channel), delayCycles) == kStatus_Success);
      }
#endif /* FSL_FEATURE_PWM_HAS_PHASE_DELAY */

      /**
        @brief Sets up the PWM input capture

         Each PWM submodule has 3 pins that can be configured for use as input capture pins. This function
         sets up the capture parameters for each pin and enables the pin for input capture operation.

        @param channel         Channel in the submodule to setup
        @param inputCaptureParams Parameters passed in to set up the input pin
      */
      inline void setupInputCapture (Channel channel, const pwm_input_capture_param_t *inputCaptureParams) {
        PWM_SetupInputCapture (ptr(), SM[m_smidx], kPwmChan (channel), inputCaptureParams);
      }

      /**
        @brief Selects the signal to output on a PWM pin when a FORCE_OUT signal is asserted.

         The user specifies which channel to configure by supplying the submodule number and whether
         to modify PWM A or PWM B within that submodule.

        @param channel Channel to configure
        @param mode       Signal to output when a FORCE_OUT is triggered
      */
      inline void setupForceSignal (Channel channel, pwm_force_signal_t mode) {
        PWM_SetupForceSignal (ptr(), SM[m_smidx], kPwmChan (channel), mode);
      }

      /**
        @brief Gets the PWM status flags

        @return The status flags. This is the logical OR of members of the
                 enumeration ::pwm_status_flags_t
      */
      inline uint32_t statusFlags () {
        return PWM_GetStatusFlags (ptr(), SM[m_smidx]);
      }

      /**
        @brief Clears the PWM status flags

        @param mask      The status flags to clear. This is a logical OR of members of the
                          enumeration ::pwm_status_flags_t
      */
      inline void clearStatusFlags (uint32_t mask) {
        PWM_ClearStatusFlags (ptr(), SM[m_smidx], mask);
      }

      /**
        @brief Enables the selected PWM interrupts

        @param mask      The interrupts to enable. This is a logical OR of members of the
                          enumeration ::pwm_interrupt_enable_t
      */
      inline void enableInterrupts (uint32_t mask) {
        PWM_EnableInterrupts (ptr(), SM[m_smidx], mask);
      }

      /**
        @brief Disables the selected PWM interrupts

        @param mask      The interrupts to enable. This is a logical OR of members of the
                          enumeration ::pwm_interrupt_enable_t
      */
      inline void disableInterrupts (uint32_t mask) {
        PWM_DisableInterrupts (ptr(), SM[m_smidx], mask);
      }

      /**
        @brief Gets the enabled PWM interrupts

        @return The enabled interrupts. This is the logical OR of members of the
                 enumeration ::pwm_interrupt_enable_t
      */
      inline uint32_t enabledInterrupts () {
        return PWM_GetEnabledInterrupts (ptr(), SM[m_smidx]);
      }

      /**
        @brief Capture DMA Enable Source Select.

        @param pwm_watermark_control PWM FIFO watermark and control
      */
      inline void setDMAWatermarkControl (pwm_watermark_control_t pwm_watermark_control) {
        PWM_DMAFIFOWatermarkControl (ptr(), SM[m_smidx], pwm_watermark_control);
      }

      /**
        @brief Capture DMA Enable Source Select.

        @param pwm_dma_source_select PWM capture DMA enable source select
      */
      inline void setDMACaptureSourceSelect (pwm_dma_source_select_t pwm_dma_source_select) {
        PWM_DMACaptureSourceSelect (ptr(), SM[m_smidx], pwm_dma_source_select);
      }

      /**
        @brief Enables or disables the selected PWM DMA Capture read request.

        @param mask      The DMA to enable or disable. This is a logical OR of members of the
                          enumeration ::pwm_dma_enable_t
        @param activate  true: Enable DMA read request; false: Disable DMA read request
      */
      inline void enableDMACapture (uint16_t mask, bool activate = true) {
        PWM_EnableDMACapture (ptr(), SM[m_smidx], mask, activate);
      }

      /**
        @brief Enables or disables the PWM DMA write request.

        @param activate  true: Enable DMA write request; false: Disable DMA write request
      */
      inline void enableDMAWrite (bool activate) {
        PWM_EnableDMAWrite (ptr(), SM[m_smidx], activate);
      }

      /**
        @brief Set the PWM VALx registers.

         This function allows the user to write value into VAL registers directly. And it will destroying the PWM clock period
         set by the setupPwm()/PWM_SetupPwmPhaseShift() functions.
         Due to VALx registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

        @param valueRegister VALx register that will be writen new value
        @param value         Value that will been write into VALx register
      */
      inline void setVALxValue (pwm_value_register_t valueRegister, uint16_t value) {
        PWM_SetVALxValue (ptr(), SM[m_smidx], valueRegister,  value);
      }

      /**
        @brief Get the PWM VALx registers.

        @param valueRegister VALx register that will be read value
        @return The VALx register value
      */
      inline uint16_t VALxValue (pwm_value_register_t valueRegister) {
        return PWM_GetVALxValue (ptr(), SM[m_smidx], valueRegister);
      }

      /**
        @brief Enables or disables the PWM output trigger.

         This function allows the user to enable or disable the PWM trigger. The PWM has 2 triggers. Trigger 0
         is activated when the counter matches VAL 0, VAL 2, or VAL 4 register. Trigger 1 is activated
         when the counter matches VAL 1, VAL 3, or VAL 5 register.

        @param valueRegister Value register that will activate the trigger
        @param activate      true: Enable the trigger; false: Disable the trigger
      */
      inline void enableOutputTrigger (pwm_value_register_t valueRegister, bool activate = true) {
        PWM_OutputTriggerEnable (ptr(), SM[m_smidx], valueRegister, activate);
      }

      /**
        @brief Enables the PWM output trigger.

         This function allows the user to enable one or more (VAL0-5) PWM trigger.

        @param valueRegisterMask Value register mask that will activate one or more (VAL0-5) trigger
                                  enumeration ::_pwm_value_register_mask
      */
      inline void enableOutputTrigger (uint16_t valueRegisterMask) {
        PWM_ActivateOutputTrigger (ptr(), SM[m_smidx], valueRegisterMask);
      }

      /**
        @brief Disables the PWM output trigger.

         This function allows the user to disables one or more (VAL0-5) PWM trigger.

        @param valueRegisterMask Value register mask that will Deactivate one or more (VAL0-5) trigger
                                  enumeration ::_pwm_value_register_mask
      */
      inline void disableOutputTrigger (uint16_t valueRegisterMask) {
        PWM_DeactivateOutputTrigger (ptr(), SM[m_smidx], valueRegisterMask);
      }

      /**
        @brief Sets the software control output for a pin to high or low.

         The user specifies which channel to modify by supplying the submodule number and whether
         to modify PWM A or PWM B within that submodule.

        @param channel Channel to configure
        @param value      true: Supply a logic 1, false: Supply a logic 0.
      */
      inline void setupSwCtrlOut (Channel channel, bool value) {
        PWM_SetupSwCtrlOut (ptr(), SM[m_smidx], kPwmChan (channel), value);
      }

      /**
        @brief Set PWM output fault status

         These bits determine the fault state for the PWM_A output in fault conditions
         and STOP mode. It may also define the output state in WAIT and DEBUG modes
         depending on the settings of CTRL2[WAITEN] and CTRL2[DBGEN].
         This function can update PWM output fault status.

        @param channel Channel to configure
        @param faultState PWM output fault status
      */
      inline void setPwmFaultState (Channel channel, pwm_fault_state_t faultState) {
        PWM_SetPwmFaultState (ptr(), SM[m_smidx], kPwmChan (channel),  faultState);
      }

      /**
        @brief Set PWM fault disable mapping

         Each of the four bits of this read/write field is one-to-one associated
         with the four FAULTx inputs of fault channel 0/1. The PWM output will be turned
         off if there is a logic 1 on an FAULTx input and a 1 in the corresponding
         bit of this field. A reset sets all bits in this field.

        @param channel         PWM channel to configure
        @param pwm_fault_channels PWM fault channel to configure
        @param value              Fault disable mapping mask value
                                   enumeration ::pwm_fault_disable_t
      */
      inline void setupFaultDisableMap (Channel channel, pwm_fault_channels_t pwm_fault_channels, uint16_t value) {
        PWM_SetupFaultDisableMap (ptr(), SM[m_smidx], kPwmChan (channel),  pwm_fault_channels,  value);
      }

      /**
        @brief Set PWM output enable

         This feature allows the user to enable the PWM Output.

        @param channel         PWM channel to configure
      */
      inline void enableOutput (Channel channel) {
        PWM_OutputEnable (ptr(), kPwmChan (channel), SM[m_smidx]);
      }

      /**
        @brief Set PWM output disable

        This feature allows the user to disable the PWM output.

        @param channel         PWM channel to configure
      */
      inline void disableOutput (Channel channel) {
        PWM_OutputDisable (ptr(), kPwmChan (channel), SM[m_smidx]);
      }

      /**
        @brief Get the dutycycle value.

        @param channel  PWM channel to configure

        @return Current channel dutycycle between 0 to 100.
      */
      inline uint16_t dutyCycle (Channel channel) {
        return PWM_GetPwmDutyCycle (ptr(), SM[m_smidx], kPwmChan (channel));
      }

      /**
        @brief Set PWM output in idle status (high or low).

         @note This API should call after setupPwm() APIs, and PWMX submodule is not supported.

        @param channel         PWM channel to configure
        @param idleStatus         true: PWM output is high in idle status; false: PWM output is low in idle status.

        @return false if there was error setting up the signal; true if set output idle success
      */
      inline bool setOutputToIdle (Channel channel, bool idleStatus) {
        return (PWM_SetOutputToIdle (ptr(), kPwmChan (channel), SM[m_smidx], idleStatus) == kStatus_Success);
      }

      /**
        @brief Set the pwm submodule prescaler.

        @param prescaler          Set prescaler value
      */
      inline void setPrescaler (pwm_clock_prescale_t prescaler) {
        m_config.setPrescale (prescaler);
        m_fpmin = Timer::prescalerToMinPwmFrequency (prescaler);
        PWM_SetClockMode (ptr(), SM[m_smidx], prescaler);
      }

      /**
        @brief Set the pwm submodule prescaler.

        @deprecated will be removed in next major release, use \ref setPrescaler() instead

        @param prescaler          Set prescaler value
      */
      inline void setClockMode (pwm_clock_prescale_t prescaler) {
        setPrescaler (prescaler);
      }

      /**
        @brief the pwm submodule prescaler
      */
      inline pwm_clock_prescale_t prescaler() const {
        return config().prescale();
      }

      /**
        @brief This function enables-disables the forcing of the output of a given eFlexPwm channel to logic 0.

        @param channel         PWM channel to configure
        @param forcetozero        true: Enable the pwm force output to zero; false: Disable the pwm output resumes normal
                                   function.
      */
      inline void setPwmForceOutputToZero (Channel channel, bool forcetozero) {
        PWM_SetPwmForceOutputToZero (ptr(), SM[m_smidx], kPwmChan (channel), forcetozero);
      }

      /**
        @brief This function set the output state of the PWM pin as requested for the current cycle.

        @param channel         PWM channel to configure
        @param outputstate        Set pwm output state, see @ref pwm_output_state_t.
      */
      inline void setChannelOutput (Channel channel, pwm_output_state_t outputstate) {
        PWM_SetChannelOutput (ptr(), SM[m_smidx], kPwmChan (channel),  outputstate);
      }

      /**
        @brief Current minimal PWM frequency in Hz

        this value is calculated by \ref configure() and updated by \ref setPrescaler()
      */
      inline uint32_t minPwmFrequency() const {
        return m_fpmin;
      }

      /**
        @brief Adjust the prescaler if necessary

        Checks if pwmFreq is less than the minPwmFrequency() and increases the prescaler if necessary.
        Otherwise and if the prescaler is greater than 1, try to reduce it to the minimum.

        @param pwmFreq the desired PWM frequency
        @return true if adjustment made, false otherwise
      */
      bool adjustPrescaler (uint32_t pwmFreq);

    protected:
      inline PWM_Type *ptr() {
        return m_ptr;
      }

      inline const PWM_Type *ptr() const {
        return m_ptr;
      }

      inline pwm_signal_param_t *kPwmSignals() {
        return m_signal;
      }

      inline const pwm_signal_param_t *kPwmSignals() const {
        return m_signal;
      }

      static inline void setSignalToDefault (pwm_signal_param_t &signal) {
        signal.pwmchannelenable = true;
        signal.level = kPWM_HighTrue;
        signal.dutyCycle = 32768;
        signal.deadtimeValue = 0;
        signal.faultState = kPWM_PwmFaultState0;
      }

    private:
      Pin m_pin[NofPins];
      uint8_t m_tmidx;
      uint8_t m_smidx;
      PWM_Type *m_ptr;
      pwm_signal_param_t m_signal[NofPins];
      Config m_config;
      bool m_wasbegin;
      uint32_t m_fpmin;
      uint16_t m_duty[NofPins];
  };

  extern SubModule *SmList[NofTimers][NofSubmodules];
}
