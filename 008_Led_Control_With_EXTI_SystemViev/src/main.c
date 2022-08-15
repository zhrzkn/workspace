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
void Button_Handler(void);       //(void) yazma sebebi; bir parametre almayacak demek.


void ledFlashTask(void *params);


void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 ) == Bit_SET )
	{
		Button_Handler();   //Button Handler'a girdiðinde buton deðerini bir arttýracak.
	}
}



int main(void)
{

	DWT -> CTRL |=(1<<0);          //0.bit 1 olarak ayarlandý.
	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz
	setupConfig();

	SEGGER_SYSVIEW_Conf();  	// Olaylarýn kaydý tutuluyor.
	SEGGER_SYSVIEW_Start(); 	// Kayda baþlanýyor.



	xTaskCreate( ledFlashTask, "Task of Led Control", configMINIMAL_STACK_SIZE,

	              NULL, 3, &myLedTaskHandle);

	vTaskStartScheduler();                // Görevlerin zamanlama ayarý
  while (1)
  {

  }
}



void ledFlashTask(void *params)
{
	 while (1)
	  {

      if ( buttonCount ==0 )    //Tüm ledler sönük.
            {
    	     GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
            }

      else if(buttonCount == 1)  // Ledler 1 sn.'de bir yanýp sönecek
            {
    	     GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	     // Girilen port sürekli tersleniyor.

    	     vTaskDelay(1000);     // 1 saniyede 1 yak söndür.

            }
      else if(buttonCount == 2)  // Ledler 750 ms.'de bir yanýp sönecek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(750);
            }
      else if(buttonCount == 3)  // Ledler 500 ms.'de bir yanýp sönecek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(500);
            }
      else if(buttonCount == 4)  // Ledler 250 ms.'de bir yanýp sönecek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(250);
            }
	  }
}
void Button_Handler(void)
{
	buttonCount += 1;

	if (buttonCount==5)
		buttonCount = 0;

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
	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

		 GPIO_InitTypeDef GPIO_InitStruct = {0} ;
		 EXTI_InitTypeDef* EXTI_InitStruct = {0} ;


		 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;

		              // Ledler 12, 13,14 ve 15.pine baðlýdýr.

		 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
		 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

		 GPIO_Init(GPIOD, &GPIO_InitStruct);     // D portunun pinleri OUTPUT olarak ayarlandý.

		 EXTI_InitStruct->EXTI_Line = EXTI_Line0 ;
		 EXTI_InitStruct->EXTI_LineCmd = ENABLE ;
		 EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt ;
		 EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Rising;

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
