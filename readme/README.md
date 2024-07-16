# Self service system based on MQTT

​		Widely used in self-service systems, such as unmanned chess and card rooms, unmanned vending machines, self-service purchases, etc. Users can operate on the client to achieve control over the target. For example, in an unmanned chess and card room, users can turn on lights, air conditioners, doors, and other electrical equipment after authorization on the client. The system can provide services to users within a suitable time range based on the information filled in by the user. This achieves an efficient self-service experience.

### System Hardware

+   master chip : STM32F103
+   control circuit : Relays(DC5V)
+   Clock chip : DS1302
+   access networks : ESP8266 or ESP12F
+   store data : W25Q64 

### Development Kit

+   Keil MDK
+   MQTTX
+   Altium Designer
