#ifndef SNAP_INDEX_H
#define SNAP_INDEX_H

#include "../../HAL/hal_include.h"
#include "snap_control_base.h"

#define INVALID_INDEX 0xff

#pragma pack(1)

typedef enum : uint8_t {
  SNAP_INDEX_READ,
  SNAP_INDEX_WRITE,
} SNAP_INDEX_OPERATION_E;

typedef struct {
  SNAP_INDEX_OPERATION_E type;
  uint8_t index;  // is used only write
} SnapParamIndex_t;

typedef struct {
  uint8_t err_code;
  SNAP_INDEX_OPERATION_E type;
  uint8_t index;
} SnapReturnIndex_t;

#pragma pack()

class SnapIndex : public SnapControlBaes {
  public:
    ErrCode Write(uint8_t index) {
      if (index < INVALID_INDEX) {
        registryInstance.cfg_.index = index;
        registryInstance.SaveCfg();
        return E_SUCCESS;
      }
      return E_FAILURE;
    }
    ErrCode Read(uint8_t &index) {
      index = registryInstance.cfg_.index;
      return E_SUCCESS;
    }

    ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
      SnapReturnIndex_t * ret = (SnapReturnIndex_t *)snap_return;
      if (!ret) {
        return E_FAILURE;
      }
      ret->err_code = E_FAILURE;
      if (param.write_index) {
        ret->type = SNAP_INDEX_WRITE;
        uint8_t i = param.write_index;
        if ((param.param_type[i] == PARAM_INT) && (param.value[i].i < INVALID_INDEX)) {
          ret->err_code = Write(param.value[i].i);
        }
      } else{
        ret->type = SNAP_INDEX_READ;
        ret->err_code = E_SUCCESS;
      }
      Read(ret->index);
      return E_SUCCESS;
    }

    ErrCode ApiControl(void * api_param, void * snap_return) {
      SnapParamIndex_t *param = (SnapParamIndex_t *)api_param;
      SnapReturnIndex_t *ret = (SnapReturnIndex_t *)snap_return;
      ret->err_code = E_FAILURE;
      ret->type = param->type;
      switch (param->type) {
        case SNAP_INDEX_READ :
          ret->err_code = E_SUCCESS;
          break;
        case SNAP_INDEX_WRITE:
          ret->err_code =  Write(param->index);
          break;
        default:
          ret->err_code = E_PARAM;
      }
      Read(ret->index);
      return E_SUCCESS;
    }

    void ShowResult(void * snap_return) {
      SnapReturnIndex_t * ret = (SnapReturnIndex_t *)snap_return;
      if(ret->err_code != E_SUCCESS) {
        sys_uart.Puts("index");
        sys_uart.Puts(PARAMETER_ERR_STRING);
      } else {
        switch (ret->type) {
          case SNAP_INDEX_READ:
            sys_uart.Puts("index ");
            sys_uart.PutInt(ret->index);
            break;
          case IO_CONTROL_WRITE:
            sys_uart.Puts("ok");
            break;
          default:
            break;
        }
      }
      sys_uart.Putc('\n');
    }

  void AckControl(void * snap_return) {
    SnapReturnIndex_t *ret = (SnapReturnIndex_t *)snap_return;
    SendToContorl(API_INDEX, ret, sizeof(SnapReturnIndex_t));
  }
  private:

};

#endif
