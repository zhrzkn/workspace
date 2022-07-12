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
	// 1.Adým Sistem saati dahili saat olarak ayarlandý.
  RCC_DeInit(); // HSI ON | HSE OFF PLL OFF | SystemClock 16 MHz

    // 2.Adým Sistem saati 16 MHz olarak ayarlandý.
  SystemCoreClockUpdate();   // SystemCoreClock = 16 MHz

  Setup();

  sprintf(myMessage, "That is a FreeRTOS Task \n");
  printMessage(myMessage);

    // 3.Adým Görev oluþturma.
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
	GPIO_InitStruct.GPIO_PuPd =   GPIO_PuPd_UP; //Hat boþtayken 1 olmalý.
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);    // USART2_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // USART2_RX

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStruct = {0} ;

	USART_InitStruct.USART_BaudRate = 9600 ;    //Ýletiþim hýzý yapýlandýrýr.
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Veri iletim sýrasýnda buffer bellekte oluþan veri kaybýný engellemek için.
	USART_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx;     //Ýletim ya da gönderim yapacaðýný seç
	USART_InitStruct.USART_Parity = USART_Parity_No ;   // Ýletim sýrasýnda verilerdeki deðiþimi belirtmek için kullanýlan eþlik biti.
	                                                    // Verilerdeki deðiþim; uyumsuz baudrate oraný, elektromanyetik etkilenmeler, mesafe uzunluðu yüzünden oluþan veri aktarýmý sýkýntýsý
	                                                    // Veri çerçevesini aldýktan sonra bu çerçevedeki toplam bit sayýsýnýn deðerinin çift/tek olduðuna bakar.
		                                               // Çift ise 1, Tek ise 0 eþlik biti gönderir.
	USART_InitStruct.USART_StopBits = USART_StopBits_1  ;    // Veri paketinin sonunu belirtir.
	USART_InitStruct.USART_WordLength = USART_WordLength_8b  ; // Kelime uzunluðu.


	USART_Init(USART2, &USART_InitStruct);

	USART_Cmd(USART2, ENABLE);
}

void printMessage(char *message)
{
	for (int i = 0; i < strlen(message); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET); // TXE boþalmadýðý sürece bekle.
		USART_SendData(USART2, message[i]);   // Mesaj dizini içerisindeki tüm elemanlarý gönderir.
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
