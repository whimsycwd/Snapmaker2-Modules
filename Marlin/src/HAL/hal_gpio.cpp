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
#include "hal_gpio.h"
#include <src/HAL/std_library/inc/stm32f10x.h>

const static GPIO_TypeDef* PORTs[] = {
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD,
  GPIOE,
  GPIOF,
  GPIOG,
};

const static uint32_t RCCTable[] = {
  RCC_APB2Periph_GPIOA,
  RCC_APB2Periph_GPIOB,
  RCC_APB2Periph_GPIOC,
  RCC_APB2Periph_GPIOD,
  RCC_APB2Periph_GPIOE,
  RCC_APB2Periph_GPIOF,
  RCC_APB2Periph_GPIOG
};

static uint8_t gpio_module_table[] = {
  GPIO_Mode_AIN,
  GPIO_Mode_IN_FLOATING ,
  GPIO_Mode_IPD ,
  GPIO_Mode_IPU ,
  GPIO_Mode_Out_OD ,
  GPIO_Mode_Out_PP ,
  GPIO_Mode_AF_OD ,
  GPIO_Mode_AF_PP
};

void GpioRemap() {
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void GpioInit(uint8_t Port, uint8_t Mode) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCCTable[Port / 16], ENABLE);
  GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)gpio_module_table[Mode];
  GPIO_InitStruct.GPIO_Pin = 1 << (Port % 16);
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init((GPIO_TypeDef*)PORTs[Port / 16], &GPIO_InitStruct);
}

void GpioWrite(uint8_t Port, uint8_t IOLevel) {
  GPIO_TypeDef *pPort;
  uint16_t Bits;
  pPort = (GPIO_TypeDef*)PORTs[Port /16];
  Bits = (1 << (Port % 16));
  if(IOLevel) pPort->BSRR = Bits;
  else pPort->BRR = Bits;
}

uint8_t GpioRead(uint8_t Port) {
  GPIO_TypeDef *pPort;
  uint16_t Bits;
  pPort = (GPIO_TypeDef*)PORTs[Port / 16];
  Bits = Port % 16;
  return (pPort->IDR & (1 << Bits)) > 0;
}

uint32_t GpioGetPort(uint8_t pin) {
  return (uint32_t)PORTs[pin / 16];
}



bool HalGPIO::Init(uint8_t pin, uint8_t mode) {
  if (pin >= Pxx) {
    init_ = false;
    pin_ = Pxx;
  } else {
    GpioInit(pin, mode);
    pin_ = pin;
    init_ = true;
  }
  return init_;
}

bool HalGPIO::StaticInit(uint8_t pin, uint8_t mode) {
  if (pin < Pxx) {
    GpioInit(pin, mode);
    return  true;
  }
  return false;
}

bool HalGPIO::Read() {
  if (!init_) {
    return 0;
  }
  return GpioRead(pin_);
}

void HalGPIO::Write(bool level) {
  if (init_) {
    GpioWrite(pin_, level);
  }
}

uint32_t HalGPIO::PinToPort(uint8_t pin) {
  return GpioGetPort(pin);
}

uint8_t HalGPIO::PinToNum(uint8_t pin) {
  return pin % 16;
}

void HalGPIO::DisableDebugIO() {
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
