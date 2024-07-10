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
	V1.3		 6/27/2024     Lee  				中断后出不去的原因是,判断语句中多个引脚判断 "(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)"
												当进入中断函数后,判断if语句中,这条语句不成立,故不断进入中断,出不去的原因,标志位在语句中,故一直清除不掉
	V2.0		 7/4/2024	  Lee				增加mqtt指令控制系统复位
	V3.0		 7/7/2024     Lee				增加解析payload数据,startANDend长整形数据,增加看门狗定时,
												解除半主机模式解析失败 使用mircro LIB解析成功
												如函数 : void _ttywrch(int ch)
 ***********************************************************************/



// extern uint8_t g_uart_rx_buf[];
// bool esp_RstPin = false;

// 7/9 
extern bool CompareTime_Flag;

uchar init_time[6] = {24,2,28,23,30,05};			// 初始化时钟
uchar time_data[6] = {0};// 接收时钟数据缓冲
/* 7/6 */
void IWDG_Init(void) {
    // 使能对IWDG寄存器的写访问
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    // 设置预分频值
    IWDG_SetPrescaler(IWDG_Prescaler_128);

    IWDG_SetReload(3995);			// timeout = (Reload Value + 1) × (1/(LSI / (Prescaler)))
	// 10s
    // IWDG重装载值
    IWDG_ReloadCounter();

    // 使能IWDG
    IWDG_Enable();
}



extern volatile u16 rx_index_2;
extern volatile bool data_REC_NoPayload_Flag;
extern uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
/******************************* 函数指针 动态切换usart2中断函数 **********************************************/

extern char rx_buffer_5[RX_BUFFER_SIZE];
extern uint16_t rx_index;

volatile USART2_IRQHandler_t USART2_IRQHandler_ptr = NULL;

// USART2的实际中断处理函数
void USART2_IRQHandler(void) {
    if (USART2_IRQHandler_ptr != NULL) {
        USART2_IRQHandler_ptr();
    }
}

u8 Json_type;
bool json_YorN_flag;
extern char rx_buffer_esp8266[RX_BUFFER_SIZE];
extern bool data_REC_WithPayload_Flag;			// 7/7
/*********************************** JSON_parse 测试数据 *************************************************************************/
char JSON_parse_test_1[RX_BUFFER_SIZE] = "{\"Type\":4,\"Time\":1719746116700,\"MsgId\":\"96c158a3-1a72-4604-b699-4487b58a28b8\",\"SendId\":\"SVR01\",			\
										\"Payload\":\"{\\\"CtxId\\\":\\\"jjoodf\\\",\\\"CtxId2\\\":1974611670}\"}";
char JSON_parse_test_2[RX_BUFFER_SIZE] = "{\"w\":\"GWiFi\",\"p\":\"G@dge@n#24it&dp\",\"t\":\"0,0,26941480,140,170,40,40,100,200,150,29,39,89,45,2,75,75,2\"}";
// char JSON_parse_test_3[RX_BUFFER_SIZE] = "{\"data\":\"{\\\"a\\\":1,\\\"b\\\":2}\"}";
// char JSON_parse_test_4[RX_BUFFER_SIZE]/* 7/7 */ = "{\"Type\":10,\"Time\":1720343740875,\"MsgId\":\"5464cd57-c59f-4c53-8f46-8725013f3db9\",\"SendId\":\"SVR01\",\"Payload\":\"{\\\"CtxId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"}\"}";
char JSON_parse_test_5[RX_BUFFER_SIZE]	= "{\"Type\":92,\"Time\":1744533467325,\"MsgId\":\"4af325ke4af325ke4af325ke4af325ke\",  \
											\"SendId\":\"SVR01\",\"Payload\":		\
											\"{\\\"Start\\\":1744533467325,\\\"End\\\":17445334883254,\\\"Getter\\\":\\\"clientId_005\\\"}\"}";								
//char JSON_parse_test_6[RX_BUFFER_SIZE] = "{\"Type\":11,\"Time\":174454444467325,\"MsgId\":\"4af325ke4af325ke4af325ke4af325ke\", \
//										   \"SendId\":\"SVR01\",\"Payload\":  \
//										   \"{\\\"Start\\\":1720444624448,\\\"End\\\":17445334883254,\\\"Getter\\\":\\\"clientId_005\\\"}\"}";

// {"Type":10,"Time":1720343740875,"MsgId":"5464cd57-c59f-4c53-8f46-8725013f3db9","SendId":"SVR01","Payload":{"CtxId":"7f91c5a9f0879994d95a42e919b574af"}}
int main(void){				// a9f0879994d95a42e919b574af}

	
	

	Usart1_Init(115200);

	Usart2_Init(115200);
	
#ifdef cJSON_TEST_Start	
	cJSON * jo = cJSON_Parse(JSON_parse_test_6);
	if (jo) {
		printf("JSON ok\n");
	}
	else
		printf("JSON invalid\n");
	return 0;	
#endif	
	// 将USART2的中 断处理函数指针指向初始化阶段的处理函数
    USART2_IRQHandler_ptr = USART2_IRQHandler_Init;
	// ----test 7/3
	relay5V_Init();	
	EspRst_GPIO_Init();
	ESP8266_Init();
	MQTT_Init();
	
/* 测试json数据代码
//	cJSON * jo = cJSON_Parse(rx_buffer_5);
//	if (jo) {
//		printf("JSON ok\n");
//	}
//	else
//		printf("JSON invalid\n");
//	return 0;
*/
#ifdef MQTTPUB_Init	
	do{
		// break;
		Delay_ms(3000);	
		for(u16 i = 0;i < rx_index_2;i++){
			printf("%c",rx_buffer_esp8266[i]);
				
		}
		// printf("\r\n");
		processSecondGroupData(	(char *)rx_buffer_esp8266);		
		
		
	}while(0);
#endif	
	// 数组清零			7/3
	memset((void*)rx_buffer_esp8266, 0, RX_BUFFER_SIZE);
	rx_index = 0;
	// 转移中断, 接收非嵌套JSON  test  7/3
    // USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime2_NoPayload;
	
	// 转移中断,接收嵌套JSON	test 7/7
	USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime3_WithPayload;
	DS1302_init(init_time);
	DS1302_SetTime(init_time);
	IWDG_Init();

	while(1){
		Json_parse_WithPayload();
		
		keep_HeartBeat();			// 发送心跳回执
	 // Json_parse_NoPayload();
		Delay_ms(4995);				// 考虑每955毫秒设置定时器
		
		
		// Start与End的时间的对比 7/9
		if(CompareTime_Flag == true){
			if(CompareTime() == true){
				printf("testCompareTime Successful!\r\n");
				Pin_DoorLock_2 = 1;
				Pin_Light_2	   = 1;		
				CompareTime_Flag = false;
			}
		}
		DS1302_Readtime();
		printf("%d-%d-%d  %d:%d:%d\r\n",		\
		 time_data[0],time_data[1],time_data[2],time_data[3],time_data[4],time_data[5]);
			// 喂狗 7/6
		IWDG_ReloadCounter(); // 重装载IWDG寄存器
	}
}
