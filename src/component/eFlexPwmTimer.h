/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmPin.h"
#include "eFlexPwmSubmodule.h"

namespace eFlex {

  /**
     @brief PWM Module

  */
  class Timer {

    public:
      /**
         @brief Construct a new Timer Module

         @param index
      */
      Timer (uint8_t index);

      /**
         @brief

         @param doStart starts signal generation
         @param doSync synchronizes the operation by surrounding it with setPwmLdok (false) / setPwmLdok (true)
         @return true
         @return false
      */
      bool begin (bool doStart = true, bool doSync = true);

      void setDutyCyclePercent (uint8_t dutyCyclePercent);
      void setLevel (pwm_level_select_t level);
      void setDeadtime (uint16_t deadtimeValue);
      void setEnable (bool activate = true);
      void setFaultState (pwm_fault_state_t faultState);

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
         @brief PWM main counter clock in Hz.
      */
      inline uint32_t srcClockHz() const {
        return F_BUS_ACTUAL;
      }

      /**
        @brief Starts the PWM counter for a single or multiple submodules.

        Sets the Run bit which enables the clocks to the PWM submodule. This function can start multiple
        submodules at the same time.

        @param subModulesToStart PWM submodules to start. This is a logical OR of members of the
                                  enumeration ::pwm_module_control_t
      */
      inline void start (uint8_t subModulesToStart) {
        PWM_StartTimer (ptr(), subModulesToStart);
      }

      /**
        @brief Starts the PWM counter for all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doStart=true
      */
      inline void start () {
        start (SmMask[m_tmidx]);
      }

      /**
        @brief Stops the PWM counter for a single or multiple submodules.

        Clears the Run bit which resets the submodule's counter. This function can stop multiple
        submodules at the same time.

        @param subModulesToStop PWM submodules to stop. This is a logical OR of members of the
                                 enumeration ::pwm_module_control_t
      */
      inline void stop (uint8_t subModulesToStop) {
        PWM_StopTimer (ptr(), subModulesToStop);
      }

      /**
        @brief Stops the PWM counter for all instantiated submodules for this timer
      */
      inline void stop () {
        stop (SmMask[m_tmidx]);
      }

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
        @brief Sets or clears the PWM LDOK bit on all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doSync=true

        @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value = true) {
        setPwmLdok (SmMask[m_tmidx], value);
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
         @brief

         @return uint8_t
      */
      inline uint8_t index() const {
        return m_tmidx;
      }

      void dumpRegs (Stream &out = Serial) const;
      void dumpAllRegs (Stream &out = Serial) const;

    protected:
      inline PWM_Type *ptr() {

        return m_ptr;
      }
      inline const PWM_Type *ptr() const {

        return m_ptr;
      }

    private:
      friend class SubModule;
      friend class Pin;

    private:
      uint8_t m_tmidx;
      PWM_Type *m_ptr;
      bool m_isenabled;
  };
  extern Timer TM[NofTimers];
}

