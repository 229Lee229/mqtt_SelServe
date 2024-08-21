#ifndef __MY_INCLUDE_H
#define __MY_INCLUDE_H


#define  uchar unsigned char

#include "stm32f10x.h"                  // Device header
#include "stdbool.h"

#include "user.h"
#include "stdlib.h"
#include "cJSON.h"
// #include "cJSON2.h"
#include "string.h"
#include "relay5V.h"

#include "OLED.h"
#include "Delay.h"
#include "Matrix_Key2.h"
#include "MyGPIO.h"
#include "usart.h"
#include "SysBitBand.h"

#include "esp8266.h"
#include "mqtt_esp8266.h"
#include "main.h"

#include "EspRst.h"

#include "Rec_Json_parse.h"
#include "ConvertJsonTime.h"
#include "DS1302.h"

#include "HeartBeat.h"
#include "TIM_HeartBeat.h"
#include "switchCtrlRelay.h"
#include "timer_doorLock.h"

#include "JR6001.h"
#include "TIM_CheckConn.h"


// --- W25Q64
#include "W25Q64.h"
#endif
