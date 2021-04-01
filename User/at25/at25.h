#ifndef __AT25_H
#define __AT25_H

#include "stm32f10x.h"
#include "systick.h"
#include "fonts.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "keys.h"

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x10000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256


//AT25芯片指令
/*命令定义-开头*******************************/
#define AT25_WREN		              0x06 
#define AT25_WRDI		              0x04 
#define AT25_RDSR		              0x05 
#define AT25_WRSR		              0x01
#define AT25_Read			            0x03
#define AT25_Write		            0x02
#define AT25_SectorErase          0x52
#define AT25_ChipErase            0x62


extern void SPI_AT25_Write(u16* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
extern void SPI_AT25_Read(u16* pBuffer, u8 ReadAddr, u16 NumByteToRead);
extern void SPI_Save_Word(u16 data,u8 addr);
extern u16 SPI_Load_Word(u8 addr);



extern void	SPI_AT25_Unlock(void);
extern void AT25_Load_Data(void);
extern void AT25_Save_RecallSaveSet(u8 Number);
extern void AT25_Save_VIPSet(void);
extern void AT25_Save_RS232(void);
extern void AT25_Save_CAN(void);
extern void AT25_Save_Delay(void);
extern void AT25_Save_Cycle(void);
extern void AT25_Save_CC_CV_CP(void);
extern void AT25_Reset(void);
extern void AT25_Save_V_Rise(void);
extern void AT25_Save_V_Fall(void);
extern void AT25_Save_I_Rise(void);
extern void AT25_Save_I_Fall(void);
extern void AT25_Save_Use_Vint_or_Vext_Flag(void);



#endif /*__AT25_H*/



