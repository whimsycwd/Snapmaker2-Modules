#include "snap_pwm.h"



static const PWM_TIM_CHN_E pwm_remap[] {
  PWM_TIM2_CH1,
  PWM_TIM2_CH2,
  PWM_TIM2_CH3,
  PWM_TIM2_CH4,
  PWM_TIM3_CH1,
  PWM_TIM3_CH2,
  PWM_TIM3_CH3,
  PWM_TIM3_CH4,
  PWM_TIM4_CH1,
  PWM_TIM4_CH2,
  PWM_TIM_CH_INVALID,
};

ErrCode SnapPWM::Out(SNAP_PWN_E pwm, uint32_t period_us, float percent) {
  if ((pwm >= PWMxx) || (percent < 0) || (percent > 100) || (period_us == 0)) {
    return E_PARAM;
  }
  uint16_t div, pluse, period;
  if (period_us >= 1000) {
    div = period_us * 72 / 1000 ;
    period = 1000;
    pluse = percent * 10;
  } else if (period_us < 1000 ) {
    div = period_us * 72 / 100 ;
    period = 100;
    pluse = percent;
  }
  HalPWM hal_pwm;
  hal_pwm.InitFromDivider(pwm_remap[pwm], div, period);
  return hal_pwm.SetPulse(pluse);
}