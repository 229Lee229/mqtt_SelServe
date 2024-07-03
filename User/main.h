#ifndef __MAIN_H
#define __MAIN_H

extern volatile USART2_IRQHandler_t USART2_IRQHandler_ptr;

/*********** Debug **************/ 

// #define MQTTPUB_Init

/*******************************/ 




// 1->Init 2->运行中接收数据
//#define USART2_IRQn_EnableSel(i)	
//do{
//	if(i == 1)
//		#define USART2_IRQn_EnableInit
//	else if(i == 2)
//		#define USART2_IRQn_EnableWhile
//}while(0);	


#endif
