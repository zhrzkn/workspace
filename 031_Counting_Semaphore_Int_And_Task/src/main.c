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
void Software_Intterupt();

char myMessage[100];

/******************* TASKS *****************/

void HandlerTaskHandler(void *parameters);
void PeriodicTaskHandler(void *parameters);


/******************* SEMAPHORES *****************/
xSemaphoreHandle xCountingSemaphore = NULL;

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

	sprintf(myMessage, "Demo of Counting Semaphore Usage Between Two Tasks  \r\n");
	printMessage(myMessage);


	xCountingSemaphore = xSemaphoreCreateCounting(10,0);

	if((xCountingSemaphore != NULL))
	{
		sprintf(myMessage, "Semaphore, Interrupt and Task are Created  \r\n");
		printMessage(myMessage);

	xTaskCreate(HandlerTaskHandler, "Handler Task", 500, NULL,   1,NULL);
	xTaskCreate(PeriodicTaskHandler, "Periodic Task", 500, NULL, 3,NULL);

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


void HandlerTaskHandler(void *parameters)
{

	 while (1)
	  {
		 xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

			sprintf(myMessage, " Handler Task: Processing event.  \r\n");
			printMessage(myMessage);

	  }
}

void PeriodicTaskHandler(void *parameters)
{


	 while (1)
	  {
		 vTaskDelay(pdMS_TO_TICKS(500));

			sprintf(myMessage, " Periodic Task: Pending the Interrupt.  \r\n");
			printMessage(myMessage);

			NVIC_SetPendingIRQ(EXTI15_10_IRQn);   // Kesme meydana geldi.

			sprintf(myMessage, " Periodic Task: Resuming.  \r\n");
			printMessage(myMessage);

	  }
}

void Software_Intterupt()
{
	NVIC_SetPriority(EXTI15_10_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );      // Interrupt aktif ediliyor.

	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	sprintf(myMessage, "Interrupt is Active  \r\n");
	printMessage(myMessage);

	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);   // 	Her ikisini verdiðinde sonuç doðru olarak gelirse interrupttan çýk.
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);

	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
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
