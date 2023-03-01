#pragma once

#include "eFlexPwmTimer.h"

namespace eFlex {
  // TODO
  class PowerStage : public Timer {
      PowerStage (uint8_t index, int phASubmodule = 0, int phBSubmodule = -1, int phCSubmodule = -1) :
        Timer (index) {}

  };
}