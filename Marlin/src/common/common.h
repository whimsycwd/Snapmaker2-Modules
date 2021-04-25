#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <stdint.h>
#include "../utils/RingBuffer.h"
#include "../core/thermistor_table.h"
#include "../registry/registry.h"
#include "../core/can_bus.h"

#define STR_TYPE(s) ((uint8_t *)s)

typedef enum {
  E_SUCCESS,
  E_FAILURE,
  E_PARAM,
  E_INIT,
  E_BUSY,
  E_TIMEOUT,
  E_HARDWARE,
} ErrCode;
#include "common.h"

#define STR_MAX_LEN (0xffff)
#define IS_NUMBER(ch) (((ch) <= '9') && ((ch) >= '0'))
#define IS_LETTER(ch) ((((ch) <= 'z') && ((ch) >= 'a')) || (((ch) <= 'Z') && ((ch) >= 'A')))
uint8_t ToLower(uint8_t c);
uint16_t ToLowers(uint8_t *s, uint16_t max_len=STR_MAX_LEN);
int16_t FindCharIndex(uint8_t * str, uint8_t ch);
bool IsBeginWith(uint8_t * str,uint8_t *begin);
uint8_t StringToInt(uint8_t *str, int32_t &out);
bool StringToBool(uint8_t *str, bool &out);
bool IfStringFLoat(uint8_t *str, uint8_t &decimal_index);
bool IfStringFLoat(uint8_t *str);
bool StringToFloat(uint8_t *str, float &out);
void StringCopy(uint8_t *dst, uint8_t *src, uint8_t len);

int32_t UnpackInt(uint8_t *pack, uint8_t len);
bool PackInt(int val, uint8_t *out_pack, uint8_t pack_len);
float UnpackFloat(uint8_t *pack, uint8_t len);
bool PackFloat(float val, uint8_t *out_pack, uint8_t pack_len);
double UnpackDouble(uint8_t *pack, uint8_t len);
bool PackDouble(double val, uint8_t *out_pack, uint8_t pack_len);
#endif
