/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "component/eFlexPwmSubmodule.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                            SubModule class
  //-----------------------------------------------------------------------------
  SubModule::SubModule (int pinA, int pinB) :
    m_pin {Pin (pinA), Pin (pinB) }, m_tmidx (-1), m_smidx (-1), m_ptr (0), m_wasbegin (false) {
    static_assert ( (ChanA == 0U), "kPWM_PwmB declaration mismatch; check the code !");
    static_assert ( (ChanB == 1U), "kPWM_PwmA declaration mismatch; check the code !");

    for (uint8_t i = 0; i < NofPins; i++) {

      if (m_pin[i].isValid()) {

        setSignalToDefault (m_signal[i]);
        m_signal[i].pwmChannel = (m_pin[i].channel() == ChanA ? kPWM_PwmA : kPWM_PwmB);
      }
    }

    if (isValid()) {

      m_tmidx = m_pin[ChanA].timerIndex();
      m_ptr = PWM[m_tmidx];
      m_smidx =  m_pin[ChanA].submoduleIndex();
      SmList[m_tmidx][m_smidx] = this;
      SmMask[m_tmidx] |= (1 << m_smidx);
    }
  }

  // ----------------------------------------------------------------------------
  bool SubModule::isValid() const {
    bool success;

    success = m_pin[ChanA].isValid();
    if (success) {

      success &= (m_signal[ChanA].pwmChannel == kPWM_PwmA);

      if (success && m_pin[ChanB].isValid()) {

        success &= (m_signal[ChanB].pwmChannel == kPWM_PwmB);
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::configure (const Config &config) {

    if (isValid()) {

      for (uint8_t i = 0; i < NofPins; i++) {

        if (m_pin[i].isValid()) {

          setSignalToDefault (m_signal[i]);
        }
      }
      m_config = config;
      m_fpmin = Timer::prescalerToMinPwmFrequency (config.prescale());
      m_wasbegin = false; // force update
      return (PWM_Init (ptr(), SM[m_smidx], config.kPwmConfig()) == kStatus_Success) ;
    }
    return false;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::begin (bool doStart, bool doSync) {
    bool success = isValid();

    if (success && !m_wasbegin) {

      success &= m_pin[ChanA].begin();

      if (m_pin[ChanB].isValid()) {

        success &= m_pin[ChanB].begin();
      }

      if (success) {

        m_wasbegin = success; // if not true, no update !
        success &= updateSetting (doSync);
        m_wasbegin = success;
        if (doStart && success) {

          start();
        }
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::updateSetting (bool doSync) {
    bool  success = false;

    if (m_wasbegin) {
      if (doSync) {

        setPwmLdok (false);
      }
      success = (PWM_SetupPwm (ptr(), SM[m_smidx], m_signal, (m_pin[ChanB].isValid() ? 2 : 1),
                               m_config.mode(), m_config.pwmFreqHz(), timer().srcClockHz()) == kStatus_Success);
      if (doSync) {

        setPwmLdok (true);
      }
    }
    return success;
  }

  //-----------------------------------------------------------------------------
  void SubModule::enable (bool value) {
    uint16_t mask;
    uint16_t smFlag = _BV (m_smidx);
    uint16_t reg = m_ptr->SM[m_smidx].CTRL2;

    mask = PWM_MASK_MASKA (smFlag) | PWM_MASK_UPDATE_MASK (smFlag);

    if (m_pin[ChanB].isValid()) {

      mask |= PWM_MASK_MASKB (smFlag);
    }

    if (value) {

      /* Enables the channel output */
      m_ptr->MASK &= ~mask;
    }
    else {

      /* Disables the channel output, forcing output level to 0 */
      m_ptr->MASK |= mask;
    }

    /* Select local force signal */
    m_ptr->SM[m_smidx].CTRL2 &= ~ (uint16_t) PWM_CTRL2_FORCE_SEL_MASK;
    /* Issue a local Force trigger event */
    m_ptr->SM[m_smidx].CTRL2 |= PWM_CTRL2_FORCE_MASK;
    /* Restore the source of FORCE OUTPUT signal */
    m_ptr->SM[m_smidx].CTRL2 = reg;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::isEnabled() const {
    uint16_t mask;
    uint16_t smFlag = _BV (m_smidx);

    mask = PWM_MASK_MASKA (smFlag) | (m_pin[ChanB].isValid() ? PWM_MASK_MASKB (smFlag) : 0);

    return ( (m_ptr->MASK & mask) == 0);
  }

  //-----------------------------------------------------------------------------
  bool SubModule::adjustPrescaler (uint32_t freq) {
    pwm_clock_prescale_t prescaler = ( (freq < minPwmFrequency()) ? config().prescale() : kPWM_Prescale_Divide_1);

    while ( (freq < Timer::prescalerToMinPwmFrequency (prescaler)) && (prescaler < kPWM_Prescale_Divide_128)) {

      prescaler = static_cast<pwm_clock_prescale_t> (static_cast<unsigned> (prescaler) + 1);
    }

    if (config().prescale() != prescaler) {

      setPrescaler (prescaler);
      return true;
    }
    return false;
  }

  //-----------------------------------------------------------------------------
  bool SubModule::setPwmFrequency (uint32_t freq, bool doSync, bool adjust) {
    uint32_t pwmClock;
    uint16_t pulseCnt, pwmHighPulse;
    uint16_t modulo;
    uint8_t numOfChnls = (m_pin[ChanB].isValid() ? 2 : 1);

    if (doSync) {

      setPwmLdok (false);
    }

    if (adjust) {

      adjustPrescaler (freq);
    }

    /* Divide the clock by the prescale value */
    pwmClock = (timer().srcClockHz() / (1UL << ( (m_ptr->SM[m_smidx].CTRL & PWM_CTRL_PRSC_MASK) >> PWM_CTRL_PRSC_SHIFT)));
    pulseCnt = (uint16_t) (pwmClock / freq);

    /* Setup each PWM channel */
    for (uint8_t i = 0; i < numOfChnls; i++) {

      /* Calculate pulse width */
      pwmHighPulse = static_cast<uint32_t>(1UL * pulseCnt * m_duty[i]) / 65535U;

      /* Setup the different match registers to generate the PWM signal */
      switch (m_config.mode()) {

        case kPWM_SignedCenterAligned:
          /* Setup the PWM period for a signed center aligned signal */
          if (i == 0U) {

            modulo = (pulseCnt >> 1U);
            /* Indicates the start of the PWM period */
            m_ptr->SM[m_smidx].INIT = u16TwoCompl (modulo);
            /* Indicates the center value */
            m_ptr->SM[m_smidx].VAL0 = 0;
            /* Indicates the end of the PWM period */
            /* The change during the end to start of the PWM period requires a count time */
            m_ptr->SM[m_smidx].VAL1 = modulo - 1U;
          }

          /* Setup the PWM dutycycle */
          if (m_signal[i].pwmChannel == kPWM_PwmA) {

            m_ptr->SM[m_smidx].VAL2 = u16TwoCompl (pwmHighPulse / 2U);
            m_ptr->SM[m_smidx].VAL3 = (pwmHighPulse / 2U);
          }
          else {

            m_ptr->SM[m_smidx].VAL4 = u16TwoCompl (pwmHighPulse / 2U);
            m_ptr->SM[m_smidx].VAL5 = (pwmHighPulse / 2U);
          }
          break;

        case kPWM_CenterAligned:
          /* Setup the PWM period for an unsigned center aligned signal */
          /* Indicates the start of the PWM period */
          if (i == 0U) {

            m_ptr->SM[m_smidx].INIT = 0;
            /* Indicates the center value */
            m_ptr->SM[m_smidx].VAL0 = (pulseCnt / 2U);
            /* Indicates the end of the PWM period */
            /* The change during the end to start of the PWM period requires a count time */
            m_ptr->SM[m_smidx].VAL1 = pulseCnt - 1U;
          }

          /* Setup the PWM dutycycle */
          if (m_signal[i].pwmChannel == kPWM_PwmA) {

            m_ptr->SM[m_smidx].VAL2 = ( (pulseCnt - pwmHighPulse) / 2U);
            m_ptr->SM[m_smidx].VAL3 = ( (pulseCnt + pwmHighPulse) / 2U);
          }
          else {

            m_ptr->SM[m_smidx].VAL4 = ( (pulseCnt - pwmHighPulse) / 2U);
            m_ptr->SM[m_smidx].VAL5 = ( (pulseCnt + pwmHighPulse) / 2U);
          }
          break;

        case kPWM_SignedEdgeAligned:
          /* Setup the PWM period for a signed edge aligned signal */
          if (i == 0U) {

            modulo = (pulseCnt >> 1U);
            /* Indicates the start of the PWM period */
            m_ptr->SM[m_smidx].INIT = u16TwoCompl (modulo);
            /* Indicates the center value */
            m_ptr->SM[m_smidx].VAL0 = 0;
            /* Indicates the end of the PWM period */
            /* The change during the end to start of the PWM period requires a count time */
            m_ptr->SM[m_smidx].VAL1 = modulo - 1U;
          }

          /* Setup the PWM dutycycle */
          if (m_signal[i].pwmChannel == kPWM_PwmA) {

            m_ptr->SM[m_smidx].VAL2 = u16TwoCompl (modulo);
            m_ptr->SM[m_smidx].VAL3 = u16TwoCompl (modulo) + pwmHighPulse;
          }
          else {

            m_ptr->SM[m_smidx].VAL4 = u16TwoCompl (modulo);
            m_ptr->SM[m_smidx].VAL5 = u16TwoCompl (modulo) + pwmHighPulse;
          }
          break;

        case kPWM_EdgeAligned:
          /* Setup the PWM period for a unsigned edge aligned signal */
          /* Indicates the start of the PWM period */
          if (i == 0U) {

            m_ptr->SM[m_smidx].INIT = 0;
            /* Indicates the center value */
            m_ptr->SM[m_smidx].VAL0 = (pulseCnt / 2U);
            /* Indicates the end of the PWM period */
            /* The change during the end to start of the PWM period requires a count time */
            m_ptr->SM[m_smidx].VAL1 = pulseCnt - 1U;
          }

          /* Setup the PWM dutycycle */
          if (m_signal[i].pwmChannel == kPWM_PwmA) {

            m_ptr->SM[m_smidx].VAL2 = 0;
            m_ptr->SM[m_smidx].VAL3 = pwmHighPulse;
          }
          else {

            m_ptr->SM[m_smidx].VAL4 = 0;
            m_ptr->SM[m_smidx].VAL5 = pwmHighPulse;
          }
          break;
        default:
          return false;
          break;
      }
    }
    if (doSync) {

      setPwmLdok (true);
    }

    m_config.setPwmFreqHz (freq);
    return true;
  }

  //-----------------------------------------------------------------------------
  void SubModule::printRegs (Stream   &out) {
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
      "CVAL5CYC",                          /**< Capture Value 5 Cycle Register, array offset: 0x56, array step: 0x60 */
      "PHASEDLY"                           /**< Phase Delay Register, array offset: 0x58, array step: 0x60 */
    };

    __IM uint16_t *p;
    const char **n;

    out.println ("+++++");

    p = & (ptr()->SM[m_smidx].CNT);
    n = names1;
    while (p <= & (ptr()->SM[m_smidx].CTRL)) {
      out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }

    p = & (ptr()->SM[m_smidx].VAL0);
    n = names2;
    while (p <= & (ptr()->SM[m_smidx].DISMAP[0])) {
      out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }

    p = & (ptr()->SM[m_smidx].DTCNT0);
    n = names3;
    while (p <= & (ptr()->SM[m_smidx].CVAL5CYC)) {
      out.printf ("%s\t: 0x%04X\n", *n++, *p++);
    }
    out.println ("");
    #endif
  }

  // List of registered submodules
  // array initialized to zero, the sub-modules are saved in this array by their constructor.
  // [Timer][SubModule]
  SubModule *SmList[NofTimers][NofSubmodules];
}