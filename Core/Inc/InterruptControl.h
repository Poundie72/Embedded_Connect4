#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"



#define EXTI0_IRQ_NUMBER 6

void enableIRQ(uint8_t IRQ);
void disableIRQ(uint8_t IRQ);
void clearPendingIRQ(uint8_t IRQ);
void setPendingIRQ(uint8_t IRQ);
void clearPendingBit(uint8_t pinNumber);



#endif
