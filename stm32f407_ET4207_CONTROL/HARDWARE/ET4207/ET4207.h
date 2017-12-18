#ifndef __24CXX_H
#define __24CXX_H
#include "stm32f4xx_i2c.h"   

#define		_ET4207_CONTROL_SEND_			0x10
#define		_ET4207_CONTROL_SEND_END_			0x20
#define		_ET4207_CONTROL_START_LEARND_REC_		0x30
#define		_ET4207_CONTROL_START_LEARND_RMT_		0x40
#define		_ET4207_CONTROL_STOP_LEARN_			0x50
#define		_ET4207_CONTROL_READ_VERSION_		0x60
#define		_ET4207_CONTROL_READ_CODE_			0x70
#define		_ET4207_CONTROL_CURRENT_SET_			0x80

#define     ET_CMD_START_LEARN_RMT 					5
#define     ET_CMD_START_LEARN_REC 					7
#define     ET_CMD_STOP_LEARN 					6
#define     ET_CMD_REPEAT					10
#define     ET_CMD_VERSION					3
#define     ET_CMD_VERSION_FULL					4
#define     ET_CMD_SET_CURRENT				8

#define ET4207_ADDRESS 0xe0			//		0xe0


					  

void ET4207_Init(void); //≥ı ºªØIIC


u8 ET4207SendCode(u8 *etcode,int length);

void  Hard_IIC_WriteNByte(I2C_TypeDef * IICx, u8 SlaveAdd, u8 WriteAdd, u8 NumToWrite, u8 * pBuffer, u8 * err);


#endif
















