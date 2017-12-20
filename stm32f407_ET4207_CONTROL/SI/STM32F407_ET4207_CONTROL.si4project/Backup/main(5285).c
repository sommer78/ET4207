#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"  
#include "malloc.h" 
#include "ET4207.h" 




//STM32F4工程模板-库函数版本




u32 FLASH_SIZE;



int main(void)
{        
	u8 key;		 
	u8 t=0;	
	u8 etcode[512];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	KEY_Init();					//按键初始化  
	
	
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池


	
	ET4207_Init();



	for(t=0;t<128;t++){
		etcode[t] = t;
		}
	
	
	while(1)
	{
	key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下了
		{
		printf("KEY PUSH OK.......\r\n");
		ET4207SendCode(etcode,128);
		
		}   
		t++;
		delay_ms(10);
	
		
	if(t==20)
		{
		//	LED0=!LED0;
			LED1=!LED1;
		
			t=0;
		}
	} 
}





