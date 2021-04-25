#include "snap_pwm.h"



static const PWM_TIM_CHN_E pwm_remap[] {
  PWM_TIM2_CH1,
  PWM_TIM2_CH2,
  PWM_TIM3_CH1,
  PWM_TIM3_CH2,
  PWM_TIM4_CH1,
  PWM_TIM4_CH2,
  PWM_TIM_CH_INVALID,
};

ErrCode SnapPWM::Out(SNAP_PWN_E pwm, uint32_t period_us, float percent) {
  if ((pwm >= PWMxx) || (percent < 0) || (percent > 100)) {
    return E_PARAM;
  }
  uint32_t mc = period_us * 1000;
  HalPWM hal_pwm(pwm_remap[pwm], mc, (uint16_t)1000);
  return hal_pwm.SetPulse((uint16_t)(percent * 10));
}