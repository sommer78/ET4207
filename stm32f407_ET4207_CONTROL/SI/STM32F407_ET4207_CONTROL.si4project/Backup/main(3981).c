#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"  
#include "malloc.h" 
#include "ET4207.h" 

#include "myiic.h"





//STM32F4工程模板-库函数版本



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
			//	etcode[t] = t;
				}

		//ET4207SendCode(etcode,440);
		//ET4207SendTest();
		//ET4207StartLearn(0,0);
		ET4207StartLearnREC();
		}   
		if(key==KEY0_PRES)//KEY0按下了
		{
	for(t=0;t<512;t++){
		etcode[t] = 0xff;
		}
		ET4207ReadCode(etcode);
		//ET4207ReadVersion(etcode);
			
		
		
		}   
		t++;
		delay_ms(10);
	LED0=1;
		
	if(t==20)
		{
		//	LED0=!LED0;
			LED1=!LED1;
		
			t=0;
		}
	} 
}





