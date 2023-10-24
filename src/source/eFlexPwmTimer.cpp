/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmSubmodule.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                Timer class
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  Timer::Timer (uint8_t index) :
    m_tmidx (index), m_ptr (PWM[index]), m_isenabled (true) {
  }

  //-----------------------------------------------------------------------------
  bool Timer::beginSubModules (uint8_t subModulesToBegin, bool doStart, bool doSync) {
    bool success = true;

    if (doStart) {

      start (subModulesToBegin, false);
    }

    if (doSync) {

      setPwmLdok (subModulesToBegin, false);
    }

    for (uint8_t i = 0; (i < NofSubmodules) && success; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s && (subModulesToBegin & (1 << i))) {

        success &= s ->begin (false, false);
      }
    }

    if (doSync) {

      setPwmLdok (subModulesToBegin, true);
    }
    if (doStart && success) {

      start (subModulesToBegin, true);
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  void Timer::enable (bool value) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {
        s->enable (value);
      }
    }

    m_isenabled = value;
  }


  // ----------------------------------------------------------------------------
  void Timer::setupLevel (pwm_level_select_t level) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setupLevel (level);
      }
    }
  }

  // ----------------------------------------------------------------------------
  void Timer::setupDeadtime (uint16_t deadtimeValue, uint32_t unit) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setupDeadtime (deadtimeValue, unit);
      }
    }
  }

  // ----------------------------------------------------------------------------
  void Timer::setupOutputEnable (bool activate) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setupOutputEnable (activate);
      }
    }
  }

  // ----------------------------------------------------------------------------
  void Timer::setupDutyCycle (uint16_t dutyCycle) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {
      SubModule *s = SmList[m_tmidx][i];
      if (s) {
        s ->setupDutyCycle (dutyCycle);
      }
    }
  }

  // ----------------------------------------------------------------------------
  void Timer::setupFaultState (pwm_fault_state_t faultState) {

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        s ->setupFaultState (faultState);
      }
    }
  }

  //-----------------------------------------------------------------------------
  bool Timer::updateSetting (bool doSync) {
    bool success = true;

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        success &= s ->updateSetting (doSync);
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  void Timer::printRegs (Stream &out) const {
    #ifdef EFLEXPWM_DUMPREG_ENABLED
    const char *names[] = {
      "OUTEN",                             /**< Output Enable Register, offset: 0x180 */
      "MASK",                              /**< Mask Register, offset: 0x182 */
      "SWCOUT",                            /**< Software Controlled Output Register, offset: 0x184 */
      "DTSRCSEL",                          /**< PWM Source Select Register, offset: 0x186 */
      "MCTRL",                             /**< Master Control Register, offset: 0x188 */
      "MCTRL2",                            /**< Master Control 2 Register, offset: 0x18A */
      "FCTRL",                             /**< Fault Control Register, offset: 0x18C */
      "FSTS",                              /**< Fault Status Register, offset: 0x18E */
      "FFILT",                             /**< Fault Filter Register, offset: 0x190 */
      "FTST",                              /**< Fault Test Register, offset: 0x192 */
      "FCTRL2"                             /**< Fault Control 2 Register, offset: 0x194 */
    };
    __IM uint16_t *p = & (ptr()->OUTEN);
    const char **n = names;

    while (p <= & (ptr()->FCTRL2)) {

      out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }
    out.println ("");
    #endif
  }

  //-----------------------------------------------------------------------------
  void Timer::printAllRegs (Stream &out) const {
    #ifdef EFLEXPWM_DUMPREG_ENABLED

    delay (10);
    out.println ("Timer >>>>>");
    printRegs (out);
    out.println ("");

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        out.printf ("SubM %u +++++\n", i);
        s->printRegs (out);
        out.println ("");
      }
    }
    #endif
  }

  class TimerFactory : public Timer {
    public:
      /**
        @brief Construct a new Timer Module

        @param index the timer module index (0 for PWM1...)
      */
      TimerFactory (uint8_t index) : Timer (index) {}
  };

  // All timers are instantiated globally
  TimerFactory TMF[NofTimers] = {
    TimerFactory (Pwm1),
    TimerFactory (Pwm2),
    TimerFactory (Pwm3),
    TimerFactory (Pwm4)
  };

  Timer *TM[NofTimers] = {
    & TMF[Pwm1],
    & TMF[Pwm2],
    & TMF[Pwm3],
    & TMF[Pwm4]
  };

}
