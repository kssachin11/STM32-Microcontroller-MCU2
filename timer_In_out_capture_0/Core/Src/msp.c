/*
 * msp.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
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

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{


	GPIO_InitTypeDef tim2ch1_gpio;

	//1. Enable the clock for timer2

	__HAL_RCC_TIM12_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Configure a gpio to behave as timer2 channel 1

	 tim2ch1_gpio.Pin = GPIO_PIN_0;
	 tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	 tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	 HAL_GPIO_Init(GPIOA,&tim2ch1_gpio);

	 //3. nvic settings
	 HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	 HAL_NVIC_EnableIRQ(TIM2_IRQn);


}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

	GPIO_InitTypeDef gpio_uart;
	// Do low level inits of the USART2 peripherals

	//1.Enable the clock for USART2 peripherals
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for port a peripheral

	//2.Do the pin muxing configs
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW ;
	gpio_uart.Alternate = GPIO_AF7_USART2;// UART2_TX
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;// UART2_Rx
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3.Enable the IRQ and set up the priority (NVIC settings)
	HAL_NVIC_EnableIRQ( USART2_IRQn );
	HAL_NVIC_SetPriority(USART2_IRQn , 15, 0);
}

