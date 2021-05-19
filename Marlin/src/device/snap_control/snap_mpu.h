#ifndef SNAP_MPU_H
#define SNAP_MPU_H

#include "MPU6050/mpu6050.h"
#include "snap_control_base.h"
#include "sys_uart.h"
#pragma pack(1)

typedef enum {
  MPU_CONTROL_INIT,
  MPU_CONTROL_ZERO,
  MPU_CONTROL_READ_EULER,
  MPU_CONTROL_READ_GYRO,
  MPU_CONTROL_READ_ACCE,
} MPU_CONTROL_TYPE_E;

typedef struct {
  float  y, x,z;
} ValueOfMPU_t;

typedef struct {
  uint8_t sda_pin;
  uint8_t scl_pin;
} SnapParamMPU_t;

typedef struct {
  uint8_t err_code;
  MPU_CONTROL_TYPE_E type;
  ValueOfMPU_t value;
} SnapReturnMPU_t;

#pragma pack()
#define MPU_DEFAULT_ADDR				0X68
class SnapMPU : public SnapControlBaes {
  public:
    SnapMPU() {};
    ErrCode Init(uint8_t sda_io, uint8_t scl_io, uint8_t addr=MPU_DEFAULT_ADDR);
    ErrCode Read(ValueOfMPU_t &value);
    ErrCode ReadGyroscope(ValueOfMPU_t &value);
    ErrCode RreadAcceler(ValueOfMPU_t &value);
    ErrCode Zero();
    ErrCode ApiControl(void *api_param, void * snap_return) {
      return E_FAILURE;
    }

    ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {
      SnapReturnMPU_t *ret = (SnapReturnMPU_t *)snap_return;
      if (!snap_return)
        return E_FAILURE;

      ret->err_code = E_FAILURE;

      if (param.write_index) {
        uint8_t i = param.write_index;
        uint8_t param_count = param.count - i;
        if (param_count == 1) {
          ret->type = MPU_CONTROL_ZERO;
          ret->err_code = Zero();
        } else if (param_count == 2) {
          ret->type = MPU_CONTROL_INIT;
          ret->err_code = Init(param.value[i].i, param.value[i+1].i, MPU_DEFAULT_ADDR);
        } else {
          ret->type = MPU_CONTROL_INIT;
          ret->err_code = E_FAILURE;
        }
      } else{
        ret->type = MPU_CONTROL_READ_EULER;
        if ((param.count) == 1 && (param.param_type[0] == PARAM_STRING)) {
          switch (param.value[0].s[0]) {
          case 'e':
            ret->type = MPU_CONTROL_READ_EULER;
            break;
          case 'a':
            ret->type = MPU_CONTROL_READ_ACCE;
            break;
          case 'g':
            ret->type = MPU_CONTROL_READ_GYRO;
            break;         
          default:
            break;
          }
        }
        switch (ret->type) {
          case MPU_CONTROL_READ_EULER:
            ret->err_code = Read(ret->value);
            break;
          case MPU_CONTROL_READ_ACCE:
            ret->err_code = RreadAcceler(ret->value);
            break;
          case MPU_CONTROL_READ_GYRO:
            ret->err_code = ReadGyroscope(ret->value);
            break;
          default:
            ret->err_code = E_FAILURE;
            break;
        }
      }
      return E_SUCCESS;
    }

    void AckControl(void * snap_return) {

    }
    void ShowResult(void * snap_return) {
      SnapReturnMPU_t *ret = (SnapReturnMPU_t *)snap_return;
      sys_uart.Puts("mpu");
      switch (ret->type) {
        case MPU_CONTROL_ZERO:
          sys_uart.Puts(" zero ");
          break;
        case MPU_CONTROL_INIT:
          sys_uart.Puts(" init ");
          break;
        case MPU_CONTROL_READ_ACCE:
          sys_uart.Puts(" read acc ");
          break;
        case MPU_CONTROL_READ_GYRO:
          sys_uart.Puts(" read gyro ");
          break;
        case MPU_CONTROL_READ_EULER:
          sys_uart.Puts(" read eluer ");
          break;
      }
      if (ret->err_code != E_SUCCESS) {
        sys_uart.Puts("fail");
      } else {
        switch (ret->type) {
          case MPU_CONTROL_ZERO:
          case MPU_CONTROL_INIT:
            sys_uart.Puts(":ok");
            break;
          case MPU_CONTROL_READ_EULER:
          case MPU_CONTROL_READ_ACCE:
          case MPU_CONTROL_READ_GYRO:
            sys_uart.Puts(" x:");
            sys_uart.PutFloat(ret->value.x);
            sys_uart.Puts(" y:");
            sys_uart.PutFloat(ret->value.y);
            sys_uart.Puts(" z:");
            sys_uart.PutFloat(ret->value.z);
            break;
        }
      }
      sys_uart.Putc('\n');
    }
  private:
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    bool inited_;
    Quaternion init_q_;
    Quaternion last_q_;
};

#endif
