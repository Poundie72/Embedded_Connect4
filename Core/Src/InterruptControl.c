
#include "InterruptControl.h"

void enableIRQ(uint8_t IRQ){
	HAL_NVIC_EnableIRQ(IRQ);
}



void disableIRQ(uint8_t IRQ){
	HAL_NVIC_DisableIRQ(IRQ);

}
//void clearPendingIRQ(uint8_t IRQ){
//	if(IRQ < 32){
//		*(NVIC_ICPR0) |= (0x1 << IRQ);
//	}
//}

//void setPendingIRQ(uint8_t IRQ){
//
//}

void clearPendingBit(uint8_t pinNumber){
	__HAL_GPIO_EXTI_CLEAR_FLAG(pinNumber);
}
