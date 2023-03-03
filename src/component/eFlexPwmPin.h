/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmGlobal.h"

namespace eFlex {

  /**
     @brief Arduino pin

     This class allows to configure, initialize the PWM pins
  */
  class Pin {
    public:
      /**
         @brief Construct a new Pin object

         @param number Arduino pin number (must be PWM eFlexPWM pin)
      */
      Pin (int number = -1);

      /**
         @brief Enables pin for PWM use

         @return \c isValid()
      */
      bool begin();

      /**
         @brief Checks if pin is valid

         A pin is valid if its number is different from -1 and if channel is different from kPWM_PwmX
      */
      inline bool isValid() const {

        return (m_number != -1);
      }

      /**
         @brief Timer index of this pin

         @return Timer index starting from zero (0 for PWM1, 1 for PWM2, ...)
      */
      inline uint8_t timerIndex () const {
        return ( (m_module >> 4) & 3);
      }

      /**
         @brief Sumbmodule index of this pin

         @return Sumbmodule index starting from zero (0 for Submodule0, 1 for Submodule1, ...)
      */
      inline uint8_t submoduleIndex () const {
        return (m_module & 3);
      }

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t muxval () const {
        return m_muxval;
      }

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t channel () const {
        return m_channel;
      }

    private:
      int m_number;
      uint8_t m_module;
      uint8_t m_muxval;
      uint8_t m_channel;
  };
}
