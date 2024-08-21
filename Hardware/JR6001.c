#include "JR6001.h"

void My_JR6001_SetVolume(u8 level){
	if(level > 20)	USART_SendString(USART3, JR6001_VOLUME_MAX);
	else if(level > 10)USART_SendString(UART3_JR6001, JR6001_VOLUME_MID);
	else if(level > 0)USART_SendString(UART3_JR6001, JR6001_VOLUME_MIN);
	
}

bool My_JR6001_IsBusy(void){
	// if(PinRead(PA4) == 1)
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 1)
		return false;
	return true;
}

void My_JR6001_Init(){		// JR6001 初始化
	JR6001_Usart3_Init(9600);
	Delay_ms(20);
	My_JR6001_SetVolume(30);			// 最大音量
	Delay_ms(20);
	JR6001_Set_Single_Stop();
}

/**
  * @brief  JR6001串口初始化,ReadPin-Busy初始化
  *         
  * @param  波特率9600（芯片内部确定好的），初始化STN32的TX引脚（PB10）,RX引脚无需设置，无需接收来自
			JR6001的确认信息
  *        
  * @retval None
  */
void JR6001_Usart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 
 
 	USART_DeInit(USART3);  
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
   
//	// 初始化读BUSY引脚 PA10 (原usart1-RX)	  在串口一已配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	
    // 不初始化 USART3_RX	  PB11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	USART_InitStructure.USART_BaudRate = bound; // 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	
  
	USART_Init(USART3, &USART_InitStructure); 
  
 
	USART_Cmd(USART3, ENABLE);  
}
