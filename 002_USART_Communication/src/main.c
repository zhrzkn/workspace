#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "stdio.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"


char myMessage[200];

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
void Task1_Handler(void *params);
void Task2_Handler(void *params);

void Setup(void);
void GPIO_Config();
void USART_Config(void);
void printMessage(char *message);

int main(void)
{
	// 1.Ad�m Sistem saati dahili saat olarak ayarland�.
  RCC_DeInit(); // HSI ON | HSE OFF PLL OFF | SystemClock 16 MHz

    // 2.Ad�m Sistem saati 16 MHz olarak ayarland�.
  SystemCoreClockUpdate();   // SystemCoreClock = 16 MHz

  Setup();

  sprintf(myMessage, "That is a FreeRTOS Task \n");
  printMessage(myMessage);

    // 3.Ad�m G�rev olu�turma.
  xTaskCreate(Task1_Handler, "myFirstTask", 130, NULL, 4, &myTask1Handle);
  xTaskCreate(Task2_Handler, "mySecondTask", 130, NULL, 4, &myTask2Handle);

  vTaskStartScheduler();

  while (1)
  {

  }
}
void Task1_Handler(void *params)
{
    while(1)
    {

    }
}
void Task2_Handler(void *params)
{
	 while(1)
	    {

	    }
}


void Setup(void)
{
	GPIO_Config();
	USART_Config();
}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =   GPIO_PuPd_UP; //Hat bo�tayken 1 olmal�.
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);    // USART2_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // USART2_RX

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStruct = {0} ;

	USART_InitStruct.USART_BaudRate = 9600 ;    //�leti�im h�z� yap�land�r�r.
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Veri iletim s�ras�nda buffer bellekte olu�an veri kayb�n� engellemek i�in.
	USART_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx;     //�letim ya da g�nderim yapaca��n� se�
	USART_InitStruct.USART_Parity = USART_Parity_No ;   // �letim s�ras�nda verilerdeki de�i�imi belirtmek i�in kullan�lan e�lik biti.
	                                                    // Verilerdeki de�i�im; uyumsuz baudrate oran�, elektromanyetik etkilenmeler, mesafe uzunlu�u y�z�nden olu�an veri aktar�m� s�k�nt�s�
	                                                    // Veri �er�evesini ald�ktan sonra bu �er�evedeki toplam bit say�s�n�n de�erinin �ift/tek oldu�una bakar.
		                                               // �ift ise 1, Tek ise 0 e�lik biti g�nderir.
	USART_InitStruct.USART_StopBits = USART_StopBits_1  ;    // Veri paketinin sonunu belirtir.
	USART_InitStruct.USART_WordLength = USART_WordLength_8b  ; // Kelime uzunlu�u.


	USART_Init(USART2, &USART_InitStruct);

	USART_Cmd(USART2, ENABLE);
}

void printMessage(char *message)
{
	for (int i = 0; i < strlen(message); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET); // TXE bo�almad��� s�rece bekle.
		USART_SendData(USART2, message[i]);   // Mesaj dizini i�erisindeki t�m elemanlar� g�nderir.
	}
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
