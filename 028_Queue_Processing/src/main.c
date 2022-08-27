#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "string.h"
#include "stdio.h"
/***************** PRIVATE DEFINES *****************/
#define LED_ON         	 1
#define LED_OFF          2
#define LED_TOGGLE_ON    3
#define LED_TOGGLE_OFF   4
#define LED_STATUS       5
#define EXIT_APP         6


/***************** PRIVATE VARIBALES *****************/
char myMessage[100];
char menuDisplay[] = {"\r\n LED_ON    -> 	 1 \
				      \r\n LED_OFF    -> 	 2 \
	      	  	  	  \r\n LED_TOGGLE_ON ->  3 \
  	  	              \r\n LED_TOGGLE_OFF -> 4 \
					  \r\n LED_STATUS ->     5 \
					  \r\n EXIT_APP   -> 	 6 \
					  \r\n                 \
                      \r\n Type your option here: "};

void Sys_Init();
void GPIO_Config();
void USART_Config();
uint8_t getCommandCode(uint8_t *buffer);
uint8_t getArguments(uint8_t *buffer);
void LED_ON_Func();
void LED_OFF_Func();
void LED_TOGGLE_ON_Func();
void LED_TOGGLE_OFF_Func();
void LED_STATUS_Func();
void EXIT_APP_Func();
void ERROR_MESSAGE();




/********************** TASKS ******************/

TaskHandle_t myTask1MenuHandle= 				 NULL;
TaskHandle_t myTask2CommandHandle =				 NULL;
TaskHandle_t myTask3CommandProcessHandle= 		 NULL;
TaskHandle_t myTask4UartWriteHandle =			 NULL;

void Task1_Menu_Display_Handler(void *params);
void Task2_Command_Handling_Handler(void *params);
void Task3_Command_Process_Handler(void *params);
void Task4_Uart_Write_Handler(void *params);

/********************** QUEUES ******************/

QueueHandle_t UartWriteQueue =  NULL;
QueueHandle_t CommandQueue =  NULL;

uint8_t commandbuffer[20];
uint8_t commandlenght=0;


void USART2_IRQHandler(void)
{
	uint8_t rxCaracter;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE))   // Flag 1 ise temizle.
	{
		rxCaracter = USART_ReceiveData(USART2);  // Usart2'den mesaj alýnacak.

	commandbuffer[commandlenght] = rxCaracter &  0xFF;


	if(commandlenght == 10)
	{
		commandlenght = 0;

		xTaskNotifyFromISR(myTask1MenuHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
		xTaskNotifyFromISR(myTask2CommandHandle, 0, eNoAction, &xHigherPriorityTaskWoken);

		// Veri geldiði için Task2'ye bildirim yapýlýyor.
	}
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

	if(xHigherPriorityTaskWoken == pdTRUE) // Hazýrda bekleyen görev var ise..
	{
		taskYIELD();
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

//	Kuyrukta yazýlacak görevin "baytý" bu yapý sayesinde azalmýþ olacak ve az yer kaplanacak.

typedef struct APP_COMMAND
{
	uint8_t COMMAND_NUM;
	uint8_t COMMAND_ARGS[10];
}APP_COMMAND_t;



int main(void)
{

	RCC_DeInit();                   // 	HSEOFF | HSION  | PLL OFF
	SystemCoreClockUpdate();        //SystemCoreClock = 16MHz

	Sys_Init();

	sprintf(myMessage, "This is Queue Command Processing Demo Application. \r\n");
	printMessage(myMessage);

	UartWriteQueue = xQueueCreate(10, sizeof( char*)) ;   		//40 bayt
	CommandQueue   = xQueueCreate(10, sizeof(APP_COMMAND_t)); 	//40 bayt

 if((UartWriteQueue != NULL) && (CommandQueue != NULL))     // Kuyruklar baþarýlý þekilde oluþturuldu ise..
 {
	sprintf(myMessage, "Queue creation succesful. \r\n");
	printMessage(myMessage);

	xTaskCreate( Task1_Menu_Display_Handler, "Menu Display", 		 500, NULL, 1, &myTask1MenuHandle);
	xTaskCreate( Task2_Command_Handling_Handler, "Command Handling", 500, NULL, 2, &myTask2CommandHandle);
	xTaskCreate( Task3_Command_Process_Handler, "Command Process", 	 500, NULL, 2, &myTask3CommandProcessHandle);
	xTaskCreate( Task4_Uart_Write_Handler, "Uart Write", 			 500, NULL, 2, &myTask4UartWriteHandle);

	vTaskStartScheduler();
 }
 else
 {
		sprintf(myMessage, "Queue creation failed. \r\n");
		printMessage(myMessage);
 }


  while (1)
  {
  }
}

void Task1_Menu_Display_Handler(void *params)
{
	char *txData = menuDisplay;   // Display'deki verile rxData'ya yazýldý.

	while (1)
	  {
		//	send data -> uart queue
		xQueueSend(UartWriteQueue, &txData, portMAX_DELAY);
								// portMAX_DELAY: Kuyruk boþalana kadar bekle.

		// wait notify
		xTaskNotifyWait(0,0,NULL, portMAX_DELAY);
	  }

}

void Task2_Command_Handling_Handler(void *params)
{
	uint8_t commandCode=0;
	APP_COMMAND_t *appTxCmd;   // Bu görevde struct türünden veri okunacaðý için struct türünden tanýmlama yaptýk.

		while (1)
		  {
			// wait notify
			xTaskNotifyWait(0,0,NULL, portMAX_DELAY);

			// command buffer -> command queue
			commandCode = getCommandCode(commandbuffer);
			appTxCmd = (APP_COMMAND_t *) pvPortMalloc(sizeof(APP_COMMAND_t)); // Belleði oluþturuldu.
									//pvPortMalloc: Belirli bir bellek tahsis ediyor.

			appTxCmd -> COMMAND_NUM = commandCode;   // USART üzerinden gelen mesaj numarasý kopyalandý command num'a koyuldu.
			getArguments(appTxCmd -> COMMAND_ARGS);

			xQueueSend(CommandQueue, &appTxCmd, portMAX_DELAY); // Command gönderildi.

		  }

}

void Task3_Command_Process_Handler(void *params)
{

	APP_COMMAND_t *appRxCmd= NULL;

	while (1)
	  {
		// read from command queue
		xQueueReceive(CommandQueue, (void*)&appRxCmd, portMAX_DELAY);
		// process
		if(appRxCmd->COMMAND_NUM == LED_ON)
		{
			LED_ON_Func();
		}
		else if(appRxCmd->COMMAND_NUM == LED_OFF)
		{
			LED_OFF_Func();
		}
		else if(appRxCmd->COMMAND_NUM == LED_STATUS)
		{
			LED_STATUS_Func();
		}
		else if(appRxCmd->COMMAND_NUM == LED_TOGGLE_ON)
		{
			LED_TOGGLE_ON_Func();
		}
		else if(appRxCmd->COMMAND_NUM == LED_TOGGLE_OFF)
		{
			LED_TOGGLE_OFF_Func();
		}
		else if(appRxCmd->COMMAND_NUM == EXIT_APP)
		{
			EXIT_APP_Func();
		}
		else
		{
		    ERROR_MESSAGE();
		}
	  }

}

void Task4_Uart_Write_Handler(void *params)
{
		char *rxData = NULL;

		while (1)
		  {
			// read from uart write queue
			xQueueReceive(UartWriteQueue, &rxData, portMAX_DELAY);

			// send message
			printMessage(rxData);
		  }

}
void LED_ON_Func()
{
	GPIO_SetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);
}
void LED_OFF_Func()
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);
}
void LED_TOGGLE_ON_Func()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);
}
void LED_TOGGLE_OFF_Func()
{

}
void LED_STATUS_Func()
{
	sprintf(myMessage, "Status of the Leds : %d \r\n", GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12));
	printMessage(myMessage);
	// print yerine xQueueSend(UartWriteQueue, myMessage, portMAX_DELAY); yazdýk ama kuyruk dolu olduðu için çalýþmaz.
}
void EXIT_APP_Func()
{

}
void ERROR_MESSAGE()
{
	sprintf(myMessage, "Wrong Value");
	xQueueSend(UartWriteQueue, myMessage, portMAX_DELAY);
}
uint8_t getCommandCode(uint8_t *buffer)
{
	return buffer[0] - 48;

	/* ASCII tablosunda 0'ýn deðeri  48
						1'in deðeri  49
						2'nin deðeri 50 gibi
						normalde terminale 1 yazýnca 49 yazacaktý.
						þimdi terminale 1 yazýnca 49 - 48'den 1 yazacak.
						Kafa karýþýklýðý gitmiþ olacak. */
}

uint8_t getArguments(uint8_t *buffer)
{
	return buffer[0];
}
void Sys_Init(void)
{

	GPIO_Config();
	USART_Config();

}
void GPIO_Config()
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	 GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOD, &GPIO_InitStruct);

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

		 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE) ;

		 // Gelen registerýn içi boþ deðil -> RXNE
		 // Herhangi bir görev kaçma/kaçýrma durumu olmamasý için.


		 // NVIC
		 // Kesme deðeri configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY'nin altýnda olmamalý.

		 NVIC_SetPriority(USART2_IRQn, 5);     // Öncelik ayarlandý.
		 NVIC_EnableIRQ(USART2_IRQn);

		 USART_Cmd(USART2, ENABLE);
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
