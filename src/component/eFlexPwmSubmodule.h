/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmPin.h"
#include "eFlexPwmConfig.h"

namespace eFlex {

  class Timer;
  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  /**
   * @brief Submodule
   * 
   */
  class SubModule {
    public:
      /**
         @brief Construct a new SubModule object

         If the pin is valid (eFlexPwm type), its channel is configured with default values:
         pwmchannelenable = true;
         level = kPWM_HighTrue;
         dutyCyclePercent = 50;
         deadtimeValue = 0;
         faultState = kPWM_PwmFaultState0;

         @param pinChanA Arduino pin number of output A, this pin must correspond to an output A (23) of an eFlexPWM sub-module (refer to the datasheet)
         @param pinChanB Arduino pin number of output B, this pin must correspond to output B (45) of the same submodule as pinA (refer to datasheet)
      */
      SubModule (int pinChanA, int pinChanB = -1);

      /**
         @brief Configures the peripheral for basic operation.

         @note This API should be called at the beginning of the application using the PWM driver.

         @param config Reference to user's PWM config structure.
         @return true means success; if false is returned, \c begin() should not be called !
      */
      bool configure (const Config &config);

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
         @brief Updates the PWM signal's dutycycle.

         The function updates the PWM dutycyle to the new value that is passed in.
         If the dead time insertion logic is enabled then the pulse period is reduced by the
         dead time period specified by the user.


         @param channel         Channel (PWM A or PWM B) to update
         @param dutyCyclePercent  New PWM pulse width, value should be between 0 to 100
                                  0=inactive signal(0% duty cycle)...
                                  100=active signal (100% duty cycle)
      */
      inline void updateDutyCyclePercent (uint8_t dutyCyclePercent, Channel channel = ChanA);

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
      inline void updateDutyCycle (uint16_t dutyCycle, Channel channel = ChanA);

      /**
       * @brief Set the Pwm Frequency object
       * 
       * @todo TODO
       * 
       * @param freq 
       */
      inline void setPwmFrequency (uint32_t freq);

      /**
         @brief

         @return Timer&
      */
      inline Timer &timer();

      inline void setDutyCyclePercent (Channel channel, uint8_t dutyCyclePercent);
      inline void setLevel (Channel channel, pwm_level_select_t level);
      inline void setDeadtime (Channel channel, uint16_t deadtimeValue);
      inline void setEnable (Channel channel, bool activate = true);
      inline void setFaultState (Channel channel, pwm_fault_state_t faultState);
      inline void setDutyCyclePercent (uint8_t dutyCyclePercent);
      inline void setLevel (pwm_level_select_t level);
      inline void setDeadtime (uint16_t deadtimeValue);
      inline void setEnable (bool activate = true);
      inline void setFaultState (pwm_fault_state_t faultState);

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t index() const;

      /**
         @brief

         @return true
         @return false
      */
      inline bool isValid() const;

      /**
        @brief Starts the PWM counter for this submodule

        @note This operation is not useful if \c begin() was called with doStart=true
      */
      inline void start ();

      /**
        @brief Stops the PWM counter for all instantiated submodules for this timer
      */
      inline void stop ();

      /**
        @brief Sets or clears the PWM LDOK bit on all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doSync=true

        @param value true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value = true);

      /**
         @brief Set PWM phase shift for PWM channel running on channel PWM_A, PWM_B which with 50% duty cycle..

         @param channel  PWM channel to configure
         @param pwmFreq_Hz  PWM signal frequency in Hz
         @param shiftvalue  Phase shift value
         @param doSync      true: Set LDOK bit for the submodule list;
                            false: LDOK bit don't set, need to call setPwmLdok to sync update.

         @return Returns false if there was error setting up the signal; true otherwise
      */
      inline bool setupPwmPhaseShift (Channel channel, uint32_t pwmFreq_Hz, uint8_t shiftvalue, bool doSync = true);

      /**
         @brief Sets up the PWM input capture

         Each PWM submodule has 3 pins that can be configured for use as input capture pins. This function
         sets up the capture parameters for each pin and enables the pin for input capture operation.

         @param channel         Channel in the submodule to setup
         @param inputCaptureParams Parameters passed in to set up the input pin
      */
      inline void setupInputCapture (Channel channel, const pwm_input_capture_param_t *inputCaptureParams);


      /**
         @brief Selects the signal to output on a PWM pin when a FORCE_OUT signal is asserted.

         The user specifies which channel to configure by supplying the submodule number and whether
         to modify PWM A or PWM B within that submodule.

         @param channel Channel to configure
         @param mode       Signal to output when a FORCE_OUT is triggered
      */
      inline void setupForceSignal (Channel channel, pwm_force_signal_t mode);

      /**
         @brief Enables the selected PWM interrupts

         @param mask      The interrupts to enable. This is a logical OR of members of the
                          enumeration ::pwm_interrupt_enable_t
      */
      inline void enableInterrupts (uint32_t mask);

      /**
         @brief Disables the selected PWM interrupts

         @param mask      The interrupts to enable. This is a logical OR of members of the
                          enumeration ::pwm_interrupt_enable_t
      */
      inline void disableInterrupts (uint32_t mask);

      /**
         @brief Gets the enabled PWM interrupts

         @return The enabled interrupts. This is the logical OR of members of the
                 enumeration ::pwm_interrupt_enable_t
      */
      inline uint32_t enabledInterrupts ();

      /**
         @brief Capture DMA Enable Source Select.

         @param pwm_watermark_control PWM FIFO watermark and control
      */
      inline void setDMAWatermarkControl (pwm_watermark_control_t pwm_watermark_control);

      /**
         @brief Capture DMA Enable Source Select.

         @param pwm_dma_source_select PWM capture DMA enable source select
      */
      inline void setDMACaptureSourceSelect (pwm_dma_source_select_t pwm_dma_source_select);

      /**
         @brief Enables or disables the selected PWM DMA Capture read request.

         @param mask      The DMA to enable or disable. This is a logical OR of members of the
                          enumeration ::pwm_dma_enable_t
         @param activate  true: Enable DMA read request; false: Disable DMA read request
      */
      inline void enableDMACapture (uint16_t mask, bool activate = true);

      /**
         @brief Enables or disables the PWM DMA write request.

         @param activate  true: Enable DMA write request; false: Disable DMA write request
      */
      inline void enableDMAWrite (bool activate);

      /**
         @brief Gets the PWM status flags

         @return The status flags. This is the logical OR of members of the
                 enumeration ::pwm_status_flags_t
      */
      inline uint32_t statusFlags ();

      /**
         @brief Clears the PWM status flags

         @param mask      The status flags to clear. This is a logical OR of members of the
                          enumeration ::pwm_status_flags_t
      */
      inline void clearStatusFlags (uint32_t mask);

      

      /**
         @brief Set the PWM VALx registers.

         This function allows the user to write value into VAL registers directly. And it will destroying the PWM clock period
         set by the setupPwm()/PWM_SetupPwmPhaseShift() functions.
         Due to VALx registers are bufferd, the new value will not active uless call setPwmLdok() and the reload point is
         reached.

         @param valueRegister VALx register that will be writen new value
         @param value         Value that will been write into VALx register
      */
      inline void setVALxValue (pwm_value_register_t valueRegister, uint16_t value);
      inline void setInitValue (uint16_t value);
      inline void setModuloValue (uint16_t value);
      inline void setVal2Value (uint16_t value);
      inline void setVal3Value (uint16_t value);
      inline void setVal4Value (uint16_t value);
      inline void setVal5Value (uint16_t value);

      /**
         @brief Get the PWM VALx registers.

         @param valueRegister VALx register that will be read value
         @return The VALx register value
      */
      inline uint16_t VALxValue (pwm_value_register_t valueRegister);
      inline uint16_t initValue ();
      inline uint16_t moduloValue ();
      inline uint16_t val2Value ();
      inline uint16_t val3Value ();
      inline uint16_t val4Value ();
      inline uint16_t val5Value ();

      /**
         @brief Enables or disables the PWM output trigger.

         This function allows the user to enable or disable the PWM trigger. The PWM has 2 triggers. Trigger 0
         is activated when the counter matches VAL 0, VAL 2, or VAL 4 register. Trigger 1 is activated
         when the counter matches VAL 1, VAL 3, or VAL 5 register.

         @param valueRegister Value register that will activate the trigger
         @param activate      true: Enable the trigger; false: Disable the trigger
      */
      inline void enableOutputTrigger (pwm_value_register_t valueRegister, bool activate = true);

      /**
         @brief Enables the PWM output trigger.

         This function allows the user to enable one or more (VAL0-5) PWM trigger.

         @param valueRegisterMask Value register mask that will activate one or more (VAL0-5) trigger
                                  enumeration ::_pwm_value_register_mask
      */
      inline void enableOutputTrigger (uint16_t valueRegisterMask);

      /**
         @brief Disables the PWM output trigger.

         This function allows the user to disables one or more (VAL0-5) PWM trigger.

         @param valueRegisterMask Value register mask that will Deactivate one or more (VAL0-5) trigger
                                  enumeration ::_pwm_value_register_mask
      */
      inline void disableOutputTrigger (uint16_t valueRegisterMask);

      /**
         @brief Sets the software control output for a pin to high or low.

         The user specifies which channel to modify by supplying the submodule number and whether
         to modify PWM A or PWM B within that submodule.

         @param channel Channel to configure
         @param value      true: Supply a logic 1, false: Supply a logic 0.
      */
      inline void setupSwCtrlOut (Channel channel, bool value);

      /**
         @brief Set PWM output fault status

         These bits determine the fault state for the PWM_A output in fault conditions
         and STOP mode. It may also define the output state in WAIT and DEBUG modes
         depending on the settings of CTRL2[WAITEN] and CTRL2[DBGEN].
         This function can update PWM output fault status.

         @param channel Channel to configure
         @param faultState PWM output fault status
      */
      inline void setPwmFaultState (Channel channel, pwm_fault_state_t faultState);

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
      inline void setupFaultDisableMap (Channel channel, pwm_fault_channels_t pwm_fault_channels, uint16_t value);

      /**
         @brief Set PWM output enable

         This feature allows the user to enable the PWM Output.

         @param channel         PWM channel to configure
      */
      inline void enableOutput (Channel channel);

      /**
         @brief Set PWM output disable

        This feature allows the user to disable the PWM output.

         @param channel         PWM channel to configure
      */
      inline void disableOutput (Channel channel);

      /**
         @brief Get the dutycycle value.

         @param channel  PWM channel to configure

         @return Current channel dutycycle value.
      */
      inline uint8_t dutyCycle (Channel channel);

      /**
         @brief Set PWM output in idle status (high or low).

         @note This API should call after setupPwm() APIs, and PWMX submodule is not supported.

         @param channel         PWM channel to configure
         @param idleStatus         true: PWM output is high in idle status; false: PWM output is low in idle status.

         @return false if there was error setting up the signal; true if set output idle success
      */
      inline bool setOutputToIdle (Channel channel, bool idleStatus);

      /**
         @brief Set the pwm submodule prescaler.

         @param prescaler          Set prescaler value
      */
      inline void setClockMode (pwm_clock_prescale_t prescaler);

      /**
         @brief This function enables-disables the forcing of the output of a given eFlexPwm channel to logic 0.

         @param channel         PWM channel to configure
         @param forcetozero        true: Enable the pwm force output to zero; false: Disable the pwm output resumes normal
                                   function.
      */
      inline void setPwmForceOutputToZero (Channel channel, bool forcetozero);

      /**
         @brief This function set the output state of the PWM pin as requested for the current cycle.

         @param channel         PWM channel to configure
         @param outputstate        Set pwm output state, see @ref pwm_output_state_t.
      */
      inline void setChannelOutput (Channel channel, pwm_output_state_t outputstate);

      

      void dumpRegs (Stream &out = Serial);

    protected:
      inline PWM_Type *ptr();
      inline const PWM_Type *ptr() const;
      inline pwm_signal_param_t *kPwmSignals();
      inline const pwm_signal_param_t *kPwmSignals() const;
      static inline void setSignalToDefault (pwm_signal_param_t &signal);

    private:
      Pin m_pin[NofPins];
      uint8_t m_tmidx;
      uint8_t m_smidx;
      PWM_Type *m_ptr;
      pwm_signal_param_t m_signal[NofPins];
      Config m_config;
  };
}

#include "source/eFlexPwmSubmodule_p.h"
