#pragma once

#include "eFlexPwm.h"

namespace eFlexPwm {

  class PowerStage : public Module {
      PowerStage (int module, int phASubmodule = 0, int phBSubmodule = -1, int phCSubmodule = -1) :
        Module (module, phASubmodule, phBSubmodule, phCSubmodule) {}

  };
}