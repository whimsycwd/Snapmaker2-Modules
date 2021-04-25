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


#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

#include <stdio.h>

#define POWER_MV (3.3 * 1000)  // 电压 mv
#define ADC_DEEP 6
#define ADC_MAX_DEV_COUNT 7
#define ADC_CACHE_SIZE (ADC_DEEP * ADC_MAX_DEV_COUNT)
typedef void(*ADC_CB_F) (void);  // 中断回调函数

#define ADC_ERROR 255
typedef enum {
    ADC_TIM_1,
    ADC_TIM_2,
    ADC_TIM_3,
    ADC_TIM_4,
    ADC_TIM_INVALID
} ADC_TIM_E;

typedef enum {
    ADC_CH_0 ,  // PA_0
    ADC_CH_1 ,  // PA_1
    ADC_CH_2 ,  // PA_2
    ADC_CH_3 ,  // PA_3
    ADC_CH_4 ,  // PA_4
    ADC_CH_5 ,  // PA_5
    ADC_CH_6 ,  // PA_6
    ADC_CH_7 ,  // PA_7
    ADC_CH_8 ,  // PB_0
    ADC_CH_9 ,  // PB_1
    ADC_CH_10,  // PC_0
    ADC_CH_11,  // PC_1
    ADC_CH_12,  // PC_2
    ADC_CH_13,  // PC_3
    ADC_CH_14,  // PC_4
    ADC_CH_15,  // PC_5
    ADC_CH_INVALID,
} ADC_CHN_E;

uint8_t HAL_adc_init(uint8_t pin, ADC_TIM_E tim, uint16_t period_us);
uint8_t HAL_adc_init_chn(ADC_CHN_E chn, ADC_TIM_E tim, uint16_t period_us);
uint16_t ADC_Get(uint8_t index);
uint16_t ADC_GetCusum(uint8_t index);
void ADC_CaptureEnable();
void ADC_CaptureDisable();
uint8_t hal_adc_status();

class HalADC {
 public:
  uint8_t Init(ADC_CHN_E ch, ADC_TIM_E tim, uint16_t period_us);
  // return adc index
  uint8_t Register(ADC_CHN_E ch);
  // index is Register() return
  uint16_t Read(uint8_t index);
  uint16_t ReadNext(uint8_t index);
  // Destroy a channel
  void destroy(uint8_t index);
  void enable();
  void disable();
 private:
    ADC_TIM_E tim_ = ADC_TIM_INVALID;
};

#endif

