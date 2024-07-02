#include "My_include.h"

char rx_buffer_5[RX_BUFFER_SIZE] = "{\"CtxId\":\"7f91c5a9f0879994d95a42e919b574af\"}";
extern char rx_buffer_2[RX_BUFFER_SIZE]; 
extern char rx_buffer_3[RX_BUFFER_SIZE];
extern uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
extern uint8_t g_uart_tx_buf[ESP8266_UART_TX_BUF_SIZE];
extern bool USART2_IRQn_EnableSel;

bool MQTTPUB_FLAG = false;

void printJsonError(const char *jsonStart);
void MQTT_Init(void){
	
	u8 TimeOut_mqtt;
	TimeOut_mqtt = 3;
	// mqtt Init
	// 1 - AT+MQTTUSERCFG：设置 MQTT 用户属性
	while(TimeOut_mqtt--){
		if(esp8266_at_MQTTUSERCFG(MQTTUSERCFG_MyConf_3) == 1)
			printf("MQTTUSERCFG Error!\r\n");
		else{
			printf("MQTTUSERCFG Successful!\r\n");
			break;
		}
		Delay_ms(1);
	}
	
	TimeOut_mqtt = 3;
//	// 1-2 AT+MQTTLONGCLIENTID：设置 MQTT 客户端 ID
//	while(TimeOut_mqtt--){
//		if(esp8266_at_MQTTLONGCLIENTID(LinkID,MQTTLONGCLIENTID_MyConf) == 1)
//			printf("MQTTLONGCLIENTID Error!\r\n");
//		else{
//			printf("MQTTLONGCLIENTID Successful!\r\n");
//			break;
//		}
//	}	
//	while(TimeOut_mqtt--){
//		if(esp8266_at_MQTTLONGCLIENTID(MQTTLONGCLIENTID_MyConf) == 1)
//			printf("MQTTLONGCLIENTID Error!\r\n");
//		else{
//			printf("MQTTLONGCLIENTID Successful!\r\n");
//			break;
//		}
//		Delay_ms(1);
//	}
//	
	// 1-2 AT+MQTTLONGCLIENTID：设置 MQTT 客户端 ID
//	while(TimeOut_mqtt--){
//		if(esp8266_at_MQTTLONGCLIENTID_1() == 1)
//			printf("MQTTLONGCLIENTID Error!\r\n");
//		else{
//			printf("MQTTLONGCLIENTID Successful!\r\n");
//			break;
//		}
//		Delay_ms(1);
//	}
//	
	TimeOut_mqtt = 5;
	while(TimeOut_mqtt--){

		// 2 - AT+MQTTCONNCFG：设置 MQTT 连接属性
		if(esp8266_at_MQTTCONNCFG(MQTTCONNCFG_MyConf) == 1)
			printf("MQTTCONNCFG Error!\r\n");
		else{
			printf("MQTTCONNCFG Successful!\r\n");
			break;
		}
		Delay_ms(1);
	}	
	
	TimeOut_mqtt = 5;
	// 3 - AT+MQTTCONN：连接 MQTT Broker
	while(TimeOut_mqtt--){
		if(esp8266_at_MQTTCONN(MQTTCONN_Host_MyConf_PUB,MQTTCONN_Port_MyConf) == 1)
			printf("MQTTCONN Error!\r\n");
		else {
			printf("MQTTCONN Successful!\r\n");
			break;
		}
		Delay_ms(20);
	}
	
	
	// 4 - AT+MQTTSUB=<LinkID>,<"topic">,<qos>
	Delay_ms(2);
	TimeOut_mqtt = 3;
	while(TimeOut_mqtt--){
		if(esp8266_at_MQTTSUB(MQTTCONN_Topic_4) == 1)
			printf("MQTTSUB Error!\r\n");
		else{
			printf("MQTTSUB Successful!\r\n");
			break;
		}
	}
	
	Delay_ms(2);
	
	MQTTPUB_FLAG = true;
	TimeOut_mqtt = 3;
	while(TimeOut_mqtt--){
		// 发消息
		if(esp8266_at_MQTTPUB(MQTTCONN_Topic_4,MQTTCONN_Message_3) == 1)			// 返回值已改成动态奇切换中断 6/30
			printf("MQTTPUB Send Error!\r\n");
		else{
			// USART2_IRQn_EnableSel = false;
			// USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;				// 中断函数指针切换
			printf("MQTTPUB Send Successful!\r\n");
			break;
		}
		Delay_ms(10);		
	}
}
//void MQTT_Init(void){
//	
//	u8 TimeOut_mqtt = 5;
//	
//	// mqtt Init
//	// 1 - AT+MQTTUSERCFG：设置 MQTT 用户属性
//	if(esp8266_at_MQTTUSERCFG(MQTTUSERCFG_MyConf) == 1)
//		printf("MQTTUSERCFG Error!\r\n");
//	else printf("MQTTUSERCFG Successful!\r\n");
//	Delay_ms(1);
//	
//	// 2 - AT+MQTTCONNCFG：设置 MQTT 连接属性
//	if(esp8266_at_MQTTCONNCFG(MQTTCONNCFG_MyConf) == 1)
//		printf("MQTTCONNCFG Error!\r\n");
//	else printf("MQTTCONNCFG Successful!\r\n");
//	Delay_ms(1);

//	// 3 - AT+MQTTCONN：连接 MQTT Broker
//	if(esp8266_at_MQTTCONN(MQTTCONN_Host_MyConf,MQTTCONN_Port_MyConf) == 1)
//		printf("MQTTCONN Error!\r\n");
//	else printf("MQTTCONN Successful!\r\n");
//	Delay_ms(1);

//	
//}

// MQTT Functions Implementation
uint8_t esp8266_send_command_expect(char *cmd, char *expect, char *res)
{
    uint8_t timeOut = 250;

    esp8266_clear();
    USART2_SendString(cmd);
    while (timeOut--) {
        if (esp8266_wait_receive() == ESP8266_EOK) {
            if (strstr((const char *)g_uart_rx_buf, expect) != NULL) {
                USART2_SendString(res);
                while (timeOut--) {
                    if (esp8266_wait_receive() == ESP8266_EOK) {
                        if (strstr((const char *)g_uart_rx_buf, "OK") != NULL)
                            return ESP8266_EOK;
                    }
                    Delay_ms(10);
                }
            }
        }
        Delay_ms(10);
    }
    return ESP8266_ERROR;
}
uint8_t esp8266_at_MQTTUSERCFG(char *config)
{
    char cmd[516];
	// AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
    sprintf(cmd, "AT+MQTTUSERCFG=%s\r\n", config);		
    return esp8266_send_command(cmd, "OK");
}
uint8_t esp8266_at_MQTTLONGCLIENTID(uint16_t length, char *client_id)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGCLIENTID=0,%d\r\n", length);
    return esp8266_send_command_expect(cmd, ">", client_id);
}
uint8_t esp8266_at_MQTTLONGCLIENTID_1(void)
{
    char cmd[128];
	// AT+MQTTLONGCLIENTID=<LinkID>,<length>
    sprintf(cmd, "AT+MQTTLONGCLIENTID=%d,%d\r\n", LinkID, ClientID_Length);
	printf("%s",cmd);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGCLIENTID_2(char *client_id)
{
    char cmd[256];
	// AT+MQTTLONGCLIENTID=<LinkID>,<length>
    sprintf(cmd, "AT+MQTTLONGCLIENTID=%s,%d\r\n", client_id, ClientID_Length);
	printf("%s",cmd);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGUSERNAME(char *username)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGUSERNAME=%s\r\n", username);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGPASSWORD(char *password)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGPASSWORD=%s\r\n", password);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCONNCFG(char *config)
{
    char cmd[128];
	// AT+MQTTCONNCFG=<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>
    sprintf(cmd, "AT+MQTTCONNCFG=%s\r\n", config);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCONN(char *host, int port)
{
    char cmd[128];
	// AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
    sprintf(cmd, "AT+MQTTCONN=0,\"%s\",%d,0\r\n", host, port);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTPUB(char *topic, char *message)
{
    char cmd[512];
	// AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
    sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", topic, message);
	// printf("Debug test:%s",cmd);			// 打印数据
	// USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTPUBRAW(char *topic, uint8_t *data, uint16_t len)
{
    char cmd[256];
    sprintf(cmd, "AT+MQTTPUBRAW=\"%s\",", topic);
    for (int i = 0; i < len; i++) {
        char byte[4];
        sprintf(byte, "%02X", data[i]);
        strcat(cmd, byte);
    }
    strcat(cmd, "\r\n");
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTSUB(char *topic)
{
    char cmd[128];
	// AT+MQTTSUB=<LinkID>,<"topic">,<qos>
    sprintf(cmd, "AT+MQTTSUB=0,\"%s\",0\r\n", topic);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTUNSUB(char *topic)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTUNSUB=\"%s\"\r\n", topic);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCLEAN(void)
{
    return esp8266_send_command("AT+MQTTCLEAN\r\n", "OK");
}

/*  回执数据 
+MQTTSUBRECV:0,"1111119/1111119_L16LF3HR",94,{"Type":0,"Time":12787,"MsgId":"7f91c5a9f0879994d95a42e919b574af","SendId":"2222227_U1Q4B2Z5"}
+MQTTSUBRECV:0,"1111119/1111119_L16LF3HR",150,{"Type":4,"Time":1719746116700,"MsgId":"96c158a3-1a72-4604-b699-4487b58a28b8","SendId":"SVR01","Payload":{"CtxId":"7f91c5a9f0879994d95a42e919b574af"}}

*/
/*************************************** JSON处理数据 ***********************************************************/

void processSecondGroupData(char *data) {
    // 查找第一组数据的结束位置
    char *firstGroupEnd = strstr(data, "}"); // \n
    if (firstGroupEnd == NULL) {
        // 无法找到第一组数据的结束位置
		printf("Unable to find the end position of the first set of data\r\n");
        return;
    }
    
    // 移动到第二组数据的开始位置
    char *secondGroupStart = firstGroupEnd + 1/* 2 */;		// 少了 '\n' 6/30
    
    // 查找第二组JSON数据的开始位置
    char *jsonStart = strchr(secondGroupStart, '{');
    if (jsonStart == NULL) {
        // 无法找到第二组JSON数据的开始位置
		printf("Unable to find the starting position of the second set of JSON data\r\n");
		
        return;
    }
	
	// 7/1 加转义
    add_backslashes_and_quotes(jsonStart);
	printf("\r\n%s\r\n",jsonStart);
	// {"Type":4,"Time":1719746116700,"MsgId":"96c158a3-1a72-4604-b699-4487b58a28b8","SendId":"SVR01","Payload":{"CtxId":"7f91c5a9f0879994d95a42e919b574af"}}
/***********************************************************************/	
	
	
/************************打印提取的数据 6/30 **************************/	
	
	
	
	
	
/***********************************************************************/	
	
    // 解析JSON数据
    cJSON *root = cJSON_Parse(jsonStart);
    if (root == NULL) {
        // JSON解析失败
        // printf("JSON parse error\n");
		printf("JSON parse error: %s\n", cJSON_GetErrorPtr());
        return;
    }
    
    // 从JSON数据中提取信息
    cJSON *type = cJSON_GetObjectItem(root, "Type");
    cJSON *time = cJSON_GetObjectItem(root, "Time");
    cJSON *msgId = cJSON_GetObjectItem(root, "MsgId");
    cJSON *sendId = cJSON_GetObjectItem(root, "SendId");
    cJSON *payload = cJSON_GetObjectItem(root, "Payload");
    
    if (type && time && msgId && sendId && payload) {
        printf("Type: %d\n", type->valueint);
        printf("Time: %.llf\n", time->valuedouble);  // 使用valueint或valuedouble取决于数据类型
        printf("MsgId: %s\n", msgId->valuestring);
        printf("SendId: %s\n", sendId->valuestring);


		char payload_json_string[256];

		sprintf(payload_json_string,"%s",payload->valuestring);
		// printf("%s\r\n",payload_json_string);
		/******** 解析payload中的数据 嵌套解析 *************/
        // 解析嵌套的payload JSON字符串
		//sprintf(payload_json_string,"%s",payload->valuestring);
		// strcpy(payload_json_string, payload->valuestring); // 将结果复制回原字符串
		cJSON_Delete(root);
		cJSON *payload_json = cJSON_Parse(payload_json_string);
//		return;
        if (payload_json == NULL) {
            printf("Payload JSON parse error: %s\n", cJSON_GetErrorPtr());
            cJSON_Delete(payload_json);
            return;
        }		
		
		/*********************/
		
        cJSON *ctxId = cJSON_GetObjectItem(payload_json, "CtxId");
        if (ctxId) {
            printf("CtxId: %s\n", ctxId->valuestring);// 原  %s  valuestring 7/1 
        }else{				// test 解析错误 7/2
			printf("Ctxld error!\r\n");	
		}
		// 7/1 新增payload 第二组数据作测试
//		cJSON *ctxId2 = cJSON_GetObjectItem(payload, "CtxId2");
//        if (ctxId2) {
//            printf("CtxId2: %d\n", ctxId2->valueint);// 原  %s  valuestring 7/1 
//        }
		 // 释放JSON对象
		 cJSON_Delete(payload_json);

    }
    
	printf("\r\nParsing completed!\r\n");				//  7/1
}

// 打印错误信息
void printJsonError(const char *jsonStart) {
    const char *errorPtr = cJSON_GetErrorPtr();
    if (errorPtr != NULL) {
        int pos = errorPtr - jsonStart;
        printf("JSON parse error before: %s\n", errorPtr);
        printf("Error occurred at position %d\n", pos);
    } else {
        printf("JSON parse error, but no error pointer returned\n");
    }
}



// 加转义  7/1
void add_backslashes_and_quotes(char *str) {
    char result[1024] = {0}; // 确保有足够的空间存储结果
    int len = strlen(str);
    int res_index = 0;
    int curly_braces_count = 0;
    int inside_second_braces = 0;
//    int second_brace_found = 0;
//    int third_brace_found = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] == '{') {
            curly_braces_count++;
            if (curly_braces_count == 2) {
                inside_second_braces = 1;
               //  second_brace_found = 1;
                result[res_index++] = '\"'; // 在第二个 '{' 前加双引号
            }
        } else if (str[i] == '}') {
            curly_braces_count--;
            if (curly_braces_count < 2 && inside_second_braces) {
                inside_second_braces = 0;
                // third_brace_found = 1;
                result[res_index++] = str[i];
                result[res_index++] = '\"'; // 在第三个 '}' 后加双引号
                continue; // 跳过这个字符的默认处理
            }
        }

        if (str[i] == '\"' && inside_second_braces) {
            result[res_index++] = '\\';
            // result[res_index++] = '\\';
        }

        result[res_index++] = str[i];
    }

    strcpy(str, result); // 将结果复制回原字符串
}
