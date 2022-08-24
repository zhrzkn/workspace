#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"

#include "string.h"
#include "stdio.h"

TaskHandle_t myTask1Handle= NULL;
TaskHandle_t myTask2Handle = NULL;

char myMessage[100];
uint8_t buttonState=0;

void Sys_Init();
void GPIO_Config();
void EXTI_Config();
void USART_Config();



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



	xTaskCreate( Task1_Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 4, &myTask1Handle);
	xTaskCreate( Task2_Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, &myTask2Handle);

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Handler(void *params)
{
	UBaseType_t p1, p2;
	sprintf(myMessage, "Task 1 is running\r\n");
	printMessage(myMessage);

	sprintf(myMessage, "Task 1 is priority: %d  \r\n", uxTaskPriorityGet(myTask1Handle));
	printMessage(myMessage);

	sprintf(myMessage,"Task 2 is priority: %d  \r\n", uxTaskPriorityGet(myTask2Handle));
	printMessage(myMessage);

	while (1)
	  {
		if(buttonState == 1)
		{
			buttonState == 0;
			p1 = uxTaskPriorityGet(myTask1Handle);  //task1'in önceliði 4'tür.
			p2 = uxTaskPriorityGet(myTask2Handle);  //task2'in önceliði 2'dir.

			vTaskPrioritySet(myTask1Handle,p2);    //task1'in önceliði 2 oldu.
			vTaskPrioritySet(myTask2Handle,p1);   //task2'in önceliði 4 oldu.
		}
	  }

}

void Task2_Handler(void *params)
{
	UBaseType_t p1, p2;

		sprintf(myMessage, "Task 2 is running\r\n");
		printMessage(myMessage);

		sprintf(myMessage, "Task 2 is priority: %d  \r\n", uxTaskPriorityGet(myTask2Handle));
		printMessage(myMessage);

		sprintf(myMessage,"Task 1 is priority: %d  \r\n", uxTaskPriorityGet(myTask1Handle));
		printMessage(myMessage);

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
	USART_Config();
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

		 USART_Cmd(USART2, ENABLE);
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
