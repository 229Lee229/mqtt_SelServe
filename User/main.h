#ifndef __MAIN_H
#define __MAIN_H

// #define unsigned char uchar


extern volatile USART2_IRQHandler_t USART2_IRQHandler_ptr;

/*********** Debug **************/ 
#define  DEBUG_1
#define Debug_ComapreTime
// #define Debug_MQTTPUB_Init
// #define Debug_NoPing
// #define Debug_cJSON_TEST_Start
/************ Use ***************/ 

// #define use_Json_NoPayload

/********************************/ 

// 1->Init 2->运行中接收数据
//#define USART2_IRQn_EnableSel(i)	
//do{
//	if(i == 1)
//		#define USART2_IRQn_EnableInit
//	else if(i == 2)
//		#define USART2_IRQn_EnableWhile
//}while(0);	


#endif
