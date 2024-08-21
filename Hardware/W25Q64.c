#include "stm32f10x.h"                  // Device header
#include "SPI_W25.h"
#include "W25Q64_Ins.h"
#include "stdbool.h"

extern bool CompareTime_Flag;
extern bool en_judge_IfLightEnd;			// 判断是否进入到判断灯光熄灭的条件 (照明灯延长30s关闭)
extern unsigned char End_time[6];

void W25Q64_Init(void)
{
	MySPI_Init();
}





void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
void W25Q64_PageProgram_SingleInt(uint32_t Address, uint8_t DataInt)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_SwapByte(DataInt);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
uint32_t W25Q64_ReadData_SingleInt(uint32_t Address)
{
	uint32_t ReadDataInt;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	ReadDataInt = MySPI_SwapByte(W25Q64_DUMMY_BYTE);

	MySPI_Stop();
	return ReadDataInt;
}

void W25Q64_WriteFlag_SocketOff(void){
	W25Q64_PageProgram_SingleInt(0x000000,0);
}

void W25Q64_WriteFlag_SocketOn(void){
	W25Q64_PageProgram_SingleInt(0x000000,1);
}
void W25Q64_Init_Scan(void)
{
	uint8_t temp;
	// 扫描第0页的数据,若为FF,则没有数据,若为0x00,则为All Socket Off, 若为0x01, 则为All Socket On
	temp = W25Q64_ReadData_SingleInt(0x000000);
	if(temp == 0xff) return;
		
	switch(temp){
		case 1:
			/* 引脚置位 */
			for(int i = 0;i < 6;i++){
				End_time[i] = W25Q64_ReadData_SingleInt(0x000000 | ((i+1)<<8));
			}
			
			
			CompareTime_Flag = true;
			en_judge_IfLightEnd = false;
			return;
		case 0:
			/* 引脚置位 */
			for(int i = 0;i < 6;i++){
				End_time[i] = W25Q64_ReadData_SingleInt(0x000000 | ((i+7)<<8));
			}			
			CompareTime_Flag = true;
			en_judge_IfLightEnd = true;
			return;
	}
}
