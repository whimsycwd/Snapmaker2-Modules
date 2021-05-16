#ifndef SNAP_PARAM_H
#define SNAP_PARAM_H

#include <stdint.h>
#include "../../common/common.h"

#define MAX_PARAM_COUNT 8
#define SUB_NUM_INVALID (0xff)

#define PARAMETER_ERR_STRING " parameter err"
#define PUBLIC_BUF_SIZE 200

#pragma pack(1)
enum : uint8_t {
  PARAM_INT,
  PARAM_STRING,
  PARAM_FLOAT,
  PARAM_DOUBLE,
  PARAM_ADDRESS,
  PARAM_WRITE_MARK,
};

typedef enum : uint8_t {
  SNAP_DATA_STRING_MODE,
  SNAP_DATA_API_MODE,
} SNAP_CMD_TYPE_E;

typedef enum : uint8_t {
  SNAP_CMD_SOUREC_UART,
  SNAP_CMD_SOUREC_CAN,
} SNAP_CMD_SOUREC_E;

typedef enum : uint8_t {
  SNAP_CMD_ACK,
  SNAP_CMD_REQUSE,
} SNAP_CMD_FUN_E;

enum : uint8_t {
  SNAP_DATA_PACK_START,
  SNAP_DATA_PACK_DATA,
  SNAP_DATA_PACK_END,
  SNAP_DATA_PACK_SIGLE,
};

typedef struct  {
  uint8_t mode:2;  // 0-API  1-string
  uint8_t type:2;  // 0-start 1-data 2-end
  uint8_t yunxing:1;
} SnapDataHead_t;

typedef enum : uint8_t {
  API_INDEX,
  API_IO,
  API_ADC,
  API_UART,
  API_PWM,
  API_TEMP,
  API_GCODE,
  API_MPU,
  API_INVALID
}API_NUM_E;

typedef struct {
  API_NUM_E api_type;
  uint8_t sub_num;
  int8_t write_index;  // 0 是读 ； !0 写参数的索引
  uint8_t count;
  uint8_t param_type[MAX_PARAM_COUNT];
  union {
   int32_t i;
   float   f;
   double  d;
   uint8_t s[16];
   uint8_t *p;
  }value[MAX_PARAM_COUNT];
}SnapCommonParam_t;

#pragma pack()

extern const uint8_t name_to_api_table[][8];
ErrCode SendToContorl(API_NUM_E api, void * info, uint8_t len);

class SnapControlBaes {
  public:
    virtual ErrCode ApiControl(void *api_param, void * snap_return) {return E_FAILURE;}
    virtual ErrCode StrControl(SnapCommonParam_t param, void * snap_return) {return E_FAILURE;}
    virtual void AckControl(void * snap_return) {}
    virtual void ShowResult(void * snap_return) {}
};

#endif
