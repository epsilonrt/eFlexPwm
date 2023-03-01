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

