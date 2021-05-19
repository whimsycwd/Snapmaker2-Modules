#ifndef SNAP_RGB_H
#define SNAP_RGB_H

#include "../rgb_light.h"
#include "snap_control_base.h"
#include "sys_uart.h"
#include "snap_io.h"

#pragma pack(1)

typedef enum {
  SNAP_RGB_MODE_LIGHT,
  SNAP_RGB_MODE_BREATHE,
} SNAP_RGB_MODE_E;

typedef struct {
  SNAP_RGB_MODE_E mode;
  uint8_t count;
  RGB_T rgb;
} SnapParamRGB_t;

typedef struct {
  uint8_t err_code;
} SnapReturnRGB_t;

#pragma pack()

class SnapRGB : public SnapControlBaes {
 public:
  ErrCode Set(SNAP_PIN_REMEP io, RGB_T rgb, uint8_t count);
  ErrCode Breathe(SNAP_PIN_REMEP io, RGB_T rgb, uint8_t count, uint32_t breathe_period_ms);
  void Loop();
  ErrCode ApiControl(void *api_param, void * snap_return) {
    // SnapParamRGB_t * param = (SnapParamRGB_t *)api_param;
    // SnapReturnRGB_t * ret = (SnapReturnRGB_t *)snap_return;
    // if (!param) {
    //   ret->err_code = E_FAILURE;
    //   return E_FAILURE;
    // }
    return E_SUCCESS;
  }
  ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
    SnapReturnRGB_t *ret = (SnapReturnRGB_t *)snap_return;
    if (!snap_return || (param.sub_num >= IOxx))
      return E_FAILURE;
    ret->err_code = E_FAILURE;
    uint8_t write_index = param.write_index;
    SNAP_RGB_MODE_E rgb_mode = SNAP_RGB_MODE_LIGHT;
    for (uint8_t i = 0; i < param.count && (i < write_index || !write_index); i++) {
      if (param.param_type[i] == PARAM_STRING && (param.value[i].s[0] == 'b')) {
        rgb_mode = SNAP_RGB_MODE_BREATHE;
        break;
      }
    }

    RGB_T rgb = {0, 0, 0};
    uint8_t w_param_count = param.count - write_index;
    uint8_t rgb_count = 1;
    uint32_t periods = 3000;
    if (write_index && (w_param_count >= 3)) {
      rgb.r = param.value[write_index].i;
      rgb.g = param.value[write_index+1].i;
      rgb.b = param.value[write_index+2].i;
      if (rgb_mode == SNAP_RGB_MODE_LIGHT) {
        if (w_param_count >= 4) {
          rgb_count = param.value[write_index+3].i;
        }
        ret->err_code = Set((SNAP_PIN_REMEP)param.sub_num, rgb, rgb_count);
      } else {
        if (w_param_count >= 4) {
          periods = param.value[write_index+3].i;
        }
        if (w_param_count >= 5) {
          rgb_count = param.value[write_index+4].i;
        }
        ret->err_code = Breathe((SNAP_PIN_REMEP)param.sub_num, rgb, rgb_count, periods);
      }
    }
    return E_SUCCESS;
  }
  void ShowResult(void * snap_return) {
    SnapReturnRGB_t ret = *((SnapReturnRGB_t *)snap_return);
    if(ret.err_code != E_SUCCESS) {
      sys_uart.Puts("rgb");
      sys_uart.Puts(PARAMETER_ERR_STRING);
    } else {
        sys_uart.Puts("ok");
    }
    sys_uart.Putc('\n');
  }
  void AckControl(void * snap_return) {
    // SnapReturnRGB_t *ret = (SnapReturnRGB_t *)snap_return;
    // SendToContorl(API_RGB, ret, sizeof(SnapReturnRGB_t));
  }
  private:
    RGBLight rgb_;
    SNAP_PIN_REMEP io_ = IOxx;
    uint8_t rgb_count = 0;
};

#endif
