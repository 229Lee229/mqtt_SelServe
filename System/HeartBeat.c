#include "My_include.h"





void keep_HeartBeat(void){
	// 发心跳回执
	char cmd[512];
	// AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
    sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", MQTTCONN_Topic_3, MQTTCONN_Msg_4_HeartBeat);
    esp8266_clear();
	// USART2_SendString(cmd);
	
	u8 HeartBeat_Send_Timeout = 3;
	
	while(HeartBeat_Send_Timeout--){
		// 发消息
		if(esp8266_at_MQTTPUB(MQTTCONN_Topic_3,MQTTCONN_Msg_4_HeartBeat) == 1)			// 返回值已改成动态奇切换中断 6/30
			printf("MQTTPUB_Heart Send Error!\r\n");
		else{
			// USART2_IRQn_EnableSel = false;
			// USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;				// 中断函数指针切换
			printf("MQTTPUB_Heart Send Successful!\r\n");
			break;
		}
	}
	
	
	
	
	
	
	
	
	
}

