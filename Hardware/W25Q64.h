#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_PageProgram_SingleInt(uint32_t Address, uint8_t DataInt);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
uint32_t W25Q64_ReadData_SingleInt(uint32_t Address);

void W25Q64_Init_Scan(void);
void W25Q64_WriteFlag_SocketOff(void);
void W25Q64_WriteFlag_SocketOn(void);

#endif
