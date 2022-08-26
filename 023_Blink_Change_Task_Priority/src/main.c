#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"


TaskHandle_t myTask1Handle= NULL;
TaskHandle_t myTask2Handle = NULL;

uint8_t buttonState=0;

void Sys_Init();
void GPIO_Config();
void EXTI_Config();



void Task1_Handler(void *params);
void Task2_Handler(void *params);


void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 ) == Bit_SET )
		{
		buttonState= 1;
		}
}



int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();



	xTaskCreate( Task1_Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, &myTask1Handle);
	xTaskCreate( Task2_Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, &myTask2Handle);

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Handler(void *params)
{
	UBaseType_t p1, p2;

	 GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	 vTaskDelay(250);

	while (1)
	  {
		if(buttonState == 1)
		{
			buttonState == 0;
			p1 = uxTaskPriorityGet(myTask1Handle);
			p2 = uxTaskPriorityGet(myTask2Handle);

			vTaskPrioritySet(myTask1Handle,p2);
			vTaskPrioritySet(myTask2Handle,p1);
		}
	  }

}

void Task2_Handler(void *params)
{
	UBaseType_t p1, p2;

	 GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	 vTaskDelay(1000);

		while (1)
		  {
			if(buttonState == 1)
			{
				p1 = uxTaskPriorityGet(myTask1Handle);
				p2 = uxTaskPriorityGet(myTask2Handle);

				vTaskPrioritySet(myTask1Handle,p2);
				vTaskPrioritySet(myTask2Handle,p1);
			}
		  }

}

void Sys_Init(void)
{

	GPIO_Config();
	EXTI_Config();

}
void GPIO_Config()
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA, &GPIO_InitStruct);
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
}



void EXTI_Config()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		EXTI_InitTypeDef EXTI_InitStruct = {0};

		EXTI_InitStruct.EXTI_Line = EXTI_Line0 ;
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

		EXTI_Init(&EXTI_InitStruct);

		NVIC_SetPriority(EXTI0_IRQn, 2);
		NVIC_EnableIRQ(EXTI0_IRQn);

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
