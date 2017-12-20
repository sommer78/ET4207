#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"  
#include "malloc.h" 
#include "ET4207.h" 




//STM32F4����ģ��-�⺯���汾




u32 FLASH_SIZE;



int main(void)
{        
	u8 key;		 
	u8 t=0;	
	u8 etcode[512];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
 	KEY_Init();					//������ʼ��  
	
	
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��


	
	ET4207_Init();



	for(t=0;t<128;t++){
		etcode[t] = t;
		}
	
	
	while(1)
	{
	key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0������
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





