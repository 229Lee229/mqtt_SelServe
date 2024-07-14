#include "My_include.h"
#define doorLock_countDown_MAX	10				// 开关控制门磁计时时间


u8 Time_doorLock = 0;			// 按下出门开关后 门磁25s计数 按下开关开始计数
extern bool CompareTime_Flag;	// 7/13
void timer_doorLock_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision 	= 	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode 		= 	TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period 			= 	(36000 - 1);						// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler 		= 	(2000 - 1);							// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 	0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	// TIM_InternalClockConfig(TIM4);						// 选择内部时钟源
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	
	// 在按下开关前 不使能!!!!
	TIM_Cmd(TIM4, DISABLE);

	
}


void TIM4_IRQHandler(void){
	char stringDebug[20];
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

		
//		if(CompareTime_Flag == true){		// 若现在是开门时间内,则这个开关无作用 返回不执行
//			return;							// 或者 当比较时间 进入开门状态 不是能time4中断
//		}
		Time_doorLock++;
		sprintf(stringDebug,"%d\r\n",Time_doorLock);
		printf("%s",stringDebug);

		#ifdef 	DEBUG_DoorLock
			PCout(13) = !PCout(13);	
			sprintf(stringDebug,"%d\r\n",Time_doorLock);
			printf("%s",stringDebug);
		#endif
		

		if(Time_doorLock == doorLock_countDown_MAX){
			Time_doorLock = 0;
			// 12s时间到 门磁上电 不使能TIM4 自动计数关闭
			PBout(15) = 1;
			PBout(14) = 1;				// 门磁上电  不可开门
			PBout(13) = 1;
			TIM_Cmd(TIM4, DISABLE);
 		}
	}
}





