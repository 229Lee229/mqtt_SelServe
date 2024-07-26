#include "My_include.h"
extern u8 Time_doorLock;

// 开关控制门磁   PA15	 PB3   PB4
void switchCtrlRelay_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);			// 引脚重映射
	

	
	GPIO_Pin_Init(PA15, GPIO_Mode_IPU);
	GPIO_Pin_Init(PB3 , GPIO_Mode_IPU);
	GPIO_Pin_Init(PB4 , GPIO_Mode_IPU); 

	
	// Init EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
    // EXTI Line0 
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		// 上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			// 中断对象
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}


void EXTI15_10_IRQHandler(){
	u16 timeout = 50000;
	if((EXTI_GetITStatus(EXTI_Line15) == SET) /* && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0) */ ){			
		
		
		
		/* 按下开关后,使能TIM4,开始计数 */
		PBout(15) = 0;
		PBout(14) = 0;				// 门磁失电 可以开门
		PBout(13) = 0;
		PBout(12) = 0;
		EXTI_ClearITPendingBit(EXTI_Line15);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
			;
		Time_doorLock = 0;			// 若在12s的时间再次按下开关 则重新计时
		TIM_Cmd(TIM4, ENABLE);
		// Delay_ms_JKD(30);		// 防抖动
		while(timeout--){
			;
		}
	}

}
