#include "ET4207.h" 
#include "delay.h" 				 
//////////////////////////////////////////////////////////////////////////////////	 

//24CXX ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC�ӿ�
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
  * @Name   : Ӳ��IIC���Ͷ���ֽ�����
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:       IIC��
  *           SlaveAdd:   ��Ϊ���豸ʱʶ���ַ
  *           WriteAdd:   д��EEPROM�ڴ���ʼ��ַ
  *           NumToWrite: д��������
  *           *pBuffer:   д��������黺��
  *
  * @Output : *err:     ���صĴ���ֵ
  *
  * @Return : none
  *****************************************************************************
**/
void  Hard_IIC_WriteNByte(I2C_TypeDef * IICx, u8 SlaveAdd, u8 WriteAdd, u8 NumToWrite, u8 * pBuffer, u8 * err)
{
	u16 sta = 0;
	u16 temp = 0;
	
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //�ȴ�IIC
	{
		temp++;
		if (temp > 800)
		{
			*err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //������ʼ�ź�
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Transmitter);  //�����豸��ַ
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	//
	//��ȡSR2״̬�Ĵ���
	//
	sta = IICx->SR2;  //�����ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	I2C_SendData(IICx, WriteAdd);  //���ʹ洢��ַ
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	//
	//ѭ����������
	//
	while (NumToWrite--)
	{
		I2C_SendData(IICx, *pBuffer);  //��������
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
				I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
				return;
			}
		}
	}
	I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
}


/**
  *****************************************************************************
  * @Name   : Ӳ��IIC��ȡ����ֽ�����
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:      IIC��
  *           SlaveAdd:  ��Ϊ���豸ʱʶ���ַ
  *           ReadAdd:   ��ȡ��EEPROM�ڴ���ʼ��ַ
  *           NumToRead: ��ȡ����
  *
  * @Output : *pBuffer: �������������
  *           *err:     ���صĴ���ֵ
  *
  * @Return : ��ȡ��������
  *****************************************************************************
**/
void Hard_IIC_PageRead(I2C_TypeDef* IICx, uint8_t SlaveAdd, u8 ReadAdd, u8 NumToRead, u8 * pBuffer, u8 * err)
{
	u16 i = 0;
	u16 temp = 0;
	u16 sta = 0;
	
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //�ȴ�IIC
	{
		i++;
		if (i > 800)
		{
			*err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //������ʼ�ź�
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Transmitter);  //�����豸��ַ
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	sta = IICx->SR2;  //�����ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	
	I2C_SendData(IICx, ReadAdd);  //���ʹ洢��ַ
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_GenerateSTART(IICx, ENABLE);  //�����ź�
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	I2C_Send7bitAddress(IICx, SlaveAdd, I2C_Direction_Receiver);  //��ȡ����
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
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return;
		}
	}
	sta = IICx->SR2;  //�����ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	//
	//������������
	//
//	while (NumToRead--)
//	{
//		if (NumToRead == 1)  //���һ�������ˣ�������Ӧ���ź�
//		{
//			I2C_AcknowledgeConfig(IICx, DISABLE);  //����NACK
//			I2C_GenerateSTOP(IICx, ENABLE);
//		}
//		//
//		//�ж�RxNE�Ƿ�Ϊ1��EV7
//		//
//		i = 0;
//		while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		{
//			i++;
//			if (i > 1000)
//			{
//				*err |= 1<<6;
//				I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
//				return;
//			}
//		}
//		*pBuffer = I2C_ReceiveData(IICx);
//		pBuffer++;
//	}
	while (NumToRead)
	{
		if (NumToRead == 1)  //���һ�������ˣ�������Ӧ���ź�
		{
			I2C_AcknowledgeConfig(IICx, DISABLE);  //����NACK
			I2C_GenerateSTOP(IICx, ENABLE);
		}
		//
		//�ж�RxNE�Ƿ�Ϊ1��EV7
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







