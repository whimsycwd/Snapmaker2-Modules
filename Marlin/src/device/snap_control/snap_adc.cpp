#include "snap_adc.h"
#include "../../HAL/hal_include.h"
#include "io.h"

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

static const SNAP_PIN_REMEP adc_pin_remap[] = {
  IO1,  // PA1
  IO4,  // PA4
  IO5,  // PA5
  IO6,  // PA6
  IO7,  // PA7
  IO9,  // PB0
  IO10,  // PB1
};
uint16_t SnapADC::Read(SNAP_ADC_E ad) {
  uint16_t buf[ADC_BUF_SIZE];
  uint16_t max=0, min=0xffff;
  uint8_t count = 0;
  uint32_t total_1=0, total_2=0;
  if (ad >= ADxx)
    return 0;
  if (!inited_[ad]) {
    SnapIO::Init(adc_pin_remap[ad], GPIO_AIN);
    inited_[ad] = true;
  }
  for (uint8_t i = 0; i < ADC_BUF_SIZE; i++) {
    buf[i] =  analogRead(adc_pin_remap[ad]);
    if (buf[i] > max) {
      max = buf[i];
    }
    if (buf[i] < min) {
      min = buf[i];
    }
  }
  for (uint8_t i = 0; i < ADC_BUF_SIZE; i++) {
    if (max != buf[i] && min != buf[i]) {
      count++;
      total_1 += buf[i];
    }
    total_2 += buf[i];
  }
  if (count == 0) {
    return total_2 / ADC_BUF_SIZE;
  }
  return total_1 / count;
}

uint16_t SnapADC::ReadFromIO(SNAP_PIN_REMEP io) {  
  for (uint8_t i = 0; i < IOxx; i++) {
    if (adc_pin_remap[i] == io) {
      return Read((SNAP_ADC_E)i);
    }
  }
  return 0;
}