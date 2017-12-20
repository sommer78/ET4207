#include "ET4207.h" 
#include "delay.h" 				 
//////////////////////////////////////////////////////////////////////////////////	 

//24CXX 驱动代码	   
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IIC接口
void ET4207_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    RCC_ClocksTypeDef   rcc_clocks;

    /* GPIO Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
      /* Reset I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* Release reset signal of I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    /*I2C1 configuration*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    //PB6: I2C1_SCL  PB7: I2C1_SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C Struct Initialize */
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* I2C Initialize */
    I2C_Cmd(I2C1, ENABLE);


	//I2C_AcknowledgeConfig(I2C1, ENABLE); 
	


  

}

u8 ET4207SendCode(u8 *etcode,int length){
	
	u8 err=0;
	Hard_IIC_WriteNByte(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_SEND_,length,etcode,&err);
	return err;

}


/**
  *****************************************************************************
  * @Name   : 硬件IIC发送多个字节数据
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:       IIC组
  *           SlaveAdd:   作为从设备时识别地址
  *           WriteAdd:   写入EEPROM内存起始地址
  *           NumToWrite: 写入数据量
  *           *pBuffer:   写入的数据组缓存
  *
  * @Output : *err:     返回的错误值
  *
  * @Return : none
  *****************************************************************************
**/
void  Hard_IIC_WriteNByte(I2C_TypeDef * IICx, u8 SlaveAdd, u8 WriteAdd, u8 NumToWrite, u8 * pBuffer, u8 * err)
{
	u16 sta = 0;
	u16 temp = 0;
	
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //等待IIC
	{
		temp++;
		if (temp > 800)
		{
			*err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //产生起始信号
	temp = 0;
	//
	//EV5
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		temp++;
		if (temp > 800)
		{
			*err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Transmitter);  //发送设备地址
	temp = 0;
	//
	//EV6
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		temp++;
		if (temp > 1000)
		{
			*err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	//
	//读取SR2状态寄存器
	//
	sta = IICx->SR2;  //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位，不可少！！！！！！！！！
	I2C_SendData(IICx, WriteAdd);  //发送存储地址
	temp = 0;
	//
	//EV8
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		temp++;
		if (temp > 800)
		{
			*err |= 1<<3;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	//
	//循环发送数据
	//
	while (NumToWrite--)
	{
		I2C_SendData(IICx, *pBuffer);  //发送数据
		pBuffer++;
		temp = 0;
		//
		//EV8_2
		//
		while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			temp++;
			if (temp > 800)
			{
				*err |= 1<<4;
				I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
				return;
			}
		}
	}
	I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
}


/**
  *****************************************************************************
  * @Name   : 硬件IIC读取多个字节数据
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:      IIC组
  *           SlaveAdd:  作为从设备时识别地址
  *           ReadAdd:   读取的EEPROM内存起始地址
  *           NumToRead: 读取数量
  *
  * @Output : *pBuffer: 数据输出缓冲区
  *           *err:     返回的错误值
  *
  * @Return : 读取到的数据
  *****************************************************************************
**/
void Hard_IIC_PageRead(I2C_TypeDef* IICx, uint8_t SlaveAdd, u8 ReadAdd, u8 NumToRead, u8 * pBuffer, u8 * err)
{
	u16 i = 0;
	u16 temp = 0;
	u16 sta = 0;
	
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //等待IIC
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //发送起始信号
	i = 0;
	//
	//EV5
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Transmitter);  //发送设备地址
	i = 0;
	//
	//EV6
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	sta = IICx->SR2;  //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位，不可少！！！！！！！！！
	
	I2C_SendData(IICx, ReadAdd);  //发送存储地址
	i = 0;
	//
	//EV8
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		i++;
		if (i > 2000)
		{
			*err |= 1<<3;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //重启信号
	i = 0;
	//
	//EV5
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<4;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Receiver);  //读取命令
	i = 0;
	//
	//EV6
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<5;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return;
		}
	}
	sta = IICx->SR2;  //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位，不可少！！！！！！！！！
	//
	//批量接收数据
	//
//	while (NumToRead--)
//	{
//		if (NumToRead == 1)  //最后一个数据了，不发送应答信号
//		{
//			I2C_AcknowledgeConfig(IICx, DISABLE);  //发送NACK
//			I2C_GenerateSTOP(IICx, ENABLE);
//		}
//		//
//		//判断RxNE是否为1，EV7
//		//
//		i = 0;
//		while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		{
//			i++;
//			if (i > 1000)
//			{
//				*err |= 1<<6;
//				I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
//				return;
//			}
//		}
//		*pBuffer = I2C_ReceiveData(IICx);
//		pBuffer++;
//	}
	while (NumToRead)
	{
		if (NumToRead == 1)  //最后一个数据了，不发送应答信号
		{
			I2C_AcknowledgeConfig(IICx, DISABLE);  //发送NACK
			I2C_GenerateSTOP(IICx, ENABLE);
		}
		//
		//判断RxNE是否为1，EV7
		//
		if (I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*pBuffer = I2C_ReceiveData(IICx);
			pBuffer++;
			NumToRead--;
		}
	}
	I2C_AcknowledgeConfig(IICx, ENABLE);
}







