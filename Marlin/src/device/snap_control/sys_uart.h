#ifndef SYS_UART_H
#define SYS_UART_H

#include "../../common/common.h"
#include "../../hal/hal_include.h"
#include <string.h>

#define CMD_MAX_SIZE (256)

class SysUart {
 public:
  SysUart() {
  }

  void Init() {
    port_.Init(USART_1, 115200, 512);
    is_end_ = false;
    index = 0;
  }

  void Loop() {
    uint8_t data;
    if (!is_end_) {
      if (port_.GetC(data) == E_SUCCESS) {
        if ((data == '\n') || (data == ';')) {
          cmd_[index++] = 0;
          is_end_ = true;
        } else {
          cmd_[index++] = data;
        }
      }
      if (index >= CMD_MAX_SIZE) {
        is_end_ = false;
        index = 0;
      }
    }
  }
  uint8_t * CmdPop() {
    if (is_end_) {
      return cmd_;
    }
    return NULL;
  }
  uint8_t * CmdPop(uint16_t &cmd_len) {
    cmd_len = index;
    return CmdPop();
  }
  void CleanCmd() {
    is_end_ = false;
    index = 0;
  }

  int32_t Puts(uint8_t *data, uint16_t len) {
    return port_.Puts(data, len);
  }

  int32_t Puts(uint8_t *data) {
    return port_.Puts(data, strlen((char *)data));
  }

  int32_t Puts(const uint8_t *data) {
    return port_.Puts((uint8_t *)data, strlen((char *)data));
  }

  int32_t Puts(const char *data) {
    return port_.Puts((uint8_t *)data, strlen((char *)data));
  }

  ErrCode Putc(uint8_t ch) {
    return port_.PutC(ch);
  }

  void PutInt(int32_t val) {
    uint8_t buf[10];
    uint8_t index=0; 
    if (val == 0) {
      port_.PutC('0');
    } else{
      if (val < 0) {
        port_.PutC('-');
        val = -val;
      }
      while (val > 0 && (index < 10)) {
        buf[index++] = (val % 10) + '0';
        val /= 10;
      }
    }
    while (index) {
      port_.PutC(buf[--index]);
    }
  }
  void PutFloat(float val) {
    PutInt((int32_t)val);
    Putc('.');
    if (val < 0) val = -val;
    PutInt((int32_t)(val * 1000) % 1000);
  }
 private:
  uint8_t cmd_[CMD_MAX_SIZE];
  uint16_t index = 0;
  bool is_end_ = false;
  HalUart port_;
};

#endif
