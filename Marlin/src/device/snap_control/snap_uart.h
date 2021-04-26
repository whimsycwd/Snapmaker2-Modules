#ifndef SNAP_UART_H
#define SNAP_UART_H

#include "../../common/common.h"
#include "../../hal/hal_include.h"
#include <string.h>

#define UART_BUF_SIZE (256)
#define UART_DEFAULT_BAUDRATE 115200


class SnapUart {
 public:
  SnapUart() {
  }

  void Init() {
    Init(UART_DEFAULT_BAUDRATE);
  }

  void Init(uint32_t baudrate) {
    port_.Init(USART_2, baudrate, UART_BUF_SIZE);
  }

  void Loop() {
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

  ErrCode GetC(uint8_t ch) {
    return port_.GetC(ch);
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
  HalUart port_;
};


#endif
