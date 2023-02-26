/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include <map>
#include <array>

#include "fsl_pwm.h"

#define EFLEXPWM_DUMPREG_ENABLED 1

namespace eFlexPwm {

  enum {
    Pwm1 = 0,
    Pwm2,
    Pwm3,
    Pwm4
  };

  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------

  /**
     @brief PWM pin

     Cette classe permet de configurer, intialiser les broches PWM

  */
  class Pin {
    public:
      Pin (int number = -1);
      bool setNumber (int number);
      bool begin();
      bool isValid() const;

      inline uint8_t module () const;
      inline uint8_t submodule () const;
      inline uint8_t muxval () const;

      #if 0
      pwm_channels_t pwmChannel; /*!< PWM channel being configured; PWM A or PWM B */
      uint8_t dutyCyclePercent;  /*!< PWM pulse width, value should be between 0 to 100
                                    0=inactive signal(0% duty cycle)...
                                    100=always active signal (100% duty cycle)*/
      pwm_level_select_t level;  /*!< PWM output active level select */
      uint16_t deadtimeValue;    /*!< The deadtime value; only used if channel pair is operating in complementary mode */
      pwm_fault_state_t faultState; /*!< PWM output fault status */
      bool pwmchannelenable;        /*!< Enable PWM output */
      #endif

      const pwm_signal_param_t &param() const;
      inline pwm_channels_t channel() const;
      inline uint8_t dutyCyclePercent() const;
      inline pwm_level_select_t level() const;
      inline uint16_t deadtime() const;
      inline pwm_fault_state_t faultState() const;
      inline bool enabled() const;

      inline void setDutyCyclePercent (uint8_t dutyCyclePercent);
      inline void setLevel (pwm_level_select_t level);
      inline void setDeadtime (uint16_t deadtimeValue);
      inline void setEnable (bool activate = true);
      inline void setFaultState (pwm_fault_state_t faultState);

    protected:
      inline void setChannel (pwm_channels_t channel);

    private:
      int m_number;
      uint8_t m_module;
      uint8_t m_muxval;
      pwm_signal_param_t m_param;
  };

  //-----------------------------------------------------------------------------
  //                                Config class
  //-----------------------------------------------------------------------------
  class Config {
    public:
      /**
         @brief  Construct a new Config object with the default settings

         The default values are:
         @code
           config->enableDebugMode = false;
           config->enableWait = false;
           config->reloadSelect = kPWM_LocalReload;
           config->clockSource = kPWM_BusClock;
           config->prescale = kPWM_Prescale_Divide_1;
           config->initializationControl = kPWM_Initialize_LocalSync;
           config->forceTrigger = kPWM_Force_Local;
           config->reloadFrequency = kPWM_LoadEveryOportunity;
           config->reloadLogic = kPWM_ReloadImmediate;
           config->pairOperation = kPWM_Independent;
           config->pwmFreqHz = 5000;
           config->mode = kPWM_CenterAligned;
         @endcodem_sm
         @param config Pointer to user's PWM config structure.
      */
      Config();

      inline bool enableDebugMode() const;
      inline bool enableWait() const;
      inline pwm_reload_source_select_t reloadSelect() const;
      inline pwm_clock_source_t clockSource() const;
      inline pwm_clock_prescale_t prescale() const;
      inline pwm_init_source_t initializationControl() const;
      inline pwm_force_output_trigger_t forceTrigger() const;
      inline pwm_load_frequency_t reloadFrequency() const;
      inline pwm_register_reload_t reloadLogic() const;
      inline pwm_chnl_pair_operation_t pairOperation() const;
      inline uint32_t pwmFreqHz() const;
      inline pwm_mode_t mode() const;

      inline void setEnableDebugMode (bool enableDebugMode);
      inline void setEnableWait (bool enableWait);
      inline void setReloadSelect (pwm_reload_source_select_t reloadSelect);
      inline void setClockSource (pwm_clock_source_t clockSource);
      inline void setPrescale (pwm_clock_prescale_t prescale);
      inline void setInitializationControl (pwm_init_source_t initializationControl);
      inline void setForceTrigger (pwm_force_output_trigger_t forceTrigger);
      inline void setReloadFrequency (pwm_load_frequency_t reloadFrequency);
      inline void setReloadLogic (pwm_register_reload_t reloadLogic) ;
      inline void setPairOperation (pwm_chnl_pair_operation_t pairOperation) ;
      inline void setPwmFreqHz (uint32_t pwmFreqHz);
      inline void setMode (pwm_mode_t mode);

    protected:
      pwm_config_t &config();      
      const pwm_config_t &config() const;

    private:
      friend class Pwm;
      friend class SubModule;

    private:
      pwm_mode_t m_mode;
      uint32_t m_pwmfreq;
      pwm_config_t m_config;
  };

  /**
     @brief  Fill in the PWM fault config struct with the default settings

     The default values are:
     @code
       config->faultClearingMode = kPWM_Automatic;
       config->faultLevel = false;
       config->enableCombinationalPath = true;
       config->recoverMode = kPWM_NoRecovery;
     @endcode
     @param config Pointer to user's PWM fault config structure.
  */
  void PWM_FaultDefaultConfig (pwm_fault_param_t *config);
  void PWM_FaultDefaultConfig (pwm_fault_param_t *config);

  class Pwm;

  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  class SubModule {
    public:
      /**
         @brief Construct a new SubModule object

         @param pinA Arduino pin number of output A, this pin must correspond to an output A (23) of an eFlexPWM sub-module (refer to the datasheet)
         @param pinB Arduino pin number of output B, this pin must correspond to output B (45) of the same submodule as pinA (refer to datasheet)
      */
      SubModule (int pinA, int pinB = -1);

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

         @param mode        PWM operation mode, options available in enumeration ::pwm_mode_t
         @param srcClock_Hz PWM main counter clock in Hz.

         @return Returns false if there was error setting up the signal; true otherwise
      */
      inline bool setupPwm (pwm_mode_t mode, uint32_t pwmFreq_Hz);

      /**
         @brief

         @return true
         @return false
      */
      bool begin();

      /**
         @brief

         @param i
         @return Pin&
      */
      Pin &pin (uint8_t i);

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
         @brief

         @return Pwm&
      */
      inline Pwm &pwm();

      /**
         @name Module PWM output
         @{
      */

      /**
         @brief PWM main counter clock in Hz.
      */
      inline uint32_t srcClockHz() const;

      /**
         @brief Set PWM phase shift for PWM channel running on channel PWM_A, PWM_B which with 50% duty cycle..

         @param pwmChannel  PWM channel to configure
         @param pwmFreq_Hz  PWM signal frequency in Hz
         @param shiftvalue  Phase shift value
         @param doSync      true: Set LDOK bit for the submodule list;
                            false: LDOK bit don't set, need to call setPwmLdok to sync update.

         @return Returns false if there was error setting up the signal; true otherwise
      */
      inline bool setupPwmPhaseShift (pwm_channels_t pwmChannel, uint32_t pwmFreq_Hz, uint8_t shiftvalue, bool doSync = true);

      /**
         @brief Updates the PWM signal's dutycycle.

         The function updates the PWM dutycyle to the new value that is passed in.
         If the dead time insertion logic is enabled then the pulse period is reduced by the
         dead time period specified by the user.


         @param pwmSignal         Signal (PWM A or PWM B) to update
         @param currPwmMode       The current PWM mode set during PWM setup
         @param dutyCyclePercent  New PWM pulse width, value should be between 0 to 100
                                  0=inactive signal(0% duty cycle)...
                                  100=active signal (100% duty cycle)
      */
      inline void write (pwm_channels_t pwmSignal, pwm_mode_t currPwmMode, uint8_t dutyCyclePercent);

      /**
         @brief Updates the PWM signal's dutycycle with 16-bit accuracy.

         The function updates the PWM dutycyle to the new value that is passed in.
         If the dead time insertion logic is enabled then the pulse period is reduced by the
         dead time period specified by the user.


         @param pwmSignal         Signal (PWM A or PWM B) to update
         @param currPwmMode       The current PWM mode set during PWM setup
         @param dutyCycle         New PWM pulse width, value should be between 0 to 65535
                                  0=inactive signal(0% duty cycle)...
                                  65535=active signal (100% duty cycle)
      */
      inline void writeHighAccuracy (pwm_channels_t pwmSignal, pwm_mode_t currPwmMode, uint16_t dutyCycle);

      /** @}*/

      /**
         @brief Sets up the PWM input capture

         Each PWM submodule has 3 pins that can be configured for use as input capture pins. This function
         sets up the capture parameters for each pin and enables the pin for input capture operation.

         @param pwmChannel         Channel in the submodule to setup
         @param inputCaptureParams Parameters passed in to set up the input pin
      */
      inline void setupInputCapture (pwm_channels_t pwmChannel, const pwm_input_capture_param_t *inputCaptureParams);


      /**
         @brief Selects the signal to output on a PWM pin when a FORCE_OUT signal is asserted.

         The user specifies which channel to configure by supplying the submodule number and whether
         to modify PWM A or PWM B within that submodule.

         @param pwmChannel Channel to configure
         @param mode       Signal to output when a FORCE_OUT is triggered
      */
      inline void setupForceSignal (pwm_channels_t pwmChannel, pwm_force_signal_t mode);

      /**
         @name Interrupts Interface
         @{
      */

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

      /** @}*/

      /**
         @name DMA Interface
         @{
      */

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

      /** @}*/

      /**
         @name Status Interface
         @{
      */

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

      /** @}*/

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

      /**
         @brief Get the PWM VALx registers.

         @param valueRegister VALx register that will be read value
         @return The VALx register value
      */
      inline uint16_t VALxValue (pwm_value_register_t valueRegister);

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

         @param pwmChannel Channel to configure
         @param value      true: Supply a logic 1, false: Supply a logic 0.
      */
      inline void setupSwCtrlOut (pwm_channels_t pwmChannel, bool value);


      /**
         @brief Set PWM output fault status

         These bits determine the fault state for the PWM_A output in fault conditions
         and STOP mode. It may also define the output state in WAIT and DEBUG modes
         depending on the settings of CTRL2[WAITEN] and CTRL2[DBGEN].
         This function can update PWM output fault status.

         @param pwmChannel Channel to configure
         @param faultState PWM output fault status
      */
      inline void setPwmFaultState (pwm_channels_t pwmChannel, pwm_fault_state_t faultState);

      /**
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
      inline void setupFaultDisableMap (pwm_channels_t pwmChannel, pwm_fault_channels_t pwm_fault_channels, uint16_t value);

      /**
         @brief Set PWM output enable

         This feature allows the user to enable the PWM Output.

         @param pwmChannel         PWM channel to configure
      */
      inline void enableOutput (pwm_channels_t pwmChannel);

      /**
         @brief Set PWM output disable

        This feature allows the user to disable the PWM output.

         @param pwmChannel         PWM channel to configure
      */
      inline void disableOutput (pwm_channels_t pwmChannel);

      /**
         @brief Get the dutycycle value.

         @param pwmChannel  PWM channel to configure

         @return Current channel dutycycle value.
      */
      inline uint8_t dutyCycle (pwm_channels_t pwmChannel);

      /**
         @brief Set PWM output in idle status (high or low).

         @note This API should call after setupPwm() APIs, and PWMX submodule is not supported.

         @param pwmChannel         PWM channel to configure
         @param idleStatus         true: PWM output is high in idle status; false: PWM output is low in idle status.

         @return false if there was error setting up the signal; true if set output idle success
      */
      inline bool setOutputToIdle (pwm_channels_t pwmChannel, bool idleStatus);

      /**
         @brief Set the pwm submodule prescaler.

         @param prescaler          Set prescaler value
      */
      inline void setClockMode (pwm_clock_prescale_t prescaler);

      /**
         @brief This function enables-disables the forcing of the output of a given eFlexPwm channel to logic 0.

         @param pwmChannel         PWM channel to configure
         @param forcetozero        true: Enable the pwm force output to zero; false: Disable the pwm output resumes normal
                                   function.
      */
      inline void setPwmForceOutputToZero (pwm_channels_t pwmChannel, bool forcetozero);

      /**
         @brief This function set the output state of the PWM pin as requested for the current cycle.

         @param pwmChannel         PWM channel to configure
         @param outputstate        Set pwm output state, see @ref pwm_output_state_t.
      */
      inline void setChannelOutput (pwm_channels_t pwmChannel, pwm_output_state_t outputstate);

      /** @}*/

      void dumpRegs (Stream &out = Serial) const;

    private:
      uint8_t m_midx;
      uint8_t m_sidx;
      Config m_config;
      std::array<Pin,2> m_pin;
  };

  //-----------------------------------------------------------------------------
  //                                Pwm class
  //-----------------------------------------------------------------------------
  class Pwm {

    public:
      /**
         @brief Construct a new Pwm object

         @param index
      */
      Pwm (uint8_t index);

      bool begin();

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t index() const;

      inline void setDutyCyclePercent (uint8_t dutyCyclePercent);
      inline void setLevel (pwm_level_select_t level);
      inline void setDeadtime (uint16_t deadtimeValue);
      inline void setEnable (bool activate = true);
      inline void setFaultState (pwm_fault_state_t faultState);

      /**
         @name Timer Start and Stop
         @{
      */

      /**
         @brief Starts the PWM counter for a single or multiple submodules.

         Sets the Run bit which enables the clocks to the PWM submodule. This function can start multiple
         submodules at the same time.

         @param subModulesToStart PWM submodules to start. This is a logical OR of members of the
                                  enumeration ::pwm_module_control_t
      */
      inline void startTimer (uint8_t subModulesToStart);

      /**
         @brief Starts the PWM counter for all instantiated submodules for this timer
      */
      inline void startTimer ();

      /**
         @brief Stops the PWM counter for a single or multiple submodules.

         Clears the Run bit which resets the submodule's counter. This function can stop multiple
         submodules at the same time.

         @param subModulesToStop PWM submodules to stop. This is a logical OR of members of the
                                 enumeration ::pwm_module_control_t
      */
      inline void stopTimer (uint8_t subModulesToStop);

      /**
         @brief Stops the PWM counter for all instantiated submodules for this timer
      */
      inline void stopTimer ();

      /**
         @brief Sets or clears the PWM LDOK bit on a single or multiple submodules

         Set LDOK bit to load buffered values into CTRL[PRSC] and the INIT, FRACVAL and VAL registers. The
         values are loaded immediately if kPWM_ReloadImmediate option was choosen during config. Else the
         values are loaded at the next PWM reload point.
         This function can issue the load command to multiple submodules at the same time.

         @param subModulesToUpdate PWM submodules to update with buffered values. This is a logical OR of
                                   members of the enumeration ::pwm_module_control_t
         @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (uint8_t subModulesToUpdate, bool value);

      /**
         @brief Sets or clears the PWM LDOK bit on all instantiated submodules for this timer

         @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value);

      /**
         @brief Sets up the PWM fault input filter.

         @param faultInputFilterParams Parameters passed in to set up the fault input filter.
      */
      inline void setupFaultInputFilter (const pwm_fault_input_filter_param_t *faultInputFilterParams);

      /**
         @brief Sets up the PWM fault protection.

         PWM has 4 fault inputs.

         @param faultNum    PWM fault to configure.
         @param faultParams Pointer to the PWM fault config structure
      */
      inline void setupFaults (pwm_fault_input_t faultNum, const pwm_fault_param_t *faultParams);

      /** @}*/
      void dumpRegs (Stream &out = Serial) const;
      void dumpAllRegs (Stream &out = Serial) const;

    private:
      friend class SubModule;
      friend class Pin;

    private:
      uint8_t m_midx;
      uint8_t m_smmask;
      std::map<uint8_t, SubModule *> m_sm;

    protected:
      void addSubModule (SubModule *sm);
  };
}

#include "eFlexPwm_inline.h"

