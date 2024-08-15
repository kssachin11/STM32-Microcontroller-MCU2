/*
 * main_mcu.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */

#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "main_mcu.h"





void GPIO_Init(void);
void Error_Handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq );

TIM_HandleTypeDef	htimer2 ;   // handle variable of timer 6
UART_HandleTypeDef huart2;


uint32_t input_captures [2]= {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;
uint32_t  capture_difference = 0;
double timer2_cnt_freq =0;
double timer2_cnt_res =0;
double user_signal_time_period = 0;
double user_signal_freq =0;
char user_msg [100];


int main(void)
{
	uint16_t brightness = 0;

	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();


	if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	while(1)
	{

	  while(brightness < htimer2.Init.Period)
	  {
		  brightness+=10;
		  __HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1,brightness );
		  HAL_Delay(1);

	  }
	  while(brightness > 0)
	  {
		  brightness+=10;
		  __HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1,brightness );
		  HAL_Delay(1);

	  }

	}
	  return 0;

}



void SystemClock_Config_HSE(uint8_t clock_freq)
{
  RCC_OscInitTypeDef Osc_Init;
  RCC_ClkInitTypeDef Clock_Init;
  uint8_t flash_latency=0;

  Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI ;
  Osc_Init.HSEState = RCC_HSE_ON;
  Osc_Init.LSEState = RCC_LSE_ON;
  Osc_Init.HSIState = RCC_HSI_ON;
  Osc_Init.PLL.PLLState = RCC_PLL_ON;
  Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  switch(clock_freq) {
  case SYS_CLOCK_FREQ_50_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 50;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Osc_Init.PLL.PLLR = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
    flash_latency = 1;
    break;

  case SYS_CLOCK_FREQ_84_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 84;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Osc_Init.PLL.PLLR = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
    flash_latency = 2;
    break;

  case SYS_CLOCK_FREQ_120_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 120;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Osc_Init.PLL.PLLR = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
    flash_latency = 3;
    break;

  default:
    return ;
  }

  if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
  {
    Error_Handler();
  }

  /*Configure the systick timer interrupt frequency (for every 1 ms) */
  uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
  HAL_SYSTICK_Config(hclk_freq/1000);

  /**Configure the Systick
  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}




void GPIO_Init(void)
{

	__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO_InitTypeDef ledgpio;
			ledgpio.Pin = GPIO_PIN_5;
			ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
			ledgpio.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOA, &ledgpio);

}

void TIMER2_Init(void)
{

	  TIM_OC_InitTypeDef tim2PWM_Config;

	  htimer2.Instance = TIM2;
	  htimer2.Init.Period = 10000-1;
	  htimer2.Init.Prescaler = 4999;
	  if ( HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  memset(&tim2PWM_Config,0,sizeof(tim2PWM_Config));

	  tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	  tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;


	  tim2PWM_Config.Pulse =  0;

	  if( HAL_TIM_PWM_ConfigChannel(&htimer2,&tim2PWM_Config,TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }



}


 void UART2_Init(void)
 {

 	huart2.Instance =USART2;
 	huart2.Init.BaudRate = 115200;
 	huart2.Init.WordLength = UART_WORDLENGTH_8B ;
 	huart2.Init.StopBits = UART_STOPBITS_1;
 	huart2.Init.Parity = UART_PARITY_NONE;
 	huart2.Init.HwFlowCtl =UART_HWCONTROL_NONE;
 	huart2.Init.Mode =UART_MODE_TX_RX ;
 	if (HAL_UART_Init(&huart2) != HAL_OK)
 	{
 		//There is a problem
 		Error_Handler();
 	}


}




void Error_Handler(void)
{
	while(1);

}

