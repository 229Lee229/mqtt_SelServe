#ifndef __CONVERTJSONTIME_H
#define __CONVERTJSONTIME_H

void ConvMillisToDateTime_C(long long millis);
void ConvMillisToDateTime_S(long long millis);
void ConvMillisToDateTime_E(long long millis);
bool CompareTime(void);


void Store_Second_TimeStamp(long long millis);
void Store_Left3Min_TimeStamp(long long millis);
void Store_Left15Min_TimeStamp(long long millis);


#endif
