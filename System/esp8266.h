#ifndef __ESP8266_H
#define __ESP8266_H


#define ESP8266_UART_RX_BUF_SIZE            128
#define ESP8266_UART_TX_BUF_SIZE            64
#define ESP8266_EOK                         0   /* 没有错误 */
#define ESP8266_ERROR                       1   /* 通用错误 */
#define ESP8266_ETIMEOUT                    2   /* 超时错误 */
#define ESP8266_EINVAL                      3   /* 参数错误 */
#define BUFFER_SIZE 						100

//#define ESP8266_WIFI_SSID					"STM32"
//#define ESP8266_WIFI_PASSWORD				"12345678"
#define ESP8266_WIFI_SSID					"ChinaNet-PaRf"
#define ESP8266_WIFI_PASSWORD				"gj5qmdxe"

#define ESP8266_PING_DomainName_6			"http://nestbase.meseee.com"
#define ESP8266_PING_DomainName_5 			"www.baidu.com"
#define ESP8266_PING_DomainName_4			"www.aliyun.com"
#define ESP8266_PING_DomainName_3			"www.qq.com"
#define ESP8266_PING_DomainName_2			"www.sina.com.cn"
#define ESP8266_PING_DomainName_1			"www.sohu.com"
// #define ESP8266_PING_IP						"192.168.1.42"
#define ESP8266_PING_IP						"baidu.com"
/******************************* JSON格式 声明变量 **********************************************/
#define RX_BUFFER_SIZE 2048
/******************************* runtime 环形缓冲区 **********************************************/
#define BUFFER_SIZE_RUNTIME 512  // 根据实际需要调整缓冲区大小

/*************************************** USART2中断函数 ***********************************************************/
// 定义函数指针类型
typedef void (*USART2_IRQHandler_t)(void);

// 定义函数指针变量
// extern volatile USART2_IRQHandler_t USART2_IRQHandler_ptr;

/********************************** 声明USART2的中断处理函数 **************************/
void USART2_IRQHandler_Init(void);
void USART2_IRQHandler_Runtime(void);
void USART2_IRQHandler_Runtime2_NoPayload(void);
void USART2_IRQHandler_Runtime3_WithPayload(void);
/*****************************************************************************************************************/

uint8_t esp8266_wait_receive(void);
void esp8266_clear(void);
uint8_t esp8266_send_command(char *cmd, char *res);
uint8_t esp8266_send_command_PUB(char *cmd, char *res);
void USART2_IRQHandler(void);

uint8_t esp8266_at_test(void);
uint8_t esp8266_at_RST(void);
uint8_t esp8266_at_CWMODE(void);
uint8_t esp8266_at_CWJAP(char *ssid, char *pwd);
// uint8_t esp8266_at_PING(void);
uint8_t esp8266_at_PING(char *ip);

void ESP8266_GPIO_PinInit(void);
void ESP8266_Init(void);

#endif
