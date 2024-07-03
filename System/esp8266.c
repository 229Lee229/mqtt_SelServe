#include "stm32f10x.h"                  // Device header
#include "My_include.h"


volatile u16 rx_index_2;
u8 rx_flag_char = 0;			// 接收是否是'}'


extern bool MQTTPUB_FLAG;

/******************************* runtime 环形缓冲区 **********************************************/
typedef struct {
    char buffer[BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBuffer;

RingBuffer rxBuffer = { .head = 0, .tail = 0 };
/******************************* JSON格式 声明变量 **********************************************/


volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_index = 0;
volatile bool data_received_3 = false;
/*********************************************************************************************/

uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
uint8_t g_uart_tx_buf[ESP8266_UART_TX_BUF_SIZE];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;
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

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return ESP8266_ERROR;
		
	if(esp8266_cnt == esp8266_cntPre) {				//如果上一次的值和这次相同，则说明接收完毕
		esp8266_cnt = 0;							//清0接收计数
		return ESP8266_EOK;							//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	return ESP8266_ERROR;							//返回接收未完成标志
}

void esp8266_clear(void)
{
	memset(g_uart_rx_buf, 0, sizeof(g_uart_rx_buf));
	esp8266_cnt = 0;
}
uint8_t esp8266_send_command(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);
	USART2_SendString(cmd);
	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK){						//如果收到数据
			if(strstr((const char *)g_uart_rx_buf, res) != NULL)		//如果检索到关键词
				return ESP8266_EOK;
		}
		Delay_ms(10);
	} 
	return ESP8266_ERROR;

}

// MQTTPUB test 转换中断指针  6/30
uint8_t esp8266_send_command_PUB(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);
	USART2_SendString(cmd);
	USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;		// 转换中断指针 6/30

	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK) {						//如果收到数据
			if(strstr((const char *)g_uart_rx_buf, res) != NULL)		//如果检索到关键词
				// USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;		// 转换中断指针 6/30
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
		if(esp8266_cnt >= sizeof(g_uart_rx_buf))
            esp8266_cnt = 0; //防止串口被刷爆
        // HAL_UART_Receive(&g_uart_handle, &receive_data, 1, 1000);//串口2接收1位数据
		receive_data = USART_ReceiveData(USART2);
		// USART_SendData(USART1, receive_data);		// 新增  查看回传数据
//		if(USART2_IRQn_EnableSel == false){
//			USART_SendData(USART1, receive_data);
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//				;
//		}
		g_uart_rx_buf[esp8266_cnt++] = receive_data; 		
		/********************切换中断函数 6/30 ************************/
		if(MQTTPUB_FLAG == true && g_uart_rx_buf[esp8266_cnt-2] == 'O' && g_uart_rx_buf[esp8266_cnt-1] == 'K'){
			USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime;
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			MQTTPUB_FLAG = false;
			return;
		}
		/********************************************************/
		
 
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    // HAL_UART_IRQHandler(&g_uart_handle);
}

// USART2在运行时的中断处理函数  -1
//void USART2_IRQHandler_Runtime(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        char c = USART_ReceiveData(USART2);
//		// printf("%c",c);
//        if (c == '\n' || c == '\r') {
//            rx_buffer[rx_index] = '\0';
//            rx_index = 0;
//            data_received = true;
//        } else {
//            rx_buffer[rx_index++] = c;
//            if (rx_index >= RX_BUFFER_SIZE) {
//                rx_index = 0; // 防止缓冲区溢出
//            }
//        }
//        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    }
//}

// USART2在运行时的中断处理函数  -2
void USART2_IRQHandler_Runtime(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        char c = USART_ReceiveData(USART2);
		// printf("%c",c);
		rx_buffer[rx_index++] = c;
		rx_index_2 = rx_index;
		
		// 判断是否是这组数据的结束
//		if(rx_buffer[rx_index] == '}'){
//			if(rx_buffer[rx_index] == rx_buffer[rx_index - 1]){
//				rx_index_2 = rx_index;
//				data_received = 1;
//				rx_index = 0;
//			}
//		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}



// 非嵌套JSON 			接收没有payload的json数据  中断函数 -3
void USART2_IRQHandler_Runtime2(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        char Rx_c = USART_ReceiveData(USART2);
		// printf("%c",c);
		if(rx_index >= sizeof(rx_buffer))
            rx_index = 0; //防止串口被刷爆

		rx_buffer[rx_index++] = Rx_c;
		
		// 判断是否是这组数据的结束
		if(Rx_c == '}'){


			rx_buffer[rx_index] = '\0';  // 添加字符串终止符			
			// printf("%s\r\n",rx_buffer);
			data_received_3 = true;
			rx_index = 0;
//			memset((void*)rx_buffer, 0, RX_BUFFER_SIZE);

		}
//		if (c == '\n' /* || c == '\r' */ ) {  // assuming '\n' is the end of message
//            rx_buffer[rx_index] = '\0';  // Null-terminate the string
//            data_received = 1;  // Flag indicating data is received
//			rx_index_2 = rx_index;
//            rx_index = 0;  // Reset index for next message
//			goto exit;
//        }

		
		// printf("%c",c);
        // Store received character in buffer if there is space
//        if (rx_index < BUFFER_SIZE - 1) {
//            rx_buffer[rx_index++] = c;
//        }

        // Check if the received character indicates end of data (e.g., newline or specific character)
//		exit:
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

/*********************************************************************************************************************/

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
	u8 timeout_espInit;
	/* GPIO Pin Init */
//	GPIO_Pin_Init(PB8,GPIO_Mode_Out_PP);			// PB8->CH-PD  ;   PB9->RST
//	GPIO_Pin_Init(PB9,GPIO_Mode_Out_PP);			// PB8->CH-PD  ;   PB9->RST
//	PBout(8) = 1;
//	PBout(9) = 1;
	
	timeout_espInit = 3;
	// esp8266 Init 
	Delay_ms(500);
	while(timeout_espInit--){
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

	
	timeout_espInit = 5;
	Delay_ms(2);
	// Ping test
	
	
	bool ping_break = true;
	do{
		switch(timeout_espInit){
			case 1:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_1) == 1)
					// char cmds[128];
					
				
					printf("at PING Error! %s\r\n",ESP8266_PING_DomainName_1);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_1);
					ping_break = false;
				}				
			
				break;
			case 2:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_2) == 1)
					printf("at PING Error! %s\r\n",ESP8266_PING_DomainName_2);
				else{
					printf("at PING Successful %s\r\n",ESP8266_PING_DomainName_2);
					ping_break = false;
				}				
				
				
				break;
			case 3:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_3) == 1)
					printf("at PING Error! %s\r\n",ESP8266_PING_DomainName_3);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_3);
					ping_break = false;
				}				
				
				
				break;
			case 4:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_4) == 1)
					printf("at PING Error! %s\r\n",ESP8266_PING_DomainName_4);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_4);
					ping_break = false;
				}				
				
			
				break;
			case 5:			
				if(esp8266_at_PING(ESP8266_PING_DomainName_5) == 1)
					printf("at PING Error! %s\r\n",ESP8266_PING_DomainName_5);
				else{
					printf("at PING Successful! %s\r\n",ESP8266_PING_DomainName_5);
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
		
	

/*
while(timeout_espInit-- && ping_break){
		
//		switch(timeout_espInit){
//			case 1:			
//				if(esp8266_at_PING(ESP8266_PING_DomainName_1) == 1)
//					printf("at PING Error!\r\n");
//				else{
//					printf("at PING Successful!\r\n");
//					ping_break = false;
//				}				
//			
//				break;
//			case 2:			
//				if(esp8266_at_PING(ESP8266_PING_DomainName_2) == 1)
//					printf("at PING Error!\r\n");
//				else{
//					printf("at PING Successful!\r\n");
//					ping_break = false;
//				}				
//				
//				
//				break;
//			case 3:			
//				if(esp8266_at_PING(ESP8266_PING_DomainName_3) == 1)
//					printf("at PING Error!\r\n");
//				else{
//					printf("at PING Successful!\r\n");
//					ping_break = false;
//				}				
//				
//				
//				break;
//			case 4:			
//				if(esp8266_at_PING(ESP8266_PING_DomainName_4) == 1)
//					printf("at PING Error!\r\n");
//				else{
//					printf("at PING Successful!\r\n");
//					ping_break = false;
//				}				
//				
//			
//				break;
//			case 5:			
//				if(esp8266_at_PING(ESP8266_PING_DomainName_5) == 1)
//					printf("at PING Error!\r\n");
//				else{
//					printf("at PING Successful!\r\n");
//					ping_break = false;
//				}				
//				
//				
//				break;
//		}
		if(esp8266_at_PING(ESP8266_PING_IP) == 1)
			printf("at PING Error!\r\n");
		else{
			printf("at PING Successful!\r\n");
			break;
		}
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
		Delay_ms(50);
	}

*/
	
	
}




/******************************* 函数指针 动态切换usart2中断函数 **************************************************/


