#include "snap_adc.h"
#include "../../HAL/hal_include.h"

static const ADC_CHN_E adc_chn_remap[] = {
  ADC_CH_0,  // AD0
  ADC_CH_4,  // AD1
  ADC_CH_5,  // AD2
  ADC_CH_6,  // AD3
  ADC_CH_7,  // AD4
  ADC_CH_8,  // AD5
  ADC_CH_9,  // AD6
  ADC_CH_INVALID,  // ADxx
};

uint16_t SnapADC::Read(SNAP_ADC_E ad) {
  uint16_t ret = 0;
  if (ad >= ADxx)
    return 0;
  if (!inited_) {
    for (uint8_t i = 0; i < ADxx; i++) {
      ch_[i] = ADxx;
    }
    ch_[ad] = adc_.Init(adc_chn_remap[ad], ADC_TIM_2, 200);
    inited_ = true;
  }

  if (ch_[ad] >= ADxx)
    ch_[ad] = adc_.Register(adc_chn_remap[ad]);
  
  if (ch_[ad] < ADxx) {
    ret = adc_.ReadNext(ch_[ad]);
  }
  return ret;
}