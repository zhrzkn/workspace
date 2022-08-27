#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"

#include "string.h"
#include "stdio.h"

void Sys_Init();
void GPIO_Config();
void USART_Config();

char myMessage[100];


uint8_t rxData[20];  //Usart �zerinden mesaj gidip gitmedi�ini kontrol ediyoruz.
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
		rxCaracter = USART_ReceiveData(USART2);  // Usart2'den mesaj al�nacak.

	rxData[rxCount++] = rxCaracter &  0xFF;

	// Al�nan her bir karakter tek tek yaz�lacak.
	// &  0xFF; 1 olanlar 1, 0 olanlar 0 olarak kald�. Yazmasan da olur.
	// [rxCount++] yerine alt sat�rda rxCount += 1; de yaz�labilirdi.

	if(rxCaracter == 10)   // E�er karaterim 10(bo�lu tu�u) ise mesaj�m bitmi�tir.
	{
		rxCount = 0;
		printf(rxData);    // Ald��� mesaj� ayn� �ekilde geri verecek.
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

int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();

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

	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 // USART Ayarlamalar�.

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;   //GPIO Alternate function Mode
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	 GPIO_InitSruct.GPIO_PuPd =  GPIO_PuPd_UP;
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

		 // Gelen register�n i�i bo� de�il -> RXNE
		 // Herhangi bir g�rev ka�ma/ka��rma durumu olmamas� i�in.


		 // NVIC
		 // Kesme de�eri configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY'nin alt�nda olmamal�.

		 NVIC_SetPriority(USART2_IRQn, 5);     // �ncelik ayarland�.
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
