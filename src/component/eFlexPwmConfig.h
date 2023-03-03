/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmGlobal.h"

namespace eFlex {

  class Timer;
  class Submodule;

  /**
     @brief Configuration

  */
  class Config {
    public:

      /**
         @brief  Construct a new Config object with the default settings

         The default values are:
           - \c pwmFreqHz() = 5000
           - \c mode() = CenterAligned
           - \c debugModeEnabled() = false
           - \c waitModeEnabled() = false
           - \c reloadSelect() = kPWM_LocalReload
           - \c clockSource() = kPWM_BusClock
           - \c prescale() = kPWM_Prescale_Divide_1
           - \c initializationControl() = kPWM_Initialize_LocalSync
           - \c forceTrigger() = kPWM_Force_Local
           - \c reloadFrequency() = kPWM_LoadEveryOportunity
           - \c reloadLogic() = kPWM_ReloadImmediate
           - \c pairOperation() = kPWM_Independent
      */
      Config();

      /**
         @brief Set the PWM signal frequency in Hz
      */
      inline void setPwmFreqHz (uint32_t pwmFreqHz);
      /**
         @brief PWM signal frequency in Hz, default 5000
      */
      inline uint32_t pwmFreqHz() const;

      /**
        @brief Set the PWM operation mode
      */
      inline void setMode (pwm_mode_t  mode) {

        m_mode = mode;
      }
      /**
        @brief PWM operation mode, default \c CenterAligned
      */
      inline pwm_mode_t  mode() const {

        return m_mode;
      }

      /**
        @brief Set the reload source select
      */
      inline void setReloadSelect (pwm_reload_source_select_t reloadSelect);
      /**
         @brief Option to initialize the counter, default \c kPWM_LocalReload
      */
      inline pwm_reload_source_select_t reloadSelect() const;

      /**
         @brief Select the  Clock source for the counter
      */
      inline void setClockSource (pwm_clock_source_t clockSource);
      /**
         @brief Clock source for the counter, default \c kPWM_BusClock
      */
      inline pwm_clock_source_t clockSource() const;

      /**
         @brief Set the Pre-scaler to divide down the clock
      */
      inline void setPrescale (pwm_clock_prescale_t prescale);
      /**
         @brief Pre-scaler to divide down the clock, default \c kPWM_Prescale_Divide_1
      */
      inline pwm_clock_prescale_t prescale() const;

      /**
         @brief Set the  Option to initialize the counter
      */
      inline void setInitializationControl (pwm_init_source_t initializationControl);
      /**
         @brief Reload source select, default \c kPWM_Initialize_LocalSync
      */
      inline pwm_init_source_t initializationControl() const;

      /**
         @brief Specify which signal will trigger a FORCE_OUT
      */
      inline void setForceTrigger (pwm_force_output_trigger_t forceTrigger);
      /**
         @brief Specify which signal will trigger a FORCE_OUT, default \c kPWM_Force_Local
      */
      inline pwm_force_output_trigger_t forceTrigger() const;

      /**
         @brief Specifies when to reload
      */
      inline void setReloadFrequency (pwm_load_frequency_t reloadFrequency);
      /**
         @brief Specifies when to reload, default \c kPWM_LoadEveryOportunity

         used when user's choice is not immediate reload
      */
      inline pwm_load_frequency_t reloadFrequency() const;

      /**
         @brief Set the PWM Reload logic setup
      */
      inline void setReloadLogic (pwm_register_reload_t reloadLogic);
      /**
         @brief PWM Reload logic setup, default \c kPWM_ReloadImmediate
      */
      inline pwm_register_reload_t reloadLogic() const;

      /**
         @brief Set the channel pair in indepedent or complementary mode
      */
      inline void setPairOperation (pwm_chnl_pair_operation_t pairOperation);
      /**
         @brief Channel pair in indepedent or complementary mode, default \c kPWM_Independent
      */
      inline pwm_chnl_pair_operation_t pairOperation() const;


      /**
         @brief enable Debug Mode

         @param enable true PWM continues to run in debug mode, false PWM is paused in debug mode
      */
      inline void enableDebugMode (bool enable);
      /**
         @brief Debug mode, default false

         @return true PWM continues to run in debug mode
         @return false PWM is paused in debug mode
      */
      inline bool debugModeEnabled() const;

      /**
         @brief enable Wait Mode

         @param enable true PWM is paused in debug mode, false PWM is paused in WAIT mode
      */
      inline void enableWaitMode (bool enable);
      /**
         @brief Wait mode, default false

         @return true PWM is paused in debug mode
         @return false PWM is paused in WAIT mode
      */
      inline bool waitModeEnabled() const;

    protected:
      pwm_config_t *kPwmConfig();
      const pwm_config_t *kPwmConfig() const;

    private:
      friend class Timer;
      friend class SubModule;

    private:
      pwm_mode_t m_mode;
      uint32_t m_pwmfreq;
      pwm_config_t m_config;
  };
}

#include "source/eFlexPwmConfig_p.h"

