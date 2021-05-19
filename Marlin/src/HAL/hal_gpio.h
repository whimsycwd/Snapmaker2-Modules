/*
 * Snapmaker2-Modules Firmware
 * Copyright (C) 2019-2020 Snapmaker [https://github.com/Snapmaker]
 *
 * This file is part of Snapmaker2-Modules
 * (see https://github.com/Snapmaker/Snapmaker2-Modules)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GPIO_H
#define GPIO_H

#include "../common/common.h"

typedef enum {
  GPIO_AIN,
  GPIO_IN_FLOATING,
  GPIO_IPD,
  GPIO_IPU,
  GPIO_OUT_OD,
  GPIO_OUT_PP,
  GPIO_AF_OD,
  GPIO_AF_PP,
}GPIO_MODE_E;

void GpioRemap(void);
void GpioInit(uint8_t Port, GPIO_MODE_E Mode);
void GpioWrite(uint8_t Port, uint8_t IOLevel);
uint8_t GpioRead(uint8_t Port);
uint32_t GpioGetPort(uint8_t pin);


enum {
  PA_0, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13,PA_14,PA_15,
	PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7,
  Pxx,
};

typedef struct {
  uint8_t pin;
  uint8_t mode;
}GPIO_Read_t;

typedef struct {
  uint8_t pin;
  uint8_t mode;
  uint8_t level;
  uint8_t wait_ms;
  uint8_t is_inversion;
}GPIO_Write_t;

class HalGPIO {
 public:
  HalGPIO(uint8_t pin, GPIO_MODE_E mode=GPIO_OUT_PP) {Init(pin, mode);};
  HalGPIO() {pin_ = Pxx;}
  bool Init(uint8_t pin, GPIO_MODE_E mode);
  void SetMode(GPIO_MODE_E mode) {
    Init(pin_, mode);
  }
  static bool StaticInit(uint8_t pin, GPIO_MODE_E mode);
  bool Read();
  void Write(bool level);
  void High() {Write(1);}
  void Low() {Write(0);}
  void enable() {}
  void disable() {}
  static void DisableDebugIO();

  static uint32_t PinToPort(uint8_t pin);  // PA8 -> GPIOA
  static uint8_t PinToNum(uint8_t pin);  // PA8 -> 8

 private:
  uint8_t init_;
  uint8_t mode_;
  uint8_t pin_;
};
#endif
