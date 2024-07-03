#include "My_include.h"



/*
	PB12
    PB13	-> 	  门磁
    PB14
	PB15
*/ 


void relay5V_Init(void){
	// 四路继电器
	GPIO_Pin_Init(PB12 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB13 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB14 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB15 ,GPIO_Mode_Out_PP);				
	
	// 开关控制门磁短暂开启函数 初始化
	// switchCtrlRelay_Init();

	// 使用反相输出 高电平触发
	// 继电器常闭通电  IO初始状态为高电平(三极管导通5v拉低,继电器不动作) 5V拉低->不触发
	PBout(12) = 1;
	PBout(13) = 1;
	PBout(14) = 1;
	PBout(15) = 1;	
} 


