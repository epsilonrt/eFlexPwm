#pragma once

#include <map>
#include "fsl_pwm.h"

#define EFLEXPWM_DUMPREG_ENABLED 1

namespace eFlexPwm {

  enum {
    Pwm1 = 0,
    Pwm2,
    Pwm3,
    Pwm4
  };

  class Pin : public _pwm_signal_param {
    public:
      Pin (int number = -1);
      bool setNumber(int number);
      bool begin();

      inline bool isNull() const {

        return (m_number == -1);
      }
      inline uint8_t module () const {
        return ( (m_module >> 4) & 3);
      }
      inline uint8_t submodule () const {
        return (m_module & 3);
      }
      inline uint8_t muxval () const {
        return m_muxval;
      }
    
    private:
      int m_number;
      uint8_t m_module;
      uint8_t m_muxval;
  };

  class Config : public _pwm_config {
    public:
      Config() {
        PWM_GetDefaultConfig (this);
      }
  };

  class SubModule {
    public:
      SubModule (int pinA, int pinB = -1);
      bool begin();

      inline uint8_t index() const {

        return m_sidx;
      }

      inline bool isValid() const {

        return ! m_pin[0].isNull();
      }

      void dumpRegs (Stream &out = Serial) const;

    private:
      uint8_t m_midx;
      uint8_t m_sidx;
      Config m_config;
      Pin m_pin[2];

  };

  class Module {

    public:
      Module (uint8_t index);
      uint8_t index() const {
        return m_midx;
      }

      void dumpRegs (Stream &out = Serial) const;
      void dumpAllRegs (Stream &out = Serial) const;

    private:
      friend class SubModule;
      friend class Pin;

      uint8_t m_midx;
      pwm_submodule_t m_subm[3];
      pwm_module_control_t m_subctrl;
      std::map<uint8_t, SubModule *> subModule;

    protected:
      void addSubModule (SubModule *sm);
      static bool setupPinAsPwm (uint8_t pin);
  };
}

extern eFlexPwm::Module Pwm;
