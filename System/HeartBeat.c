#include "My_include.h"





void keep_HeartBeat(void){
	// 发心跳回执
	char cmd[512];
	// AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
    sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", MQTTCONN_Topic_3, MQTTCONN_Msg_3_HeartBeat);
    esp8266_clear();
	
	
	USART2_SendString(cmd);
//	u8 HeartBeat_Send_Timeout = 1;
//	
//	while(HeartBeat_Send_Timeout--){
		// 发消息
		
		// 未正确返回应答值 可能因为接收中断中的数组	
//		if(esp8266_send_command_HeartBeat(cmd, "OK") == 1){
//			printf("MQTTPUB_Heart Send Error!\r\n");
//		}else{
//			printf("MQTTPUB_Heart Send Successful!\r\n");
//			break;			
//		}

		
//		if(esp8266_at_MQTTPUB(MQTTCONN_Topic_3,MQTTCONN_Msg_3_HeartBeat) == 1)
//			printf("MQTTPUB_Heart Send Error!\r\n");			// 因为接收中断
//		else{
//			printf("MQTTPUB_Heart Send Successful!\r\n");
//			break;
//		}
//	}
}

