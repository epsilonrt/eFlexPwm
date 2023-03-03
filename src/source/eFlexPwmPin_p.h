#include "component/eFlexPwmPin.h"
/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

namespace eFlex {

  #ifndef DOXYGEN
  //-----------------------------------------------------------------------------
  //                                Pin class
  //-----------------------------------------------------------------------------
  inline bool Pin::isValid() const {

    return (m_number != -1);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::timerIndex () const {
    return ( (m_module >> 4) & 3);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::submoduleIndex () const {
    return (m_module & 3);
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::channel () const {
    return m_channel;
  }

  // ----------------------------------------------------------------------------
  inline uint8_t Pin::muxval () const {
    return m_muxval;
  }
  #endif
}
