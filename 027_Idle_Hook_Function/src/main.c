#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"


TaskHandle_t myTask1Handle= NULL;
TaskHandle_t myTask2Handle = NULL;


void Sys_Init();
void GPIO_Config();


void Task1_Handler(void *params);
void Task2_Handler(void *params);


void vApplicationIdleHook()
{

}

int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();



	xTaskCreate( Task1_Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, &myTask1Handle);
	xTaskCreate( Task2_Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, &myTask2Handle);

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Handler(void *params)
{


	while (1)
	  {
		 GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14);
		 vTaskDelay(pdMS_TO_TICKS(1000));
	  }

}

void Task2_Handler(void *params)
{


		while (1)
		  {
			 GPIO_ToggleBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_15);
			 vTaskDelay(pdMS_TO_TICKS(1000));
		  }

}

void Sys_Init(void)
{

	GPIO_Config();
}
void GPIO_Config()
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;


	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;


	 GPIO_Init(GPIOD, &GPIO_InitStruct);

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
