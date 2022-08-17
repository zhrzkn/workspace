#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"



TaskHandle_t myButtonTaskHandle= NULL;
TaskHandle_t myLedTaskHandle = NULL;

uint8_t buttonCount=0;            // Her intterrupt'a girdiðinde buttoncount deðerini arttýr.

void setupConfig(void);
void gpioInputConfig(void);
void gpioOutputConfig(void);
void delay_FreeRTOS(uint32_t delayMS);   // Oluþan arký önlemek için.


void ledFlashTask(void *params);
void readButtonStateTask(void *params);





int main(void)
{

	DWT -> CTRL |=(1<<0);          //CNC Count deðerini almak için çaðýrýyoruz.
	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz
	setupConfig();


	xTaskCreate( ledFlashTask, "Task of Led Control", 650, NULL, 2, &myLedTaskHandle);


	xTaskCreate( readButtonStateTask, "Task of Button Control", 650,  NULL, 2, &myButtonTaskHandle);

	vTaskStartScheduler();                // Görevlerin zamanlama ayarý
  while (1)
  {

  }
}

void readButtonStateTask(void *params)
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)== Bit_SET )
		{
			delay_FreeRTOS(100);
			xTaskNotify(myLedTaskHandle, 0x00, eIncrement);    //Hedef görevin bildirim deðerini eIncrement  ile arttýr dedik.
		}
	}
}


void ledFlashTask(void *params)
{
	uint32_t pulNotificationValue =0 ;
	 while (1)
	  {
		 if(xTaskNotifyWait(0x00,0x00, &pulNotificationValue, portMAX_DELAY) == pdTRUE)
		 {
			 GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );

			 buttonCount = pulNotificationValue ;  //Butona kaç kere basýldýðýný sayacak ve görebileceðiz.
		 }
	  }
}

void setupConfig(void)
{

	gpioInputConfig();
	gpioOutputConfig();

}
void gpioInputConfig(void)
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // A portunun 0.pinine baðlý olan buton için clock hattý enable edildi.
	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA, &GPIO_InitStruct);     // A portunun 0.pini INPUT olarak ayarlandý.

	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

}
void gpioOutputConfig(void)
{
	     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // D portunun clock hattý enable edildi.


		 GPIO_InitTypeDef GPIO_InitStruct = {0} ;


		 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;

		              // Ledler 12, 13,14 ve 15.pine baðlýdýr.

		 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
		 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

		 GPIO_Init(GPIOD, &GPIO_InitStruct);     // D portunun pinleri OUTPUT olarak ayarlandý.

}

void delay_FreeRTOS(uint32_t delayMS)
{
	uint32_t currentTick = xTaskGetTickCount();
	uint32_t  delayTick = delayMS * configTICK_RATE_HZ / 1000;     //ms cinsinden girilen deðerin tick karþýlýðý.
	                      // pdMS_TO_TICKS de yazýlabilirdi.

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
