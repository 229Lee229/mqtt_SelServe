#ifndef __USER_H
#define __USER_H

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


#define Pin_DoorLock_1						PBout(12)
#define Pin_DoorLock_2               	 	PBout(13)
#define Pin_Light_1                			PBout(14)
#define Pin_Light_2							PBout(15)

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
