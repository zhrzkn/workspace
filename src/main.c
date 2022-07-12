#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
void Task1_Handler(void *params);
void Task2_Handler(void *params);

int main(void)
{
	// 1.Adým Sistem saati dahili saat olarak ayarlandý.
  RCC_DeInit(); // HSI ON | HSE OFF PLL OFF | SystemClock 16 MHz

    // 2.Adým Sistem saati 16 MHz olarak ayarlandý.
  SystemCoreClockUpdate();   // SystemCoreClock = 16 MHz

    // 3.Adým Görev oluþturma.
  xTaskCreate(Task1_Handler, "myFirstTask", 500, NULL, 4, &myTask1Handle);
  xTaskCreate(Task2_Handler, "mySecondTask", 500, NULL, 4, &myTask2Handle);
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
