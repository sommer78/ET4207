/*
 * linux/drivers/char/etek_irremote.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/poll.h>
#include <linux/fb.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include<linux/timer.h> 
#include<linux/jiffies.h>

#include "et4007.h"


#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/io.h>
#include  <linux/string.h>


#define DEVICE_NAME				"irremote"

//#define ET_POWERCONTROL

#define		_ET4007_ADDRESS_					0x35

#define		_ET4007_CONTROL_SEND_CODE_3_		0x54

#define		_ET4007_CONTROL_START_LEARND_		0x57
#define		_ET4007_CONTROL_READ_VERSION_			0x5c
#define		_ET4007_CONTROL_READ_CODE_			0x5d
#define		_ET4007_CONTROL_STOP_LEARN_			0x5e

#define     ET_CMD_START_LEARN 					0
#define     ET_CMD_STOP_LEARN 					10
#define     ET_CMD_REPEAT					2
#define     ET_CMD_VERSION					3

#define PLAT_EXYNOS



#ifdef  PLAT_EXYNOS
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

#define ET4007_SCL_GPIO			    EXYNOS4_GPB(0)
#define ET4007_SDA_GPIO			    EXYNOS4_GPB(3)
#define ET4007_BUSY_GPIO			EXYNOS4_GPB(2)
#define ET4007_SET_SCL_OUTPUT()		s3c_gpio_cfgpin(ET4007_SCL_GPIO, S3C_GPIO_OUTPUT)
#define ET4007_SET_SDA_INPUT()		s3c_gpio_cfgpin(ET4007_SDA_GPIO, S3C_GPIO_INPUT)
#define ET4007_SET_SDA_OUTPUT()		s3c_gpio_cfgpin(ET4007_SDA_GPIO, S3C_GPIO_OUTPUT)
#define ET4007_SET_BUSY_INPUT()		s3c_gpio_cfgpin(ET4007_BUSY_GPIO, S3C_GPIO_INPUT)

#define ET4007_GET_SDA_STATE()		gpio_get_value(ET4007_SDA_GPIO)
#define ET4007_GET_BUSY_STATE()			gpio_get_value(ET4007_BUSY_GPIO)

#define ET4007_SET_SCL_LOW()			gpio_set_value(ET4007_SCL_GPIO, 0)
#define ET4007_SET_SCL_HIGH()			gpio_set_value(ET4007_SCL_GPIO, 1)
#define ET4007_SET_SDA_LOW()			gpio_set_value(ET4007_SDA_GPIO, 0)
#define ET4007_SET_SDA_HIGH()			gpio_set_value(ET4007_SDA_GPIO, 1)


#endif


#ifdef   PLAT_MTK_MT6572
#include <mach/mt_gpio.h>

#define ET4007_SCL_GPIO			(GPIO45|0x80000000)
#define ET4007_SDA_GPIO			(GPIO44|0x80000000)
#define ET4007_BUSY_GPIO		(GPIO59|0x80000000)
#define ET4007_POWER_GPIO		(GPIO53|0x80000000)


#define ET4007_SET_SCL_OUTPUT()	      \
{\
    mt_set_gpio_mode(ET4007_SCL_GPIO, GPIO_MODE_00);  \
    mt_set_gpio_dir(ET4007_SCL_GPIO, GPIO_DIR_OUT);\
}
#define ET4007_SET_SDA_INPUT()		\
{\
    mt_set_gpio_mode(ET4007_SDA_GPIO, GPIO_MODE_00)  ;\
    mt_set_gpio_dir(ET4007_SDA_GPIO, GPIO_DIR_IN);\
    mt_set_gpio_ies(ET4007_SDA_GPIO,true);	\
}
#define ET4007_SET_SDA_OUTPUT()		\
{\
    mt_set_gpio_mode(ET4007_SDA_GPIO, GPIO_MODE_00);  \
    mt_set_gpio_dir(ET4007_SDA_GPIO, GPIO_DIR_OUT);\
}
#define ET4007_SET_BUSY_INPUT()\
{\
    mt_set_gpio_mode(ET4007_BUSY_GPIO, GPIO_MODE_00);  \
    mt_set_gpio_dir(ET4007_BUSY_GPIO, GPIO_DIR_IN);\
    mt_set_gpio_ies(ET4007_BUSY_GPIO,true);		\
}

#define ET4007_SET_POWER_OUTPUT()		\
{\
    mt_set_gpio_mode(ET4007_POWER_GPIO, GPIO_MODE_00);  \
    mt_set_gpio_dir(ET4007_POWER_GPIO, GPIO_DIR_OUT);\
    
}

#define ET4007_GET_SDA_STATE()		        mt_get_gpio_in(ET4007_SDA_GPIO)
#define ET4007_GET_BUSY_STATE()			mt_get_gpio_in(ET4007_BUSY_GPIO)

#define ET4007_SET_SCL_LOW()			mt_set_gpio_out(ET4007_SCL_GPIO, 0)
#define ET4007_SET_SCL_HIGH()			mt_set_gpio_out(ET4007_SCL_GPIO, 1)
#define ET4007_SET_SDA_LOW()			mt_set_gpio_out(ET4007_SDA_GPIO, 0)
#define ET4007_SET_SDA_HIGH()			mt_set_gpio_out(ET4007_SDA_GPIO, 1)
#define ET4007_SET_POWER_HIGH()			mt_set_gpio_out(ET4007_POWER_GPIO, 1)
#define ET4007_SET_POWER_LOW()			mt_set_gpio_out(ET4007_POWER_GPIO, 0)
#endif 



#ifdef   PLAT_MSM8610
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

#define ET4007_SCL_GPIO			    101
#define ET4007_SDA_GPIO			    102
#define ET4007_BUSY_GPIO			103
#define ET4007_SET_SCL_OUTPUT()		gpio_tlmm_config(GPIO_CFG(ET4007_SCL_GPIO, 0, GPIO_CFG_OUTPUT,
                                                  GPIO_CFG_NO_PULL, GPIO_CFG_8MA),GPIO_CFG_ENABLE);
#define ET4007_SET_SDA_INPUT()		gpio_tlmm_config(GPIO_CFG(ET4007_SDA_GPIO, 0, GPIO_CFG_INPUT
                                                  GPIO_CFG_NO_PULL, GPIO_CFG_8MA),GPIO_CFG_ENABLE);
#define ET4007_SET_SDA_OUTPUT()		gpio_tlmm_config(GPIO_CFG(ET4007_SDA_GPIO, 0, GPIO_CFG_OUTPUT,
                                                  GPIO_CFG_NO_PULL, GPIO_CFG_8MA),GPIO_CFG_ENABLE);
#define ET4007_SET_BUSY_INPUT()		gpio_tlmm_config(GPIO_CFG(ET4007_BUSY_GPIO, 0, GPIO_CFG_INPUT
                                                  GPIO_CFG_NO_PULL, GPIO_CFG_8MA),GPIO_CFG_ENABLE);

#define ET4007_GET_SDA_STATE()		gpio_get_value(ET4007_SDA_GPIO)
#define ET4007_GET_BUSY_STATE()			gpio_get_value(ET4007_BUSY_GPIO)

#define ET4007_SET_SCL_LOW()			gpio_direction_output(ET4007_SCL_GPIO, 0)
#define ET4007_SET_SCL_HIGH()			gpio_direction_output(ET4007_SCL_GPIO, 1)
#define ET4007_SET_SDA_LOW()			gpio_direction_output(ET4007_SDA_GPIO, 0)
#define ET4007_SET_SDA_HIGH()			gpio_direction_output(ET4007_SDA_GPIO, 1)
#endif


static DECLARE_WAIT_QUEUE_HEAD(remote_waitq);

static volatile int busy_status = 0;


struct timer_list remote_timer;


static struct ir_remocon_data	*ir_data;

 char *remoteBuf;

 int remoteSize;
 
 int remoteFlag;

struct class *etek_class;

	
/******************************************************/
/*Funcation: Binary2Char                      	        				*/
/*Input:  	binary - 数值数组，输入要转换的数值； 
	      len - binary数组里的数值个数（以字节为单位）； 
	     buff - 字符数组，输出转换后的字符； 
	      size - buff数组的空间（以字节为单位）。 		*/
/*Output: 	true or false									 */

/******************************************************/
int Binary2Char( unsigned char*  binary,  int len, char*  buff,  int size)   
	{  
	    int         i, n;  
	  
	    n = len * 2;  
	  
	    if(n > size){
			printk("Binary2Char data large than array size");
			return 0;
	    	}
	  
	    for (i = 0; i < len; i++)  
	            sprintf(buff + i * 2, "%02X", binary[i]); 
	  
	    return  n;  
	}  
/******************************************************/
/*Funcation: Char2Binary                      	        				*/
/*Input:  	binary - 数值数组，输入要转换的数值； 
	      len - binary数组里的数值个数（以字节为单位）； 
	     buff - 字符数组，输出转换后的字符； 
	      size - buff数组的空间（以字节为单位）。 		*/
/*Output: 	true or false									 */

/******************************************************/
int Char2Binary(const char* token,  int len, unsigned char*  binary,  int size)   
	{  
	        const char*     p;  
	    int         i, n, m;  
	        char        buf[3] = {0,0,0};  
	  
	  
	  
	    m = len % 2 ? (len - 1) / 2 : len / 2;  
	  
	   
		if(m > size){
			printk("Binary2Char data large than array size");
			return 0;
	    	}
	       p = token;
	    // 为了提高效率，先两个两个字符地转换：  
	        for (i = 0; i < m; i++)  
	        {  
	        p = token + i * 2;  
	                buf[0] = p[0];  
	                buf[1] = p[1];  
	  
	                n = 0;  
	                sscanf(buf, "%X", &n);  
	                binary[i] = n;  
	        }  
	  
	    // 再转换最后一个字符（如果有）：  
	    if(len % 2)  
	    {  
	            buf[0] = p[2];  
	            buf[1] = 0;  
	            n = 0;  
	            sscanf(buf, "%X", &n);  
	            binary[i] = n;  
	        i++;  
	    }  
	  
	    return  i;  
	}  



/******************************************************/
/*Funcation: ET4007 TIMER HANDLE	                        	        */
/*Input:  	unsigned long _data			   */
/*Output: 					   						   */
/*Desc: 	get busy state according to time base				   */

/******************************************************/



static void remote_timer_handle(unsigned long _data)
{
	 if (!busy_status){
		mod_timer(&remote_timer,jiffies+msecs_to_jiffies(100)); 
		 
		if (!ET4007_GET_BUSY_STATE()) {
			busy_status = 1;
	//		printk("ET4007 wake up interruptiable remote watiq \n");
			wake_up_interruptible(&remote_waitq);
		}
	 }
}


/******************************************************/
/*Funcation: ET4007 GPIO I2C start bit	                                */
/*Input: 		   						   */
/*Output: 					   						   */
/******************************************************/

void ET4007_start(void)
{
	ET4007_SET_SDA_OUTPUT();
	ET4007_SET_SDA_HIGH();
	ET4007_SET_SCL_OUTPUT();
	ET4007_SET_SCL_HIGH();
	ET4007_SET_SDA_LOW();
	udelay(200);
	udelay(200);
	udelay(200);
	ET4007_SET_SCL_LOW();
	udelay(200);
	udelay(200);
}
/******************************************************/
/*Funcation: ET4007 GPIO I2C stop bit	                                */
/*Input: 		   						   */
/*Output: 					   						   */
/******************************************************/

void ET4007_stop(void)
{
	udelay(200);
	ET4007_SET_SDA_LOW();
	ET4007_SET_SDA_OUTPUT();
	udelay(200);
	ET4007_SET_SCL_HIGH();
	udelay(200);
	ET4007_SET_SDA_HIGH();
	udelay(200);
//	ET4007_SET_SDA_INPUT();
	udelay(200);
}


/******************************************************/
/*Funcation: ET4007 GPIO I2C writebyte	                                */
/*Input: 	uint8_t dat			   						   */
/*Output: 					   						   */
/******************************************************/

uint8_t ET4007_writebyte(uint8_t dat)  //写一个字节
{
	uint8_t i,dat_temp,err;
	
	err=0;
	dat_temp=dat;
	ET4007_SET_SDA_OUTPUT();
	for( i = 0; i != 8; i++ )
	{
		udelay(10);
		if( dat_temp & 0x80 ) 
		{
			ET4007_SET_SDA_HIGH();
		}
		else 
		{
			ET4007_SET_SDA_LOW();
		}
		udelay(50);
		ET4007_SET_SCL_HIGH();
		dat_temp <<= 1;
		udelay(50);  // 可选延时
		ET4007_SET_SCL_LOW();
		udelay(10);
	}
	ET4007_SET_SDA_HIGH();
	ET4007_SET_SDA_INPUT();
	udelay(50);
	ET4007_SET_SCL_HIGH();
	udelay(50);
	//if(ET4007_GET_SDA_STATE())err=1;
	//else err=0;
	ET4007_SET_SCL_LOW();
	udelay(50);
	return err;
}



/******************************************************/
/*Funcation: ET4007 GPIO I2C readbyte	                                */
/*Input: 					   							   */
/*Output: 	one byte 			   						   */
/******************************************************/

uint8_t ET4007_readbyte(void)
{
	uint8_t dat,i;
	ET4007_SET_SDA_HIGH();
	ET4007_SET_SDA_INPUT();
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		udelay(50); 
		ET4007_SET_SCL_HIGH();
		udelay(50); 
		dat <<= 1;
		if( ET4007_GET_SDA_STATE() ) dat++;
		ET4007_SET_SCL_LOW();
		udelay(10);
	}
	ET4007_SET_SDA_OUTPUT();
	ET4007_SET_SDA_LOW();
	udelay(50);
	ET4007_SET_SCL_HIGH();
	udelay(50);
	ET4007_SET_SCL_LOW();
	udelay(50);
	return dat;
}


/******************************************************/
/*Funcation: read ET4007yd18 version code					   */
/*Input: 					   							   */
/*Output: 	ir_version[] version number					   */
/******************************************************/

static int ET_IR_read_device_info(char* ir_version)
{
	
	int ret = 0;
	int i;
//	printk(KERN_INFO"%s called\n", __func__);
	//char buf_ir[8];

	
	ET4007_start(); 
	ET4007_writebyte(_ET4007_ADDRESS_);
	ET4007_writebyte(_ET4007_CONTROL_READ_VERSION_);
	for(i=0;i<4;i++){
		ir_version[i]=ET4007_readbyte();
		}
	ET4007_stop();
	


//	printk(KERN_INFO "%s: dev_id: 0x%02x, 0x%02x, 0x%02x, 0x%02x \n", __func__,
	//			ir_version[0], ir_version[1],ir_version[2],ir_version[3]);
	ret = ir_version[0]*16777216 + ir_version[1]*65536 + ir_version[2]*256+ ir_version[3];
	
	return ret;
}

/******************************************************/
/*Funcation: read ET4007 send code					   */
/*Input: 	  char	buffer[] int len			   				   */
/*Output: 	ir_version[] version number					   */
/******************************************************/
static int ET_IR_send_code(char* buffer, int len)
{
	
	int ret = 0;
	int i;
//	printk(KERN_INFO"%s called\n", __func__);
	//char buf_ir[8];

	
	if (len < 10) {
		return -1;
	}
	if(buffer[0]==_ET4007_CONTROL_SEND_CODE_3_)
	{
		if (len>384){
		printk("ET4007---count error---\n");
		return -2;
		}
		ET4007_start();
		ET4007_writebyte(_ET4007_ADDRESS_);
		for(i=0;i<len;i++)
		{
		ET4007_writebyte(buffer[i]);
		}
		ET4007_stop();
		return ret;
		
	}else {
		printk("ET4007---command  error---\n");
		return -3;
	}
	
	return ret;
}



/******************************************************/
/*Funcation: read ET4007 send code					   */
/*Input: 	  char	buffer[] 	   				   */
/*Output: 	buffer[]  int len				   */
/******************************************************/
static int ET_IR_read_code(char* buffer)
{
	
	
	int i;

	int temp;
	int ET_PartIndexCount;
	int ET_Sample;
	int ET_Index;
	int m = 0;
	int max = 0;

//		int len = 0;
	printk(KERN_INFO"%s called\n", __func__);
	
	
		
	ET4007_start(); 
	ET4007_writebyte(_ET4007_ADDRESS_);
	ET4007_writebyte(_ET4007_CONTROL_READ_CODE_);
	temp = ET4007_readbyte();
	buffer[m++] = temp;
	ET_PartIndexCount = ET4007_readbyte();
	buffer[m++] = ET_PartIndexCount;
	ET_Sample = ET4007_readbyte();
	buffer[m++] = ET_Sample;
	ET_Index = ET4007_readbyte();
	buffer[m++] = ET_Index;
	temp = ET4007_readbyte();
	buffer[m++] = temp;
	max = ET_PartIndexCount+ET_Sample+ET_Index;
	
	if (max>340){
		ET4007_stop();
		return  0;
		}
	for(i=0;i<max ;i++)
		{
		buffer[m++]=ET4007_readbyte();
		
		}
	
	ET4007_stop();


	return m;

}



/******************************************************/
/*Funcation: read ET4007 learn start 					   */
/*Input: 	 	   				   */
/*Output: 					   */
/******************************************************/
static void ET_IR_learn_start(void)
{
	
	ET4007_start();
	ET4007_writebyte(_ET4007_ADDRESS_);
	ET4007_writebyte(_ET4007_CONTROL_START_LEARND_);
	ET4007_stop();
	
}


/******************************************************/
/*Funcation: read ET4007 learn stop 					   */
/*Input: 	 	   				   */
/*Output: 					   */
/******************************************************/
static void ET_IR_learn_stop(void)
{
	
	ET4007_start();
	ET4007_writebyte(_ET4007_ADDRESS_);
	ET4007_writebyte(_ET4007_CONTROL_STOP_LEARN_);
	ET4007_stop();
	
}

/******************************************************/
/*Funcation: get ET4007 busy state 					   */
/*Input: 	 	   				   */
/*Output: 		char state			   */
/******************************************************/
static char ET_IR_busy_state(void)
{
	if (!ET4007_GET_BUSY_STATE()) {
		busy_status = 0;
		}else {
		busy_status = 1;
		}
	
	return busy_status;
	
}





static long ET_remote_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{

	char buf[4];
	
	switch(cmd) {
	
		case ET_CMD_START_LEARN:
		ET_IR_learn_start();
		busy_status =0;
		break;
		case ET_CMD_STOP_LEARN:
		ET_IR_learn_stop();
	//	mod_timer(&remote_timer,0); 
		busy_status = 1;
	//	printk("ET4007 wake up interruptiable remote watiq \n");
		wake_up_interruptible(&remote_waitq);
		break;
		
		case	ET_CMD_VERSION:
		ET_IR_read_device_info(buf);
		
		return (buf[0]<<16)|(buf[1]<<8)|(buf[2]);
		
		break;
		default:
			return -EINVAL;
	}

	return 0;
}




#ifdef CONFIG_COMPAT
static long ET_remote_ioctl_compat(struct file *filp, unsigned int cmd,
		unsigned long arg)
{

	
	char buf[4];
	printk(KERN_INFO "%s  cmd = %d \n", __func__,
				cmd);
	switch(cmd) {
	
		case ET_CMD_START_LEARN:
		ET_IR_learn_start();
		busy_status =0;
		break;
		case ET_CMD_STOP_LEARN:
		ET_IR_learn_stop();
	//	mod_timer(&remote_timer,0); 
		busy_status = 1;
		printk("ET4007 wake up interruptiable remote watiq \n");
		wake_up_interruptible(&remote_waitq);
		break;
		
		case	ET_CMD_VERSION:
		ET_IR_read_device_info(buf);
		
		return (buf[0]<<16)|(buf[1]<<8)|(buf[2]);
		
		break;
		default:
			return -EINVAL;
	}


	return 0;
}
#endif
static unsigned int ET_remote_poll( struct file *file,
		struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	mod_timer(&remote_timer, jiffies + msecs_to_jiffies(40));
	printk("ET4007---poll---\n");
	poll_wait(file, &remote_waitq, wait);
	if (busy_status){
		
		busy_status=0;
		mask |= POLLIN | POLLRDNORM;
	}
	return mask;
}

static ssize_t ET_remote_write(struct file *file, const char *buffer,
		size_t count, loff_t *ppos)
{

	int ret;
	char buf_ir[386];
	
	ret = copy_from_user(( void *)buf_ir, buffer, count);
	if(ret<0)
	return ret;
	ret = ET_IR_send_code( buf_ir,  count);

	return ret;
}

static ssize_t ET_remote_read(struct file *filp, char *buff,
		size_t count, loff_t *ppos){
		
	int ret;
	int m = 0;
	
	char buf_ir[512];
	
	m = ET_IR_read_code(buf_ir);

	if(m<10){
		return -EINVAL;
		}
	
	ret = copy_to_user((void *)buff, (const void *)(buf_ir),
			 m);
	//printk(KERN_INFO "ret err = %d m= %d  \n",err,m);


	return ret ? -EFAULT : m;
}



static int ET_remote_open(struct inode *inode, struct file *file) {

	
	// ET_remote_read_device_info();
	setup_timer(&remote_timer, remote_timer_handle,
					(unsigned long)"remote");
	
    printk("ET4007---open OK---\n");
    return 0;
}

static int ET_remote_close(struct inode *inode, struct file *file) {




	del_timer_sync(&remote_timer);

    printk("ET4007---close---\n");
	return 0;
}
/*
static irqreturn_t busy_interrupt(int irq, void * dev_id)
{
	struct remote_desc *rdata = (struct remote_desc *)dev_id;

	 printk("ET4007---busy_interrupt---\n");

	return IRQ_HANDLED;
}
*/
static struct file_operations ET_remote_ops = { 
	.owner			= THIS_MODULE,
	.open			= ET_remote_open,
	.release		= ET_remote_close, 
	.write          = ET_remote_write,
	.read			=	ET_remote_read,
	.poll			= 	ET_remote_poll,
	.unlocked_ioctl	=	ET_remote_ioctl,
	#ifdef CONFIG_COMPAT
	.compat_ioctl =    ET_remote_ioctl_compat,
	#endif
	
	
};








static ssize_t remocon_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	


	
	//unsigned int _data;

	//int  i;
	int index =0;

	uint8_t original[MAX_SEND_DATA];
	
	printk(KERN_INFO "%s:  = %s \n\r", __func__,buf);
	index = Char2Binary(buf,size,original,MAX_SEND_DATA);
	if(index>0){
	ET_IR_send_code(original,index);
		}


/*
	for (i = 0; i < size; i++) {

		if (sscanf(buf++, "%u", &_data) == 1) {
		
		
			if ( buf == '\0'){
				printk(KERN_INFO "%s:  =  first out \n\r", __func__);
				break;
				}
		

				original[index++] = (uint8_t)_data ;
				
				if(index>MAX_SEND_DATA-1){
				printk("index > MAX_SEND_DATA error \n\r");
				return -1;
			}
			
			while (_data > 0) {
				buf++;
				_data /= 10;
			}
		} else {
	//		printk(KERN_INFO "second  = %s \n\r",buf);
		
		//	break;
		}
	}
	*/
	
	

	return size;
}



static ssize_t remocon_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{

	int len = 0;
	int i;
	char buf_ir[4];
	//printk(KERN_INFO"%s called\n", __func__);
	

	ET_IR_read_device_info(buf_ir);
	
	printk(KERN_INFO "%s:  read version = 0x%02x 0x%02x 0x%02x 0x%02x \n", __func__,buf_ir[0],buf_ir[1],buf_ir[2],buf_ir[3]);
	
			
	/*
	printk(KERN_INFO "%s: dev_id: 0x%02x, 0x%02x, 0x%02x, 0x%02x \n", __func__,
				buf_ir[0], buf_ir[1],buf_ir[2],buf_ir[3]);
	*/
	//len =  buf_ir[0]*16777216 + buf_ir[1]*65536 + buf_ir[2]*256+ buf_ir[3];
	for(i=0;i<4;i++){
		len += sprintf(buf + len, "%02x", buf_ir[i]);
		}
	printk(KERN_INFO "%s:  learn len = %d \n", __func__,len);
	return  len;
}

static DEVICE_ATTR(ir_send, 0666, remocon_show, remocon_store);

static ssize_t learn_cmd(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	
	
	unsigned int _data;
	//int err;


	printk(KERN_INFO "%s:  = %s \n\r", __func__,buf);
	

	if (sscanf(buf++, "%d", &_data) == 1) {
		
	//	printk("data buf[%d] = %d  \n\r",buf_count,buf[buf_count]);
		
		
		if(_data>0){
		ET_IR_learn_start();
			}
		if (_data==0){
		ET_IR_learn_stop();
		}
			
	}
	
	return size;
}

static ssize_t learn_read(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int m = 0;
	int len = 0;
	int i = 0;
	char buf_ir[386];
	
	m = ET_IR_read_code(buf_ir);

	if(m<10){
		return 0;
		}

	for(i=0;i<m;i++){
	//	len += sprintf(buf + len, "%d,", buf_ir[i]);
		printk(KERN_INFO "%s: learn read get[%d] = 0x%02x \n", __func__,i,buf_ir[i]);
		}

	
	len = Binary2Char( buf_ir, m,  buf,  MAX_SEND_DATA*2);   
		if(len>0)	{
			printk(KERN_INFO "%s: learn buf = %s  \n", __func__,buf);
			return len;
			}
	return	0;

}

static DEVICE_ATTR(ir_learn, 0666, learn_read, learn_cmd);

static ssize_t check_ir_state(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int len=0;
	char state;
	state=ET_IR_busy_state();
	
	len = sprintf(buf + len, "%d\n", state);
		
	return	len;

}



static DEVICE_ATTR(ir_state, 0666, check_ir_state, NULL);







static struct miscdevice ET_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &ET_remote_ops,
};

static int __init ET_remote_dev_init(void) {

	struct device *ir_remocon_dev;
	int error;
//	int irq;
#ifdef   ET_POWERCONTROL

	ET4007_SET_POWER_HIGH();
	mdelay(1);
#endif

	
#ifdef   PLAT_EXYNOS

	error = gpio_request(ET4007_SCL_GPIO, DEVICE_NAME);
	if (error) {
		printk("request ET4007_SCL_GPIO %d for remote failed\n", ET4007_SCL_GPIO);
		return error;
	}
	error = gpio_request(ET4007_SDA_GPIO, DEVICE_NAME);
	if (error) {
		printk("request ET4007_SDA_GPIO %d for remote failed\n", ET4007_SDA_GPIO);
		return error;
	}

//	error = gpio_request(ET4007_BUSY_GPIO, DEVICE_NAME);
//	if (error) {
//		printk("request ET4007_BUSY_GPIO %d for remote failed\n", ET4007_BUSY_GPIO);
//		return error;
//	}

#endif

#ifdef   PLAT_MSM8610

	error = gpio_request(ET4007_SCL_GPIO, DEVICE_NAME);
	if (error) {
		printk("request ET4007_SCL_GPIO %d for remote failed\n", ET4007_SCL_GPIO);
		return error;
	}
	error = gpio_request(ET4007_SDA_GPIO, DEVICE_NAME);
	if (error) {
		printk("request ET4007_SDA_GPIO %d for remote failed\n", ET4007_SDA_GPIO);
		return error;
	}

	error = gpio_request(ET4007_BUSY_GPIO, DEVICE_NAME);
	if (error) {
		printk("request ET4007_BUSY_GPIO %d for remote failed\n", ET4007_BUSY_GPIO);
		return error;
	}

#endif
	ET4007_SET_SCL_OUTPUT();
	ET4007_SET_SDA_OUTPUT();
	ET4007_SET_BUSY_INPUT();
	ET4007_SET_SCL_HIGH();
	ET4007_SET_SDA_HIGH();



	
//	ret = misc_register(&ET_misc_dev);
	ir_data = kzalloc(sizeof(struct ir_remocon_data), GFP_KERNEL);
	if (NULL == ir_data) {
		printk("Failed to data allocate %s\n", __func__);
		error = -ENOMEM;
		goto err_free_mem;
	}

	
	
	etek_class = class_create(THIS_MODULE, "etek");
		if (IS_ERR(etek_class)) {
			pr_err("Failed to create class(etek)!\n");
			return PTR_ERR(etek_class);
		}
		
	ir_remocon_dev = device_create(etek_class, NULL, 0, ir_data, "sec_ir");
	if (IS_ERR(ir_remocon_dev))
		printk("Failed to create ir_remocon_dev device\n");
	
	if (device_create_file(ir_remocon_dev, &dev_attr_ir_send) < 0)
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_send.attr.name);	
	
	if (device_create_file(ir_remocon_dev, &dev_attr_ir_state) < 0)
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_state.attr.name);

	if (device_create_file(ir_remocon_dev, &dev_attr_ir_learn) < 0)
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_learn.attr.name);
	

		
	if((error = misc_register(&ET_misc_dev)))
		{
		printk("ET4007: misc_register register failed\n");
		}


	printk("ET4007 initialized\n");


	
	return error;

err_free_mem:
#ifdef   ET_POWERCONTROL
	ET4007_SET_POWER_LOW();
#endif
		kfree(ir_data);
	return error;
}

static void __exit ET_remote_dev_exit(void) {
	misc_deregister(&ET_misc_dev);	
}

module_init(ET_remote_dev_init);
module_exit(ET_remote_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ETEK Inc.");
MODULE_DESCRIPTION("ETEK consumerir  Driver");

