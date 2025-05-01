#include "Button_Driver.h"

void Button_Interrupt_Init(){
	GPIO_InitTypeDef buttonConfig;



	buttonConfig.Pin = GPIO_PIN_0;
	buttonConfig.Mode = GPIO_MODE_IT_RISING;
	buttonConfig.Pull = GPIO_PULLDOWN;
	buttonConfig.Speed = GPIO_SPEED_FREQ_MEDIUM;
	// buttonConfig.Alternate = ;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &buttonConfig);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
