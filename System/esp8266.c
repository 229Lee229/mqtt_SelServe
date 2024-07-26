#include "stm32f10x.h"                  // Device header
#include "My_include.h"


// volatile u16 rx_index_2;    // 优化 7/14
bool REC_WithPayload_Flag = false;			// 在中断中接收嵌套Json 判断中括号 7/7
bool data_REC_WithPayload_Flag = false;
extern bool MQTTPUB_FLAG;
extern char Rx_WithPayload_temp[RX_BUFFER_SIZE];
/******************************* runtime 环形缓冲区 **********************************************/
//typedef struct {
//    char buffer[BUFFER_SIZE];
//    volatile uint16_t head;
//    volatile uint16_t tail;
//} RingBuffer;

//RingBuffer rxBuffer = { .head = 0, .tail = 0 };
/******************************* JSON格式 声明变量 **********************************************/


uchar rx_buffer_esp8266[RX_BUFFER_SIZE];
volatile bool data_REC_NoPayload_Flag = false;
/*********************************************************************************************/
uint16_t esp8266_rx_index = 0, esp8266_cntPre = 0;
// bool USART2_IRQn_EnableSel = true;

/******************************* 函数指针 动态切换usart2中断函数 **********************************************/
// 声明USART2的中断处理函数
//extern void USART2_IRQHandler_Init(void);
//extern void USART2_IRQHandler_Runtime(void);
/*************************************************************************************************************/


/* ------------------------------ 发送指令接口 ------------------------------- */
uint8_t esp8266_at_test(void){
    return esp8266_send_command("AT\r\n", "OK");
}
uint8_t esp8266_at_RST(void){
	return esp8266_send_command("AT+RST\r\n","OK");
}
uint8_t esp8266_at_CWMODE(void){
	return esp8266_send_command("AT+CWMODE=1\r\n","OK");
}
//uint8_t esp8266_at_PING(void){
//	return esp8266_send_command("AT+PING=\"baidu.com\"\r\n","OK");
////	return esp8266_send_command("AT+PING=\"http://nestbase.meseee.com\"\r\n","OK");
//}
uint8_t esp8266_at_PING(char *ip){
	char cmd[32];
	sprintf(cmd,"AT+PING=%s\r\n",ip);
	return esp8266_send_command(cmd,"OK");
}
uint8_t esp8266_at_CWJAP(char *ssid, char *pwd)
{
    char cmd[64];
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    // USART1_SendString(cmd);	// test send String
    // return esp8266_send_command(cmd, "WIFI GOT IP");
	return esp8266_send_command(cmd, "WIFI CONNECTED");				/* 先接收此信息 */
}



/* ------------------------------- 驱动函数 -------------------------*/
uint8_t esp8266_wait_receive(void)
{

	if(esp8266_rx_index == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return ESP8266_ERROR;
		
	if(esp8266_rx_index == esp8266_cntPre) {				//如果上一次的值和这次相同，则说明接收完毕
		esp8266_rx_index = 0;							//清0接收计数
		return ESP8266_EOK;							//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_rx_index;					//置为相同
	return ESP8266_ERROR;							//返回接收未完成标志
}

void esp8266_clear(void)
{
	memset(rx_buffer_esp8266, 0, sizeof(rx_buffer_esp8266));
	esp8266_rx_index = 0;
}
uint8_t esp8266_send_command(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);
	USART2_SendString(cmd);
	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK){						//如果收到数据
			if(strstr((const char *)rx_buffer_esp8266, res) != NULL)		//如果检索到关键词
				return ESP8266_EOK;
		}
		Delay_ms(10);
	} 
	return ESP8266_ERROR;

}
/* 7/12 心跳发送 */
uint8_t esp8266_send_command_HeartBeat(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);
	USART2_SendString(cmd);
	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK){						//如果收到数据
			if(strstr((const char *)Rx_WithPayload_temp, res) != NULL)		//如果检索到关键词
				return ESP8266_EOK;
		}
		Delay_ms(10);
	} 
	return ESP8266_ERROR;

}

/*************************************** USART2中断函数 ***********************************************************/
// USART2在初始化阶段的中断处理函数 -1
void USART2_IRQHandler_Init(void)
{
    uint8_t receive_data = 0;   
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		if(esp8266_rx_index >= sizeof(rx_buffer_esp8266))
            esp8266_rx_index = 0; 
		receive_data = USART_ReceiveData(USART2);
		rx_buffer_esp8266[esp8266_rx_index++] = receive_data; 		
		/********************切换中断函数 6/30 ************************/
//		if(MQTTPUB_FLAG == true && rx_buffer_esp8266[esp8266_rx_index-2] == 'O' && rx_buffer_esp8266[esp8266_rx_index-1] == 'K'){

//			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			// esp8266_clear();
			// USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;
//			MQTTPUB_FLAG = false;
			// printf("rx_buffer_esp8266:\r\n%s\r\n",rx_buffer_esp8266);
			// printf("\r\ndone!\r\n");

//			return;
//		}
		/********************************************************/
		
 
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

// USART2在运行时的中断处理函数  -2
void USART2_IRQHandler_Runtime(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uchar Rx_c = USART_ReceiveData(USART2);
		// printf("%c",c);
		rx_buffer_esp8266[esp8266_rx_index++] = Rx_c;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}



// USART2在运行时的中断处理函数 -3	接收没有payload的json数据  非嵌套JSON 中断函数 
void USART2_IRQHandler_Runtime2_NoPayload(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        char Rx_c = USART_ReceiveData(USART2);
		if(esp8266_rx_index >= sizeof(rx_buffer_esp8266))
            esp8266_rx_index = 0; //防止串口被刷爆

		rx_buffer_esp8266[esp8266_rx_index++] = Rx_c;
		
		// 判断是否是这组数据的结束
		if(Rx_c == '}'){
			rx_buffer_esp8266[esp8266_rx_index] = '\0';  // 添加字符串终止符			
			// printf("%s\r\n",rx_buffer);
			data_REC_NoPayload_Flag = true;
			esp8266_rx_index = 0;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}


//void USART2_IRQHandler_Runtime3_WithPayload(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        char Rx_c = USART_ReceiveData(USART2);
//		if(esp8266_rx_index >= sizeof(rx_buffer_esp8266))
//            esp8266_rx_index = 0; //防止串口被刷爆

//		rx_buffer_esp8266[esp8266_rx_index++] = Rx_c;
//		if(Rx_c == '}' && REC_WithPayload_Flag == true){
//			// rx_buffer_esp8266[esp8266_rx_index] = '\0';  // 添加字符串终止符			

//			data_REC_WithPayload_Flag = true;
//			REC_WithPayload_Flag = false;
//		// *	esp8266_rx_index = 0;	
//			USART_ClearITPendingBit(USART2, USART_IT_RXNE);			// 清除中断标志位的位置对程序有影响
//			// printf("%s\r\n",rx_buffer_esp8266);
//			// printf("Done!\r\n");			// 打印接收到的字符串		7/11
//			strcpy(Rx_WithPayload_temp,(char *)rx_buffer_esp8266); // 将结果复制到中间变量
////			printf("\r\nrx_buffer_esp8266:\r\n");
////			printf("%s\r\n",rx_buffer_esp8266);
//			// * memset(rx_buffer_esp8266, 0, sizeof(rx_buffer_esp8266));
////			printf("\r\nclean after:\r\n");
////			printf("%s\r\n",rx_buffer_esp8266);
//			return;			
//		}
//		// 判断是否是这组数据的结束
//		if(Rx_c == '}' && REC_WithPayload_Flag == false){
//			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//			// printf("1 Done!\r\n");			// 打印接收到的字符串		7/11
//			REC_WithPayload_Flag = true;
//			return;
//		}
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    }
//}
void USART2_IRQHandler_Runtime3_WithPayload(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        char Rx_c = USART_ReceiveData(USART2);
		if(esp8266_rx_index >= sizeof(rx_buffer_esp8266))
            esp8266_rx_index = 0; //防止串口被刷爆

		rx_buffer_esp8266[esp8266_rx_index++] = Rx_c;
		if(rx_buffer_esp8266[esp8266_rx_index-1] == '}' && rx_buffer_esp8266[esp8266_rx_index-2] == '}'){
			strcpy(Rx_WithPayload_temp,(char *)rx_buffer_esp8266); // 将结果复制到中间变量
			data_REC_WithPayload_Flag = true;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
/******************************************* 以上为 USART2中断函数 **************************************************************************/

void ESP8266_GPIO_PinInit(void){
	
	GPIO_Pin_Init(PB8,GPIO_Mode_Out_PP);			// PB8->CH-PD  ;   PB9->RST
	GPIO_Pin_Init(PB9,GPIO_Mode_Out_PP);			// PB8->CH-PD  ;   PB9->RST
	PBout(8) = 0;
	PBout(9) = 0;
	Delay_ms(1);
	PBout(8) = 1;
	PBout(9) = 1;
}

void ESP8266_Init(void){
	ESP8266_GPIO_PinInit();
	u8 timeout_espInit;			// 7/10	
	timeout_espInit = 3;
	// esp8266 Init 
	Delay_ms(500);
	while(timeout_espInit--){

		// 若多次AT不成功则重启esp8266		7/10
		if(timeout_espInit == 0){
			PBout(9) = 0;			// ESP8266 RST引脚拉低
			Delay_ms(40);
			PBout(9) = 1;
			timeout_espInit = 3;
		}
		
		if(esp8266_at_test() == 1)
			printf("at test Error!\r\n");
		else{
			printf("at test Successful!\r\n");
			break;
		}
		// USART1_SendString((char *)g_uart_rx_buf);   				// 检测回传指令是否正确
		Delay_ms(10);
	}


	if(esp8266_at_CWMODE() == 1)
		printf("at CWMODE Error!\r\n");
	else printf("at CWMODE Successful!\r\n");
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
	
	timeout_espInit = 5;
	while(timeout_espInit--){
		if(esp8266_at_CWJAP(ESP8266_WIFI_SSID, ESP8266_WIFI_PASSWORD) == 1)
			 printf("at CWJAP Error!\r\n");
		else{
			printf("at CWJAP Successful!\r\n");
			break;
		}
		// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
		Delay_ms(10);
	}
	if(timeout_espInit == 255)			NVIC_SystemReset(); // 触发软件复位
	
	timeout_espInit = 5;
	Delay_ms(2);
	// Ping test
	
#ifndef Debug_NoPing	
	bool ping_break = true;
	do{
		switch(timeout_espInit){
			case 5:
				if(esp8266_at_PING(MQTTCONN_Host_MyConf) == 1)
						printf("at PING timeout! %s\r\n",MQTTCONN_Host_MyConf);
				else{
						printf("at PING Successful! %s\r\n",MQTTCONN_Host_MyConf);
					ping_break = false;
				}
				break;
			case 4:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_5) == 1)
					printf("at PING timeout! %s\r\n",ESP8266_PING_DomainName_5);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_5);
					ping_break = false;
				}
				break;
			case 3:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_4) == 1)
					printf("at PING timeout! %s\r\n",ESP8266_PING_DomainName_4);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_4);
					ping_break = false;
				}
				break;
			case 2:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_3) == 1)
					printf("at PING timeout! %s\r\n",ESP8266_PING_DomainName_3);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_3);
					ping_break = false;
				}
				break;
			case 1:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_2) == 1)
					printf("at PING timeout! %s\r\n",ESP8266_PING_DomainName_2);
				else{
					printf("at PING Successful %s\r\n",ESP8266_PING_DomainName_2);
					ping_break = false;
				}
				break;
		}
//		if(esp8266_at_PING(ESP8266_PING_IP) == 1)
//			printf("at PING Error!\r\n");
//		else{
//			printf("at PING Successful!\r\n");
//			break;
//		}
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
		Delay_ms(50);
	}while(timeout_espInit-- && ping_break);
		
	
#endif
	
}




/******************************* 函数指针 动态切换usart2中断函数 **************************************************/


