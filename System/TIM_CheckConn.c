#include "TIM_CheckConn.h"



void TIM_CheckConn_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision 	= 	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode 		= 	TIM_CounterMode_Up;	// 30s			6s			60s
	TIM_TimeBaseInitStructure.TIM_Period 			= 	(12000 - 1);	// 	   36000		12000		60000	// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler 		= 	(36000 - 1);	//     60000		36000		60000	// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 	0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	// ?????? ???!!!!
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
		
		// ·¢ËÍAT+MQTTCONN?
		
		
		
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}	
}
