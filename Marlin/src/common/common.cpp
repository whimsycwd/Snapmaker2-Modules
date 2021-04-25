#include "common.h"

uint8_t ToLower(uint8_t c) {
  #define TOLOWER_DIFF  ('a' - 'A')
  if (c <= 'Z' && c >=  'A') {
    c += TOLOWER_DIFF;
  }
  return c;
}

uint16_t ToLowers(uint8_t *s, uint16_t max_len) {
  uint16_t i = 0;
  for ( ; s[i] != '\0' && i < max_len; i++) {
    s[i] = ToLower(s[i]);
  }
  return i;
}

int16_t FindCharIndex(uint8_t * str, uint8_t ch) {
  int16_t ret  = -1;
  if (str != NULL) {
    for (uint16_t index = 0; str[index] != '\0'; index++) {
      if (str[index] == ch) {
        ret = index;
        break;
      }
    }
  }
  return ret;
}

bool IsBeginWith(uint8_t * str,uint8_t *begin) {
  if(str == NULL || begin == NULL)
    return false;
  int len1 = strlen((char *)str);
  int len2 = strlen((char *)begin);
  if((len1 < len2) || (len1 == 0 || len2 == 0))
    return false;
  uint8_t *p = begin;
  int i = 0;
  while(*p != '\0') {
    if(*p != str[i])
      return false;
    p++;
    i++;
  }
  return true;
}

bool IfStringFLoat(uint8_t *str, uint8_t &decimal_index) {
  uint8_t index = 0;
  if (!IS_NUMBER(str[index])) {
    return false;
  }
  while(IS_NUMBER(str[index])) index++;
  if (str[index] == '.' && IS_NUMBER(str[index+1])) {
    decimal_index = index+1;
    return true;
  }
  return false;
}

bool IfStringFLoat(uint8_t *str) {
  uint8_t index;
  return IfStringFLoat(str, index);
}

bool StringToFloat(uint8_t *str, float &out) {
  int32_t num = 0;
  float decimal = 0;
  out = 0;
  if (StringToInt(str, num)) {
    out = num;
    uint8_t decimal_index;
    if (IfStringFLoat(str, decimal_index)) {
      if (StringToInt(&str[decimal_index], num)) {
        while (num) {
          decimal += (num % 10);
          decimal /= 10;
          num /= 10;
        }
      }
    }
  } else {
    return false;
  }
  out += decimal;
  return true;
}

uint8_t StringToInt(uint8_t *str, int32_t &out) {
  int32_t num = 0;
  int8_t sign = -1;
  uint8_t bit = 0;
  out = 0;
  if (str == NULL) {
    return false;
  }

  while ((*str) == ' ') {
    str++;
  }

  if ((*str) == '-') {
    sign = 1;
    str++;
  }

  while (IS_NUMBER(*str)) {
    num = num * 10 + ((*str) - '0');
    str++;
    bit++;
  }
  out = num * (sign * (-1));
  return bit;
}

bool StringToBool(uint8_t *str, bool &out) {

  if (str == NULL) {
    return false;
  }

  while (!IS_NUMBER(*str) && (*str != '\0')) {
    str++;
  }

  if (IS_NUMBER(*str)) {
    out = ((*str) - '0') > 0;
    return true;
  }
  return false;
}

void StringCopy(uint8_t *dst, uint8_t *src, uint8_t len) {
  if (dst && src) {
    for (uint8_t i = 0; i < len; i++) {
      dst[i] = src[i];
    }
  }
}

bool UnpackDataType(uint8_t *pack, uint8_t size, void * out) {
  uint8_t *data = (uint8_t *)out;
  if (!pack || !out) {
    return false;
  }
  for (uint8_t i = 0; i < size; i++) {
    data[size - 1 -i] = pack[i];
  }
  return true;
}

bool PackDataType(void *data, uint8_t size, uint8_t * out) {
  uint8_t *p_data = (uint8_t *)data;
  if (!data || !out) {
    return false;
  }
  for (uint8_t i = 0; i < size; i++) {
    out[size - 1 -i] = p_data[i];
  }
  return true;
}

int32_t UnpackInt(uint8_t *pack, uint8_t len) {
  int ret = 0;
  if (len <= 4 && pack) {
    UnpackDataType(pack, len, &ret);
  }
  return ret;
}

bool PackInt(int val, uint8_t *out_pack, uint8_t pack_len) {
  if (pack_len > 4 || !out_pack) {
    return false;
  }
  PackDataType(&val, pack_len, out_pack);
  return true;
}

float UnpackFloat(uint8_t *pack, uint8_t len) {
  float ret = 0;
  if (len <= 4 && pack) {
    UnpackDataType(pack, len, &ret);
  }
  return ret;
}

bool PackFloat(float val, uint8_t *out_pack, uint8_t pack_len) {
  if (pack_len > 4 || !out_pack) {
    return false;
  }
  PackDataType(&val, pack_len, out_pack);
  return true;
}

double UnpackDouble(uint8_t *pack, uint8_t len) {
  double ret = 0;
  if (len <= 8 && pack) {
    UnpackDataType(pack, len, &ret);
  }
  return ret;
}

bool PackDouble(double val, uint8_t *out_pack, uint8_t pack_len) {
  if (pack_len > 8 || !out_pack) {
    return false;
  }
  PackDataType(&val, pack_len, out_pack);
  return true;
}