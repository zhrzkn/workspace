#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"

#include "string.h"
#include "stdio.h"

TaskHandle_t myTask1Handle= NULL;
TaskHandle_t myTask2Handle = NULL;

char myMessage[100];

void Sys_Init();
void GPIO_Config();
void USART_Config();



void Task1_Handler(void *params);
void Task2_Handler(void *params);



void printMessage(char *message)
{
	for(int i=0; i< strlen(message); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!= SET);
		USART_SendData(USART2, message[i]);
	}
}

int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();



	xTaskCreate( Task1_Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, &myTask1Handle);
	xTaskCreate( Task2_Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, &myTask2Handle);

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Handler(void *params)
{


	while (1)
	  {
		sprintf(myMessage, "State of Led: %d  \r\n",
				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ));
		printMessage(myMessage);
		vTaskDelay(pdMS_TO_TICKS(1000));
	  }

}

void Task2_Handler(void *params)
{


		while (1)
		  {
			 GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
			 vTaskDelay(pdMS_TO_TICKS(1000));    			  // ms cinsinden Tick cinsine dönüþtürüyor.
		  }

}

void Sys_Init(void)
{

	GPIO_Config();
	USART_Config();

}
void GPIO_Config()
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2 );   // USART2_Tx
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2 );	 // USART2_	Rx


	 GPIO_Init(GPIOA, &GPIO_InitStruct);

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;



	 GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void USART_Config()
{
	     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		 USART_InitTypeDef USART_InitStruct = {0} ;


		 USART_InitStruct.USART_BaudRate = 9600;
		 USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
		 USART_InitStruct.USART_Mode= USART_Mode_Rx | USART_Mode_Tx;
		 USART_InitStruct.USART_Parity = USART_Parity_No ;
		 USART_InitStruct.USART_StopBits = USART_StopBits_1 ;
		 USART_InitStruct.USART_WordLength = USART_WordLength_8b;


		 USART_Init(USART2, &USART_InitStruct);

		 USART_Cmd(USART2, ENABLE);
}




void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
