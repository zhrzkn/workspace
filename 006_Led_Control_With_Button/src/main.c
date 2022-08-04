#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"



TaskHandle_t myButtonTaskHandle= NULL;
TaskHandle_t myLedTaskHandle = NULL;

uint8_t buttonCount=0;            // Butona her bas�ld���nda farkl� s�relerde ledler yanacak.

void setupConfig(void);
void gpioInputConfig(void);
void gpioOutputConfig(void);


void readButtonStateTask(void *params);
void ledFlashTask(void *params);

int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz
	setupConfig();

	xTaskCreate( readButtonStateTask, "Task of Button Read", configMINIMAL_STACK_SIZE,
			     NULL, 3, &myButtonTaskHandle);                 // G�rev olu�turma fonk.


	xTaskCreate( ledFlashTask, "Task of Led Control", configMINIMAL_STACK_SIZE,

	              NULL, 3, &myLedTaskHandle);

	vTaskStartScheduler();                // G�revlerin zamanlama ayar�
  while (1)
  {

  }
}


void readButtonStateTask(void *params)
{
	 while (1)
	  {
         if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)   // Butona bas�ld�.
         {
        	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET);  // Butona bas�ld��� s�rece bekle.
        	// Profesyonel yaz�l�mc�l�kta bu istenmez.
        	// Bunu yaparak t�m i�lemleri durdurmu� olursun.
        	// Donan�msal olarak butona cap. ba�lars�n ya da timer kullan�rs�n.

        	buttonCount++;
        	if (buttonCount == 5)   // En fazla 5 kere bas�lmas� isteniyor.
        	{
        		buttonCount =0;
        	}
         }
	  }
}
void ledFlashTask(void *params)
{
	 while (1)
	  {

      if ( buttonCount ==0 )    //T�m ledler s�n�k.
            {
    	     GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
            }

      else if(buttonCount == 1)  // Ledler 1 sn.'de bir yan�p s�necek
            {
    	     GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	     // Girilen port s�rekli tersleniyor.

    	     vTaskDelay(1000);     // 1 saniyede 1 yak s�nd�r.

            }
      else if(buttonCount == 2)  // Ledler 750 ms.'de bir yan�p s�necek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(750);
            }
      else if(buttonCount == 3)  // Ledler 500 ms.'de bir yan�p s�necek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(500);
            }
      else if(buttonCount == 4)  // Ledler 250 ms.'de bir yan�p s�necek
            {
    	    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	    vTaskDelay(250);
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
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // A portunun 0.pinine ba�l� olan buton i�in clock hatt� enable edildi.
	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA, &GPIO_InitStruct);     // A portunun 0.pini INPUT olarak ayarland�.


}
void gpioOutputConfig(void)
{
	     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // D portunun clock hatt� enable edildi.
		 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

		 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;

		              // Ledler 12, 13,14 ve 15.pine ba�l�d�r.

		 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
		 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

		 GPIO_Init(GPIOD, &GPIO_InitStruct);     // D portunun pinleri OUTPUT olarak ayarland�.
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
