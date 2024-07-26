#include "My_include.h"





void keep_HeartBeat(void){
	// ·¢ÐÄÌø»ØÖ´
	char cmd[512];
	// AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
    sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", MQTTCONN_Topic_4, MQTTCONN_Msg_3_HeartBeat);
    esp8266_clear();
	USART2_SendString(cmd);
}

