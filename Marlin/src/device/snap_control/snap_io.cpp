#include "snap_io.h"

static const uint8_t pin_remep[] = {
  PA_0,  // IO0 
  PA_1,  // IO1 
  PA_2,  // IO2 
  PA_3,  // IO3 
  PA_4,  // IO4 
  PA_5,  // IO5 
  PA_6,  // IO6 
  PA_7,  // IO7 
  PA_8,  // IO8 
  PB_0,  // IO9 
  PB_1,  // IO10
  PB_2,  // IO11
  PB_3,  // IO12
  PB_4,  // IO13
  PB_5,  // IO14
  PB_6,  // IO15
  PB_7,  // IO16
  Pxx,   // IOxx
};

ErrCode SnapIO::Write(SNAP_PIN_REMEP io, bool level, uint8_t mode) {
  if (io >= IOxx) 
    return E_PARAM;
  HalGPIO hal_io(pin_remep[io], mode);
  hal_io.Write(level);
  return E_SUCCESS;
}

ErrCode SnapIO::Read(SNAP_PIN_REMEP io, bool &out, uint8_t mode) {
  if (io >= IOxx) 
    return E_PARAM;
  HalGPIO hal_io(pin_remep[io], mode);
  out = hal_io.Read();
  return E_SUCCESS;
}

ErrCode SnapIO::ReadSwitch(SNAP_PIN_REMEP io, bool &out, uint8_t mode) {
  return E_SUCCESS;
}

ErrCode SnapIO::Pwm(SNAP_PIN_REMEP io, uint32_t period_ms, uint8_t duty_cycle, uint8_t mode) {
  return E_SUCCESS;
}

ErrCode SnapIO::Reset(SNAP_PIN_REMEP io, uint32_t ms, uint8_t mode) {
  return E_SUCCESS;
}
