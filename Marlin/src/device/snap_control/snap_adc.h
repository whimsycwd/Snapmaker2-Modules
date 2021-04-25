#ifndef SNAP_ADC_H
#define SNAP_ADC_H

#include "../../HAL/hal_include.h"

typedef enum {
  AD0,
  AD1,
  AD2,
  AD3,
  AD4,
  AD5,
  AD6,
  ADxx,
}SNAP_ADC_E;

#define ADC_DEFAULT_TIM ADC_TIM_2

class SnapADC {
 public:
  uint16_t Read(SNAP_ADC_E ad);
 private:
  HalADC adc_;
  bool inited_ = false;
  uint8_t ch_[ADxx] = {ADxx};
};

#endif
