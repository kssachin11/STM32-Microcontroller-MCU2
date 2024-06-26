/*
 * it.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */

#include "main_mcu.h"
void SysTick_Handler(void)
{
	HAL_IncTick();// global variable for Systick increment

	HAL_SYSTICK_IRQHandler(); // for processing Systick Interrupt in the handler

}
