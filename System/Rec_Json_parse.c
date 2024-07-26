#include "My_include.h"
#include "time.h"
extern volatile bool data_REC_NoPayload_Flag;
extern bool data_REC_WithPayload_Flag;		// 7/7
extern uchar rx_buffer_esp8266[RX_BUFFER_SIZE];// 7/10
// extern uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];// 7/10			待优化
extern bool json_YorN_flag;
extern u8 Json_type;
char Rx_WithPayload_temp[RX_BUFFER_SIZE];
extern char JSON_parse_test_5[RX_BUFFER_SIZE];

// extern char *a_test;
extern uchar time_Current[6];			// 7/12		记录实时时间
bool BackSound_Flag = false;
//long long Start_time_stamp;
//long long End_time_stamp;
// 非嵌套Json
#ifdef use_Json_NoPayload
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

#endif
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

void Json_parse_WithPayload2(void){
		int i = 0;
		//int BackSound_cnt = 0;
		BackSound_Flag = false;
		printf("\r\n%s\r\n",Rx_WithPayload_temp);
		char *jsonStart = strchr((char *)Rx_WithPayload_temp, '{');
	
		// -----
			uchar json_temp[RX_BUFFER_SIZE];

		strcpy((char *)json_temp,(char *)jsonStart); // 将结果复制到中间变量	

		do{
			jsonStart++;
		}while(0);
		// strcpy((char *)json_temp,(char *)jsonStart); // 将结果复制到中间变量

		// 判断是否是回声数据
		for(i = 0;jsonStart[i] != '{';i++){
			if(jsonStart[i] == '}'){
				BackSound_Flag = true;
				break;
			}
		}
			
// 是回声
if(BackSound_Flag == true)
{
	
#ifdef DEBUG_1
	printf("It is back sound\r\n");
#endif	
		BackSound_Flag = false;
		jsonStart = strchr((char *)jsonStart,'{');		// 查找下一个回声数据 
		jsonStart++;
		jsonStart = strchr((char *)jsonStart,'{');		// 查找下一个回声数据 
		strcpy((char *)json_temp,(char *)jsonStart); // 将结果复制到中间变量
		Json_parse_WithPayload_input((char *)json_temp);
		return;
}

// 不是回声
else if(BackSound_Flag == false)
{
	printf("it isnot backsound\r\n");
	// char *jsonStart_Payload = strchr((char *)jsonStart, '{');
	// char *jsonStart_Payload;
	// strcpy((char *)json_temp,(char *)jsonStart); // 将结果复制到中间变量	

		
		// printf("\r\n%s\r\n",json_temp);
		
		// 嵌套Json 加转义和引号
		add_backslashes_and_quotes((char *)json_temp);
		printf("\r\n%s\r\n",json_temp);			// 测试数据 7/7
	// return;
/****************解析嵌套JSON数据***********************/////////////////////////
		cJSON *jo = cJSON_Parse((char *)json_temp);
		// printf("arry:\r\n%s\r\n",JSON_parse_test_5);			// 测试数据 7/7

		if (jo == NULL) {
			// JSON解析失败
			printf("JSON parse error: %s\n", cJSON_GetErrorPtr());
			cJSON_Delete(jo);				// 回声会被返回
			memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));
			return;
		}else printf("Json OK\r\n");
		
		// 从JSON数据中提取信息
		cJSON *type = cJSON_GetObjectItem(jo, "Type");
		cJSON *time = cJSON_GetObjectItem(jo, "Time");
		cJSON *msgId = cJSON_GetObjectItem(jo, "MsgId");
		cJSON *sendId = cJSON_GetObjectItem(jo, "SendId");
		cJSON *payload = cJSON_GetObjectItem(jo, "Payload");
		
		
		
		if (type && time && msgId && sendId && payload){
			printf("Type: %d\n", type->valueint);
			printf("Time: %.llf\n", time->valuedouble);  // 使用valueint或valuedouble取决于数据类型
			printf("MsgId: %s\n", msgId->valuestring);
			printf("SendId: %s\n", sendId->valuestring);
			
			char payload_json_string[256];
			sprintf(payload_json_string,"%s",payload->valuestring);
			long long Current_time_stamp;
				switch(type->valueint){
										/* 增加心跳检测数据  */
					case HeartBeat_Syn:
								printf("HeartBeat Syn Successfcul!\r\n");
								
								
								Current_time_stamp = time->valuedouble;
								ConvMillisToDateTime_C(Current_time_stamp);		// 更新时钟
								cJSON_Delete(jo);	
								memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));					
								return;
								// break;
					case RELAY_AllOn_EXC_1:			// 全部上电，除去一号（门磁）
								Relay_1_Pin_DoorLock = PwrLoss_OpenDoor;
								Relay_2_Pin 		 = Machine_PwrOn_Relay;
								Relay_3_Pin 		 = Machine_PwrOn_Relay;
								Relay_4_Pin 		 = Machine_PwrOn_Relay;
								break;
					case RELAY_1_ON:		
								break;
					case RELAY_1_OFF:		
						
								break;
					case SysReset:
								printf("System will reset...\r\n");
								Delay_ms(1000); // 发送完信息后延时一会儿
								NVIC_SystemReset(); // 触发软件复位
								break;
					
					case 0:		// 回声 不处理
								printf("It is BackSound2!\r\n");
								cJSON_Delete(jo);
								return;
					default:
								cJSON_Delete(jo);
								memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));	
								break;
					
				}
		/*** 以上为发送心跳回执 7/9 */
			
			
			
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
		long long End_time_stamp;
		// 解析time  7/7
		End_time_stamp   = End  ->valuedouble;
		ConvMillisToDateTime_E(End_time_stamp);		
		
/********************************************************************/
		// 释放JSON对象	
		cJSON_Delete(payload_json);	
		memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));		
		}else cJSON_Delete(jo);
	
	
	
	return;
}



	
}

static void Json_parse_WithPayload_input(char * WithPayload_temp_input){
		// char Rx_WithPayload_temp[RX_BUFFER_SIZE];
		// 打印接收到的数据 嵌套Json数据
#ifdef parse_WithPayload_1	
		if(data_REC_WithPayload_Flag){
			printf("\r\ntemp:\r\n");
			printf("%s\r\n",Rx_WithPayload_temp);
			data_REC_WithPayload_Flag = false;
		}else {
			memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));	
			return;
		}
		
#endif		
		// 查找Json数据开始的位置
		char *jsonStart = strchr((char *)WithPayload_temp_input, '{');
		if (jsonStart == NULL) {
			// 无法找到JSON数据的开始位置
			printf("Unable to find the starting position of the JSON data\r\n");
			return;
		}
		else printf("Json Start Successful!\r\n");		// test HeartBeat 7/11
		// return;

			
		
		
		// 打印Json数据 7/7
//		else{
			printf("\r\n%s\r\n",jsonStart);
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
			cJSON_Delete(jo);				// 回声会被返回
			// memset(Rx_WithPayload_temp, 0, sizeof(WithPayload_temp_input));
			return;
		}else printf("Json OK\r\n");
		
		// 从JSON数据中提取信息
		cJSON *type = cJSON_GetObjectItem(jo, "Type");
		cJSON *time = cJSON_GetObjectItem(jo, "Time");
		cJSON *msgId = cJSON_GetObjectItem(jo, "MsgId");
		cJSON *sendId = cJSON_GetObjectItem(jo, "SendId");
		cJSON *payload = cJSON_GetObjectItem(jo, "Payload");
		
			// test
//			printf("Type: %d\n", type->valueint);
//			printf("Time: %.llf\n", time->valuedouble);  // 使用valueint或valuedouble取决于数据类型
//			printf("MsgId: %s\n", msgId->valuestring);
//			printf("SendId: %s\n", sendId->valuestring);

		
		if (type && time && msgId && sendId && payload) {
			printf("Type: %d\n", type->valueint);
			printf("Time: %.llf\n", time->valuedouble);  // 使用valueint或valuedouble取决于数据类型
			printf("MsgId: %s\n", msgId->valuestring);
			printf("SendId: %s\n", sendId->valuestring);
			
			char payload_json_string[256];
			sprintf(payload_json_string,"%s",payload->valuestring);
			// printf("%s\r\n",payload_json_string);		// 查看数据
		/******** 解析payload中的数据 嵌套解析 *************/
		
		/*** 7/9 定时开门 检测Start与end */ 
			// Json_type = type->valueint;
			long long Current_time_stamp;
				switch(type->valueint){
										/* 增加心跳检测数据  */
					case HeartBeat_Syn:
								printf("HeartBeat Syn Successfcul!\r\n");
								
								
								Current_time_stamp = time->valuedouble;
								ConvMillisToDateTime_C(Current_time_stamp);		// 更新时钟
								cJSON_Delete(jo);	
								// memset(WithPayload_temp_input, 0, (char *)sizeof(WithPayload_temp_input));					
								return;
								// break;
					case RELAY_AllOn_EXC_1:			// 全部上电，除去一号（门磁）
								Relay_1_Pin_DoorLock = PwrLoss_OpenDoor;
								Relay_2_Pin 		 = Machine_PwrOn_Relay;
								Relay_3_Pin 		 = Machine_PwrOn_Relay;
								Relay_4_Pin 		 = Machine_PwrOn_Relay;
					
					case RELAY_1_ON:		
								break;
					case RELAY_1_OFF:		
						
								break;
					case SysReset:
								printf("System will reset...\r\n");
								Delay_ms(200); // 发送完信息后延时一会儿
								NVIC_SystemReset(); // 触发软件复位
								break;
					
					case 0:		// 回声 不处理
								printf("It is BackSound2!\r\n");
								cJSON_Delete(jo);
								return;
					default:
								cJSON_Delete(jo);
								// memset(WithPayload_temp_input, 0, sizeof(WithPayload_temp_input));	
								break;
					
				}
		/*** 以上为发送心跳回执 7/9 */
			
			
			
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
		long long End_time_stamp;
		// 解析time  7/7
//		Start_time_stamp = Start->valuedouble;
		End_time_stamp   = End  ->valuedouble;
//		printf("Start_time_stamp: %lld\r\n",Start_time_stamp);
//		ConvMillisToDateTime_S(Start_time_stamp);
		ConvMillisToDateTime_E(End_time_stamp);		
		// 转化为秒
//		time_t seconds = Start_time_stamp / 1000;
		// 转化为时间结构体
//		struct tm *timeinfo;
//		timeinfo = gmtime(&seconds);
		// 以上为解析time
		
/********************************************************************/
		// 释放JSON对象	
		cJSON_Delete(payload_json);	
		// memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));		
		}else cJSON_Delete(jo);
}

// 嵌套Json
void Json_parse_WithPayload(void){
		// char Rx_WithPayload_temp[RX_BUFFER_SIZE];
		// 打印接收到的数据 嵌套Json数据
#ifdef parse_WithPayload_1	
		if(data_REC_WithPayload_Flag){
			// strcpy(Rx_WithPayload_temp,(char *)rx_buffer_esp8266); // 将结果复制到中间变量   结果已在中断中赋值给此变量 7/11
			printf("\r\ntemp:\r\n");
			printf("%s\r\n",Rx_WithPayload_temp);
			data_REC_WithPayload_Flag = false;
			
			// ----------------		
//			if(strstr((const char *)Rx_WithPayload_temp, "OK") == NULL){
//				;
//			}else{
//				uchar *array = (uchar *)malloc(RX_BUFFER_SIZE* sizeof(uchar));
//				array = (uchar *)(strstr((const char *)Rx_WithPayload_temp, "OK"));				
//				array++;
//				array++;
//				printf("array:\r\n%s\r\n",array);
//				processSecondGroupData(array);
//				free(array);
//				array = NULL; // 将指针设置为NULL，防止悬空指针
//				return;
//			}
			
			// -------------
			
			
			
			
			// memset(g_uart_rx_buf, 0, sizeof(g_uart_rx_buf));
			// memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));

			// esp8266_clear();			// 清除串口缓存数据	 7/10
//			printf("\r\n");
//			printf("rx_buffer_esp8266 after:\r\n");
//			printf("%s\r\n",rx_buffer_esp8266);
//			printf("\r\n");
		}else {
			memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));	
			return;
		}
		
#endif		
		// 查找Json数据开始的位置
		char *jsonStart = strchr((char *)Rx_WithPayload_temp, '{');
		
		// 清理Rx_WithPayload_temp 数组清理		7/10
		// memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));
		// printf("Rx_WithPayload_temp after:\r\n%s\r\nRx_WithPayload_tempend\r\n",Rx_WithPayload_temp);
		//////////////////////////////////////////////////////// Rx_WithPayload_temp 数组清理	
		// printf("---%s\r\n",jsonStart); 
//		return;
		if (jsonStart == NULL) {
			// 无法找到JSON数据的开始位置
			printf("Unable to find the starting position of the JSON data\r\n");
			return;
		}
		else printf("Json Start Successful!\r\n");		// test HeartBeat 7/11
		// return;

			
		
		
		// 打印Json数据 7/7
//		else{
			printf("\r\n%s\r\n",jsonStart);
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
			cJSON_Delete(jo);				// 回声会被返回
			memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));
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
		
		/*** 7/9 定时开门 检测Start与end */ 
			// Json_type = type->valueint;
			long long Current_time_stamp;
				switch(type->valueint){
										/* 增加心跳检测数据  */
					case HeartBeat_Syn:
								printf("HeartBeat Syn Successfcul!\r\n");
								
								
								Current_time_stamp = time->valuedouble;
								ConvMillisToDateTime_C(Current_time_stamp);		// 更新时钟
								cJSON_Delete(jo);	
								memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));					
								return;
								// break;
					case RELAY_AllOn_EXC_1:			// 全部上电，除去一号（门磁）
								Relay_1_Pin_DoorLock = PwrLoss_OpenDoor;
								Relay_2_Pin 		 = Machine_PwrOn_Relay;
								Relay_3_Pin 		 = Machine_PwrOn_Relay;
								Relay_4_Pin 		 = Machine_PwrOn_Relay;
					
					case RELAY_1_ON:		
								break;
					case RELAY_1_OFF:		
						
								break;
					case SysReset:
								printf("System will reset...\r\n");
								Delay_ms(200); // 发送完信息后延时一会儿
								NVIC_SystemReset(); // 触发软件复位
								break;
					
					case 0:		// 回声 不处理
								printf("It is BackSound2!\r\n");
								cJSON_Delete(jo);
								return;
					default:
								cJSON_Delete(jo);
								memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));	
								break;
					
				}
		/*** 以上为发送心跳回执 7/9 */
			
			
			
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
		long long End_time_stamp;
		// 解析time  7/7
//		Start_time_stamp = Start->valuedouble;
		End_time_stamp   = End  ->valuedouble;
//		printf("Start_time_stamp: %lld\r\n",Start_time_stamp);
//		ConvMillisToDateTime_S(Start_time_stamp);
		ConvMillisToDateTime_E(End_time_stamp);		
		// 转化为秒
//		time_t seconds = Start_time_stamp / 1000;
		// 转化为时间结构体
//		struct tm *timeinfo;
//		timeinfo = gmtime(&seconds);
		// 以上为解析time
		
/********************************************************************/
		// 释放JSON对象	
		cJSON_Delete(payload_json);	
		memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));		
		}else cJSON_Delete(jo);
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
