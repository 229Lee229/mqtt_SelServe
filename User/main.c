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
	V1.3		 6/27/2024    Lee  				中断后出不去的原因是,判断语句中多个引脚判断 "(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)"
												当进入中断函数后,判断if语句中,这条语句不成立,故不断进入中断,出不去的原因,标志位在语句中,故一直清除不掉
	V2.0		 7/4/2024	  Lee				增加mqtt指令控制系统复位
	V3.0		 7/7/2024     Lee				增加解析payload数据,startANDend长整形数据,增加看门狗定时,
												解除半主机模式解析失败 使用mircro LIB解析成功
												如函数 : void _ttywrch(int ch)
    V4.0       	 7/11/2024    Lee				增加心跳检测 定时器TIM2												
 ***********************************************************************/
bool en_judge_IfLightEnd = false;			// 判断是否进入到判断灯光熄灭的条件 (照明灯延长30s关闭)
long long en_judge_IfLightEnd_val;


extern bool Enable_HeartBeat_Send_Flag;
extern bool CompareTime_Flag;
uchar time_Current[6] = {97,6,30,23,58,05};

extern char Rx_WithPayload_temp[RX_BUFFER_SIZE];
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



extern volatile bool data_REC_NoPayload_Flag;
extern uint16_t esp8266_rx_index;
/******************************* 函数指针 动态切换usart2中断函数 **********************************************/
extern uint16_t rx_index;

volatile USART2_IRQHandler_t USART2_IRQHandler_ptr = NULL;

// USART2的实际中断处理函数
void USART2_IRQHandler(void) {
    if (USART2_IRQHandler_ptr != NULL) {
        USART2_IRQHandler_ptr();
    }
}

// u8 Json_type;
// bool json_YorN_flag;
extern uchar rx_buffer_esp8266[RX_BUFFER_SIZE];
extern bool data_REC_WithPayload_Flag;			// 7/7
/*********************************** JSON_parse 测试数据 *************************************************************************/
// char JSON_parse_test_1[RX_BUFFER_SIZE] = "{\"Type\":4,\"Time\":1719746116700,\"MsgId\":\"96c158a3-1a72-4604-b699-4487b58a28b8\",\"SendId\":\"SVR01\",			\
//										\"Payload\":\"{\\\"CtxId\\\":\\\"jjoodf\\\",\\\"CtxId2\\\":1974611670}\"}";
// char JSON_parse_test_2[RX_BUFFER_SIZE] = "{\"w\":\"GWiFi\",\"p\":\"G@dge@n#24it&dp\",\"t\":\"0,0,26941480,140,170,40,40,100,200,150,29,39,89,45,2,75,75,2\"}";
// char JSON_parse_test_3[RX_BUFFER_SIZE] = "{\"data\":\"{\\\"a\\\":1,\\\"b\\\":2}\"}";
// char JSON_parse_test_4[RX_BUFFER_SIZE]/* 7/7 */ = "{\"Type\":10,\"Time\":1720343740875,\"MsgId\":\"5464cd57-c59f-4c53-8f46-8725013f3db9\",\"SendId\":\"SVR01\",\"Payload\":\"{\\\"CtxId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"}\"}";
// char JSON_parse_test_5[RX_BUFFER_SIZE]	= "{\"Type\":92,\"Time\":1744533467325,\"MsgId\":\"4af325ke4af325ke4af325ke4af325ke\",  \
											\"SendId\":\"SVR01\",\"Payload\":		\
											\"{\\\"Start\\\":1744533467325,\\\"End\\\":17445334883254,\\\"Getter\\\":\\\"clientId_005\\\"}\"}";								
// char JSON_parse_test_6[RX_BUFFER_SIZE] = "{\"Type\":31,\"Time\":174454444467325,\"MsgId\":\"4af325ke4af325ke4af325ke4af325ke\",\"SendId\":\"SVR01\",\"Payload\":\"{\\\"art\\\":1720444624448,\\\"End\\\":17445334883254,\\\"ett\\\":\\\"clientId_005\\\",\\\"L\\\":14448}\"}";

// {"Type":10,"Time":1720343740875,"MsgId":"5464cd57-c59f-4c53-8f46-8725013f3db9","SendId":"SVR01","Payload":{"CtxId":"7f91c5a9f0879994d95a42e919b574af"}}
// char *a_test = "{\"Type\":4,\"Time\":1721824139120,\"MsgId\":\"b51b9c69-deb0-4e4d-b2e4-eb7294266b50\",\"SendId\":\"SVR01\",\"Payload\":\"{\\\"CtxId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"}\"}";
int main(void){				// a9f0879994d95a42e919b574af}

	Usart1_Init(115200);

	Usart2_Init(115200);
	
#ifdef Debug_cJSON_TEST_Start	
	cJSON * jo = cJSON_Parse(a);
	if (jo) {
		printf("JSON ok\n");
	}
	else{
		printf("JSON parse error: %s\n", cJSON_GetErrorPtr());
	}
	cJSON_Delete(jo);				// 回声会被返回
	// printf("JSON invalid\n");
	return 0;	
#endif	
	// 将USART2的中 断处理函数指针指向初始化阶段的处理函数
    USART2_IRQHandler_ptr = USART2_IRQHandler_Init;
	
	timer_doorLock_Init();
	switchCtrlRelay_Init();
	// ----test 7/3
	relay5V_Init();	
	ESP8266_Init();

	MQTT_Init();
	DS1302_init(time_Current);
#ifdef Debug_MQTTPUB_Init	
	do{
		Delay_ms(200);
		uchar *array = (uchar *)malloc(RX_BUFFER_SIZE* sizeof(uchar));
		array = (uchar *)(strstr((const char *)rx_buffer_esp8266, "OK"));
		array++;
		array++;
		printf("\r\n%s\r\n",array);
		printf("done!\r\n");
		processSecondGroupData((uchar *)array);		
		
		// 使用完毕，释放内存
		free(array);
		array = NULL; // 将指针设置为NULL，防止悬空指针
	}while(0);
	// 加上此代码 后续解析json错误 改为发送心跳包获取初始化时间
#endif	
	



	
	// 数组清零 序列清零
	esp8266_clear();
	
	// 转移中断, 接收非嵌套JSON 
    // USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime2_NoPayload;
	
	// 转移中断,接收嵌套JSON	
	USART2_IRQHandler_ptr = USART2_IRQHandler_Runtime3_WithPayload;
	
/*-----------------初始化实时时间------------------------*/
/*|*/	keep_HeartBeat();			// 发送心跳回执	  /*|*/	
/*|*/	Delay_ms(500);
    esp8266_clear();
//	printf("\r\n%s\r\n",rx_buffer_esp8266);
//	printf("\r\n%s\r\n",Rx_WithPayload_temp);
	// memset(Rx_WithPayload_temp, 0, sizeof(Rx_WithPayload_temp));	

//  char *a_test = "+MQTTSUBRECV:0,\"1111119/1111119_L16LF3HR\",150,{\"Type\": 101,\"Time\": 1744533467325,\"MsgId\":\"4af325ke4af325ke4af325ke4af325ke\",\"SendId\":\"SVR01\",\"Payload\":{\"Start\": 1744533467325,\"End\":17445334883254,\"Getter\":\"clientId_001\"}}";
//	strcpy(Rx_WithPayload_temp,(char *)a_test); // 将结果复制到中间变量
	Json_parse_WithPayload2();                     /*|*/
	data_REC_WithPayload_Flag = false;
/*-------------------------------------------------------*/
	TIM_HeartBeat_Init();
	
	// JR6001 初始化   8/10
	My_JR6001_Init();
	
	
	
	
	
	
	IWDG_Init();
	for(;;){
		if(Enable_HeartBeat_Send_Flag){
			keep_HeartBeat();			// 发送心跳回执		
			Enable_HeartBeat_Send_Flag = false;
		}
		if(data_REC_WithPayload_Flag){
			esp8266_clear();
			Json_parse_WithPayload2(); 
			data_REC_WithPayload_Flag = false;
		}					
	 // Json_parse_WithPayload();
	 // Json_parse_NoPayload();
		Delay_ms(995);				// 考虑每955毫秒设置定时器
		// Start与End的时间的对比 7/9
		if(CompareTime_Flag == true){
			// 除了照明灯的其余继电器
			
			if(en_judge_IfLightEnd == false){
				if(CompareTime() == true){			
					printf("else-Relay testCompareTime Successful!\r\n");
					// Relay_1_Pin_DoorLock = PwrOn_LockDoor;
					Relay_3_Pin_AllSocket_out 	 = Machine_PwrLoss_Relay;
					Relay_4_Pin_else_out 		 = Machine_PwrLoss_Relay;
		
					
				}
			}
			else if(en_judge_IfLightEnd == true){
				if(CompareTime() == true){
					printf("light-Relay testCompareTime Successful!\r\n");	
					Relay_1_Pin_DoorLock_out 	 = PwrOn_LockDoor;	
					Relay_2_Pin_Light_out 		 = Machine_PwrLoss_Relay;
					
					en_judge_IfLightEnd = false;
									/* 在开门时间内 开关控制不起作用 7/13 */
					EXTI_InitTypeDef EXTI_InitStructure;
					EXTI_InitStructure.EXTI_Line = EXTI_Line15;
					EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		// 上升沿触发
					EXTI_InitStructure.EXTI_LineCmd = ENABLE;
					EXTI_Init(&EXTI_InitStructure);
					CompareTime_Flag = false;
				}
			}
		}
		DS1302_Readtime();
		printf("%d-%d-%d  %d:%d:%d\r\n",		\
		 time_Current[0],time_Current[1],time_Current[2],time_Current[3],time_Current[4],time_Current[5]);
			// 喂狗 7/6
		IWDG_ReloadCounter(); // 重装载IWDG寄存器
	}
}
