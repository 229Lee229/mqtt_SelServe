#include "My_include.h"
#include "time.h"
extern uchar init_time[6];		// 7/8 接收json后转换时间
// 查看年月日时分秒代码 未处理
//void convertMillisToDateTime(long long millis) {
//    // ???????
//    time_t seconds = millis / 1000;
//    // long long seconds_2 = millis / 1000;
//	// printf("total seconds: %u\n", seconds);
// 
//    // ??gmtime??????struct tm??
//    // struct tm *timeinfo = gmtime(&seconds);
//	struct tm *timeinfo = localtime(&seconds);

//	
////	if(timeinfo == NULL)
////		printf("Null\r\n");
//    // ????
//    printf("year: %d\n", timeinfo->tm_year + 1900);
//    printf("month: %d\n", timeinfo->tm_mon + 1);
//    printf("day: %d\n", timeinfo->tm_mday);
//    printf("hour: %d\n", timeinfo->tm_hour + 8);
//    printf("min: %d\n", timeinfo->tm_min);
//    printf("sec: %d\n", timeinfo->tm_sec);
//}



void convertMillisToDateTime(long long millis) {
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
    printf("year: %d\n", timeinfo->tm_year + 1900);
	init_time[0] = timeinfo->tm_year + 1900;
    printf("month: %d\n", timeinfo->tm_mon + 1);
	init_time[1] =  timeinfo->tm_mon + 1;
    printf("day: %d\n", timeinfo->tm_mday);
	init_time[2] = timeinfo->tm_mday;
    printf("hour: %d\n", timeinfo->tm_hour + 8);
	init_time[3] = timeinfo->tm_hour + 8;
    printf("min: %d\n", timeinfo->tm_min);
	init_time[4] = timeinfo->tm_min;
    printf("sec: %d\n", timeinfo->tm_sec);
	init_time[5] = timeinfo->tm_sec;
}

