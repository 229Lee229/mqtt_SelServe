#ifndef __MQTT_ESP8266_H
#define __MQTT_ESP8266_H


// AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
#define MQTTUSERCFG_MyConf_1 			"0,1,\"Michael_esp\",\"esp8266_li\",\"1234567\",0,0,\"\""
#define MQTTUSERCFG_MyConf_2 			"0,1,\"1234567_T9BX11TG\",\"esp8266_li\",\"1234567\",0,0,\"\""
#define MQTTUSERCFG_MyConf_3 			"0,1,\"2222227_U1Q4B2Z5\",\"esp8266_li\",\"1234567\",0,0,\"\""

#define MQTTLONGCLIENTID_MyConf 		"1234567_T9BX11TG"
#define MQTTCONNCFG_MyConf  			"0,60,1,\"\",\"\",0,0"
#define MQTTCONN_Host_MyConf_LAN 		"192.168.1.42"
#define MQTTCONN_Host_MyConf_PUB 		"nestbase.meseee.com"
#define MQTTCONN_Port_MyConf			1883


#define MQTTCONN_Topic_1				"Room001"
#define MQTTCONN_Topic_2				"1234567/1234567_T9BX11TG"
#define MQTTCONN_Topic_3				"2222227/2222227_U1Q4B2Z5"
#define MQTTCONN_Topic_4				"1111119/1111119_L16LF3HR"


#define MQTTCONN_Message_1				"Play_two_hours"
#define MQTTCONN_Message_2				"{\\\"Type\\\":0\\,\\\"Time\\\":12787\\,\\\"MsgId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"\\,\\\"SendId\\\":\\\"1234567_T9BX11TG\\\"}"
//#define MQTTCONN_Message_2			"{\\\"Type\\\":0\\,\\\"Time\\\":12787\\\,\\\"MsgId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"\\\,\\\"SendId\\\":\\\"1234567_T9BX11TG\\\"}"
#define MQTTCONN_Message_3				"{\\\"Type\\\":0\\,\\\"Time\\\":12787\\,\\\"MsgId\\\":\\\"7f91c5a9f0879994d95a42e919b574af\\\"\\,\\\"SendId\\\":\\\"2222227_U1Q4B2Z5\\\"}"


#define ClientID_Length					1024
#define LinkID							0
// #define MQTTCONN_Message_2		"{\"Type\":0,\"Time\":12787,\"MsgId\":\"7f91c5a9f0879994d95a42e919b574af\",\"SendId\":\"1234567_T9BX11TG\"}"
// {"Type":0,"Time":12787,"MsgId":"7f91c5a9f0879994d95a42e919b574af","SendId":"2222227_U1Q4B2Z5"}
void MQTT_Init(void);


// MQTT Functions
uint8_t esp8266_send_command_expect(char *cmd, char *expect, char *res);		// with ClientID
uint8_t esp8266_at_MQTTUSERCFG(char *config);
uint8_t esp8266_at_MQTTLONGCLIENTID(uint16_t length, char *client_id);			// ÐÂÔö
uint8_t esp8266_at_MQTTLONGCLIENTID_1(void);			
uint8_t esp8266_at_MQTTLONGCLIENTID_2(char *client_id);			// 
uint8_t esp8266_at_MQTTLONGUSERNAME(char *username);
uint8_t esp8266_at_MQTTLONGPASSWORD(char *password);
uint8_t esp8266_at_MQTTCONNCFG(char *config);
uint8_t esp8266_at_MQTTCONN(char *host, int port);
uint8_t esp8266_at_MQTTPUB(char *topic, char *message);
uint8_t esp8266_at_MQTTPUBRAW(char *topic, uint8_t *data, uint16_t len);
uint8_t esp8266_at_MQTTSUB(char *topic);
uint8_t esp8266_at_MQTTUNSUB(char *topic);
uint8_t esp8266_at_MQTTCLEAN(void);
void processSecondGroupData(char *data);
// 7/1
void add_backslashes_and_quotes(char *str);
#endif
