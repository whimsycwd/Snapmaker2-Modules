#ifndef SNAP_CONTROL_H
#define SNAP_CONTROL_H

#include "./snap_control/snap_io.h"
#include "./snap_control/sys_uart.h"
#include "./snap_control/snap_adc.h"
#include "./snap_control/snap_pwm.h"
#include "./snap_control/snap_index.h"
#include "./snap_control/snap_uart.h"
#include "./snap_control/snap_time.h"
#include "./snap_control/snap_gcode.h"
#include "./snap_control/snap_mpu.h"
#include "./snap_control/snap_rgb.h"

#pragma pack(1)

typedef struct {
  uint8_t type:3;
  uint8_t len:3;
} SnapApiParamHead_t;

typedef struct {
  uint8_t api_type;
  union {
    SnapParamIO_t io;
    SnapParamPWM_t pwm;
    SnapParamIndex_t index;
    SnapParamADC_t adc;
    SnapParamGcode_t gcode;
    SnapParamMPU_t mpu;
    SnapParamRGB_t rgb;
  };
} SnapApiParam_t;

typedef struct {
  union {
    SnapReturnIO_t io;
    SnapReturnPWM_t pwm;
    SnapReturnIndex_t index;
    SnapReturnADC_t adc;
    SnapReturnGcode_t gcode;
    SnapReturnMPU_t mpu;
    SnapReturnRGB_t rgb;
  };
} SnapApiRetuen_t;

#pragma pack()

class SnapControl {
 public:
  void Loop() {
    sys_uart.Loop();
    io.Loop();
    rgb.Loop();
  }
  void Init() {
    sys_uart.Init();
    HalGPIO::DisableDebugIO();
  }

 public:
  SnapIO io;
  SnapADC adc;
  SnapPWM pwm;
  SnapIndex index;
  SnapUart uart;
  SnapTimer timer;
  SnapGcode gcode;
  SnapMPU mpu;
  SnapRGB rgb;
};

extern SnapControl control_;

#endif
