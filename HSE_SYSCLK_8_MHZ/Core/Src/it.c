/*
 * it.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */
#include "main.h"


void SysTick_Handler(void)
{
	HAL_IncTick();// global variable for Systick increment

	HAL_SYSTICK_IRQHandler(); // for processing Systick Interrupt in the handler



}
