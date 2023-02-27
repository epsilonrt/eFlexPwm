/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "eFlexPwmGlobal.h"

namespace eFlex {

  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------

  /**
     @brief PWM pin

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
      bool isValid() const;

      /**
         @brief Timer index of this pin

         @return Timer index starting from zero (0 for PWM1, 1 for PWM2, ...)
      */
      inline uint8_t timerIndex () const;

      /**
         @brief Sumbmodule index of this pin

         @return Sumbmodule index starting from zero (0 for Submodule0, 1 for Submodule1, ...)
      */
      inline uint8_t submoduleIndex () const;

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t muxval () const;

      /**
         @brief

         @return uint8_t
      */
      inline uint8_t channel () const;

    private:
      int m_number;
      uint8_t m_module;
      uint8_t m_muxval;
      uint8_t m_channel;
  };
}

#include "eFlexPwmPin_p.h"

