#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include "../../HAL/hal_include.h"
#include "snap_control_base.h"
#include "sys_uart.h"

#define DEFAULT_PIN_R_MODE GPIO_IPU
#define DEFAULT_PIN_W_MODE GPIO_OUT_PP

#define SNAP_IO_STATUS_MASK 0xf

#pragma pack(1)

typedef enum {
 IO0,  IO1,  IO2,  IO3,  IO4,  IO5,  IO6, IO7, IO8, IO9,
 IO10, IO11, IO12, IO13, IO14, IO15, IO16, IOxx,
}SNAP_PIN_REMEP;

typedef enum : uint8_t {
  IO_CONTROL_READ,
  IO_CONTROL_WRITE,
  IO_CONTROL_READ_SWITCH,
  IO_CONTROL_READ_RESET,
} IO_CONTROL_TYPE_E;

typedef  struct{
  IO_CONTROL_TYPE_E type;
  uint8_t io;
  union {
    struct {
      uint8_t level;
      uint8_t mode;
    }write;
    struct {
      uint8_t level;
      uint16_t time_ms;
      uint8_t mode;
    } reset;
  };
} SnapParamIO_t;

typedef struct {
  uint8_t err_code;
  IO_CONTROL_TYPE_E type;
  uint8_t io;
  bool value;
} SnapReturnIO_t;

typedef struct {
  uint8_t last_status : 4;
  uint8_t status : 4;
  uint8_t mode: 3;
  uint8_t is_port : 1;
} SnapIO_Status_t;

#pragma pack()

extern SnapIO_Status_t snap_io_satatus[IOxx];
extern const uint8_t snap_io_remap[];

class SnapIO : public SnapControlBaes {
 public:
  SnapIO() {
    HalGPIO::DisableDebugIO();
  }
  static void Init(SNAP_PIN_REMEP io, uint8_t mode=DEFAULT_PIN_W_MODE);
  ErrCode Write(SNAP_PIN_REMEP io, bool level, uint8_t mode=DEFAULT_PIN_W_MODE);
  ErrCode Read(SNAP_PIN_REMEP io, bool &out, uint8_t mode=DEFAULT_PIN_R_MODE);
  ErrCode ReadSwitch(SNAP_PIN_REMEP io, bool &out, uint8_t mode=DEFAULT_PIN_R_MODE);
  ErrCode Pwm(SNAP_PIN_REMEP io, uint32_t period_ms, uint8_t duty_cycle, uint8_t mode=DEFAULT_PIN_W_MODE);
  ErrCode Reset(SNAP_PIN_REMEP io, uint32_t ms, uint8_t mode=DEFAULT_PIN_W_MODE);
  void Loop();
  ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
    SnapReturnIO_t *ret = (SnapReturnIO_t *)snap_return;
    if (!snap_return) {
      return E_FAILURE;
    }
    uint8_t write_index = param.write_index;
    uint8_t read_mode = DEFAULT_PIN_R_MODE;
    uint8_t write_mode = DEFAULT_PIN_W_MODE;
    bool is_report = false;
    for (uint8_t i = 0; i < param.count && (i < write_index || !write_index); i++) {
      if (param.param_type[i] == PARAM_STRING) {
        switch (param.value[i].s[0]) {
          case 'u': read_mode = GPIO_IPU; break;
          case 'd': read_mode = GPIO_IPD; break;
          case 'f': read_mode = GPIO_IN_FLOATING; break;
          case 'p': write_mode = GPIO_OUT_PP; break;
          case 'o': write_mode = GPIO_OUT_OD; break;
          case 'k': is_report = true; break;
        }
      }
    }

    ret->err_code = E_FAILURE;
    ret->io = param.sub_num;
    if (ret->io >= IOxx) {
      ret->type = param.write_index ? IO_CONTROL_WRITE : IO_CONTROL_READ;
      return E_FAILURE;
    }
    if (param.write_index) {
      ret->type = IO_CONTROL_WRITE;
      if (param.param_type[write_index] == PARAM_INT) {
        ret->err_code = Write((SNAP_PIN_REMEP)param.sub_num, param.value[write_index].i, write_mode);
      }
      snap_io_satatus[ret->io].mode = write_mode;
      is_report = false;
    } else{
      ret->type = IO_CONTROL_READ;
      ret->err_code = Read((SNAP_PIN_REMEP)param.sub_num, ret->value, read_mode);
      snap_io_satatus[ret->io].last_status = ret->value ? SNAP_IO_STATUS_MASK : 0;
      snap_io_satatus[ret->io].mode = read_mode;
    }
    snap_io_satatus[ret->io].is_port = is_report;
    return E_SUCCESS;
  }

  ErrCode ApiControl(void * api_param, void * snap_return) {
    SnapReturnIO_t *ret = (SnapReturnIO_t *)snap_return;
    SnapParamIO_t * parameter = (SnapParamIO_t * )api_param;
    if (!api_param || !snap_return) {
      return E_FAILURE;
    }
    ret->err_code = E_FAILURE;
    ret->type = parameter->type;
    ret->io = parameter->io;
    switch (parameter->type) {
      case IO_CONTROL_READ :
        ret->err_code = Read((SNAP_PIN_REMEP)parameter->io, ret->value);
        break;
      case IO_CONTROL_WRITE:
        ret->err_code =  Write((SNAP_PIN_REMEP)parameter->io,parameter->write.level);
        break;
      default:
        ret->err_code = E_PARAM;
    }
    return E_SUCCESS;
  }

  void ShowResult(void * snap_return) {
    SnapReturnIO_t ret = *((SnapReturnIO_t *)snap_return);
    if(ret.err_code != E_SUCCESS) {
      sys_uart.Puts("io");
      sys_uart.Puts(PARAMETER_ERR_STRING);
    } else {
      switch (ret.type) {
      case IO_CONTROL_READ:
        sys_uart.Puts("io.");
        sys_uart.PutInt(ret.io);
        sys_uart.Putc(':');
        sys_uart.PutInt(ret.value);
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
    SnapReturnIO_t *ret = (SnapReturnIO_t *)snap_return;
    SendToContorl(API_IO, ret, sizeof(SnapReturnIO_t));
  }
};

#endif
