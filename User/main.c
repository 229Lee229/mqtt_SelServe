#include "stm32f10x.h"                  // Device header
#include "My_include.h"
/**********************************************************************
 * flie:  main
 * brief:  
 * Input: 
 * Output:
 * Retval:
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 6/23/2024    Lee				reset后无法正常设置mqtt配置属性 可能是重置引脚问题 重新拉高引脚
												AT调试时测试一下是否可以正常设置 考虑mqtt_clean的问题 重连mqtt的时候考虑重新启动esp
												
    V1.2		 6/25/2024	  Lee				尝试中断函数中不加延时函数 使用标志位进行esp重置 
	V1.3		 6/272024     Lee  				中断后出不去的原因是,判断语句中多个引脚判断 "(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)"
												当进入中断函数后,判断if语句中,这条语句不成立,故不断进入中断,出不去的原因,标志位在语句中,故一直清除不掉
 ***********************************************************************/



// extern uint8_t g_uart_rx_buf[];
// bool esp_RstPin = false;

extern volatile u16 rx_index_2;
extern volatile bool data_received;
extern uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
/******************************* 函数指针 动态切换usart2中断函数 **********************************************/



volatile USART2_IRQHandler_t USART2_IRQHandler_ptr = NULL;

// USART2的实际中断处理函数
void USART2_IRQHandler(void) {
    if (USART2_IRQHandler_ptr != NULL) {
        USART2_IRQHandler_ptr();
    }
}

/*****************************************************************************************************************/
extern volatile bool data_received;
extern volatile char rx_buffer[RX_BUFFER_SIZE];
char rx_buffer_2[RX_BUFFER_SIZE] = "{\"Type\":4,\"Time\":1719746116700,\"MsgId\":\"96c158a3-1a72-4604-b699-4487b58a28b8\",\"SendId\":\"SVR01\",			\
	\"Payload\":\"{\\\"CtxId\\\":\\\"jjoodf\\\",\\\"CtxId2\\\":1974611670}\"}";


char rx_buffer_3[RX_BUFFER_SIZE] = "{\"w\":\"GWiFi\",\"p\":\"G@dge@n#24it&dp\",\"t\":\"0,0,26941480,140,170,40,40,100,200,150,29,39,89,45,2,75,75,2\"}";
char rx_buffer_4[RX_BUFFER_SIZE] = "{\"data\":\"{\\\"a\\\":1,\\\"b\\\":2}\"}";

int main(void){				// a9f0879994d95a42e919b574af}
	
	Usart1_Init(115200);
	Usart2_Init(115200);
	
	
	// 将USART2的中断处理函数指针指向初始化阶段的处理函数
    USART2_IRQHandler_ptr = USART2_IRQHandler_Init;
	
	EspRst_GPIO_Init();
	ESP8266_Init();
	MQTT_Init();
//	cJSON * jo = cJSON_Parse(rx_buffer_2);

//	if (jo) {
//		printf("JSON ok\n");
//	}
//	else
//		printf("JSON invalid\n");
	do{
		// break;
		Delay_ms(3000);	
		for(u16 i = 0;i < rx_index_2;i++){
			printf("%c",rx_buffer[i]);
				
		}
		// printf("\r\n");
		processSecondGroupData((char *)rx_buffer);		
		
		
	}while(0);
	
	while(1){
		 // if (data_received) {
			// process_received_data((const char *)rx_buffer);
//			for(u16 i = 0;i < rx_index_2;i++){
//				printf("%c",rx_buffer[i]);
//				
//			}
		  // data_received = false;
		// }		
		
		// esp重置
//		if(esp_RstPin == true){
//			Delay_ms(300);
//			PBout(9) = 1;

//			printf("ESP8266 Reset Successful!\r\n");
//			esp_RstPin = false;
//		}

//		Delay_ms(5000);	
//		for(u16 i = 0;i < rx_index_2;i++){
//			printf("%c",rx_buffer[i]);
//				
//		}
//		printf("\r\n");
//		processSecondGroupData((char *)rx_buffer);
		// USART1_SendString("test...\r\n");
			
	}
}
