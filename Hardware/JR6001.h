
#ifndef __JR6001_H
#define __JR6001_H

#include "My_include.h"


#define UART3_JR6001                USART3
#define PIN_JR6001_BUSY             PA4

#define CMD_JR6001_PLAY             "A2"//播放
#define CMD_JR6001_PAUSE            "A3"//暂停
#define CMD_JR6001_STOP             "A4"//停止
#define CMD_JR6001_LAST             "A5"//上一曲
#define CMD_JR6001_NEXT             "A6"//下一曲
#define CMD_JR6001_SELECT           "A7:"//指定文件名播放
#define CMD_JR6001_SELECT_FLASH     "A8:02"//指定FLASH内路径和文件名播放
#define CMD_JR6001_SELECT_TF        "A8:01"//指定TF卡内路径和文件名播放
#define CMD_JR6001_SELECT_UDISK     "A8:00"//指定U盘内路径和文件名播放
#define CMD_JR6001_END_PLAY			"AE"	// 结束播放
#define CMD_JR6001_VOLUME_SET       "AF:"//设置音量等级
#define CMD_JR6001_VOLUME_UP      	"B0"//音量加
#define CMD_JR6001_VOLUME_DOWN      "B1"//音量减
#define CMD_JR6001_MODE_SET         "B4:"//设置播放模式
#define CMD_JR6001_PLAY_GROUP       "B7:"//组合播放 ZH 文件夹内的文件
#define CMD_JR6001_STOP_GROUP       "B8"//组合播放 ZH 文件夹内的文件

#define JR6001_SCAN_QR				"A7:00014\r\n"		/* 请扫描二维码 */

#define JR6001_Single_Stop 			"B4:02\r\n"			/* 单曲停止 */
#define JR6001_VOLUME_MAX			"AF:30\r\n"			/* 音量设置 */
#define JR6001_VOLUME_MID			"AF:20\r\n"
#define JR6001_VOLUME_MIN			"AF:10\r\n"
#define JR6001_Service_finish		"A7:00001\r\n"		/* x号房间，服务时间已到，请收拾好物品，欢迎下次光临 */
#define JR6001_left_3min			"A7:00002\r\n"		/* x号房间，剩余三分钟，请及时续费 */
#define JR6001_left_15min			"A7:00003\r\n"	    /* x号房间，剩余十五分钟，请及时续费 */
typedef enum
{
	JR6001_FULL_CYCLE = 0,		// 全盘循环
    JR6001_CYCLE_SINGEL=1,		// 单曲循环
    JsR6001_ONCE_SINGEL,		// 单曲停止 单曲播放一次
    JR6001_RANDOM_ALL,			// 全盘随机
	JR6001_ORDER_ALL,			// 目录循环 全盘顺序播放
    JR6001_RANDOM_DIR,			// 目录随机 目录内随机 不含子目录
    JsR6001_ORDER_DIR,			// 目录顺序播放 播放完后停止 目录内顺序播放
	JsR6001_ORDER_ALL,			// 全盘播放 播放完后停止 


}PlayMode_JR6001;//播放模式

#define JR6001_SEND_CMD(n)      					 USART_SendString(UART3_JR6001,n)
#define JR6001_Play_Service_Finish() 				 JR6001_SEND_CMD(JR6001_Service_finish)
#define JR6001_Play_left_3min() 					 JR6001_SEND_CMD(JR6001_left_3min)
#define JR6001_Play_left_15min() 					 JR6001_SEND_CMD(JR6001_left_15min)
#define JR6001_Set_Single_Stop()					 JR6001_SEND_CMD(JR6001_Single_Stop)
// #define JR6001_Stop()            					 JR6001_SEND_CMD(CMD_JR6001_STOP)


//#define JR6001_SEND_CMD(n)       USART_SendString(UART1_JR6001,n)
//#define JR6001_Play()            JR6001_SEND_CMD(CMD_JR6001_PLAY)
//#define JR6001_Stop()            JR6001_SEND_CMD(CMD_JR6001_STOP)
//#define JR6001_Pause()           JR6001_SEND_CMD(CMD_JR6001_PAUSE)
//#define JR6001_LAST()        	 JR6001_SEND_CMD(CMD_JR6001_LAST)
//#define JR6001_Next()            JR6001_SEND_CMD(CMD_JR6001_NEXT)
//#define JR6001_VolumeUp()        JR6001_SEND_CMD(CMD_JR6001_VOLUME_UP)
//#define JR6001_VolumeDown()      JR6001_SEND_CMD(CMD_JR6001_VOLUME_DOWN)
//#define JR6001_StopGroup()       JR6001_SEND_CMD(CMD_JR6001_STOP_GROUP)

// char* My_JR6001_select_order(u8 order);
void My_JR6001_Init(void);//初始化
void My_JR6001_SetVolume(u8 level);
bool My_JR6001_IsBusy(void);
void JR6001_Usart3_Init(u32 bound);
#endif


