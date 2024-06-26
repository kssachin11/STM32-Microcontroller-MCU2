/*
 * main_mcu.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */


#include "stm32f4xx_hal.h"
#include "main_mcu.h"
#include "string.h"


void 	TIMER6_Init(void);
void SystemClockConfig(void);

void Error_Handler(void);

TIM_HandleTypeDef	htimer6 ;   // handle variable of timer 6

int main(void)
{

	HAL_Init();
	SystemClockConfig();
	TIMER6_Init();
	while(1);

	return 0;

}

void SystemClockConfig(void)
{


}





void 	TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler  = 24;
	htimer6.Init.Period = 64000-1 ;
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}

}
void Error_Handler(void)
{
	while(1);

}
