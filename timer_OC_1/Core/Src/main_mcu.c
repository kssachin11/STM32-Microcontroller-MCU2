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




void SystemClockConfig(uint8_t clock_freq );
void GPIO_Init(void);
void Error_Handler(void);
void TIMER2_Init(void);
void LSE_Configuration(void);
void UART2_Init(void);

TIM_HandleTypeDef	htimer2 ;   // handle variable of timer 6
uint32_t input_captures [2]= {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;
uint32_t  capture_difference = 0;
double timer2_cnt_freq =0;
double timer2_cnt_res =0;
double user_signal_time_period = 0;
double user_signal_freq =0;
char user_msg [100];


volatile uint32_t pulse1_value = 25000; //to produce 500Hz
volatile uint32_t pulse2_value = 12500; //to produce 1000HZ
volatile uint32_t pulse3_value = 6250;  //to produce 2000Hz
volatile uint32_t pulse4_value = 3125;  //to produce 4000Hz


UART_HandleTypeDef huart2;

int main(void)
{

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();


	while(1)

	{
		if (is_capture_done)
		{
			if (input_captures[1]>input_captures[0])
			 {
					capture_difference = input_captures[1] - input_captures[0];
			 }else
			 {
				 capture_difference = (0XFFFFFFFF- input_captures[0]) + input_captures[1];
			 }
		}

		timer2_cnt_freq =(HAL_RCC_GetPCLK1Freq()*2) / htimer2.Init.Prescaler;
		timer2_cnt_res = 1/timer2_cnt_freq;
		user_signal_time_period = capture_difference*timer2_cnt_res;
		user_signal_freq = 1/user_signal_time_period;

		sprintf(user_msg, "Frequency of signal applied = %f\r\n",user_signal_freq);   // @suppress("Float formatting support")
		HAL_UART_Transmit(&huart2, user_msg, strlen(user_msg), HAL_MAX_DELAY);

		is_capture_done= FALSE;
	}

	return 0;

}



void SystemClockConfig(uint8_t clock_freq )
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_Handler();
	}



	if (HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2) != HAL_OK)
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

	TIM_OC_InitTypeDef tim2OC_init;

   	htimer2.Instance = TIM2;
   	htimer2.Init.Period = 0XFFFFFFFF;
   	htimer2.Init.Prescaler = 1;

	 if ( HAL_TIM_OC_Init(&htimer2) != HAL_OK)
			 {
				 Error_handler();
			 }

	  tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	  tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	  tim2OC_init.Pulse  = pulse1_value;


 	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_1) != HAL_OK)
				  {
					   Error_handler();
				  }

 	tim2OC_init.Pulse  = pulse2_value;

 	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_2) != HAL_OK)
		  {
			   Error_handler();
		  }

 	tim2OC_init.Pulse  = pulse3_value;
	  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_3) != HAL_OK)
	  {
		   Error_handler();
	  }

	  tim2OC_init.Pulse  = pulse4_value;
	  if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_4) != HAL_OK)
	  {
		   Error_handler();
	  }




}

void LSE_Configuration(void)
{

#if 0
	RCC_OscInitTypeDef Osc_Init;
	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	Osc_Init.LSEState = RCC_LSE_ON;

	if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
		{
				Error_Handler();
		}
#endif
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}


 void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	 if (! is_capture_done)

	 {	 if (count==1)
		 {
			input_captures[0] =__HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		 }
		 else if(count==2)
		 {
			 input_captures[1] =__HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			 count=1;

			 is_capture_done = TRUE;
		 }
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

