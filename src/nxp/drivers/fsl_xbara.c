

/*
   Copyright (c) 2015, Freescale Semiconductor, Inc.
   Copyright 2016-2019 NXP
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/

#include "fsl_xbara.h"

/* Macros for entire XBARA_SELx register.  */
#define XBARA_SELx(base, output) (((volatile uint16_t *)(&((base)->SEL0)))[(uint32_t)(output) / 2UL])

typedef union {
  uint8_t _u8[2];
  uint16_t _u16;
} xbara_u8_u16_t;

/*!
   brief Sets a connection between the selected XBARA_IN[*] input and the XBARA_OUT[*] output signal.

   This function connects the XBARA input to the selected XBARA output.
   If more than one XBARA module is available, only the inputs and outputs from the same module
   can be connected.

   Example:
   code
   XBARA_SetSignalsConnection(XBARA, kXBARA_InputPIT_TRG0, kXBARA_OutputDMAMUX18);
   endcode

   param base XBARA peripheral address.
   param input XBARA input signal.
   param output XBARA output signal.
*/
void XBARA_SetSignalsConnection (XBARA_Type *base, xbar_input_signal_t input, xbar_output_signal_t output) {
  xbara_u8_u16_t regVal;
  uint8_t byteInReg;
  uint8_t outputIndex = (uint8_t) output;

  byteInReg = outputIndex % 2U;

  regVal._u16 = XBARA_SELx (base, outputIndex);

  regVal._u8[byteInReg] = (uint8_t) input;

  XBARA_SELx (base, outputIndex) = regVal._u16;
}
