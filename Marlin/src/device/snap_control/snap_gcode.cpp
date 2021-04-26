#include "snap_gcode.h"
#include "snap_time.h"

ErrCode SnapGcode::Send(uint8_t *gcode, bool forced_send) {
  if (forced_send || !busy_) {
    uint8_t len = strlen((char *)gcode);
    SnapParamGcode_t param;

    StringCopy(param.data, gcode, len);
    SendToContorl(API_GCODE, &param, len);
    busy_ = true;
  }
  return E_SUCCESS;
}

ErrCode SnapGcode::SendSync(uint8_t *gcode, uint32_t timeout_ms) {
  uint32_t timeout = SnapTimer::Millis() + timeout_ms;
  Send(gcode, true);
  while ((timeout > SnapTimer::Millis()) || !timeout_ms) {
    if (!busy_) {
      return E_SUCCESS;
    }
    SnapTimer::DelayMs(2);
  }
  return E_FAILURE;
}
