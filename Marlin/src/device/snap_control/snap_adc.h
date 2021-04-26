#ifndef SNAP_ADC_H
#define SNAP_ADC_H

#include "../../HAL/hal_include.h"
#include "snap_control_base.h"
#include "sys_uart.h"
#include "snap_io.h"

#pragma pack(1)
#define ADC_BUF_SIZE 15
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

typedef struct {
  SNAP_ADC_E adc;
} SnapParamADC_t;

typedef struct {
  uint8_t err_code;
  uint8_t ad;
  uint16_t value;
} SnapReturnADC_t;

#pragma pack()
#pragma pack()


#define ADC_DEFAULT_TIM ADC_TIM_2

class SnapADC : public SnapControlBaes {
 public:
  SnapADC() {
    for (uint8_t i = 0; i < ADxx; i++) {
      inited_[i] = false;
    }
  }
  uint16_t Read(SNAP_ADC_E ad);
  uint16_t ReadFromIO(SNAP_PIN_REMEP io);
  ErrCode ApiControl(void *api_param, void * snap_return) {
    return E_SUCCESS;
  }
  ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
    SnapReturnADC_t *ret = (SnapReturnADC_t *)snap_return;
    if (!snap_return)
      return E_FAILURE;
    ret->err_code = E_FAILURE;
    ret->ad = param.sub_num;
    if (param.sub_num >= ADxx) {
      return E_FAILURE;
    }
    ret->value = Read((SNAP_ADC_E)param.sub_num);
    ret->err_code = E_SUCCESS;
    return E_SUCCESS;
  }
  void ShowResult(void * snap_return) {
    SnapReturnADC_t *ret = (SnapReturnADC_t *)snap_return;
    
    sys_uart.Puts("adc");
    sys_uart.PutInt(ret->ad);
    sys_uart.Putc(':');
    if (ret->err_code == E_SUCCESS) {
      sys_uart.PutInt(ret->value);
    } else {
      sys_uart.Puts(PARAMETER_ERR_STRING);
    }
    sys_uart.Putc('\n');
  }

  void AckControl(void * snap_return) {
    SnapReturnADC_t *ret = (SnapReturnADC_t *)snap_return;
    SendToContorl(API_ADC, ret, sizeof(SnapReturnADC_t));
  }
 private:
  HalADC adc_;
  bool inited_[ADxx];
  uint8_t ch_[ADxx] = {ADxx};
};

#endif
