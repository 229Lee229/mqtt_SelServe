#include "My_include.h"
#include "time.h"

extern bool en_judge_IfLightEnd;
extern long long en_judge_IfLightEnd_val;

// extern uchar init_time[6];		// 7/8 ½ÓÊÕjsonºó×ª»»Ê±¼ä
uchar End_time[6];
// extern uchar time_data[6];// ½ÓÊÕÊ±ÖÓÊı¾İ»º³å
bool CompareTime_Flag = false;
extern uchar time_Current[6];			// 7/12		¼ÇÂ¼ÊµÊ±Ê±¼ä

void ConvMillisToDateTime_C(long long millis) {
    time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);

	time_Current[0] = (timeinfo->tm_year + 1900)%100;
	time_Current[1] = timeinfo->tm_mon + 1;
	time_Current[2] = timeinfo->tm_mday;
	time_Current[3] = timeinfo->tm_hour + 8;
	time_Current[4] = timeinfo->tm_min;
	time_Current[5] = timeinfo->tm_sec;
	DS1302_SetTime(time_Current);
}


void ConvMillisToDateTime_S(long long millis) {
    // ???????
    time_t seconds = millis / 1000;
    // long long seconds_2 = millis / 1000;
	// printf("total seconds: %u\n", seconds);
 
    // ??gmtime??????struct tm??
    // struct tm *timeinfo = gmtime(&seconds);
	struct tm *timeinfo = localtime(&seconds);

	
//	if(timeinfo == NULL)
//		printf("Null\r\n");
    // ????
    printf("year: %d\n", (timeinfo->tm_year + 1900)%100);
	time_Current[0] = (timeinfo->tm_year + 1900)%100;
	// printf("timeinfo:%d\r\n",timeinfo->tm_year + 1900);				// test 7/9	
	// printf("%d\r\n",time_data[0]);				// test 7/9
    printf("month: %d\n", timeinfo->tm_mon + 1);
	time_Current[1] =  timeinfo->tm_mon + 1;
    printf("day: %d\n", timeinfo->tm_mday);
	time_Current[2] = timeinfo->tm_mday;
    printf("hour: %d\n", timeinfo->tm_hour + 8);
	time_Current[3] = timeinfo->tm_hour + 8;
    printf("min: %d\n", timeinfo->tm_min);
	time_Current[4] = timeinfo->tm_min;
    printf("sec: %d\n", timeinfo->tm_sec);
	time_Current[5] = timeinfo->tm_sec;
	// DS1302_SetTime(time_data);

}

void ConvMillisToDateTime_E(long long millis){
	time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);
	// End_time[0] = timeinfo->tm_year + 1900;
	End_time[0] = (timeinfo->tm_year + 1900)%100;
	End_time[1] = timeinfo->tm_mon + 1;
	End_time[2] = timeinfo->tm_mday;
	End_time[3] = timeinfo->tm_hour + 8;
	End_time[4] = timeinfo->tm_min;
	End_time[5] = timeinfo->tm_sec;
	
	
	
	// Ã¿´ÎĞ´ÈëÇ°²Á³ıÉÈÇøÊı¾İ
	// W25Q64_SectorErase(0);			// Ö®Ç°Ğ´½øÈ¥µÄÓÖ²Á³ıµôÁË..... 8/19   ÏÈ²Á³ı ÔÙĞ´Èë±êÖ¾Î»
	
	// ½«½ØÖ¹Ê±¼äĞ´Èëµ½FlashÖĞ				8/18
	for(int i = 0;i < 6;i++){	
		W25Q64_PageProgram_SingleInt((0x000000 | ((i+1)<<8)),End_time[i]);	
	}
	
	
	
	CompareTime_Flag = true;
	
	/* ÔÚ¿ªÃÅÊ±¼äÄÚ ¿ª¹Ø¿ØÖÆrelay²»Æğ×÷ÓÃ Ö±µ½¿ªÃÅÊ±¼ä½áÊø 7/13 */
//				EXTI_InitTypeDef EXTI_InitStructure;
//				EXTI_InitStructure.EXTI_Line = EXTI_Line15;
//				EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		// ÉÏÉıÑØ´¥·¢
//				EXTI_InitStructure.EXTI_LineCmd = DISABLE;
//				EXTI_Init(&EXTI_InitStructure);

}
void Store_Left3Min_TimeStamp(long long millis){
	uint8_t Left3End[6];
	time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);
	// End_time[0] = timeinfo->tm_year + 1900;
	Left3End[0] = (timeinfo->tm_year + 1900)%100;
	Left3End[1] = timeinfo->tm_mon + 1;
	Left3End[2] = timeinfo->tm_mday;
	Left3End[3] = timeinfo->tm_hour + 8;
	Left3End[4] = timeinfo->tm_min;
	Left3End[5] = timeinfo->tm_sec;
	
	
	
	// Ã¿´ÎĞ´ÈëÇ°²Á³ıÉÈÇøÊı¾İ
	// W25Q64_SectorErase(0);			// Ö®Ç°Ğ´½øÈ¥µÄÓÖ²Á³ıµôÁË..... 8/19   ÏÈ²Á³ı ÔÙĞ´Èë±êÖ¾Î»
	
	// ½«½ØÖ¹Ê±¼äĞ´Èëµ½FlashÖĞ				8/18
	for(int i = 0;i < 6;i++){	
		W25Q64_PageProgram_SingleInt((0x001000 | ((i+7)<<8)),Left3End[i]);	
	}
}

void Store_Left15Min_TimeStamp(long long millis){
	uint8_t Left15End[6];
	time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);
	// End_time[0] = timeinfo->tm_year + 1900;
	Left15End[0] = (timeinfo->tm_year + 1900)%100;
	Left15End[1] = timeinfo->tm_mon + 1;
	Left15End[2] = timeinfo->tm_mday;
	Left15End[3] = timeinfo->tm_hour + 8;
	Left15End[4] = timeinfo->tm_min;
	Left15End[5] = timeinfo->tm_sec;
	
	
	
	// Ã¿´ÎĞ´ÈëÇ°²Á³ıÉÈÇøÊı¾İ
	// W25Q64_SectorErase(0);			// Ö®Ç°Ğ´½øÈ¥µÄÓÖ²Á³ıµôÁË..... 8/19   ÏÈ²Á³ı ÔÙĞ´Èë±êÖ¾Î»
	
	// ½«½ØÖ¹Ê±¼äĞ´Èëµ½FlashÖĞ				8/18
	for(int i = 0;i < 6;i++){	
		W25Q64_PageProgram_SingleInt((0x001000 | ((i+1)<<8)),Left15End[i]);	
	}
}



void Store_Second_TimeStamp(long long millis){
	uint8_t SecondEnd[6];
	time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);
	// End_time[0] = timeinfo->tm_year + 1900;
	SecondEnd[0] = (timeinfo->tm_year + 1900)%100;
	SecondEnd[1] = timeinfo->tm_mon + 1;
	SecondEnd[2] = timeinfo->tm_mday;
	SecondEnd[3] = timeinfo->tm_hour + 8;
	SecondEnd[4] = timeinfo->tm_min;
	SecondEnd[5] = timeinfo->tm_sec;
	
	
	
	// Ã¿´ÎĞ´ÈëÇ°²Á³ıÉÈÇøÊı¾İ
	// W25Q64_SectorErase(0);			// Ö®Ç°Ğ´½øÈ¥µÄÓÖ²Á³ıµôÁË..... 8/19   ÏÈ²Á³ı ÔÙĞ´Èë±êÖ¾Î»
	
	// ½«½ØÖ¹Ê±¼äĞ´Èëµ½FlashÖĞ				8/18
	for(int i = 0;i < 6;i++){	
		W25Q64_PageProgram_SingleInt((0x000000 | ((i+7)<<8)),SecondEnd[i]);	
	}
}


bool CompareTime(void){
#ifdef Debug_ComapreTime
	printf("\r\n");
	printf("time_Current_Y: %d\tEnd_time:%d\r\n", time_Current[0],End_time[0]);
	printf("time_Current_M: %d \tEnd_time:%d\r\n",time_Current[1],End_time[1]);
	printf("time_Current_D: %d\tEnd_time:%d\r\n", time_Current[2],End_time[2]);
	printf("time_Current_H: %d\tEnd_time:%d\r\n", time_Current[3],End_time[3]);
	printf("time_Current_M: %d\tEnd_time:%d\r\n", time_Current[4],End_time[4]);
	printf("time_Current_S: %d\tEnd_time:%d\r\n", time_Current[5],End_time[5]);

	
	
//	for(int i = 0;i < 6;i++){
//		printf("time_Current: %d\tEnd_time:%d\r\n",time_Current[i],End_time[i]);
//	}
#endif	
	
//	if(time_Current[0] >= End_time[0]){
//		if(time_Current[1] >= End_time[1]){
//			if(time_Current[2] >= End_time[2]){
//				if(time_Current[3] >= End_time[3]){
//					if(time_Current[4] >= End_time[4]){
//						if(time_Current[5] >= End_time[5]){
//							return true;
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
if(en_judge_IfLightEnd == false){	
	for(int i = 0;i < 6;i++){
		if(time_Current[i] >= End_time[i]){
			if(i == 5){
				
			// JR6001_play_finish
			JR6001_Play_Service_Finish();	
			Delay_ms(300);
			while(My_JR6001_IsBusy() == false)
				// Waiting...
				;				
				
				
				
				en_judge_IfLightEnd = true;	
				// ²Á³öµ±Ç°Êı¾İ,(×¢Òâ:Èô±£´æÎ¨Ò»Ê¶±ğÂë,×¢ÒâÉÈÇøÎ»ÖÃ)		ÒÑ¾­ÔÚ×ª»»º¯ÊıÖĞ½øĞĞÁËÌáÇ°²Á³ı	8/18
				// W25Q64_SectorErase(0);
				W25Q64_WriteFlag_SocketOff();
				// printf("%lld\n",en_judge_IfLightEnd_val);
				// ConvMillisToDateTime_E(en_judge_IfLightEnd_val);	
				// ¸üĞÂµÚ¶ş¸öEndÊ±¼,Ò²¾ÍÊÇ¹ØµÆ¹ØÃÅÊ±¼ä  ´ÓFlash¶ÁÈ¡
				for(int i = 0;i < 6;i++){	
					End_time[i] = W25Q64_ReadData_SingleInt((0x000000 | ((i+7)<<8)));	
				}
				
			
				
				return true;
			}
		}
		else return false;
	} 
	return false;
}

if(en_judge_IfLightEnd == true){
	printf("\r\nThe above are lighting fixtures\r\n");
	for(int i = 0;i < 6;i++){
		if(time_Current[i] >= End_time[i]){
			if(i == 5){
				
				// Ê±¼äµ½ÁË ²Á³ıÉÈÇøÊı¾İ
				W25Q64_SectorErase(0);
				W25Q64_SectorErase(0x001000);

				return true;
			}
		}
		else return false;
	}
}




return false;			// ´ËĞĞÓÀÔ¶¶¼²»»áµ½´ï. ¿¼ÂÇÔõÃ´ÓÅ»¯
}
