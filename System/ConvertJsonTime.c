#include "My_include.h"
#include "time.h"
extern uchar init_time[6];		// 7/8 接收json后转换时间
uchar End_time[6];
extern uchar time_data[6];// 接收时钟数据缓冲
bool CompareTime_Flag = false;

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
	time_data[0] = (timeinfo->tm_year + 1900)%100;
	// printf("timeinfo:%d\r\n",timeinfo->tm_year + 1900);				// test 7/9	
	printf("%d\r\n",time_data[0]);				// test 7/9
    printf("month: %d\n", timeinfo->tm_mon + 1);
	time_data[1] =  timeinfo->tm_mon + 1;
    printf("day: %d\n", timeinfo->tm_mday);
	time_data[2] = timeinfo->tm_mday;
    printf("hour: %d\n", timeinfo->tm_hour + 8);
	time_data[3] = timeinfo->tm_hour + 8;
    printf("min: %d\n", timeinfo->tm_min);
	time_data[4] = timeinfo->tm_min;
    printf("sec: %d\n", timeinfo->tm_sec);
	time_data[5] = timeinfo->tm_sec;
	DS1302_SetTime(time_data);

}

void ConvMillisToDateTime_E(long long millis){
	time_t seconds = millis / 1000;
	struct tm *timeinfo = localtime(&seconds);
	// End_time[0] = timeinfo->tm_year + 1900;
	End_time[0] = (timeinfo->tm_year + 1900)%100;
	End_time[1] =  timeinfo->tm_mon + 1;
	End_time[2] = timeinfo->tm_mday;
	End_time[3] = timeinfo->tm_hour + 8;
	End_time[4] = timeinfo->tm_min;
	End_time[5] = timeinfo->tm_sec;
	
	CompareTime_Flag = true;
//	printf("\r\n\r\n");
//	printf("year: %d\n", timeinfo->tm_year + 1900);
//    printf("month: %d\n", timeinfo->tm_mon + 1);
//    printf("day: %d\n", timeinfo->tm_mday);
//    printf("hour: %d\n", timeinfo->tm_hour + 8);
//    printf("min: %d\n", timeinfo->tm_min);
//    printf("sec: %d\n", timeinfo->tm_sec);

}

bool CompareTime(void){
	for(int i = 0;i < 6;i++){
		printf("time_data: %d\tEnd_time:%d\r\n",time_data[i],End_time[i]);
		if(time_data[i] >= End_time[i]){
			// printf("time_data: %d\tEnd_time:%d\r\n",time_data[i],End_time[i]);
			// printf("test:%d\r\n",i);
			if(i == 5)
				return true;
		}
		else return false;
	}
	return false;
}

