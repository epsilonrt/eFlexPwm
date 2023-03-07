/*
   Copyright (c) 2023, epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/

#include "component/XBara.h"
//#include <pins_arduino.h>

struct XBaraPinInfo {
  uint8_t index;
  xbar_input_signal_t input;
  uint8_t xbar_alt;
};

#ifdef ARDUINO_TEENSY41
//#if 1
const XBaraPinInfo PinInfo[] = {
  {0, kXBARA1_InputIomuxXbarInout17, 1},
  {1, kXBARA1_InputIomuxXbarInout16, 1},
  {2, kXBARA1_InputIomuxXbarInout06, 3},
  {3, kXBARA1_InputIomuxXbarInout07, 3},
  {4, kXBARA1_InputIomuxXbarInout08, 3},
  {5, kXBARA1_InputIomuxXbarInout17, 3},
  {7, kXBARA1_InputIomuxXbarInout15, 1},
  {8, kXBARA1_InputIomuxXbarInout14, 1},
  {30, kXBARA1_InputIomuxXbarIn23, 1},
  {31, kXBARA1_InputIomuxXbarIn22, 1},
  {32, kXBARA1_InputIomuxXbarInout10, 1},
  {33, kXBARA1_InputIomuxXbarInout09, 3},
  {36, kXBARA1_InputIomuxXbarInout16, 1},
  {37, kXBARA1_InputIomuxXbarInout17, 1},
  {42, kXBARA1_InputIomuxXbarInout07, 3},
  {43, kXBARA1_InputIomuxXbarInout06, 3},
  {44, kXBARA1_InputIomuxXbarInout05, 3},
  {45, kXBARA1_InputIomuxXbarInout04, 3},
  {46, kXBARA1_InputIomuxXbarInout09, 3},
  {47, kXBARA1_InputIomuxXbarInout08, 3}
};
#else
// TODO Teensy4.0
#error board not supported
#endif

//-----------------------------------------------------------------------------
const xbar_input_signal_t *XBaraSetupInput (int pin, uint8_t mode) {
  const xbar_input_signal_t *input = nullptr;

  for (unsigned i = 0; i < ARRAY_SIZE (PinInfo); i++) {

    if (pin == PinInfo[i].index) {
      const struct digital_pin_bitband_and_config_table_struct *p = &digital_pin_to_info_PGM[pin];
      // Serial.printf ("%s:PAD 0x%04X\n", __func__, * (p->pad));
      // Serial.printf ("%s:MUX 0x%04X\n", __func__, * (p->mux));

      switch (mode) {
        case INPUT_PULLUP:
          * (p->pad) =  IOMUXC_PAD_DSE (7) | IOMUXC_PAD_PKE | IOMUXC_PAD_PUE | IOMUXC_PAD_PUS (3) | IOMUXC_PAD_HYS;
          break;
        case INPUT_PULLDOWN:
          * (p->pad) =  IOMUXC_PAD_DSE (7) | IOMUXC_PAD_PKE | IOMUXC_PAD_PUE | IOMUXC_PAD_PUS (0) | IOMUXC_PAD_HYS;
          break;
        default :
          * (p->pad) =  IOMUXC_PAD_DSE (7);
          break;
      }
      * (p->mux) = PinInfo[i].xbar_alt;
      input = &PinInfo[i].input;
      // Serial.printf ("%s:PAD 0x%04X\n", __func__, * (p->pad));
      // Serial.printf ("%s:MUX 0x%04X\n", __func__, * (p->mux));
      break;
    }
  }
  return input;
}
