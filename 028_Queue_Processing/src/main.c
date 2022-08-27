#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "string.h"
#include "stdio.h"

void Sys_Init();
void GPIO_Config();
void USART_Config();

uint8_t rxData[20];  //Usart üzerinden mesaj gidip gitmediðini kontrol ediyoruz.
uint8_t rxCount=0;

/********************** TASKS ******************/

TaskHandle_t myTask1MenuHandle= 				 NULL;
TaskHandle_t myTask2CommandHandle =				 NULL;
TaskHandle_t myTask3CommandProcessHandle= 		 NULL;
TaskHandle_t myTask4UartWriteHandle =			 NULL;

void Task1_Menu_Display_Handler(void *params);
void Task2_Command_Handling_Handler(void *params);
void Task3_Command_Process_Handler(void *params);
void Task4_Uart_Write_Handler(void *params);

/********************** QUEUES ******************/

QueueHandle_t UartWriteQueue =  NULL;
QueueHandle_t CommandQueue =  NULL;


void USART2_IRQHandler(void)
{
	uint8_t rxCaracter;

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE))   // Flag 1 ise temizle.
	{
		rxCaracter = USART_ReceiveData(USART2);  // Usart2'den mesaj alýnacak.

	rxData[rxCount++] = rxCaracter &  0xFF;

	// Alýnan her bir karakter tek tek yazýlacak.
	// &  0xFF; 1 olanlar 1, 0 olanlar 0 olarak kaldý. Yazmasan da olur.
	// [rxCount++] yerine alt satýrda rxCount += 1; de yazýlabilirdi.

	if(rxCaracter == 10)   // Eðer karaterim 10(boþlu tuþu) ise mesajým bitmiþtir.
	{
		rxCount = 0;
		printf(rxData);    // Aldýðý mesajý ayný þekilde geri verecek.
	}
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}

void printMessage(char *message)
{
	for(int i=0; i< strlen(message); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!= SET);
		USART_SendData(USART2, message[i]);
	}
}

//	Kuyrukta yazýlacak görevin "baytý" bu yapý sayesinde azalmýþ olacak ve az yer kaplanacak.

typedef struct APP_COMMAND
{
	uint8_t COMMAND_NUM;
	uint8_t COMMAND_ARGS[10];
};



int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();


	UartWriteQueue = xQueueCreate(10, sizeof( char*)) ;


	xTaskCreate( Task1_Menu_Display_Handler, "Menu Display", 		 500, NULL, 2, &myTask1MenuHandle);
	xTaskCreate( Task2_Command_Handling_Handler, "Command Handling", 500, NULL, 2, &myTask2CommandHandle);
	xTaskCreate( Task3_Command_Process_Handler, "Command Process", 	 500, NULL, 2, &myTask3CommandProcessHandle);
	xTaskCreate( Task4_Uart_Write_Handler, "Uart Write", 			 500, NULL, 2, &myTask4UartWriteHandle);

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Menu_Display_Handler(void *params)
{

	while (1)
	  {

	  }

}

void Task2_Command_Handling_Handler(void *params)
{

		while (1)
		  {

		  }

}

void Task3_Command_Process_Handler(void *params)
{

	while (1)
	  {

	  }

}

void Task4_Uart_Write_Handler(void *params)
{


		while (1)
		  {

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
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOD, &GPIO_InitStruct);

	 // USART Ayarlamalarý.

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;   //GPIO Alternate function Mode
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

		 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE) ;

		 // Gelen registerýn içi boþ deðil -> RXNE
		 // Herhangi bir görev kaçma/kaçýrma durumu olmamasý için.


		 // NVIC
		 // Kesme deðeri configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY'nin altýnda olmamalý.

		 NVIC_SetPriority(USART2_IRQn, 5);     // Öncelik ayarlandý.
		 NVIC_EnableIRQ(USART2_IRQn);

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
