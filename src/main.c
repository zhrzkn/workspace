#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"

int _write(int32_t file, uint8_t *ptr, int32_t len)   //printf çalýþtýrmak için gerekli.
{
	for (int i = 0; i< len; i++)

		ITM_SendChar((*ptr++));

		return len;

}

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
void Task1_Handler(void *params);
void Task2_Handler(void *params);

void Setup(void);
void GPIO_Config();

int main(void)
{
	// 1.Adým Sistem saati dahili saat olarak ayarlandý.
  RCC_DeInit(); // HSI ON | HSE OFF PLL OFF | SystemClock 16 MHz

    // 2.Adým Sistem saati 16 MHz olarak ayarlandý.
  SystemCoreClockUpdate();   // SystemCoreClock = 16 MHz

  printf("Starting RTOS project \n");

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
    	printf("Hello World From Task-1 \n");
    }
}
void Task2_Handler(void *params)
{
	 while(1)
	    {
		 printf("Hello World From Task-2 \n");
	    }
}


void Setup(void)
{
	GPIO_Config();
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

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
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
