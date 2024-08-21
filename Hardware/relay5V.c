#include "My_include.h"



/*
	PB12
    PB13	
    PB14
	PB15
*/ 


void relay5V_Init(void){
	// 四路继电器
	GPIO_Pin_Init(Relay_1_Pin_DoorLock 	,		GPIO_Mode_Out_PP);
	GPIO_Pin_Init(Relay_2_Pin_Light 	,		GPIO_Mode_Out_PP);
	GPIO_Pin_Init(Relay_3_Pin_AllSocket ,		GPIO_Mode_Out_PP);
	GPIO_Pin_Init(Relay_4_Pin_else 		,		GPIO_Mode_Out_PP);				
	
	// 开关控制门磁短暂开启函数 初始化
	// switchCtrlRelay_Init();

	// 使用反相输出 高电平触发
	// 继电器常闭通电  IO初始状态为高电平(三极管导通5v拉低,继电器不动作) 5V拉低->不触发
	PBout(12) = 0;
	PBout(13) = 0;
	PBout(14) = 0;
	PBout(15) = 0;	
} 


