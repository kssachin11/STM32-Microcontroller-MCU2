/*
 * msp.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */

#include "stm32f4xx_hal.h"
#include "main_mcu.h"
void HAL_MspInit(void)
{
  // Here we will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	 SCB->SHCSR |= 0x7 << 16; // usg fault,memoryfault and bus fault system exceptions

	//3. configure the priority for the system exceptions
	 HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	 HAL_NVIC_SetPriority( BusFault_IRQn,0,0);
	 HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);



}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef*htimer)
{
	//1. Enable the clock for timer 6 peripherals'

	__HAL_RCC_TIM6_IS_CLK_ENABLED();

	//2. Enable IRQ o timer 6

	HAL_NVIC_EnableIRQ(  TIM6_DAC_IRQn );

	//3. Setup the priority

	HAL_NVIC_SetPriority(  TIM6_DAC_IRQn , 15, 0);




}
