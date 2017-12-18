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
#include <linux/i2c.h>
#include<linux/timer.h> 
#include<linux/jiffies.h>
#include <linux/irq.h>
#include <asm/io.h>

#include "et4207.h"




#define DEVICE_NAME				"irremote"
#define ET4207_NAME				"et4207"
//#define ET_POWERCONTROL

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

#define SIZE  32
#define PLAT_EXYNOS




#ifdef  PLAT_EXYNOS
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>


#define ET4207_BUSY_GPIO			EXYNOS4_GPB(2)

#define ET4207_GET_BUSY_STATE()			gpio_get_value(ET4207_BUSY_GPIO)
#define ET4207_SET_BUSY_INPUT()		s3c_gpio_cfgpin(ET4207_BUSY_GPIO, S3C_GPIO_INPUT)


#endif





static DECLARE_WAIT_QUEUE_HEAD(remote_waitq);

static volatile int busy_status = 0;


struct timer_list remote_timer;




struct class *etek_class;
static struct i2c_client	*et4207_client;



/******************************************************/
/*Funcation: write_data_to_et4207                        	      		  */
/*Input:  	char *buffer	 size_t count		  			  */
/*Output: 	int size				   					  */
/*Desc: 	write data to et4207 through i2c			                */
/******************************************************/

int write_data_to_et4207(char *buffer, size_t count){
	char iic_buf[SIZE];
	int ret = 0;
	int size = 0;
	int i;
	int allLength;
	int i2c_len = count/SIZE;
	if(count>MAX_SEND_DATA){
	return -1;
	}
	if ((count%SIZE) >0){
		i2c_len += 1;
		}
	

	allLength = ((i2c_len-1)<<4)&0xf0;

	for (i=0;i<i2c_len;i++){
	memset(iic_buf,0,SIZE);
	memcpy(iic_buf,&buffer[i*SIZE],SIZE);

	ret = i2c_smbus_write_i2c_block_data(et4207_client,_ET4207_CONTROL_SEND_+i, SIZE,iic_buf);
	if(ret<0){
		ret = i2c_smbus_write_i2c_block_data(et4207_client,_ET4207_CONTROL_SEND_+i, SIZE,iic_buf);
		}
	if(ret == 0){
		size += SIZE;
		}
	
		//printk(KERN_INFO "%s: write_data_to_et4207 ret = %d \n", __func__,
		//		ret);
	udelay(100);
	}
	 ret = i2c_smbus_write_byte(et4207_client,_ET4207_CONTROL_SEND_END_);

		
	//memcpy(iic_buf,buffer,count);
	//ret = i2c_smbus_write_i2c_block_data(et4207_client,_ET4207_CONTROL_SEND_,count,iic_buf);

	
	
	return size;
	
}


/******************************************************/
/*Funcation: read_data_from_et4207                        	      		  */
/*Input:  	char *buffer	 size_t count		  			  */
/*Output: 	int size				   					  */
/*Desc: 	write data to et4207 through i2c			                */
/******************************************************/



int read_data_from_et4207( char *buffer,int count){

	char iic_buf[SIZE];
	int i;
//	int count = 416;
	int i2c_len = count/SIZE;
	int size =0;
	int ret = 0;
	memset(buffer,0,count);
	for (i=0;i<i2c_len;i++){
	
	ret = i2c_smbus_read_i2c_block_data(et4207_client,_ET4207_CONTROL_READ_CODE_+i, SIZE,iic_buf);
	udelay(100);
	if(ret !=SIZE){
		ret = i2c_smbus_read_i2c_block_data(et4207_client,_ET4207_CONTROL_READ_CODE_+i, SIZE,iic_buf);
		udelay(100);
		size += ret;
		}
		printk(KERN_INFO "%s: i2c_smbus_read_i2c_block_data ret = %d \n", __func__,
				ret);
	memcpy(&buffer[i*SIZE],iic_buf,SIZE);
	
	}
	
	//i2c_smbus_write_byte(et4207_client,_ET4207_CONTROL_READ_END_);

	//int ret = 0;
	//memset(buffer,0,count);
	/*
	printk(KERN_INFO "%s: count : %d \n", __func__,
				count);
				*/
	//ret = i2c_smbus_read_i2c_block_data(et4207_client,_ET4207_CONTROL_READ_CODE_, count,buffer);
	return size;
	
}

/******************************************************/
/*Funcation: ET4207 TIMER HANDLE	                        	        */
/*Input:  	unsigned long _data			   */
/*Output: 					   						   */
/*Desc: 	get busy state according to time base				   */

/******************************************************/



static void remote_timer_handle(unsigned long _data)
{
	 if (!busy_status){
		mod_timer(&remote_timer,jiffies+msecs_to_jiffies(100)); 
		 
		if (ET4207_GET_BUSY_STATE()) {
			busy_status = 1;
	//		printk("ET4207 wake up interruptiable remote watiq \n");
			wake_up_interruptible(&remote_waitq);
		}
	 }
}



/******************************************************/
/*Funcation: read ET4207 version code					   */
/*Input: 					   							   */
/*Output: 	version 			   						   */
/******************************************************/

static int ET_remote_read_version(char* buf)
{
	
	int ret = 0;
	

	ret = i2c_smbus_read_i2c_block_data(et4207_client,_ET4207_CONTROL_READ_VERSION_,4,buf);
	
	if(ret!=4){
		ret = i2c_smbus_read_i2c_block_data(et4207_client,_ET4207_CONTROL_READ_VERSION_,4,buf);
		}

	printk(KERN_INFO "%s: dev_id: 0x%02x, 0x%02x, 0x%02x, 0x%02x \n", __func__,
				buf[0], buf[1],buf[2],buf[3]);
	ret = buf[0]*16777216 + buf[1]*65536 + buf[2]*256+ buf[3];
	
	return ret;
}

/******************************************************/
/*Funcation: read ET4207 start learn 						   */
/*Input: 					   							   */
/*Output: 	version 			   						   */
/******************************************************/

static int ET_remote_learn_start(void)
{
	
	int ret = 0;

	ret = i2c_smbus_write_byte(et4207_client,_ET4207_CONTROL_START_LEARND_RMT_);
	
	return ret;
}


static int ET_remote_learn_rec_start(void)
{
	
	int ret = 0;

	ret = i2c_smbus_write_byte(et4207_client,_ET4207_CONTROL_START_LEARND_REC_);
	
	return ret;
}

static int ET_remote_set_current(int value)
{
	
	int ret = 0;
	uint8_t command=0;
	if(value>8){
		value = 7;
		}
	if(value<0){
		value = 0;
		}
	printk(KERN_INFO "%s  = %d \n", __func__,value);
	command = _ET4207_CONTROL_CURRENT_SET_+value;
printk(KERN_INFO "%s  = %x \n", __func__,command);
	ret = i2c_smbus_write_byte(et4207_client,command);
	
	return ret;
}


/******************************************************/
/*Funcation: read ET4207 stop learn 						   */
/*Input: 					   							   */
/*Output: 	version 			   						   */
/******************************************************/

static int ET_remote_learn_stop(void)
{
	
	int ret = 0;
	ret = i2c_smbus_write_byte(et4207_client,_ET4207_CONTROL_STOP_LEARN_);
	printk(KERN_INFO "%s  = %d \n", __func__,ret);
		
	return ret;
}




static long ET_remote_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	int ret;
	char buf[4];
	
	printk(KERN_INFO "%s  cmd = %d \n", __func__,
				cmd);

	switch(cmd) {
	
		case ET_CMD_START_LEARN_RMT:
		ret = ET_remote_learn_start();
		busy_status =0;
		break;
		case ET_CMD_START_LEARN_REC:
		ret = ET_remote_learn_rec_start();
		busy_status =0;
		break;
		case ET_CMD_STOP_LEARN:
		ret = ET_remote_learn_stop();
		busy_status = 1;
		
		wake_up_interruptible(&remote_waitq);
		break;
		
		case	ET_CMD_VERSION:
		ret = ET_remote_read_version(buf);
		return (buf[0]<<16)|(buf[1]<<8)|(buf[2]);
		
		case	ET_CMD_VERSION_FULL:
		ret = ET_remote_read_version(buf);
		return ret;

		case	ET_CMD_SET_CURRENT:
		ret = ET_remote_set_current((int)arg);
		return ret;
		
		default:
		return -EINVAL;
	}

	return 0;
}


static long ET_remote_ioctl_compat(struct file *filp, unsigned int cmd,
		unsigned long arg)
{

	char buf[4];
	int ret;
	printk(KERN_INFO "%s  cmd = %d \n", __func__,
				cmd);
	
	switch(cmd) {
	
		case ET_CMD_START_LEARN_RMT:
		ret = ET_remote_learn_start();
		busy_status =0;
		break;
		case ET_CMD_START_LEARN_REC:
		ret = ET_remote_learn_rec_start();
		busy_status =0;
		break;
		case ET_CMD_STOP_LEARN:
		ret = ET_remote_learn_stop();
		busy_status = 1;
		
		wake_up_interruptible(&remote_waitq);
		break;
		
		case	ET_CMD_VERSION:
		ret = ET_remote_read_version(buf);
		return (buf[0]<<16)|(buf[1]<<8)|(buf[2]);
		
		case	ET_CMD_VERSION_FULL:
		ret = ET_remote_read_version(buf);
		return ret;
		case	ET_CMD_SET_CURRENT:
		ret = ET_remote_set_current(arg);
		return ret;
		
		default:
		return -EINVAL;
	}

	return 0;
}


static unsigned int ET_remote_poll( struct file *file,
		struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	mod_timer(&remote_timer, jiffies + msecs_to_jiffies(40));
	printk("ET4207---poll---\n");
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
	int size= 0;
	char send_buff[MAX_SEND_DATA];
	int ret = 0;
	memset(send_buff,0x00,MAX_SEND_DATA);
	if(count>MAX_SEND_DATA){
		return -1;
		}
	ret = copy_from_user(send_buff, buffer, count) ;
	if(ret<0){
			return ret;
			}
	//printk("ET4207---ET_remote_write--- %d\n",ret);
	size = write_data_to_et4207(send_buff,count);

	return size;
}



static ssize_t ET_remote_read(struct file *filp, char *buff,
		size_t count, loff_t *ppos){
		
		char buf[count];
		
		int ret = 0;
		
		ret = read_data_from_et4207(buf,count);
		if(ret<0){
			return ret;
			}
		ret = copy_to_user((void *)buff, (const void *)(buf),count);
		
	
	return ret;
}



static int ET_remote_open(struct inode *inode, struct file *file) {

	char buf[4];
	ET_remote_read_version(buf);
	setup_timer(&remote_timer, remote_timer_handle,
					(unsigned long)"remote");
	
    printk("ET4207---open OK---\n");
    return 0;
}

static int ET_remote_close(struct inode *inode, struct file *file) {

	del_timer_sync(&remote_timer);

    printk("ET4207---close---\n");
	return 0;
}

static struct file_operations et_remote_ops = { 
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

static struct miscdevice et_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &et_remote_ops,
};






static ssize_t remocon_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	int i = 0;
	int index = 0;
	unsigned int _data;
	uint8_t code[MAX_SEND_DATA];
	printk(KERN_INFO "%s:  = %s \n\r", __func__,buf);
	
	for (i = 0; i < size; i++) {

		if (sscanf(buf++, "%x", &_data) == 1) {
		
		
			if ( buf == '\0'){
				printk(KERN_INFO "%s:  =  first out \n\r", __func__);
				break;
				}
		

				code[index++] = (uint8_t)_data ;
				
				if(index>MAX_SEND_DATA-1){
				printk("index > MAX_SEND_DATA error \n\r");
				return -1;
			}
			
		//	while (_data > 0) {
				buf++;
		//	}
		}
		else {
	//		printk(KERN_INFO "second  = %s \n\r",buf);
		
		//	break;
		}
	}
	for(i=0;i<index;i++){
		printk("code[%d] = 0x%x \n\r",i,code[i]);
		}
	write_data_to_et4207(code,index);
	return size;
}




static ssize_t remocon_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int i;
	int len = 0;
	char test_buf[4];
	
	len = ET_remote_read_version(test_buf);
	len = 0;
	for(i=0;i<4;i++){
		len += sprintf(buf + len, "0x%02x,", test_buf[i]);
		}

	return  len;
}

static DEVICE_ATTR(ir_send, 0664, remocon_show, remocon_store);





static ssize_t remocon_state(struct device *dev, struct device_attribute *attr,
		char *buf)
{
 
	int len = 0;
	len += sprintf(buf + len, "%d,",ET4207_GET_BUSY_STATE());
	

	return  len;
}

static DEVICE_ATTR(ir_state, 0664, remocon_state, NULL);

static ssize_t remocon_learn(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int i;
	int len = 0;
	int count = MAX_SEND_DATA;
	char read_buf[count];
	int ret = 0;
	ret = read_data_from_et4207(read_buf,count); 
	
	for(i=0;i<ret;i++){
		len += sprintf(buf + len, "[%2x],", read_buf[i]);
		}
	len += sprintf(buf + len, "\n");
	return  len;
}

static DEVICE_ATTR(ir_learn, 0664, remocon_learn, NULL);






/******************************************************/
/*Funcation: create_et4207_class		                     		   */
/*Input: 			void		   							   */
/*Output: 	error 			   						   */
/******************************************************/

int create_et4207_class(void){
	struct device *ir_remote_dev;
	struct ir_remocon_data	*ir_data;
	ir_data = kzalloc(sizeof(struct ir_remocon_data), GFP_KERNEL);

	
	etek_class = class_create(THIS_MODULE, "etek");
		if (IS_ERR(etek_class)) {
			pr_err("Failed to create class(etek)!\n");
			return PTR_ERR(etek_class);
		}
		
	ir_remote_dev = device_create(etek_class, NULL, 0, ir_data, "sec_ir");
	if (IS_ERR(ir_remote_dev)){
		printk("Failed to create ir_remote_dev device\n");
		return -1;
		}
	
	if (device_create_file(ir_remote_dev, &dev_attr_ir_send) < 0){
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_send.attr.name);	
		return -1;
		}
	
	if (device_create_file(ir_remote_dev, &dev_attr_ir_learn) < 0){
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_state.attr.name);
			return -1;
		}


	if (device_create_file(ir_remote_dev, &dev_attr_ir_state) < 0){
			printk("Failed to create device file(%s)!\n",
				   dev_attr_ir_state.attr.name);
			return -1;
		}

	
	return 1;
	
}


/******************************************************/
/*Funcation: et4207_probe ii2 probe	                     		   */
/*Input: 			void		   							   */
/*Output: 	error 			   						   */
/******************************************************/
static int __devinit et4207_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret;
		
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	

	ret = i2c_check_functionality(adapter,
			I2C_FUNC_SMBUS_BYTE | I2C_FUNC_SMBUS_BYTE_DATA);
	if (!ret) {
		dev_err(&client->dev, "I2C check functionality failed\n");
		return -ENXIO;
	}
	
	et4207_client = client;
	
	dev_info(&client->dev, "et4207 device is probed successfully.\n");

	return 0;

}

static int __devexit et4207_remove(struct i2c_client *client)
{
	
	et4207_client = NULL;

	return 0;
}


static const struct i2c_device_id et4207_ids[] = {
	{ "et4207", 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, et4207_ids);

static struct i2c_driver i2c_et4207_driver = {
	.driver		= {
		.name	= ET4207_NAME,
	},

	.probe		= et4207_probe,
	.remove		= __devexit_p(et4207_remove),

	.id_table	= et4207_ids,
};



static int __init init_et4207(void)
{
	int ret;
	ret = gpio_request(ET4207_BUSY_GPIO, DEVICE_NAME);
	if (ret) {
		printk("request ET4207_BUSY_GPIO %d for remote failed\n", ET4207_BUSY_GPIO);
		return ret;
	}
	ET4207_SET_BUSY_INPUT();
	ret = create_et4207_class();
	if(ret<0){
			printk("Failed to create device class \n");
			return ret;
		}
	
	
	ret = i2c_add_driver(&i2c_et4207_driver);

	if((ret = misc_register(&et_misc_dev)))
		{
		printk("ET4207: misc_register register failed\n");
		}


	printk("init_et4207 finished\n");
	
	return ret;
}

static void __exit exit_et4207(void)
{
	i2c_del_driver(&i2c_et4207_driver);	
	misc_deregister(&et_misc_dev);	
	printk(KERN_INFO "et4207 remote  driver removed.\n");
}


module_init(init_et4207);
module_exit(exit_et4207);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ETEK Inc.");
MODULE_DESCRIPTION("ETEK consumerir  Driver");

