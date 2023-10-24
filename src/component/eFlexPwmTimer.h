/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmPin.h"

namespace eFlex {

  /**
    @brief PWM Module

    This class cannot be instantiated directly and can only be used by a call to SubModule::timer()

  */
  class Timer {

    public:
      /**
        @brief Sets up the PWM signals for the sub-modules passed as parameters

          The function initializes the submodule according to the parameters passed in by the user. The function
          also sets up the value compare registers to match the PWM signal requirements.
          If the dead time insertion logic is enabled, the pulse period is reduced by the
          dead time period specified by the user.

        @param subModulesToBegin PWM submodules to begin. This is a logical OR of members of the
                                  enumeration ::pwm_module_control_t
        @param doStart starts signal generation
        @param doSync synchronizes the operation by surrounding it with setPwmLdok (false) / setPwmLdok (true)

        @return Returns false if there was error setting up the signal; true otherwise
       */
      bool beginSubModules (uint8_t subModulesToBegin, bool doStart = true, bool doSync = true);

      /**
        @brief Sets up the PWM signals for all instantiated submodules of the timer

          The function initializes the submodule according to the parameters passed in by the user. The function
          also sets up the value compare registers to match the PWM signal requirements.
          If the dead time insertion logic is enabled, the pulse period is reduced by the
          dead time period specified by the user.

        @param doStart starts signal generation
        @param doSync synchronizes the operation by surrounding it with setPwmLdok (false) / setPwmLdok (true)

        @return Returns false if there was error setting up the signal; true otherwise
      */
      inline bool begin (bool doStart = true, bool doSync = true) {
        return beginSubModules (SmMask[m_tmidx], doStart, doSync);
      }

      /**
        @brief Returns the timer module index (0 for PWM1...)
      */
      inline uint8_t index() const {
        return m_tmidx;
      }

      /**
        @brief Enable or disable all instantiated submodules of the timer

         This function allows you to enable/disable the output pins without
         changing anything in the configuration. When the timer is
         disabled, its output pins are forced to zero.

        @param value true to enable, false otherwise
      */
      void enable (bool value = true);

      /**
        @brief Disable all instantiated submodules of the timer

         This function allows you to disable the output pins without
         changing anything in the configuration. When the submodule is
         disabled, its output pins are forced to zero.
      */
      inline void disable () {
        enable (false);
      }

      /**
        @brief Returns true if the timer is enabled
      */
      inline bool isEnabled() const {
        return m_isenabled;
      }

      /**
        @brief Starts or stops the PWM counter for a single or multiple submodules.

        Sets the Run bit which enables the clocks to the PWM submodule. This function can start multiple
        submodules at the same time.

        @param subModulesToStart PWM submodules to start. This is a logical OR of members of the
                                  enumeration ::pwm_module_control_t
        @param startit true to start, false to stop
      */
      inline void start (uint8_t subModulesToStart, bool startit = true) {
        if (startit) {
          PWM_StartTimer (ptr(), subModulesToStart);
        }
        else {
          PWM_StopTimer (ptr(), subModulesToStart);
        }
      }

      /**
        @brief Starts or stops the PWM counter for all instantiated submodules for this timer

        @param startit true to start, false to stop
        @note This operation is not useful if \c begin() was called with doStart=true
      */
      inline void start (bool startit = true) {
        start (SmMask[m_tmidx], startit);
      }

      /**
        @brief Stops the PWM counter for a single or multiple submodules.

        Clears the Run bit which resets the submodule's counter. This function can stop multiple
        submodules at the same time.

        @param subModulesToStop PWM submodules to stop. This is a logical OR of members of the
                                 enumeration ::pwm_module_control_t
      */
      inline void stop (uint8_t subModulesToStop) {
        start (subModulesToStop, false);
      }

      /**
        @brief Stops the PWM counter for all instantiated submodules for this timer
      */
      inline void stop () {
        stop (false);
      }

      /**
        @brief Setting the duty cycle for all submodules before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param dutyCycle duty cycle 0-65535
      */
      void setupDutyCycle (uint16_t dutyCycle);

      /**
        @brief Setting the output pulse mode for all submodules before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param level output pulse mode
      */
      void setupLevel (pwm_level_select_t level);

      /**
        @brief Setting the deadtime for all submodules before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param deadtimeValue The deadtime value in clock cycles; only used if channel pair is operating in complementary mode
        @param unit The unit of the deadtimeValue; 1: clock cycles; 1000: milliseconds; 1000000: microseconds; 1000000000: nanoseconds
      */
      void setupDeadtime (uint16_t deadtimeValue, uint32_t unit = 1);

      /**
        @brief Setting output enable for all submodules before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting
        @param activate true to enable the outputs
      */
      void setupOutputEnable (bool activate = true);

      /**
        @brief Setting the output fault status for all submodules before calling begin

        @note If you want this value to take effect after the call to \ref begin, you must call \ref updateSetting

        @param faultState the output fault status
      */
      void setupFaultState (pwm_fault_state_t faultState);


      /*!
        @brief Update PWM signals for all submodules

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
        @brief Bus clock in Hz.
      */
      static inline uint32_t busClockHz()  {
        return F_BUS_ACTUAL;
      }

      /**
        @brief PWM main counter clock in Hz.
      */
      inline uint32_t srcClockHz() const {
        return busClockHz();
      }

      /**
        @brief Print PWM module registers to the output stream

        @note submodule registers are not printed.

        @param out the output stream, Serial by default
      */
      void printRegs (Stream &out = Serial) const;

      /**
        @brief Print registers of PWM module and its submodules to the output stream

        @param out the output stream, Serial by default
      */
      void printAllRegs (Stream &out = Serial) const;

      /**
        @brief Sets or clears the PWM LDOK bit on all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doSync=true

        @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value = true) {
        setPwmLdok (SmMask[m_tmidx], value);
      }

      //-----------------------------------------------------------------------
      //                            NXP SDK WRAPPER
      //-----------------------------------------------------------------------

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
      inline void setPwmLdok (uint8_t subModulesToUpdate, bool value) {
        PWM_SetPwmLdok (ptr(), subModulesToUpdate, value);
      }

      /**
        @brief Sets up the PWM fault input filter.

        @param faultInputFilterParams Parameters passed in to set up the fault input filter.
      */
      inline void setupFaultInputFilter (const pwm_fault_input_filter_param_t *faultInputFilterParams) {
        PWM_SetupFaultInputFilter (ptr(), faultInputFilterParams);
      }

      /**
        @brief Sets up the PWM fault protection.

         PWM has 4 fault inputs.

        @param faultNum    PWM fault to configure.
        @param faultParams Pointer to the PWM fault config structure
      */
      inline void setupFaults (pwm_fault_input_t faultNum, const pwm_fault_param_t *faultParams) {
        PWM_SetupFaults (ptr(), faultNum, faultParams);
      }

      /**
       * @brief Calculates the minimum PWM frequency corresponding to a prescaler
       * 
       * @param prescaler 1,2,4,8,16,32,64,128
       * @return minimum PWM frequency in Hz
       */
      static inline uint32_t prescalerToMinPwmFrequency (unsigned prescaler) {
        return (busClockHz() / (static_cast<uint32_t> (prescaler & 0x7F) * 65535UL) + 1);
      }

      /**
       * @overload  uint32_t prescalerToMinPwmFrequency (unsigned prescaler)
       */
      static inline uint32_t prescalerToMinPwmFrequency (pwm_clock_prescale_t prescaler) {
        return prescalerToMinPwmFrequency (1U << prescaler);
      }

    protected:
      /**
        @brief Construct a new Timer Module
        cannot be publicly instantiated
        @param index the timer module index (0 for PWM1...)
      */
      Timer (uint8_t index);
      inline PWM_Type *ptr() {
        return m_ptr;
      }
      inline const PWM_Type *ptr() const {
        return m_ptr;
      }

    private:
      uint8_t m_tmidx;
      PWM_Type *m_ptr;
      bool m_isenabled;
  };
  extern Timer *TM[NofTimers];
}

