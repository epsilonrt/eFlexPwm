/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmTimer.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                Timer class
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  Timer::Timer (uint8_t index) :
    m_tmidx (index), m_ptr (PWM[index]), m_isenabled (true) {
  }

  //-----------------------------------------------------------------------------
  bool Timer::begin (bool doStart, bool doSync) {
    bool success = true;

    if (doStart) {

      stop();
    }

    if (doSync) {

      setPwmLdok (false);
    }

    for (uint8_t i = 0; (i < NofSubmodules) && success; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        success &= s ->begin (false, false);
      }
    }

    if (doSync) {

      setPwmLdok (true);
    }
    if (doStart && success) {

      start();
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

  //-----------------------------------------------------------------------------
  void Timer::dumpRegs (Stream &out) const {
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
  void Timer::dumpAllRegs (Stream &out) const {
    #ifdef EFLEXPWM_DUMPREG_ENABLED

    delay (10);
    out.println ("Timer >>>>>");
    dumpRegs (out);
    out.println ("");

    for (uint8_t i = 0; i < NofSubmodules; i++) {

      SubModule *s = SmList[m_tmidx][i];
      if (s) {

        out.printf ("SubM %u +++++\n", i);
        s->dumpRegs (out);
        out.println ("");
      }
    }
    #endif
  }
}
