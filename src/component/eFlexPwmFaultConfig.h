/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmGlobal.h"

namespace eFlex {

  class Timer;

  /**
    @brief Fault Configuration

    Class is used to hold the parameters to configure a PWM fault
  */

  class FaultConfig {
    public:

      /**
        @brief  Construct a new FaultConfig object with the default settings

         The default values are:
          - \c faultClearingMode = kPWM_Automatic
          - \c faultLevel = false
          - \c enableCombinationalPath = true
          - \c recoverMode = kPWM_NoRecovery
      */
      FaultConfig();

      /**
        @brief Set the Fault clearing mode

        @param mode to choose from the following values : 
          - kPWM_Automatic    : Automatic fault clearing
          - kPWM_ManualNormal : Manual fault clearing with no fault safety mode
          - kPWM_ManualSafety : Manual fault clearing with fault safety mode
      */
      inline void setFaultClearingMode (pwm_fault_clear_t mode) {

        m_config.faultClearingMode = mode;
      }

      /**
        @brief Fault clearing mode \ref pwm_fault_clear_t

        @return mode from the following values : 
          - kPWM_Automatic    : Automatic fault clearing
          - kPWM_ManualNormal : Manual fault clearing with no fault safety mode
          - kPWM_ManualSafety : Manual fault clearing with fault safety mode
      */
      inline pwm_fault_clear_t faultClearingMode() const {

        return m_config.faultClearingMode;
      }

      /**
        @brief Specify when to re-enable the PWM output

        @param mode to choose from the following values : 
          - kPWM_NoRecovery               : PWM output will stay inactive
          - kPWM_RecoverHalfCycle         : PWM output re-enabled at the first half cycle
          - kPWM_RecoverFullCycle         : PWM output re-enabled at the first full cycle
          - kPWM_RecoverHalfAndFullCycle  : PWM output re-enabled at the first half or full cycle
      */
      inline void setRecoverMode (pwm_fault_recovery_mode_t mode) {

        m_config.recoverMode = mode;
      }

      /**
        @brief Options available on how to re-enable the PWM output when recovering from a fault

        @return mode from the following values : 
          - kPWM_NoRecovery               : PWM output will stay inactive
          - kPWM_RecoverHalfCycle         : PWM output re-enabled at the first half cycle
          - kPWM_RecoverFullCycle         : PWM output re-enabled at the first full cycle
          - kPWM_RecoverHalfAndFullCycle  : PWM output re-enabled at the first half or full cycle
      */
      inline pwm_fault_recovery_mode_t recoverMode() const {

        return m_config.recoverMode;
      }

      /**
        @brief active Debug Mode

         @param active true Logic 1 indicates fault, false Logic 0 indicates fault
      */
      inline void setFaultLevel (bool active) {

        m_config.faultLevel = active;
      }

      /**
        @brief Debug mode, default false

         @return true Logic 1 indicates fault
         @return false Logic 0 indicates fault
      */
      inline bool faultLevel() const {

        return m_config.faultLevel;
      }

      /**
        @brief enable Combinational Path

        @param enable true Combinational Path from fault input is enabled, false No combination path is available
      */
      inline void enableCombinationalPath (bool enable) {

        m_config.enableCombinationalPath = enable;
      }
      
      /**
        @brief Wait mode, default false

         @return true Combinational Path from fault input is enabled
         @return false No combination path is available
      */
      inline bool isCombinationalPathEnabled() const {

        return m_config.enableCombinationalPath;
      }

    protected:
      pwm_fault_param_t *kPwmConfig();
      const pwm_fault_param_t *kPwmConfig() const;

    private:
      friend class Timer;

    private:
      pwm_fault_param_t m_config;
  };
}
