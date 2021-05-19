#include "snap_io.h"
#include "snap_time.h"

const uint8_t snap_io_remap[] = {
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

SnapIO_Status_t snap_io_satatus[IOxx] = {0};
static uint32_t status_renew_time = 0;


void SnapIO::Init(SNAP_PIN_REMEP io, GPIO_MODE_E mode) {
  HalGPIO::StaticInit(snap_io_remap[io], mode);
}

ErrCode SnapIO::Write(SNAP_PIN_REMEP io, bool level, GPIO_MODE_E mode) {
  if (io >= IOxx) 
    return E_PARAM;
  HalGPIO hal_io(snap_io_remap[io], mode);
  hal_io.Write(level);
  return E_SUCCESS;
}

ErrCode SnapIO::Read(SNAP_PIN_REMEP io, bool &out, GPIO_MODE_E mode) {
  if (io >= IOxx) 
    return E_PARAM;
  HalGPIO hal_io(snap_io_remap[io], mode);
  out = hal_io.Read();
  return E_SUCCESS;
}

ErrCode SnapIO::ReadSwitch(SNAP_PIN_REMEP io, bool &out, GPIO_MODE_E mode) {
  return E_SUCCESS;
}

ErrCode SnapIO::Pwm(SNAP_PIN_REMEP io, uint32_t period_ms, uint8_t duty_cycle, GPIO_MODE_E mode) {
  return E_SUCCESS;
}

ErrCode SnapIO::Reset(SNAP_PIN_REMEP io, uint32_t ms, GPIO_MODE_E mode) {
  return E_SUCCESS;
}

void SnapIO::Loop() {
  SnapIO_Status_t * p_status;
  if (status_renew_time > SnapTimer::Millis()) {
    return ;
  }
  status_renew_time = SnapTimer::Millis() + 2;
  for (uint8_t i = 0; i < IOxx; i++) {
    p_status = &snap_io_satatus[i];
    if (p_status->is_port) {
      bool status  = 0;
      GPIO_MODE_E mode = (GPIO_MODE_E)p_status->mode;
      Read((SNAP_PIN_REMEP)i, status, mode);
      p_status->status <<= 1;
      p_status->status |= status;
      if ((p_status->status == SNAP_IO_STATUS_MASK) || (p_status->status == 0)) {
        if (p_status->status != p_status->last_status) {
          p_status->last_status = p_status->status;
          SnapReturnIO_t ret;
          ret.err_code = E_SUCCESS;
          ret.io = i;
          ret.type = IO_CONTROL_READ;
          ret.value = p_status->status;
          ShowResult(&ret);
        }
      }
    }
  }
}