/*
 * it.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */
#include "main.h"

extern UART_HandleTypeDef huart2;  // taking handler variable from main.c

void SysTick_Handler(void)
{
	HAL_IncTick();// global variable for Systick increment

	HAL_SYSTICK_IRQHandler(); // for processing Systick Interrupt in the handler\



}
 void USART2_IRQHandler (void)
 {
	 HAL_UART_IRQHandler(&huart2);
 }
