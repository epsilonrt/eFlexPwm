/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmPin.h"
#include "eFlexPwmSubmodule.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                Timer class
  //-----------------------------------------------------------------------------
  /**
   * @brief Timer
   * 
   */
  class Timer {

    public:
      /**
         @brief Construct a new Timer object

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

      inline void setDutyCyclePercent (uint8_t dutyCyclePercent);
      inline void setLevel (pwm_level_select_t level);
      inline void setDeadtime (uint16_t deadtimeValue);
      inline void setEnable (bool activate = true);
      inline void setFaultState (pwm_fault_state_t faultState);

      /**
         @brief PWM main counter clock in Hz.
      */
      inline uint32_t srcClockHz() const;

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
      inline void start (uint8_t subModulesToStart);

      /**
        @brief Starts the PWM counter for all instantiated submodules for this timer

        @note This operation is not useful if \c begin() was called with doStart=true
      */
      inline void start ();

      /**
        @brief Stops the PWM counter for a single or multiple submodules.

        Clears the Run bit which resets the submodule's counter. This function can stop multiple
        submodules at the same time.

        @param subModulesToStop PWM submodules to stop. This is a logical OR of members of the
                                 enumeration ::pwm_module_control_t
      */
      inline void stop (uint8_t subModulesToStop);

      /**
        @brief Stops the PWM counter for all instantiated submodules for this timer
      */
      inline void stop ();

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

        @note This operation is not useful if \c begin() was called with doSync=true

        @param value              true: Set LDOK bit for the submodule list; false: Clear LDOK bit
      */
      inline void setPwmLdok (bool value = true);

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

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t index() const;

      void dumpRegs (Stream &out = Serial) const;
      void dumpAllRegs (Stream &out = Serial) const;

    protected:
      inline PWM_Type *ptr();
      inline const PWM_Type *ptr() const;

    private:
      friend class SubModule;
      friend class Pin;

    private:
      uint8_t m_tmidx;
      PWM_Type *m_ptr;
  };
}

#include "source/eFlexPwmTimer_p.h"
