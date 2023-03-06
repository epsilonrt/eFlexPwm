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
          - \c faultFilterCount = 0
          - \c faultFilterPeriod = 0 (filter disabled)
          - \c faultGlitchStretch = false
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
        @brief Set the input FAULTx level

        @param active true Logic 1 indicates fault, false Logic 0 indicates fault
      */
      inline void setFaultLevel (bool active) {

        m_config.faultLevel = active;
      }

      /**
        @brief Input FAULTx level

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
        @brief Check Combinational Path is enabled

        @return true Combinational Path from fault input is enabled
        @return false No combination path is available
      */
      inline bool isCombinationalPathEnabled() const {

        return m_config.enableCombinationalPath;
      }

      /**
        @brief enable Fault Glitch Stretch

        This bit is used to enable the fault glitch stretching logic. This logic ensures that narrow fault glitches are
        stretched to be at least 2 IPBus clock cycles wide. In some cases a narrow fault input can cause
        problems due to the short PWM output shutdown/re-activation time. The stretching logic ensures that a
        glitch on the fault input, when the fault filter is disabled, will be registered in the fault flags.
        
        - false - Fault input glitch stretching is disabled.
        - true  - Input fault signals will be stretched to at least 2 IPBus clock cycles

        @param enable true means that input fault signals will be stretched to at least 2 IPBus clock cycles
      */
      inline void enableFaultGlitchStretch (bool enable) {

        m_filter.faultGlitchStretch = enable;
      }
      
      /**
        @brief Check Fault Glitch Stretch is enabled

        This bit is used to enable the fault glitch stretching logic. This logic ensures that narrow fault glitches are
        stretched to be at least 2 IPBus clock cycles wide. In some cases a narrow fault input can cause
        problems due to the short PWM output shutdown/re-activation time. The stretching logic ensures that a
        glitch on the fault input, when the fault filter is disabled, will be registered in the fault flags.
        
        - false - Fault input glitch stretching is disabled.
        - true  - Input fault signals will be stretched to at least 2 IPBus clock cycles

        @return  true means that input fault signals will be stretched to at least 2 IPBus clock cycles
      */
      inline bool isFaultGlitchStretchEnabled() const {

        return m_filter.faultGlitchStretch;
      }

      /**
        @brief Set Fault filter count

        These value represent the number of consecutive samples that must agree prior to the input filter accepting
        an input transition. The number of samples is the decimal value of this field plus three: the bitfield value of
        0-7 represents 3-10 samples, respectively. This value affects the input latency.

        @param value Fault filter count (0:7)
      */
      inline void setFaultFilterCount (uint8_t value) {

        m_filter.faultFilterCount = value;
      }

      /**
        @brief Fault filter count

        These value represent the number of consecutive samples that must agree prior to the input filter accepting
        an input transition. The number of samples is the decimal value of this field plus three: the bitfield value of
        0-7 represents 3-10 samples, respectively. This value  affects the input latency.

        @return Fault filter count (0:7)
      */
      inline uint8_t faultFilterCount() const {

        return m_filter.faultFilterCount;
      }

      /**
        @brief Set Fault Filter Period

        This 8-bit field applies universally to all fault inputs.
        These bits represent the sampling period (in IPBus clock cycles) of the fault pin input filter. Each input is
        sampled multiple times at the rate specified by this field. If faultFilterPeriod is 0x00 (default), then the input filter
        is bypassed. The value of faultFilterPeriod affects the input latency.
        @note When changing values for faultFilterPeriod from one non-zero value to another non-zero value, first
        write a value of zero to clear the filter.

        @param value Fault Filter Period (0:255), 0 will bypass the filter
      */
      inline void setFaultFilterPeriod (uint8_t value) {

        m_filter.faultFilterPeriod = value;
      }

      /**
        @brief Fault Filter Period

        This 8-bit field applies universally to all fault inputs.
        These bits represent the sampling period (in IPBus clock cycles) of the fault pin input filter. Each input is
        sampled multiple times at the rate specified by this field. If faultFilterPeriod is 0x00 (default), then the input filter
        is bypassed. The value of faultFilterPeriod affects the input latency.
        @note When changing values for faultFilterPeriod from one non-zero value to another non-zero value, first
        write a value of zero to clear the filter.

        @return Fault Filter Period (0:255)
      */
      inline uint8_t faultFilterPeriod() const {

        return m_filter.faultFilterPeriod;
      }

    protected:
      pwm_fault_param_t *kPwmConfig();
      const pwm_fault_param_t *kPwmConfig() const;
      pwm_fault_input_filter_param_t *kPwmFilter();
      const pwm_fault_input_filter_param_t *kPwmFilter() const;

    private:
      friend class Timer;

    private:
      pwm_fault_param_t m_config;
      pwm_fault_input_filter_param_t m_filter;
  };
}
