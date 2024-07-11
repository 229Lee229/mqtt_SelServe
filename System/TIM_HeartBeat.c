#include "My_include.h"
bool Enable_HeartBeat_Send_Flag = false;
void TIM_HeartBeat_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision 	= 	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode 		= 	TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period 			= 	(36000 - 1);	// 30s					// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler 		= 	(60000 - 1);							// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 	0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// TIM_InternalClockConfig(TIM4);						// ???????
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	
	// ?????? ???!!!!
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		Enable_HeartBeat_Send_Flag = true;
		// keep_HeartBeat();			// ·¢ËÍÐÄÌø»ØÖ´
		// printf("TIM2 Successful!\r\n");
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}

