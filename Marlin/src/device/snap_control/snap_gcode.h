#ifndef SNAP_GCODE_H
#define SNAP_GCODE_H

#include "snap_control_base.h"
#include "sys_uart.h"

#define GCODE_MAX_SIZE 80


#pragma pack(1)

typedef  struct{
  uint8_t data[GCODE_MAX_SIZE];
} SnapParamGcode_t;

typedef struct {
  uint8_t err_code;
} SnapReturnGcode_t;

#pragma pack()

class SnapGcode : public SnapControlBaes {
 public:
  SnapGcode() {
    busy_ = false;
  }
  ErrCode Send(uint8_t *gcode, bool Forced_send=false);
  ErrCode SendSync(uint8_t *gcode, uint32_t timeout_ms=0);
  bool isBusy();

  ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
    SnapReturnGcode_t *ret = (SnapReturnGcode_t *)snap_return;
    if (!snap_return)
      return E_FAILURE;
    if (param.param_type[0] == PARAM_ADDRESS) {
      ToCapitals(param.value->p);
      ret->err_code = SendSync(param.value->p, 10000);
      sys_uart.Putsln(param.value->p);
      return E_SUCCESS;
    }
    ret->err_code = E_FAILURE;
    return E_FAILURE;
  }

  ErrCode ApiControl(void * api_param, void * snap_return) {
    busy_ = false;
    return E_SUCCESS;
  }

  void ShowResult(void * snap_return) {
    SnapReturnGcode_t ret = *((SnapReturnGcode_t *)snap_return);
    sys_uart.Puts(name_to_api_table[API_GCODE]);
    if(ret.err_code != E_SUCCESS) {
      sys_uart.Puts(" fail");
    } else {
      sys_uart.Putc(':');
      sys_uart.Puts("ok");
    }
    sys_uart.Putc('\n');
  }

  void AckControl(void * snap_return) {
    
  }
  bool busy_;
};

#endif
