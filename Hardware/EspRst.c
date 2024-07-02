#include "My_include.h"
extern bool esp_RstPin;

void EspRst_GPIO_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);			// 引脚重映射
	

	
	GPIO_Pin_Init(PA15, GPIO_Mode_IPU);				// test EspRst

	
	// Init EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
    // EXTI Line0 
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;			// 上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			// 中断对象
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(){
	u16 timeout = 50000;
	if((EXTI_GetITStatus(EXTI_Line15) == SET) /* && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0) */){	
		// Delay_ms(500);
		PBout(9) = 0;			// ESP8266 RST引脚拉低
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
			;
		// Delay_ms(300);
		printf("ESP8266 Reset Successful!\r\n");
		PBout(9) = 1;
		// esp_RstPin = true;
		EXTI_ClearITPendingBit(EXTI_Line15);
		// PBout(9) = 1;
		while(timeout--)
			;
	}
}

