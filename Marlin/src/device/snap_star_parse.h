#include "../HAL/hal_include.h"
#include "./snap_control.h"
#include "snap_control/snap_control_base.h"


class SnapStarParse {
 public:
  static ErrCode ParamAndExecute(uint8_t *cmd, SNAP_CMD_TYPE_E cmd_type, SNAP_CMD_SOUREC_E source);
  static void loop();
};
