#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include "../../HAL/hal_include.h"
#include "snap_control_base.h"
#include "sys_uart.h"

#pragma pack(1)

typedef enum {
  PWM0,
  PWM1,
  PWM2,
  PWM3,
  PWM4,
  PWM5,
  PWM6,
  PWM7,
  PWM8,
  PWM9,
  PWMxx,
}SNAP_PWN_E;

typedef struct {
  SNAP_PWN_E pwm;
  uint32_t period_us;
  float percent;
} SnapParamPWM_t;

typedef struct {
  uint8_t err_code;
} SnapReturnPWM_t;

#pragma pack()

class SnapPWM : public SnapControlBaes {
 public:
  ErrCode Out(SNAP_PWN_E pwm, uint32_t period_us, float percent);

  ErrCode ApiControl(void *api_param, void * snap_return) {
    SnapParamPWM_t * param = (SnapParamPWM_t *)api_param;
    SnapReturnPWM_t * ret = (SnapReturnPWM_t *)snap_return;
    if (!param) {
      ret->err_code = E_FAILURE;
      return E_FAILURE;
    }

    ret->err_code = Out(param->pwm, param->period_us, param->percent);
    return E_SUCCESS;
  }
  ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
    SnapReturnPWM_t *ret = (SnapReturnPWM_t *)snap_return;
    float percent = 0;
    uint8_t percent_index = 1;
    uint32_t time_rate = 1;
    if (!snap_return)
      return E_FAILURE;

    if (param.write_index) {
      uint8_t i = param.write_index;
      // 获取百分比
      if (param.param_type[i+percent_index] == PARAM_INT) {
        percent = param.value[i+percent_index].i;
      } else if (param.param_type[i+percent_index] == PARAM_FLOAT){
        percent = param.value[i+percent_index].f;
      }

      // 时间转换比例
      if (i >= 2 && param.param_type[0] == PARAM_STRING) {
        if (param.value[0].s[0] == 'm') {
          time_rate = 1000;
        }
      }
      if (param.param_type[i] == PARAM_INT) {
        ret->err_code = Out((SNAP_PWN_E)param.sub_num, param.value[i].i * time_rate, percent);
      }
    } else{
      ret->err_code = E_FAILURE;
    }
    return E_SUCCESS;
  }
  void ShowResult(void * snap_return) {
    SnapReturnPWM_t ret = *((SnapReturnPWM_t *)snap_return);
    if(ret.err_code != E_SUCCESS) {
      sys_uart.Puts("pwm");
      sys_uart.Puts(PARAMETER_ERR_STRING);
    } else {
        sys_uart.Puts("ok");
    }
    sys_uart.Putc('\n');
  }
  void AckControl(void * snap_return) {
    SnapReturnPWM_t *ret = (SnapReturnPWM_t *)snap_return;
    SendToContorl(API_ADC, ret, sizeof(SnapReturnPWM_t));
  }
};

#endif
