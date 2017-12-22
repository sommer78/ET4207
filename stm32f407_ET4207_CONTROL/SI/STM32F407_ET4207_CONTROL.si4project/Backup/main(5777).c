#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"  
#include "malloc.h" 
#include "ET4207.h" 

#include "myiic.h"





//STM32F4工程模板-库函数版本

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void I2C_WriteByteS(u8 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{	
	u8 t;

    IIC_Start();  
	
	IIC_Send_Byte(0XE0);	    //发送写命令 	 
	IIC_Wait_Ack();	
	 IIC_Send_Byte(WriteAddr);   //发送地址
	IIC_Wait_Ack(); 	
	for(t=0;t<NumToWrite;t++)
	{
	IIC_Send_Byte(*pBuffer);
	IIC_Wait_Ack(); 
	pBuffer++;
	}	
    										  		   		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}


u32 FLASH_SIZE;



int main(void)
{        
	u8 key;		 
	u16 t=0;	
	u8 etcode[512];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	KEY_Init();					//按键初始化  
	
	
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池


	
	ET4207_Init();

	//IIC_Init();


	
	
	
	while(1)
	{
	key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY0按下了
		{
		printf("START LEARN.......\r\n");
		for(t=0;t<512;t++){
				etcode[t] = t;
				}

		//ET4207SendCode(etcode,440);
		
		ET4207StartLearn();
		}   
		if(key==KEY0_PRES)//KEY0按下了
		{
	for(t=0;t<512;t++){
		etcode[t] = 0xff;
		}
		ET4207ReadCode(etcode);
		//ET4207ReadVersion(etcode);
			
		for(t=0;t<440;t++){
			printf("0x%02x,",etcode[t]);
		}
		printf("\r\n");
		
		}   
		t++;
		delay_ms(10);
	LED0=0;
		
	if(t==20)
		{
		//	LED0=!LED0;
			LED1=!LED1;
		
			t=0;
		}
	} 
}





