#ifndef _EXTI_H_
#define _EXTI_H_
#include <stdint.h>
typedef void(*EXTI_CB_F)(uint8_t pin_source);

typedef enum {
    EXTI_Rising = 0x08,
    EXTI_Falling = 0x0C,  
    EXTI_Rising_Falling = 0x10
}EXTI_MODE_E;

typedef enum {
    SOFT_EXTI_LINE0 = 0,
    SOFT_EXTI_LINE1 = 1,
    SOFT_EXTI_LINE2 = 2,
    SOFT_EXTI_LINE3 = 3,
    SOFT_EXTI_LINE4 = 4,
    SOFT_EXTI_LINE5 = 5,
    SOFT_EXTI_LINE6 = 6,
    SOFT_EXTI_LINE7 = 7,
    SOFT_EXTI_LINE8 = 8,
    SOFT_EXTI_LINE9 = 9,
    SOFT_EXTI_LINE10 = 10,
    SOFT_EXTI_LINE11 = 11,
    SOFT_EXTI_LINE12 = 12,
    SOFT_EXTI_LINE13 = 13,
    SOFT_EXTI_LINE14 = 14,
    SOFT_EXTI_LINE15 = 15,
}SOFT_EXTI_LINE_E;


uint8_t ExtiInit(uint8_t pin, EXTI_MODE_E exti_mode, EXTI_CB_F cb);
uint8_t SoftExtiInit(SOFT_EXTI_LINE_E exti_line, EXTI_CB_F cb, uint8_t pro=1, uint8_t sub_pro=1);
void SoftExtiTrigger(SOFT_EXTI_LINE_E exti_line);
#endif
