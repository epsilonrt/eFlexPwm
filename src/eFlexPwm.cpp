/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "eFlexPwm.h"

#include <imxrt.h>
#include <core_pins.h>
#include <debug/printf.h>


struct pwm_pin_info_struct {
  uint8_t type;    // 0=no pwm, 1=flexpwm, 2=quad
  uint8_t module;  // 0-3, 0-3
  uint8_t channel; // 0=X, 1=A, 2=B
  uint8_t muxval;  //
};

extern const struct pwm_pin_info_struct pwm_pin_info[];

namespace eFlexPwm {

  PWM_Type *PWM[4] = {
    PWM1,
    PWM2,
    PWM3,
    PWM4
  };

  Pwm PM[4] = {
    Pwm (Pwm1),
    Pwm (Pwm2),
    Pwm (Pwm3),
    Pwm (Pwm4)
  };

  const pwm_submodule_t SM[4] = {
    kPWM_Module_0,
    kPWM_Module_1,
    kPWM_Module_2,
    kPWM_Module_3
  };

  #if defined(ARDUINO_TEENSY40)
  //-----------------------------------------------------------------------------
  // Teensy 4.0
#error TODO
  const int eFlexPwmPin[4][4][3] = {
  };
  #elif defined(ARDUINO_TEENSY41)
  //-----------------------------------------------------------------------------
  // Teensy 4.1
  const int PwmPin[4][4][3] = {
    // PWM1
    {
      { 45,  44,  1 },      // Submodule 0
      { 43,  42,  0 },      // Submodule 1
      { 47,  46, 24 },      // Submodule 2
      {  8,   7, 25 }       // Submodule 3
    },
    // PWM2
    {
      {  4,  33, -1 },      // Submodule 0
      {  5,  -1, -1 },      // Submodule 1
      {  6,   9, -1 },      // Submodule 2
      { 36,  37, -1 }       // Submodule 3
    },
    // PWM3
    {
      { -1,  -1, -1 },      // Submodule 0
      { 29,  28, -1 },      // Submodule 1
      { -1,  -1, -1 },      // Submodule 2
      { -1,  -1, -1 }       // Submodule 3
    },
    // PWM4
    {
      { 22,  -1, -1 },      // Submodule 0
      { 23,  -1, -1 },      // Submodule 1
      {  2,   3, -1 },      // Submodule 2
      { -1,  -1, -1 }       // Submodule 3
    },
  };
  #else
#error Unsupported board, only Teensy 4.x boards are supported !
  #endif

  //-----------------------------------------------------------------------------
  const struct pwm_pin_info_struct *pinInfo (uint8_t pin) {

    if (pin < CORE_NUM_DIGITAL) {
      const struct pwm_pin_info_struct *info = &pwm_pin_info[pin];

      if ( (info->type == 1) && (info->channel != 0)) {

        return info;
      }
    }
    return nullptr;
  }

  //-----------------------------------------------------------------------------
  // ***********                       API                            ***********
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------
  Pin::Pin (int number) {

    m_param.pwmchannelenable = true;
    m_param.level = kPWM_HighTrue;
    m_param.dutyCyclePercent = 50;
    m_param.deadtimeValue = 0;
    m_param.faultState = kPWM_PwmFaultState0;
    (void) setNumber (number);
  }

  //-----------------------------------------------------------------------------
  bool Pin::setNumber (int number) {

    if (number != -1) {
      const struct pwm_pin_info_struct *info = pinInfo (number);

      if (info) {
        if (info->channel != 0) {
          setChannel (info->channel == 1 ? kPWM_PwmA : kPWM_PwmB);
          m_number = number;
          m_module = info->module;
          m_muxval = info->muxval;
          return true;
        }
      }
    }
    setChannel (kPWM_PwmX);
    return false;
  }

  //-----------------------------------------------------------------------------
  bool Pin::begin() {

    if (isValid()) {

      * (portConfigRegister (m_number)) = m_muxval;
    }
    return isValid();
  }

  // ----------------------------------------------------------------------------
  const pwm_signal_param_t  &Pin::param() const {
    return m_param;
  }

  //-----------------------------------------------------------------------------
  //                                Config class
  //-----------------------------------------------------------------------------
  Config::Config() :
    m_mode (kPWM_CenterAligned), m_pwmfreq (5000) {

    PWM_GetDefaultConfig (& m_config);
  }

  // ----------------------------------------------------------------------------
  // protected
   pwm_config_t &Config::config() {

    return m_config;
  }

  // ----------------------------------------------------------------------------
  // protected
  const pwm_config_t &Config::config() const{

    return m_config;
  }

  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  SubModule::SubModule (int pinA, int pinB) :
    m_pin ({Pin (pinA), Pin (pinB) }) {

    if (isValid()) {

      m_midx = m_pin[0].module();
      m_sidx =  m_pin[0].submodule();
      PM[m_midx].addSubModule (this);
    }
  }

  // ----------------------------------------------------------------------------
  bool SubModule::isValid() const {
    bool success = m_pin[0].isValid();

    if (success) {
      success &= (m_pin[0].channel() == kPWM_PwmA);
      if (success && m_pin[1].isValid()) {

        success &= (m_pin[1].channel() == kPWM_PwmB);
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::configure (const Config &config) {

    if (isValid()) {

      if (PWM_Init (PWM[m_midx], SM[m_sidx], &config.config()) == kStatus_Success) {

        return (PWM_SetupPwm (PWM[m_midx], SM[m_sidx], m_pin.data(), (m_pin[1].isValid() ? 2 : 1), config.mode(), config.pwmFreqHz(), srcClockHz()) == kStatus_Success);
      }
    }
    return false;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::begin() {
    bool success = isValid();

    if (success) {

      success &= m_pin[0].begin();
      if (success) {

        enableOutput (m_pin[0].channel());
      }
      if (m_pin[1].isValid()) {

        success &= m_pin[1].begin();
        if (success) {

          enableOutput (m_pin[1].channel());
        }
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  void SubModule::dumpRegs (Stream &out) const {
    #ifdef EFLEXPWM_DUMPREG_ENABLED
    const char *names1[] = {
      "CNT",                               /**< Counter Register, array offset: 0x0, array step: 0x60 */
      "INIT",                              /**< Initial Count Register, array offset: 0x2, array step: 0x60 */
      "CTRL2",                             /**< Control 2 Register, array offset: 0x4, array step: 0x60 */
      "CTRL"                              /**< Control Register, array offset: 0x6, array step: 0x60 */
    };
    const char *names2[] = {
      "VAL0",                              /**< Value Register 0, array offset: 0xA, array step: 0x60 */
      "FRACVAL1",                          /**< Fractional Value Register 1, array offset: 0xC, array step: 0x60 */
      "VAL1",                              /**< Value Register 1, array offset: 0xE, array step: 0x60 */
      "FRACVAL2",                          /**< Fractional Value Register 2, array offset: 0x10, array step: 0x60 */
      "VAL2",                              /**< Value Register 2, array offset: 0x12, array step: 0x60 */
      "FRACVAL3",                          /**< Fractional Value Register 3, array offset: 0x14, array step: 0x60 */
      "VAL3",                              /**< Value Register 3, array offset: 0x16, array step: 0x60 */
      "FRACVAL4",                          /**< Fractional Value Register 4, array offset: 0x18, array step: 0x60 */
      "VAL4",                              /**< Value Register 4, array offset: 0x1A, array step: 0x60 */
      "FRACVAL5",                          /**< Fractional Value Register 5, array offset: 0x1C, array step: 0x60 */
      "VAL5",                              /**< Value Register 5, array offset: 0x1E, array step: 0x60 */
      "FRCTRL",                            /**< Fractional Control Register, array offset: 0x20, array step: 0x60 */
      "OCTRL",                             /**< Output Control Register, array offset: 0x22, array step: 0x60 */
      "STS",                               /**< Status Register, array offset: 0x24, array step: 0x60 */
      "INTEN",                             /**< Interrupt Enable Register, array offset: 0x26, array step: 0x60 */
      "DMAEN",                             /**< DMA Enable Register, array offset: 0x28, array step: 0x60 */
      "TCTRL",                             /**< Output Trigger Control Register, array offset: 0x2A, array step: 0x60 */
      "DISMAP"                             /**< Fault Disable Mapping Register 0, array offset: 0x2C, array step: index*0x60, index2*0x2 */
    };
    const char *names3[] = {
      "DTCNT0",                            /**< Deadtime Count Register 0, array offset: 0x30, array step: 0x60 */
      "DTCNT1",                            /**< Deadtime Count Register 1, array offset: 0x32, array step: 0x60 */
      "CAPTCTRLA",                         /**< Capture Control A Register, array offset: 0x34, array step: 0x60 */
      "CAPTCOMPA",                         /**< Capture Compare A Register, array offset: 0x36, array step: 0x60 */
      "CAPTCTRLB",                         /**< Capture Control B Register, array offset: 0x38, array step: 0x60 */
      "CAPTCOMPB",                         /**< Capture Compare B Register, array offset: 0x3A, array step: 0x60 */
      "CAPTCTRLX",                         /**< Capture Control X Register, array offset: 0x3C, array step: 0x60 */
      "CAPTCOMPX",                         /**< Capture Compare X Register, array offset: 0x3E, array step: 0x60 */
      "CVAL0",                             /**< Capture Value 0 Register, array offset: 0x40, array step: 0x60 */
      "CVAL0CYC",                          /**< Capture Value 0 Cycle Register, array offset: 0x42, array step: 0x60 */
      "CVAL1",                             /**< Capture Value 1 Register, array offset: 0x44, array step: 0x60 */
      "CVAL1CYC",                          /**< Capture Value 1 Cycle Register, array offset: 0x46, array step: 0x60 */
      "CVAL2",                             /**< Capture Value 2 Register, array offset: 0x48, array step: 0x60 */
      "CVAL2CYC",                          /**< Capture Value 2 Cycle Register, array offset: 0x4A, array step: 0x60 */
      "CVAL3",                             /**< Capture Value 3 Register, array offset: 0x4C, array step: 0x60 */
      "CVAL3CYC",                          /**< Capture Value 3 Cycle Register, array offset: 0x4E, array step: 0x60 */
      "CVAL4",                             /**< Capture Value 4 Register, array offset: 0x50, array step: 0x60 */
      "CVAL4CYC",                          /**< Capture Value 4 Cycle Register, array offset: 0x52, array step: 0x60 */
      "CVAL5",                             /**< Capture Value 5 Register, array offset: 0x54, array step: 0x60 */
      "CVAL5CYC"                           /**< Capture Value 5 Cycle Register, array offset: 0x56, array step: 0x60 */
    };

    __I uint16_t *p;
    const char **n;

    out.println ("+++++");

    p = & (PWM[m_midx]->SM[m_sidx].CNT);
    n = names1;
    while (p <= & (PWM[m_midx]->SM[m_sidx].CTRL)) {
      //out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }

    p = & (PWM[m_midx]->SM[m_sidx].VAL0);
    n = names2;
    while (p <= & (PWM[m_midx]->SM[m_sidx].DISMAP[0])) {
      //out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }

    p = & (PWM[m_midx]->SM[m_sidx].DTCNT0);
    n = names3;
    while (p <= & (PWM[m_midx]->SM[m_sidx].CVAL5CYC)) {
      //out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }
    out.println ("");
    #endif
  }

}

//-----------------------------------------------------------------------------
//                                Pwm class
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Pwm::Pwm (uint8_t index) :
  m_midx (index), m_smmask (0) {
}

//-----------------------------------------------------------------------------
bool eFlexPwm::Pwm::begin() {
  bool success = true;

  stopTimer();
  setPwmLdok (false);
  for (auto it = m_sm.begin(); it != m_sm.end(); ++it) {

    success &= it->second->begin();
  }
  setPwmLdok ();
  if (success) {

    startTimer();
  }
  return success;
}

//-----------------------------------------------------------------------------
// protected
void Pwm::addSubModule (SubModule *sm) {

  if (m_sm.count (sm->index()) == 0) {

    m_sm[sm->index()] = sm;
    m_smmask |= (1 << sm->index());
  }
}

//-----------------------------------------------------------------------------
void Pwm::dumpRegs (Stream &out) const {
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
  __I uint16_t *p = & (PWM[m_midx]->OUTEN);
  const char **n = names;

  while (p <= & (PWM[m_midx]->FCTRL2)) {

    // out.printf ("%s\t: 0x%04X\n", *n++, *p++);
  }
  out.println ("");
  #endif
}

//-----------------------------------------------------------------------------
void Pwm::dumpAllRegs (Stream &out) const {
  #ifdef EFLEXPWM_DUMPREG_ENABLED

  delay (10);
  out.println ("Pwm >>>>>");
  dumpRegs (out);
  out.println ("");

  for (auto it = m_sm.cbegin(); it != m_sm.cend(); ++it) {

    // out.printf ("SubM %u +++++\n", it->first);
    it->second->dumpRegs (out);
    out.println ("");
  }

  #endif
}

//-----------------------------------------------------------------------------
// Teensy 4.1
// eFlexPwm::Pwm Pwm (2, 0, 2, 3);
