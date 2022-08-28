#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

void Sys_Init();
void GPIO_Config();
void USART_Config();

char myMessage[100];

/******************* TASKS *****************/

void Task1Handler(void *parameters);
void Task2Handler(void *parameters);


/******************* SEMAPHORES *****************/
xSemaphoreHandle xSemaphoreKEY;

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

	vSemaphoreCreateBinary(xSemaphoreKEY);     // Karmaþýklýðý düzeltiyoruz.

	if (xSemaphoreKEY != NULL)
	{

	sprintf(myMessage, "Demo of Mutex with Binary Semaphore.  \r\n");
	printMessage(myMessage);

	xTaskCreate(Task1Handler, "Task-1", 500, NULL, 2,NULL);
	xTaskCreate(Task2Handler, "Task-2", 500, NULL, 2,NULL);

	vTaskStartScheduler();
	}


  while (1)
  {

  }
}


void Task1Handler(void *parameters)     // Sabit char dizisi gönderiyor.
{
	const char *pvTaskName = "Task-1 is running. \r\n";

	 while (1)
	  {
		 	xSemaphoreTake(xSemaphoreKEY, portMAX_DELAY);   // Karmaþýklýðý düzeltiyoruz.
			sprintf(myMessage, " %s", pvTaskName);
			printMessage(myMessage);
			xSemaphoreGive(xSemaphoreKEY);			// Karmaþýklýðý düzeltiyoruz.

			vTaskDelay(pdMS_TO_TICKS(500));
	  }
}

void Task2Handler(void *parameters)
{
	const char *pvTaskName = "Task-2 is running. \r\n";

	 while (1)
	  {
		 xSemaphoreTake(xSemaphoreKEY, portMAX_DELAY);	// Karmaþýklýðý düzeltiyoruz.
		 sprintf(myMessage, " %s", pvTaskName);
		 printMessage(myMessage);
		 xSemaphoreGive(xSemaphoreKEY);					// Karmaþýklýðý düzeltiyoruz.

		 vTaskDelay(pdMS_TO_TICKS(500));

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


	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2 );   // USART2_Tx
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2 );	 // USART2_	Rx


	 GPIO_Init(GPIOA, &GPIO_InitStruct);


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
