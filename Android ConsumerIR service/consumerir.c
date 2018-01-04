/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "ConsumerIrHal"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cutils/log.h>
#include <hardware/hardware.h>
#include <hardware/consumerir.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MAXSIZE 8192
#define	_ET4007_CONTROL_SEND_CODE_3_		0x54
#define MAX_SIZE 1024
#define MAX_INDEX 64
#define MAX_SAMPLE 16
#define MAX_SAMPLE_INDEX 32
#define MAX_DATA 310
#define MAX_ORIGINAL_DATA 620
#define MAX_SEND_DATA 380

#define COMPARE_OFFSET 2

//#define CONSUMER_IR_CHAR
/******************************************************/
/*Funcation: et_xCal_crc                      	        				*/
/*Input:  	char *ptr	uint32_t len						*/
/*Output: 	char crc 	  								 */
/*Desc: 		get whole ptr data array crc					*/

/******************************************************/
uint8_t xCal_crc(uint8_t *ptr,int len)
{
	uint8_t crc;
 	uint8_t i;
    crc = 0;
    while(len--)
    {
       crc ^= *ptr++;
       for(i = 0;i < 8;i++)
       {
           if(crc & 0x01)
           {
               crc = (crc >> 1) ^ 0x8C;
           }else crc >>= 1;
       }
    }
    return crc;
}



	
/******************************************************/
/*Funcation: Binary2Char                      	        				*/
/*Input:  	binary - 数值数组，输入要转换的数值； 
	      len - binary数组里的数值个数（以字节为单位）； 
	     buff - 字符数组，输出转换后的字符； 
	      size - buff数组的空间（以字节为单位）。 		*/
/*Output: 	true or false									 */
/*Desc: 				*/

/******************************************************/
int Binary2Char( unsigned char*  binary,  int len, char*  buff,  int size)   
	{  
	    int         i, n;  
	  
	    n = len * 2;  
	  
	    if(n > size){
			ALOGE("Binary2Char data large than array size");
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
/*Desc: 						*/

/******************************************************/
int Char2Binary( char* token,  int len, unsigned char*  binary,  int size)   
	{  
	        const char*     p;  
	    int         i, n, m;  
	        char        buf[3] = {0,0,0};  
	  
	  
	  
	    m = len % 2 ? (len - 1) / 2 : len / 2;  
	  
	   
		if(m > size){
			ALOGE("Binary2Char data large than array size");
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
/*Funcation: et_compare_time                      	        				*/
/*Input:  	emote_data data, int high_level,
		int low_level					*/
/*Output: 	true or false									 */
/*Desc: 		compare  remote data time 				*/

/******************************************************/
  char compareCenter(int data, int center) {

        if ((data > center - COMPARE_OFFSET) && (data < center + COMPARE_OFFSET)) {

            return 1;
        }

        return 0;

    }

 char compare_time(int origHigh, int origLow, int sampHigh, int sampLow) {
        if (compareCenter(origHigh, sampHigh)) {
            if (compareCenter(origLow, sampLow)) {
                return 1;
            }
        }

        return 0;
    }


/******************************************************/
/*Funcation: et_compare_alldata                      	        			*/
/*Input:  	emote_data data, int *sample int index		*/
/*Output: 	true or false						 			*/
/*Desc: 		compare  remote data to all sample  				*/

/******************************************************/
char et_compare_alldata(int high_level,int low_level, int *sample, int index) {
	int i;
	int timeHigh, timeLow;

	for (i = 0; i < index; i += 2) {
		timeHigh = sample[i];
		timeLow = sample[i + 1];

		if (compare_time( high_level, low_level, timeHigh, timeLow)) {
			return 1;			
			}
		}
	return 0;	
}
		

void et_push_sample_time_data(int high_level,int low_level, int *sample, int index) {
	sample[index] = high_level;
	sample[index + 1] = low_level;
}

int et_sample_time_selection(int *original,int orig_count,int *sample) {
	int i, index;
	uint32_t high_level,low_level;
	
	index = 0;

	for (i = 0; i < orig_count; i += 2) {
		high_level = original[i];
		low_level = original[i + 1];

		if (index != 0) {
			if (et_compare_alldata(high_level, low_level, sample, index)==0) {
				et_push_sample_time_data(high_level, low_level, sample, index);
				index += 2;
				if (index>MAX_SAMPLE){
					return -2;
					}
			}
		} else { /* first data send*/
			et_push_sample_time_data(high_level, low_level, sample, index);
			index += 2;
		}
	}
	
	return index;
}
/******************************************************/
/*Funcation: et_get_index                        	        			*/
/*Input:  	ir_remocon_data *ir_data
		int *sample, int index	   					*/
/*Output: 	index 	  								 */
/*Desc: 	data compare sample to get sample index  */

/******************************************************/

int et_get_index(int high_level, int low_level,int *sample, int index) {
	int i = 0;
	int timeHigh, timeLow;

	for (i = 0; i < index; i += 2) {
		timeHigh = sample[i];
		timeLow = sample[i + 1];

		if (compare_time(high_level, low_level,timeHigh,timeLow)) {
			
			return i;
			
		}
	}
	return MAX_SAMPLE_INDEX;

}
/******************************************************/
/*Funcation: et_get_data_index                        	        			*/
/*Input:  	ir_remocon_data *ir_data, char *data,
		int *sample, int index	   					*/
/*Output: 	index 	  								 */
/*Desc: 	original data to get sample index to compress data     */

/******************************************************/
int et_get_data_index(int *original, int *sample,int index,int orig_count,uint8_t *data) {
	int i, count = 0;
	char temp;
	uint32_t high_level,low_level;

	for (i = 0; i < orig_count; i += 2) {
		high_level = original[i];
		low_level = original[i + 1];

		temp = et_get_index(high_level,low_level, sample, index);
		if (temp>MAX_SAMPLE_INDEX){
			return -3;
			}
		data[count++] = (temp/2 ) ;
		if(count>MAX_ORIGINAL_DATA){
			return MAX_ORIGINAL_DATA;
			}
		
	}


	return  count;
}


int et_compress_data(uint8_t *data, uint8_t *compress,int data_length){
   int compress_length;
   int temp;
   int j = 0;
   int i = 0;
   if (data_length % 2 == 1) {
        compress_length  = data_length/ 2 + 1;
        } else {
        compress_length = data_length / 2 ;

   }
      
     
   while(i<data_length){

            temp = (data[i++] << 4) & 0xf0;
            if(i!=data_length) {
                temp |= (data[i++]) & 0x0f;
            }
          
            compress[j] = temp;
          
            j++;

        }

        return compress_length;
    }



   


/******************************************************/
/*Funcation: et_compress_sample                        	        */
/*Input:  	int *in int index			   */
/*Output: 	char *out  	   */
/*Desc: 	change int sample to double char sample  */

/******************************************************/
int et_compress_sample(int *sample,uint8_t *zp_sample,int sample_len) {
        int i, j = 0;
      
        for (i = 0; i <sample_len; i++) {
            zp_sample[j++] = (char) ((sample[i] >> 8) & 0xff);
            zp_sample[j++] = (char) sample[i];
        }
        return j;
    }

/******************************************************/
/*Funcation: et_compress_original_data                        	        */
/*Input:  	ir_remocon_data *ir_data			   */
/*Output: 	ir_remocon_data *ir_data	ir_data length   	   */
/*Desc: 	translate original consumer data to ET compress data	   */

/******************************************************/

int et_compress_original_data(int *original, uint8_t freq,int orig_count ,uint8_t *et_data) {
	
	uint8_t temp[MAX_SEND_DATA];
	uint8_t data[MAX_ORIGINAL_DATA];
	int sample[MAX_INDEX];
	uint8_t zp_sample[MAX_INDEX];
	uint8_t zp_data[MAX_DATA];
	int i;

	int length;
	int sample_len;
	int data_len;
	int couple_len;
	int zp_sample_len;
	memset(data,0x00,MAX_ORIGINAL_DATA);
	memset(temp,0x00,MAX_SEND_DATA);
	
	memset(sample,0x00,MAX_INDEX);
	memset(zp_sample,0x00,MAX_INDEX);
	memset(zp_data,0x00,MAX_DATA);
	
	sample_len = et_sample_time_selection(original,orig_count,sample);
	if (sample_len<0){
		ALOGE( "  et_sample_time_selection  error \n\r");
		return sample_len;
		}
	

	couple_len = et_get_data_index(original, sample,sample_len,orig_count,data);
		
	if (couple_len<0){
		ALOGE("  et_get_data_index  error \n\r");
		return couple_len;
		}


	data_len = et_compress_data(data,zp_data,couple_len);
	if (data_len<0){
		ALOGE( "  et_compress_data  error \n\r");
		return data_len;
		}
	
	zp_sample_len = et_compress_sample(sample,zp_sample,sample_len);
	
	

	
	
	length= MAX_INDEX + data_len  +10;

	for (i=0;i<zp_sample_len;i++){
		temp[i] = zp_sample[i];
	}
	for (i=0;i<data_len;i++){
		temp[i  + MAX_INDEX] = zp_data[i];
		//printk("temp[%d] is 0x%x",i,ir_data->data[i]);
		}

	et_data[0] = _ET4007_CONTROL_SEND_CODE_3_;
	et_data[1] = (length>>8)&0xff;
	
	et_data[2] = length&0xff;
	et_data[3] = freq;
	
	et_data[4] = (couple_len>>8)&0xff;
	et_data[5] = (couple_len)&0xff;;  
	et_data[6] = 0x00;	
	et_data[7] = 0x00;
	et_data[8] = 0x01;
	et_data[9] = xCal_crc(temp,length-10);
	
		for (i=0;i<MAX_INDEX+data_len;i++){
			et_data[i+10]= temp[i];
			}

	return length;

}



static const consumerir_freq_range_t consumerir_freqs[] = {
    {.min = 30000, .max = 30000},
    {.min = 33000, .max = 33000},
    {.min = 36000, .max = 36000},
    {.min = 38000, .max = 38000},
    {.min = 40000, .max = 40000},
    {.min = 56000, .max = 56000},
};
//static  int consumerir_code[1024] = {0};



int et4007_Learndata_UnCompress(char *datas, int *code_buffer, int *freq) {

	int temp;
	uint8_t  n_Crc;
	uint8_t  n_PartIndexCount;
	uint8_t  n_Sample;
	uint8_t  n_Index;
	uint8_t  n_Freq;

	uint8_t * learn_buffer;

	uint8_t unzip_end = 1;
	uint8_t dat_temp;

	uint8_t PartIndexCount;
	int Sample0_nHighLevel;
	int Sample0_nLowLevel;
	int Sample1_nHighLevel;
	int Sample1_nLowLevel;

	uint8_t n_PartIndexCount_p;
	uint8_t n_Sample_p;
	uint8_t n_Index_p;

	int m, n;
	uint8_t shift = 0x80;

	n_Crc = datas[0];
	n_PartIndexCount = datas[1];
	n_Sample = datas[2];
	n_Index = datas[3];
	n_Freq = datas[4];
	 ALOGD("freq = %x", n_Freq);
	 ALOGD("n_Crc = %x", n_Crc);
	 ALOGD("n_PartIndexCount = %d", n_PartIndexCount);
	 ALOGD("n_Sample = %d", n_Sample);
	
		 
	if(n_PartIndexCount<2)
		return -2;
	if(n_Sample<1)
		return -3;
	if(n_Index<2)
		return -4;
	
	if((n_Freq>0x80)||(n_Freq<0x15))
		return -1;
	if((n_PartIndexCount+n_Sample+n_Index)>386)
		return -6;
	n_Freq++;
	*freq = 2500000 / n_Freq;
	// ALOGD("freq = %d ",*freq);
	 
	learn_buffer = datas + 5;

	if( n_Crc != xCal_crc(learn_buffer, n_PartIndexCount + n_Sample + n_Index )) return -5;
	// ALOGD("crc right");
	n = 0;
	n_PartIndexCount_p = 0;
	n_Sample_p = n_PartIndexCount;
	n_Index_p = n_PartIndexCount + n_Sample;

	PartIndexCount = learn_buffer[n_PartIndexCount_p];
	Sample0_nHighLevel = 0x00000000;
	Sample0_nLowLevel = 0x00000000;
	Sample1_nHighLevel = (int) learn_buffer[n_Sample_p];
	Sample1_nHighLevel <<= 8;
	Sample1_nHighLevel |= (int) learn_buffer[n_Sample_p + 1];
	Sample1_nLowLevel = (int) learn_buffer[n_Sample_p + 2];
	Sample1_nLowLevel <<= 8;
	Sample1_nLowLevel |= (int) learn_buffer[n_Sample_p + 3];

	dat_temp = learn_buffer[n_Index_p];
	while (PartIndexCount--) {
		if (0x00 == shift) {
			n_Index_p++;
			dat_temp = learn_buffer[n_Index_p];
			shift = 0x80;
		}
		if (dat_temp & shift) {
			code_buffer[n] = Sample1_nHighLevel;
			n++;
			code_buffer[n] = Sample1_nLowLevel * 8 / n_Freq;
			code_buffer[n]++;
			n++;
			if (0x0000ffff == Sample1_nLowLevel) {
				unzip_end = 0;
				break;
			}
		} else {
			code_buffer[n] = Sample0_nHighLevel;
			n++;
			code_buffer[n] = Sample0_nLowLevel * 8 / n_Freq;
			code_buffer[n]++;
			n++;
		}
		shift >>= 1;
	}
	n_PartIndexCount_p++;
 //	ALOGD("PartIndexCount is right n = %d",n);
	while (unzip_end) {
		PartIndexCount = learn_buffer[n_PartIndexCount_p];
		Sample0_nHighLevel = (int) learn_buffer[n_Sample_p];
		Sample0_nHighLevel <<= 8;
		Sample0_nHighLevel |= (int) learn_buffer[n_Sample_p + 1];
		Sample0_nLowLevel = (int) learn_buffer[n_Sample_p + 2];
		Sample0_nLowLevel <<= 8;
		Sample0_nLowLevel |= (int) learn_buffer[n_Sample_p + 3];
		Sample1_nHighLevel = (int) learn_buffer[n_Sample_p + 4];
		Sample1_nHighLevel <<= 8;
		Sample1_nHighLevel |= (int) learn_buffer[n_Sample_p + 5];
		Sample1_nLowLevel = (int) learn_buffer[n_Sample_p + 6];
		Sample1_nLowLevel <<= 8;
		Sample1_nLowLevel |= (int) learn_buffer[n_Sample_p + 7];
		while (PartIndexCount--) {
			if (0x00 == shift) {
				n_Index_p++;
				dat_temp = learn_buffer[n_Index_p];
				shift = 0x80;
			}
			if (dat_temp & shift) {
				code_buffer[n] = Sample1_nHighLevel;
				n++;
				code_buffer[n] = Sample1_nLowLevel * 8 / n_Freq;
				code_buffer[n]++;
				n++;
			//	ALOGD("unzip   n = %d",n);
				if (0x0000ffff == Sample1_nLowLevel) {
					unzip_end = 0;
					break;

				if(n>960){
					unzip_end = 0;
					break;

					}
				}
			} else {
				code_buffer[n] = Sample0_nHighLevel;
				n++;
				code_buffer[n] = Sample0_nLowLevel * 8 / n_Freq;
				code_buffer[n]++;
				n++;
			//	ALOGD("unzip   n = %d",n);
				if(n>960){
					unzip_end = 0;
					break;

					}
			}
			shift >>= 1;
		}
		n_PartIndexCount_p++;
		n_Sample_p += 4;
	}
	//ALOGD("unzip_end is right n = %d",n);
	return n;
}



static int fd_send = 0;




static int consumerir_transmit(struct consumerir_device *dev,
   int carrier_freq, int pattern[], int pattern_len)
{
    int strlen;
    int i;
	int len ;
	int ret =0;
    char buffer[MAXSIZE];
	int freqTime;
	uint8_t freq;
	uint8_t et_data[MAX_SEND_DATA];
    memset(buffer, 0, MAXSIZE);
	
	
	
	freqTime = 1000000/carrier_freq;
	freq= (uint8_t)(1200000/carrier_freq+1) ;
	// ALOGD("freq = %d", freq);
	for (i = 0; i < pattern_len; i++)
	{
		pattern[i] = pattern[i]*carrier_freq/1000000;
	}
	
	len = et_compress_original_data(pattern, freq,pattern_len ,et_data);
	if(len<1){
		ALOGD("et_compress_original_data error = %d",strlen);
			return 0;
		}
	/*
	strlen = 0;
	for (i = 0; i < len; i++)
	{
		strlen += sprintf(buffer + strlen, "%d,", et_data[i]);
		// ALOGD("et_data[%d] = %d",i, et_data[i]);
		if(strlen>(MAXSIZE-1))
		{
			ALOGD("strlen is to lagre %d",strlen);
			return 0;
		}
	}

    buffer[strlen - 1] = '\0';
    */
    strlen =Binary2Char( et_data,  len, buffer,  MAXSIZE) ;  
    ALOGD("transmit string is %s", buffer);
	if(strlen>0){
		 ret =  write(fd_send, buffer, strlen );
		}
 
   

    return ret;

	


}

static void consumerir_learn_cmd(struct consumerir_device *dev,
   int cmd)
{
    int strlen;

	int ret =0;

	int fd;
    char buffer[MAXSIZE];

    memset(buffer, 0, MAXSIZE);

   strlen =  sprintf(buffer, "%d\n",cmd); 
 
    ALOGD("consumerir_learn_cmd string is %s", buffer);
	if(strlen>0){
		
	fd= open("/sys/class/etek/sec_ir/ir_learn", O_RDWR);
    ALOGD("cnsumerir  open fd_learn is %d", fd);
	
   	 ret =  write(fd, buffer, strlen );
	
	close(fd);
	
		}
 
    return ret;

	


}
char learnCode[] ="6F09240246010101010301010101001F0106003E010A001F0211001F010A003E0105001F0214001F0112003E0109001FFFFFF9E0";
static int consumerir_read_code(struct consumerir_device *dev,int* consumerir_code)
{
    int i;
	int ret =1;
	unsigned char code[386];
	int code_buffer[960];
	int freq;
	int freqTime;
    char buffer[MAXSIZE];
	int fd;
	memset(buffer, 0, MAXSIZE);

	
	memset(code, 0, 386);

	fd= open("/sys/class/etek/sec_ir/ir_learn", O_RDWR);
	if(fd<0)
    ALOGE("cnsumerir  open fd_learn is %d", fd);
	
    ret =  read(fd, buffer,2048);
	
	close(fd);
	
	
    //ALOGD("consumerir_read_code %s ",buffer);

/*
	if(ret<0){
		return 0;
		}
*/
	if(ret>0){
		ret = Char2Binary( buffer, ret, code, 386)   ;
		
		if(ret>0){
			//for(i=0;i<ret;i++){
			//	 ALOGD("consumerir[%d] = 0x%x", i,code[i]);
			//	}
			ret = et4007_Learndata_UnCompress(code, code_buffer, &freq);
			if(ret>0){
				freqTime = 1000000/freq;
				consumerir_code[1] = freq;
				consumerir_code[0] = 0;
			//	 ALOGD(" freqTime = %d ",freqTime);
				for (i = 0; i < ret; i++)
					{
						consumerir_code[i+2] = code_buffer[i]*freqTime;
					}
			//	ALOGE(" et4007_Learndata_UnCompress success = %d",ret);
				
				return ret+2;
				}else {
					ALOGE(" et4007_Learndata_UnCompress error = %d",ret);
					consumerir_code[0] = 1;
					return 1;
					}
			}else {
			ALOGE(" Char2Binary error");
			consumerir_code[0] = 2;
			return 1;
			}
		}else {
		ALOGE(" read error");
		consumerir_code[0] = 3;
			return 1;
			}
	consumerir_code[0] = 4;
	return 1;


}


static int consumerir_read_info(struct consumerir_device *dev)
{
    int len;
    int i;
	int ret =0;
	char version[4];
    char buffer[128];
	int fd = 0;
	memset(buffer, 0, 128);

	
	memset(version, 0, 4);
	
	fd= open("/sys/class/etek/sec_ir/ir_send", O_RDWR);
	if(fd<0)
    ALOGE("cnsumerir  open ir_send is %d", fd);
	
    ret =  read(fd, buffer,16);
	if(ret<0)
	ALOGE("cnsumerir  read is %d", ret);
	close(fd);
   // ALOGD("consumerir_read_info %s ",buffer);
   
/*
	if(ret<0){
		return 0;
		}
*/
	if(ret>0){
		len = Char2Binary( buffer,  ret, version, 4)   ;

		if(len==4){
			ret =  version[0]*16777216 + version[1]*65536 + version[2]*256+ version[3];
			 return ret;
			}else {
			ALOGE("consumerir_read_info Char2Binary error");
			return 2;
			}
		}else {
			ALOGE("consumerir_read_info read error");
			return 1;
			}
	
    return ret;

}




static int consumerir_read_state()
{
    int len;
    int i;
	int ret =0;
	int fd = 0;
	int state;
    char buffer[128];
	memset(buffer, 0, 128);
	fd= open("/sys/class/etek/sec_ir/ir_state", O_RDWR);
	if(fd<0)
    ALOGE("cnsumerir  open fd_state is %d", fd);
    ret =  read(fd, buffer,128);
	
  //  ALOGD("consumerir_read_state %s ",buffer);
    close(fd);
/*
	if(ret<0){
		return 0;
		}
*/
	if(ret>0){
		if (sscanf(buffer, "%d", &state) == 1) {
		
			return state;
			}else {
			ALOGE("consumerir_read_state sscanf data error");
			return -1;
			}
		}else {
		ALOGE("consumerir_read_state read error");
		return -2;
			}
	
    return ret;

}







static int consumerir_get_num_carrier_freqs(struct consumerir_device *dev)
{
    return ARRAY_SIZE(consumerir_freqs);
}

static int consumerir_get_carrier_freqs(struct consumerir_device *dev,
    size_t len, consumerir_freq_range_t *ranges)
{
    size_t to_copy = ARRAY_SIZE(consumerir_freqs);

    to_copy = len < to_copy ? len : to_copy;
    memcpy(ranges, consumerir_freqs, to_copy * sizeof(consumerir_freq_range_t));
    return to_copy;
}

static int consumerir_close(hw_device_t *dev)
{
    free(dev);
    close(fd_send);
	
	
	
		
    return 0;
}

/*
 * Generic device handling
 */
static int consumerir_open(const hw_module_t* module, const char* name,
        hw_device_t** device)
{
    if (strcmp(name, CONSUMERIR_TRANSMITTER) != 0) {
        return -EINVAL;
    }
    if (device == NULL) {
        ALOGE("NULL device on open");
        return -EINVAL;
    }

    consumerir_device_t *dev = malloc(sizeof(consumerir_device_t));
    memset(dev, 0, sizeof(consumerir_device_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = consumerir_close;

    dev->transmit = consumerir_transmit;
    dev->get_num_carrier_freqs = consumerir_get_num_carrier_freqs;
    dev->get_carrier_freqs = consumerir_get_carrier_freqs;
	dev->get_device_info = consumerir_read_info;
	dev->get_learn_code= consumerir_read_code;
	dev->get_device_state= consumerir_read_state;
	dev->learnCmd = consumerir_learn_cmd;
    *device = (hw_device_t*) dev;
    fd_send = open("/sys/class/etek/sec_ir/ir_send", O_RDWR);
   //  ALOGD("cnsumerir  open fd_send is %d", fd_send);
	
	
    return 0;
}

static struct hw_module_methods_t consumerir_module_methods = {
    .open = consumerir_open,
};

consumerir_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                = HARDWARE_MODULE_TAG,
        .module_api_version = CONSUMERIR_MODULE_API_VERSION_1_0,
        .hal_api_version    = HARDWARE_HAL_API_VERSION,
        .id                 = CONSUMERIR_HARDWARE_MODULE_ID,
        .name               = "Consumer IR Module",
        .author             = "The ETEK Project",
        .methods            = &consumerir_module_methods,
    },
};

