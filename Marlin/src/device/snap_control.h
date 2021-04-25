#ifndef SNAP_CONTROL_H
#define SNAP_CONTROL_H

#include "./snap_control/snap_io.h"
#include "./snap_control/sys_uart.h"
#include "./snap_control/snap_adc.h"
#include "./snap_control/snap_pwm.h"


class SnapControl {
 public:
  void Loop() {
    uart1.Loop();
  }
  void Init() {
    uart1.Init();
  }

 public:
  SnapIO io;
  SysUart uart1;
  SnapADC adc;
  SnapPWM pwm;
};

extern SnapControl control_;

#endif
