#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include "../../HAL/hal_include.h"

#define DEFAULT_PIN_R_MODE GPIO_IN_FLOATING
#define DEFAULT_PIN_W_MODE GPIO_OUT_OD

typedef enum {
 IO0,  IO1,  IO2,  IO3,  IO4,  IO5,  IO6, IO7, IO8, IO9,
 IO10, IO11, IO12, IO13, IO14, IO15, IO16, IOxx,
}SNAP_PIN_REMEP;

class SnapIO {
 public:
  SnapIO() {
    HalGPIO::DisableDebugIO();
  }
  ErrCode Write(SNAP_PIN_REMEP io, bool level, uint8_t mode=DEFAULT_PIN_W_MODE);
  ErrCode Read(SNAP_PIN_REMEP io, bool &out, uint8_t mode=DEFAULT_PIN_R_MODE);
  ErrCode ReadSwitch(SNAP_PIN_REMEP io, bool &out, uint8_t mode=DEFAULT_PIN_R_MODE);
  ErrCode Pwm(SNAP_PIN_REMEP io, uint32_t period_ms, uint8_t duty_cycle, uint8_t mode=DEFAULT_PIN_W_MODE);
  ErrCode Reset(SNAP_PIN_REMEP io, uint32_t ms, uint8_t mode=DEFAULT_PIN_W_MODE);
};

#endif
