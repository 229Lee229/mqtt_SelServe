#include "My_include.h"
#include "time.h"
extern volatile bool data_REC_NoPayload_Flag;
extern bool data_REC_WithPayload_Flag;		// 7/7
extern volatile char rx_buffer_esp8266[RX_BUFFER_SIZE];
extern bool json_YorN_flag;
extern u8 Json_type;

extern char JSON_parse_test_5[RX_BUFFER_SIZE];

long long Start_time_stamp;

// 非嵌套Json
void Json_parse_NoPayload(void){
		if (data_REC_NoPayload_Flag) {
			printf("%s",rx_buffer_esp8266);
			data_REC_NoPayload_Flag = false;	


			char *jsonStart = strchr((char *)rx_buffer_esp8266, '{');
			cJSON * jo = cJSON_Parse(jsonStart);
			if (jo == NULL) {			// 若解析失败 7/3
				// JSON解析失败
				// printf("JSON parse error\n");
				printf("JSON parse error: %s\n", cJSON_GetErrorPtr());
				cJSON_Delete(jo);
				json_YorN_flag = false;
				// continue;
				return;
			}else	json_YorN_flag = true;

			cJSON *type = cJSON_GetObjectItem(jo, "Type");
			if (type && json_YorN_flag) {
				printf("Type: %d\n", type->valueint);
				Json_type = type->valueint;
				
				switch(Json_type){
					case RELAY_1_ON:		
								Pin_DoorLock_2 = 0;
								Pin_Light_2	   = 0;		
								break;
					case RELAY_1_OFF:		
								Pin_DoorLock_2 = 1;
								Pin_Light_2	   = 1;		
								break;
					case SysReset:
								printf("System will reset...\r\n");
								Delay_ms(1000); // 发送完信息后延时一会儿
								NVIC_SystemReset(); // 触发软件复位
								break;
					default:				
								break;
					
				}
			}
			cJSON_Delete(jo);
	}

}

/*

{
    "Type": 11,
    "Time": 1744533467325,
    "MsgId": "4af325ke4af325ke4af325ke4af325ke",
    "SendId": "SVR01",
    "Payload": {
        "Start": 1744533467325,
        "End": 17445334883254,
        "Getter": "clientId_001"
    }
}

*/
// 嵌套Json
void Json_parse_WithPayload(void){
		char Rx_WithPayload_temp[RX_BUFFER_SIZE];
		// 打印接收到的数据 嵌套Json数据
		if(data_REC_WithPayload_Flag){
			// USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
			strcpy(Rx_WithPayload_temp,(char *)rx_buffer_esp8266); // 将结果复制到中间变量
			printf("%s\r\n",Rx_WithPayload_temp);
			data_REC_WithPayload_Flag = false;
			// USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
			
		}else return;
		
		
		// 查找Json数据开始的位置
		char *jsonStart = strchr((char *)Rx_WithPayload_temp, '{');
//		printf("---%s\r\n",jsonStart); 
//		return;
		if (jsonStart == NULL) {
			// 无法找到JSON数据的开始位置
			printf("Unable to find the starting position of the JSON data\r\n");
			
			return;
		}
	

		// 打印Json数据 7/7
//		else{
//			printf("%s\r\n",jsonStart);
//		}
		// 解析Json
		
		// 嵌套Json 加转义和引号
		add_backslashes_and_quotes(jsonStart);
		printf("\r\n%s\r\n",jsonStart);			// 测试数据 7/7
		
/****************解析嵌套JSON数据***********************/////////////////////////
		cJSON *jo = cJSON_Parse(jsonStart);
		// printf("arry:\r\n%s\r\n",JSON_parse_test_5);			// 测试数据 7/7

		if (jo == NULL) {
			// JSON解析失败
			// printf("JSON parse error\n");
			printf("JSON parse error: %s\n", cJSON_GetErrorPtr());
			cJSON_Delete(jo);		
			return;
		}else printf("Json OK\r\n");
		
		// 从JSON数据中提取信息
		cJSON *type = cJSON_GetObjectItem(jo, "Type");
		cJSON *time = cJSON_GetObjectItem(jo, "Time");
		cJSON *msgId = cJSON_GetObjectItem(jo, "MsgId");
		cJSON *sendId = cJSON_GetObjectItem(jo, "SendId");
		cJSON *payload = cJSON_GetObjectItem(jo, "Payload");
		
		
		
		if (type && time && msgId && sendId && payload) {
			printf("Type: %d\n", type->valueint);
			printf("Time: %.llf\n", time->valuedouble);  // 使用valueint或valuedouble取决于数据类型
			printf("MsgId: %s\n", msgId->valuestring);
			printf("SendId: %s\n", sendId->valuestring);
			
			char payload_json_string[256];
			sprintf(payload_json_string,"%s",payload->valuestring);
			// printf("%s\r\n",payload_json_string);		// 查看数据
		/******** 解析payload中的数据 嵌套解析 *************/

		cJSON_Delete(jo);
		cJSON *payload_json = cJSON_Parse(payload_json_string);
        if (payload_json == NULL) {
            printf("Payload JSON parse error: %s\n", cJSON_GetErrorPtr());
            cJSON_Delete(payload_json);
            return;
        }else printf("Payload JSON OK!\r\n");		
		
		
		cJSON *Start  = cJSON_GetObjectItem(payload_json, "Start");
		cJSON *End 	  = cJSON_GetObjectItem(payload_json, "End");
		cJSON *Getter = cJSON_GetObjectItem(payload_json, "Getter");
        if (Start && End && Getter) {
            printf("Start: %.llf\n", Start->valuedouble);// 原  %s  valuestring 7/1 
			printf("End:%.llf\n",	 End  ->valuedouble);
			printf("Getter:%s\n",	 Getter->valuestring);
        }else{				// test 解析错误 7/2
			printf("Start error!\r\n");	
		}
		
		/*********************/
	
		// 解析time  7/7
		Start_time_stamp = Start->valuedouble;
		printf("Start_time_stamp: %lld\r\n",Start_time_stamp);
		// 转化为秒
//		time_t seconds = Start_time_stamp / 1000;
		// 转化为时间结构体
//		struct tm *timeinfo;
//		timeinfo = gmtime(&seconds);
		// 以上为解析time
		
/********************************************************************/
		// 释放JSON对象	
			 cJSON_Delete(payload_json);		
		}
}

/*

// 
{
    "Type": 11,
    "Time": 1744533467325,
    "MsgId": "4af325ke4af325ke4af325ke4af325ke",
    "SendId": "SVR01",
    "Payload": {
        "Start": 1744533467325,
        "End": 17445334883254,
        "Getter": "clientId_001"
    }
}

*/
