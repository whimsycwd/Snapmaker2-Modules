#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include "../../HAL/hal_include.h"

typedef enum {
  PWM0,
  PWM1,
  PWM2,
  PWM3,
  PWM4,
  PWM5,
  PWMxx,
}SNAP_PWN_E;

class SnapPWM {
 public:
  ErrCode Out(SNAP_PWN_E pwm, uint32_t period_us, float percent);
};

#endif
