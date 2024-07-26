#ifndef __USER_H
#define __USER_H


// 门磁继电器常闭
#define PwrOn_LockDoor				1   
#define PwrLoss_OpenDoor			0	  


// 其它继电器常开
#define Machine_PwrOn_Relay					0
#define Machine_PwrLoss_Relay				1
//typedef enum
//{
//	HeartBeat_Syn					4,
//	RELAY_1_ON						11,
//	RELAY_1_OFF				        10,
//	RELAY_2_ON				        21,
//	RELAY_2_OFF				        20,
//	RELAY_3_ON				        31,
//	RELAY_3_OFF				        30,                           
//	RELAY_AllOn_EXC_1	            101,
//	RELAY_ALLOff_EXC_1		        100
//}Json_Type;


//struct Json_type{
//	u8 Json_type : 7;
//	double Json_time;
//	char *MsgId;
//	
//	
//	
//}Json_type_NoPayload;


#define Relay_1_Pin_DoorLock				PBout(12)
#define Relay_2_Pin				            PBout(13)
#define Relay_3_Pin		                	PBout(14)
#define Relay_4_Pin							PBout(15)

#define HeartBeat_Syn						4

#define RELAY_1_ON							11
#define RELAY_1_OFF							10
#define RELAY_2_ON							21
#define RELAY_2_OFF							20
#define RELAY_3_ON							31
#define RELAY_3_OFF							30
#define SysReset							90
#define RELAY_AllOn_EXC_1					101			// 全部上电 除一号
#define RELAY_ALLOff_EXC_1					100			// 全部关电 除一号
#endif
