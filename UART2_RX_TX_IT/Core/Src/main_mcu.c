/*
 * main_mcu.c
 *
 *  Created on: May 17, 2024
 *      Author: Sachin K S
 */


#include "main.h"
#include "string.h"

#define TRUE  1			 // defining macros for true or false
#define FALSE 0

void SystemClockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
uint8_t convert_to_capital(uint8_t data);


UART_HandleTypeDef huart2;
uint8_t data_buffer[100];
uint32_t count = 0;
uint8_t rcvd_data; // variable to store received data from user
uint8_t reception_complete = FALSE;

char *user_data = "The application is running\r\n";

int main(void)
{

	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t len_of_data = strlen(user_data);
	HAL_UART_Transmit(&huart2,(uint8_t*)user_data,len_of_data,HAL_MAX_DELAY);


	while (reception_complete != TRUE)
	{
		HAL_UART_Receive_IT(&huart2, &rcvd_data, 1); // API for receiving data  through interrupt method
	}
	while(1);

	return 0;

}

void SystemClockConfig(void)
{


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

uint8_t convert_to_capital(uint8_t data)
{
	if (data >= 'a' && data <= 'z')
	{
		data = data - ('a'-'A'); // converting to capital by ASCI value
	}

	return data;

}
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)   // when 1 byte is received the interrupt processing API process the interrupt and calls this function
{// collects the data bytes received

	 if(rcvd_data== "\r")
	 {
		reception_complete = TRUE;
		data_buffer[count++]= "\r";
		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY); // transmiting data to user
	 }
	 else
	 {
		 data_buffer[count++]=rcvd_data;
	 }
}
void Error_Handler(void)
{
	while(1);

}
