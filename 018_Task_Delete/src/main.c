#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"


void setupConfig(void);
void gpioInputConfig(void);
void gpioOutputConfig(void);
void delay_FreeRTOS(uint32_t delayMS);

void Task1Handler(void *params);
void Task2Handler(void *params);



TaskHandle_t myTask1Hadle = NULL;
TaskHandle_t myTask2Hadle = NULL;

int main(void)
{
		RCC_DeInit();
		SystemCoreClockUpdate();
		setupConfig();



		xTaskCreate(Task1Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 3, &myTask1Hadle);
		xTaskCreate(Task2Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 4, &myTask2Hadle);

  while (1)
  {

  }
}


void Task1Handler(void *params)
{

	 while (1)
	  {
		 GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13);
		 delay_FreeRTOS(1000);

		 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 ))
				 {
				 vTaskDelete(myTask1Hadle);
				 }
	  }
}

void Task2Handler(void *params)
{

	 while (1)
	  {
		 GPIO_ToggleBits(GPIOD, GPIO_Pin_14 | GPIO_Pin_15);
		 delay_FreeRTOS(500);
	  }
}

void setupConfig(void)
{

	gpioInputConfig();
	gpioOutputConfig();

}
void gpioInputConfig(void)
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA, &GPIO_InitStruct);

}
void gpioOutputConfig(void)
{
	     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

		 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;



		 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
		 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

		 GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void delay_FreeRTOS(uint32_t delayMS)
{
	uint32_t currentTick = xTaskGetTickCount();
	uint32_t  delayTick = delayMS * configTICK_RATE_HZ / 1000;

	while(xTaskGetTickCount() < (currentTick + delayTick));

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
