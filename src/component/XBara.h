/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "nxp/drivers/fsl_xbara.h"
#include <Arduino.h>

/**
   @brief Setting up pin as XBARA input

   @param arduinoPinNum Arduino pin number
   @param inputMode Arduino input pin mode : INPUT, INPUT_PULLUP, INPUT_PULLDOWN (any other value is considered INPUT)
   @return XBARA input signal to pass to \ref XBaraConnect, nullptr if not found
*/
const xbar_input_signal_t *XBaraSetupInput (int arduinoPinNum, uint8_t inputMode = INPUT);

/*!
   @brief Sets a connection between the selected XBARA_IN[*] input and the XBARA_OUT[*] output signal.

   This function connects the XBARA input to the selected XBARA output.
   If more than one XBARA module is available, only the inputs and outputs from the same module
   can be connected.

   Example:
   @code
   XBaraConnect (kXBARA_InputPIT_TRG0, kXBARA_OutputDMAMUX18);
   @endcode

   @param input XBARA input signal \ref xbar_input_signal_t
   @param output XBARA output signal \ref xbar_output_signal_t
*/
inline void XBaraConnect (xbar_input_signal_t input, xbar_output_signal_t output) {
  XBARA_SetSignalsConnection (XBARA1, input, output);
}
