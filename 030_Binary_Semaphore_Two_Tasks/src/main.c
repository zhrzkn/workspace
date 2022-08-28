#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

void Sys_Init();
void GPIO_Config();
void USART_Config();

char myMessage[100];

/******************* TASKS *****************/

void MasterTaskHandler(void *parameters);
void SlaveTaskHandler(void *parameters);

/******************* QUEUES *****************/
xQueueHandle xQueue = NULL;

/******************* SEMAPHORES *****************/
xSemaphoreHandle xSemaphore = NULL;

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

	sprintf(myMessage, "Demo of Binary Semaphore Usage Between Two Tasks  \r\n");
	printMessage(myMessage);

	xQueue = xQueueCreate(1, sizeof(unsigned int));

	vSemaphoreCreateBinary(xSemaphore);

	if((xQueue != NULL) && (xSemaphore != NULL))
	{
		sprintf(myMessage, "Queues, Semaphores and Tasks are Created  \r\n");
		printMessage(myMessage);

	xTaskCreate(MasterTaskHandler, "Master Task", 500, NULL, 3,NULL);
	xTaskCreate(SlaveTaskHandler, "Slave Task", 500, NULL,   1,NULL);

	vTaskStartScheduler();
	}

	else
	{
		sprintf(myMessage, " Tasks aren't Created  \r\n");
		printMessage(myMessage);
	}
  while (1)
  {

  }
}


void MasterTaskHandler(void *parameters)
{
	unsigned int xMasterPassId;
	portBASE_TYPE xStatus;

	xSemaphoreGive(xSemaphore);    // 1 kereliðine semaphore aktif edildi.

	 while (1)
	  {
		 xMasterPassId = (rand() & 0x1FF);

		 xStatus = xQueueSend(xQueue, &xMasterPassId, portMAX_DELAY);    // Kuyruða veri yazýyoruz.

		 if(xStatus!= pdPASS)   		// Kuyruða veri yazýlmadý ise..
		 {
				sprintf(myMessage, " Couldn't sent to the Queue  \r\n");
				printMessage(myMessage);
		 }
		 else
		 {
			 xSemaphoreGive(xSemaphore);

			 taskYIELD();   	 // Manuel baðlam deðiþimi yapýldý.
		 }
	  }
}

void SlaveDoWork(unsigned int passId)
{
	sprintf(myMessage, " Slave task working on Passed Id: %d  \r\n", passId);
	printMessage(myMessage);

	vTaskDelay(passId);
}

void SlaveTaskHandler(void *parameters)
{
	unsigned int xSlavePassId;
	portBASE_TYPE xStatus;


	 while (1)
	  {
		 xSemaphoreTake(xSemaphore, 0);
		 xStatus = xQueueReceive(xQueue, &xSlavePassId, 0);

		 if(xStatus!= pdPASS)
		 		 {
		 				sprintf(myMessage, " Couldn't sent to the Queue  \r\n");
		 				printMessage(myMessage);
		 		 }
		 		 else
		 		 {
		 			 SlaveDoWork(xSlavePassId);

	  }
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
