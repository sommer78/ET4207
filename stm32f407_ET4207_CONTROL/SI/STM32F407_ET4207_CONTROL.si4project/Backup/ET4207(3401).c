#include "ET4207.h" 
#include "delay.h" 	
#include "usart.h"
#include  <string.h>




#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MAXSIZE 8192
#define	_ET4007_CONTROL_SEND_CODE_3_		0x56
#define MAX_SIZE 1024
#define MAX_INDEX 64
#define MAX_SAMPLE 16
#define MAX_SAMPLE_INDEX 32
#define MAX_DATA 310
#define MAX_ORIGINAL_DATA 620
#define MAX_SEND_DATA 380

#define COMPARE_OFFSET 2

#define ET_DEBUG 1

//////////////////////////////////////////////////////////////////////////////////	 

//ET4207 ��������	   
					  
////////////////////////////////////////////////////////////////////////////////// 	

u8	 send_etcode[]  ={0x54, 0x00 ,0x83 ,0x20 ,0x00 ,0x72 ,0x00 ,0x00 ,0x01 ,0xca ,0x00 ,0x71 ,0x00 ,0x43 ,0x00 ,0x10 ,0x00 ,0x2b ,0x00 ,0x10 ,0x00 ,0x0f ,0x00 ,0x10 ,0x05 ,0xf0,
		  0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
		  0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x12 ,0x22 ,0x12 ,0x21 ,0x12 ,0x12 ,0x21 ,0x12 ,0x11 ,0x22 ,0x12,
		  0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x12 ,0x22 ,0x22 ,0x12 ,0x22 ,0x21 ,0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22,
		  0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x12 ,0x22 ,0x22 ,0x13 };

u8 normal_learn[] = {0x56,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x30,0x00,0x14,0x00,0x00,0x4c,0x43,0x01,
	0x40,0x05,0x0e,0x00,0x14,0x01,0xde,0x00,0x14,0x00,0x94,0x00,0x14,0x00,0x94,0x00,0x14,0x00,0x95,0x00,0x14,
	0x00,0x94,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,0x05,0x0e,0x00,0x14,0x00,
	0x95,0x00,0x14,0x01,0xdd,0x00,0x14,0x01,0xdd,0x00,0x14,0x01,0xdd,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x95,
	0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,0x1d,0x77,0x01,0x40,0x05,0x0d,0x00,0x14,0x01,0xde,0x00,
	0x14,0x00,0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x94,0x00,0x14,0x00,0x94,0x00,0x14,0x00,0x96,0x00,0x14,
	0x00,0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x05,0x0f,0x00,0x14,0x00,0x96,0x00,0x14,0x01,0xde,0x00,0x14,0x01,
	0xdd,0x00,0x14,0x01,0xdd,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x95,
	0x00,0x14,0x1d,0x77,0x01,0x40,0x05,0x0d,0x00,0x14,0x01,0xdd,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,
	0x14,0x00,0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,
	0x05,0x0e,0x00,0x14,0x00,0x95,0x00,0x14,0x01,0xdd,0x00,0x14,0x01,0xde,0x00,0x14,0x01,0xde,0x00,0x14,0x00,
	0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,0x1d,0x76,0x01,0x40,0x05,0x0e,
	0x00,0x14,0x01,0xde,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,
	0x14,0x00,0x95,0x00,0x14,0x00,0x96,0x00,0x14,0x00,0x95,0x00,0x14,0x05,0x0f,0x00,0x14,0x00,0x96,0x00,0x14,
	0x01,0xde,0x00,0x14,0x01,0xde,0x00,0x14,0x01,0xde,0x00,0x14,0x00,0x95,0x00,0x14,0x00,0x95,0x00,0x14,0x00,
	0x94,0x00,0x14,0x00,0x94,0x00,0x14,0xff,0xff,};

u8 nec6122[]= {0x54,0x00,0x5c,0x20,0x00,0x24,0x00,0x00,0x01,0xaf,0x01,0x5d,0x00,0xab,0x00,0x17,
	0x00,0x14,0x00,0x16,0x00,0x3f,0x00,0x17,0x06,0x2d,0x01,0x5d,0x00,0x55,0x00,0x17,
	0x1e,0x06,0x00,0x46,0x0f,0x02,0x00,0x16,0x0f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x12,0x22,
	0x22,0x21,0x22,0x12,0x21,0x12,0x11,0x21,0x12,0x21,0x23,0x45};

u8 zip_learn[] = {0x32,0x00,
	0x00,0x4e,0x00,0x05,0x06,0x00,0x00,0x3f,0x00,0x6c,0x00,0x00,0x06,0x2d,0x01,0x00,
	0x04,0xcf,0x00,0x1c,0x01,0x2c,0x00,0x1b,0x02,0x63,0x00,0x1c,0x09,0xb0,0x00,0xe3,
	0x04,0xd0,0x00,0x1c,0xff,0xff,0x01,0x22,0x21,0x12,0x22,0x12,0x12,0x11,0x12,0x21,
	0x11,0x21,0x23,0x41,0x22,0x21,0x12,0x22,0x12,0x12,0x11,0x12,0x21,0x11,0x21,0x23,
	0x41,0x22,0x21,0x12,0x22,0x12,0x12,0x11,0x12,0x21,0x11,0x21,0x25,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x22,
	0x21,0x12,0x22,0x12,0x12,0x11,0x12,0x21,0x11,0x21,0x23,0x41,0x22,0x21,0x12,0x22,
	0x12,0x12,0x11,0x12,0x21,0x11,0x21,0x23,0x41,0x22,0x21,0x12,0x22,0x12,0x12,0x11,
	0x12,0x21,0x11,0x21,0x25,
};


u8 rec_learn[] = {
	0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0b,0x07,0x0a,0xce,0x01,0x6a,0x01,0x66,0x01,0x3e,0x01,0x66,0x01,0x6b,0x04,0x03,
	0x01,0x6a,0x04,0x03,0x01,0x6b,0x01,0x3a,0x01,0x6a,0x04,0x02,0x01,0x6b,0x01,0x66,
	0x01,0x6a,0x01,0x3a,0x01,0x6b,0x01,0x67,0x01,0x3d,0x01,0x66,0x01,0x6a,0x04,0x03,
	0x01,0x6b,0x04,0x02,0x01,0x6a,0x01,0x3a,0x01,0x6b,0x04,0x03,0x01,0x6a,0x01,0x66,
	0x01,0x3e,0x01,0x66,0x01,0x6b,0x04,0x02,0x01,0x6a,0x01,0x66,0x01,0x6a,0x01,0x3a,
	0x01,0x6b,0x01,0x3a,0x01,0x6b,0x01,0x66,0x01,0x6b,0x01,0x3a,0x01,0x6b,0x04,0x02,
	0x01,0x6a,0x01,0x66,0x01,0x3d,0x01,0x66,0x01,0x6a,0x04,0x02,0x01,0x6b,0x04,0x03,
	0x01,0x6a,0x04,0x03,0x01,0x6b,0x04,0x03,0x01,0x6b,0x04,0x03,0x01,0x6b,0x01,0x67,
	0x01,0x3e,0x04,0x30,0x01,0x3e,0x71,0x8e,0x0a,0xed,0x0a,0xce,0x01,0x6b,0x04,0x03,
	0x01,0x6b,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};


int rec_learn1[] ={-76, 20, 80, 10, 66, 1, 6, 10, 1, 1, 1, 6, 10, 1, 1, 1, 6, 10, 1, 1, 1, 6, 10, 1, 1, 0, 113, 3, -94, 0, 19, 1, -55, 0, 19, 3, 6, 0, 19, 4, -42, 0, 19, 29, 125, 0, 113, 3, -95, 0, 19, 1, -55, 0, 19, 3, 6, 0, 19, 4, -43, 0, 19, 29, 125, 0, 113, 3, -94, 0, 19, 1, -55, 0, 19, 3, 6, 0, 19, 4, -42, 0, 19, 29, 125, 0, 113, 3, -95, 0, 19, 1, -54, 0, 19, 3, 6, 0, 19, 4, -42, 0, 19, -1, -1, -1, 9, 127, -31, 47, -4, 37, -1, -124, -80, 0, -88, -91, -34, 114, 96, 0, 4, 19, -32, 0, 1, 19};



Consumer_IR_T comsumer_ir;


//#define CONSUMER_IR_CHAR
/******************************************************/
/*Funcation: et_xCal_crc                      	        				*/
/*Input:  	char *ptr	u32 len						*/
/*Output: 	char crc 	  								 */
/*Desc: 		get whole ptr data array crc					*/

/******************************************************/
u8 xCal_crc(u8 *ptr,int len)
{
	u8 crc;
 	u8 i;
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
/*Input:  	binary - ��ֵ���飬����Ҫת������ֵ�� 
	      len - binary���������ֵ���������ֽ�Ϊ��λ���� 
	     buff - �ַ����飬���ת������ַ��� 
	      size - buff����Ŀռ䣨���ֽ�Ϊ��λ���� 		*/
/*Output: 	true or false									 */
/*Desc: 				*/

/******************************************************/
int Binary2Char( u8*  binary,  int len, char*  buff,  int size)   
	{  
	    int         i, n;  
	  
	    n = len * 2;  
	  
	    if(n > size){
			printf("Binary2Char data large than array size");
			return 0;
	    	}
	  
	    for (i = 0; i < len; i++)  
	            sprintf(buff + i * 2, "%02X", binary[i]); 
	  
	    return  n;  
	}  
/******************************************************/
/*Funcation: Char2Binary                      	        				*/
/*Input:  	binary - ��ֵ���飬����Ҫת������ֵ�� 
	      len - binary���������ֵ���������ֽ�Ϊ��λ���� 
	     buff - �ַ����飬���ת������ַ��� 
	      size - buff����Ŀռ䣨���ֽ�Ϊ��λ���� 		*/
/*Output: 	true or false									 */
/*Desc: 						*/

/******************************************************/
int Char2Binary( char* token,  int len, u8*  binary,  int size)   
	{  
	        const char*     p;  
	    int         i, n, m;  
	        char        buf[3] = {0,0,0};  
	  
	  
	  
	    m = len % 2 ? (len - 1) / 2 : len / 2;  
	  
	   
		if(m > size){
			printf("Binary2Char data large than array size");
			return 0;
	    	}
	      p = token;
	    // Ϊ�����Ч�ʣ������������ַ���ת����  
	        for (i = 0; i < m; i++)  
	        {  
	        p = token + i * 2;  
	                buf[0] = p[0];  
	                buf[1] = p[1];  
	  
	                n = 0;  
	                sscanf(buf, "%X", &n);  
	                binary[i] = n;  
	        }  
	  
	    // ��ת�����һ���ַ�������У���  
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
  char compareCenter(u16 data, u16 center) {

        if ((data > center - COMPARE_OFFSET) && (data < center + COMPARE_OFFSET)) {

            return 1;
        }

        return 0;

    }

 char compare_time(u16 origHigh, u16 origLow, u16 sampHigh, u16 sampLow) {
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
char et_compare_alldata(u16 high_level,u16 low_level, u16 *sample, u16 index) {
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
		

void et_push_sample_time_data(u16 high_level,u16 low_level, u16 *sample, u16 index) {
	sample[index] = high_level;
	sample[index + 1] = low_level;
}

int et_sample_time_selection(u16 *original,u16 orig_count,u16 *sample) {
	int i, index;
	u16 high_level,low_level;
	
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

int et_get_index(u16 high_level, u16 low_level,u16 *sample, u16 index) {
	int i = 0;
	u16 timeHigh, timeLow;

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
int et_get_data_index(u16 *original, u16 *sample,u16 index,u16 orig_count,u8 *data) {
	int i, count = 0;
	char temp;
	u16 high_level,low_level;

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


int et_compress_data(u8 *data, u8 *compress,int data_length){
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
int et_compress_sample(u16 *sample,u8 *zp_sample,int sample_len) {
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
/*Desc: 	translate original consumerit data to ET compress data	   */

/******************************************************/

int et_compress_original_data(Consumer_IR_T consumer_ir ,u8 *et_data) {
	
	u8 temp[MAX_SEND_DATA];
	u8 data[MAX_ORIGINAL_DATA];
	u16 sample[MAX_INDEX];
	u8 zp_sample[MAX_INDEX];
	u8 zp_data[MAX_DATA];
	int i;
	u8 freq;

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

	
	freq= (u8)(1200000/consumer_ir.freq+1) ;
	for(i=0;i<consumer_ir.length;i++){
		consumer_ir.datas[i] = consumer_ir.datas[i]*consumer_ir.freq/1000000;
		}
	
	sample_len = et_sample_time_selection(consumer_ir.datas,consumer_ir.length,sample);
	if (sample_len<0){
		printf( "  et_sample_time_selection  error \n\r");
		return sample_len;
		}
	

	couple_len = et_get_data_index(consumer_ir.datas, sample,sample_len,consumer_ir.length,data);
		
	if (couple_len<0){
		printf("  et_get_data_index  error \n\r");
		return couple_len;
		}


	data_len = et_compress_data(data,zp_data,couple_len);
	if (data_len<0){
		printf( "  et_compress_data  error \n\r");
		return data_len;
		}
	
	zp_sample_len = et_compress_sample(sample,zp_sample,sample_len);
	
	

	
	
	length= MAX_INDEX + data_len  +10;

	for (i=0;i<zp_sample_len;i++){
		temp[i] = zp_sample[i];
	}
	for (i=0;i<data_len;i++){
		temp[i  + MAX_INDEX] = zp_data[i];
		//printf("temp[%d] is 0x%x",i,ir_data->data[i]);
		}

	et_data[0] = _ET4007_CONTROL_SEND_CODE_3_;
	et_data[1] = (length>>8)&0xff;
	et_data[2] = length&0xff;
	et_data[4] = freq;
	et_data[5] = sample_len&0x0f;  

		for (i=0;i<MAX_INDEX+data_len;i++){
			et_data[i+6]= temp[i];
			}
	et_data[3] =  xCal_crc(et_data,length-4);

	return length;

}





/******************************************************/
/*Funcation: et4207_UnCompress_zip                        	        */
/*Input:  	u8 array datas			   */
/*Output: 	state < 0 error u16 freq u16 array irpluse	   */
/*Desc: 	translate original consumerit data to ET compress data	   */

/******************************************************/

int et4207_UnCompress_zip(u8 *datas, u16 *irpluse, u16 *freq) {
    u8  n_Crc;
    u8  n_PartIndexCount;
    u8  n_Sample;
    u8  n_Index;
    u8  n_Freq;
//    u8 n_flag;
    u32 i;
	u8 n_type;
	u16 data_len;

//    int temp;
    u8 learn_buffer[512];
    
    u8 unzip_end = 1;
    u8 dat_temp;

    u8 PartIndexCount;
    u16 Sample0_nHighLevel;
    u16 Sample0_nLowLevel;
    u16 Sample1_nHighLevel;
    u16 Sample1_nLowLevel;

    u8 n_PartIndexCount_p;
    u8 n_Sample_p;
    u8 n_Index_p;

    int  n;
    u8 shift = 0x80;
	data_len = (u16)(datas[8]*256+datas[9]);

//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
    n_PartIndexCount = datas[12];
    n_Sample = datas[13];
    n_Index = datas[14]; 
    n_Freq = datas[15];
	n_type = datas[0];
	if(n_type!=0x31){
		 printf("n_type ZIP 1 error \r\n" );
        return -1;
		}
#ifdef ET_DEBUG
	 printf("len = %d \r\n  ",data_len );

	 printf("type = %x \r\n  ",n_type );

	 printf("state = %x \r\n  ",datas[10] );
	 printf("n_PartIndexCount = %d  n_Sample = %d n_Index = %d \r\n",n_PartIndexCount,n_Sample,n_Index );
#endif
		
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        printf("n_Freq  error \r\n" );
        return -2;
    }


    n_Freq--;
    *freq = 2500000 / n_Freq;

    printf("n_Freq = %d \r\n", 2500000 / n_Freq );

	if( n_Crc != xCal_crc(&datas[16], data_len )){
		   printf("n_Crc  error \r\n" );
		   return -3;
	   }
	
	if( data_len != n_PartIndexCount + n_Sample + n_Index ){
			printf("n length  error \r\n" );
			return -4;
		}



    for(i=0;i<n_PartIndexCount;i++){
        learn_buffer[i] =datas[i+16];
    //   printf("partIndex[%d] = 0x%02x \r\n", i,learn_buffer[i] );
    }
   
    for(i=0;i<n_Sample;i++){
        learn_buffer[n_PartIndexCount+i] =datas[i+16+n_PartIndexCount+n_Index];
    //   printf("sample[%d] = 0x%02x \r\n", i,learn_buffer[n_PartIndexCount+i] );
    }

	
	for(i=0;i<n_Index;i++){
		   learn_buffer[n_PartIndexCount+n_Sample+i] =datas[i+16+n_PartIndexCount];
	//	 printf("n_Index[%d] = 0x%02x \r\n", i,learn_buffer[n_PartIndexCount+n_Sample+i] );
	   }
	
   

	
    n = 0;
	n_PartIndexCount_p = 0;
	n_Sample_p = n_PartIndexCount;
	n_Index_p = n_PartIndexCount + n_Sample;
	
#ifdef ET_DEBUG

	for(i=0;i<data_len;i++)
		{
		printf("0x%02x,",learn_buffer[i]);
		}
	printf("\r\n");
#endif

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
            irpluse[n] = Sample1_nHighLevel;
			n++;
            irpluse[n] = Sample1_nLowLevel * 8 / n_Freq;
            irpluse[n]++;
			n++;
			if (0x0000ffff == Sample1_nLowLevel) {
				unzip_end = 0;
				break;
			}
			
		} else {
            irpluse[n] = Sample0_nHighLevel;
			n++;
            irpluse[n] = Sample0_nLowLevel * 8 / n_Freq;
            irpluse[n]++;
			n++;
			if(n>1000){
				 printf("index out error \r\n" );
       				 return -5;
				}
		}
		shift >>= 1;
	}
	n_PartIndexCount_p++;


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
			if(n_Index_p>data_len)
				{
					 printf("n_Index_p out error \r\n" );
       				 return -6;
				}
			if (dat_temp & shift) {
                irpluse[n] = Sample1_nHighLevel;
				n++;
                irpluse[n] = Sample1_nLowLevel * 8 / n_Freq;
                irpluse[n]++;
				n++;
				if (0x0000ffff == Sample1_nLowLevel) {
					unzip_end = 0;
					break;
				}
				if(n>1000){
				 printf("index out error \r\n" );
       				 return -7;
				}
			} else {
                irpluse[n] = Sample0_nHighLevel;
				n++;
                irpluse[n] = Sample0_nLowLevel * 8 / n_Freq;
                irpluse[n]++;
				n++;
				if(n>1000){
				 printf("index out error \r\n" );
       				 return -8;
				}
			}
			shift >>= 1;
		}
		n_PartIndexCount_p++;
		n_Sample_p += 4;
	}
	return n;
}

/******************************************************/
/*Funcation: et4207_UnCompress_normal                        	        */
/*Input:  	u8 array datas			   */
/*Output: 	state < 0 error u16 freq u16 array irpluse	   */
/*Desc: 	translate original consumerit data to ET compress data	   */

/******************************************************/

int et4207_UnCompress_normal(u8 *datas, u16 *irpluse, u16 *freq) {
    u8  n_Crc;
    u8  n_Index;
    u8  n_Freq;
//    u8 n_flag;

	u8 n_type;
	u16 data_len;

    
	u8	t_crc;


    //u8 PartIndexCount;
  
	int index;
    int  n;

	u16 nHighLevel ;
	u16 nLowLevel ;

	data_len = (u16)(datas[8]*256+datas[9]);
	
//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
  
    n_Index = datas[14]; 
	n_Index++;
    n_Freq = datas[15];
	n_type = datas[0];
	if(n_type!=0x30){
		 printf("n_type RMT NORMAL error \r\n" );
        return -1;
		}
#ifdef ET_DEBUG
	 printf("len = %d \r\n  ",data_len );

	 printf("type = %x \r\n  ",n_type );

	 printf("state = %x \r\n  ",datas[10] );
	 printf(" n_Index = %d \r\n",n_Index );
#endif
		
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        printf("n_Freq  error \r\n" );
        return -2;
    }

	index = 16;

    n_Freq--;
    *freq = 2500000 / n_Freq;

    printf("n_Freq = %d \r\n", 2500000 / n_Freq );
	t_crc = xCal_crc(&datas[16], data_len );
	printf("code crc  =%x \r\n",t_crc );

	if( n_Crc != t_crc){
		   printf("n_Crc  error \r\n" );
		   return -3;
	   }
	



	
    n = 0;

	
	while (n_Index--) {
		
			
			nHighLevel = (u16) datas[index++];
			nHighLevel <<= 8;
			nHighLevel |= (u16) datas[index++];
			nLowLevel = (u16) datas[index++];
			nLowLevel <<= 8;
			nLowLevel |= (u16) datas[index++];
		
            irpluse[n] = nHighLevel;
			n++;
            irpluse[n] =(nLowLevel * 8 / n_Freq)  +1;
			n++;
			if (0x0000ffff == nLowLevel) {
				return n;
			}
			if(index>(data_len+16)){
			 printf("data_len  out \r\n" );
				return n;
				}
			
		
	}
	

	return n;
}



/******************************************************/
/*Funcation: et4207_UnCompress_normal                        	        */
/*Input:  	u8 array datas			   */
/*Output: 	state < 0 error u16 freq u16 array irpluse	   */
/*Desc: 	translate original consumerit data to ET compress data	   */

/******************************************************/

int et4207_UnCompress_ZIP2(u8 *datas, u16 *irpluse, u16 *freq) {
    u8  n_Crc;
    u8  n_Samle;
    u8  n_Freq;
//    u8 n_flag;

	u8 n_type;
	u16 data_len;

    u8 unzip_end = 1;
	u8	t_crc;


    u16 partIndexCount;
  
	int index;
    int  n;

	int sIndex;
	u8 dIndex;
	u16 allDataIndex;

	u16 nHighLevel[16] ;
	u16 nLowLevel[16] ;

	data_len = (u16)(datas[8]*256+datas[9]);
	allDataIndex = (u16)(datas[2]*256+datas[3]);
//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
  
    n_Samle = datas[13]; 
    n_Freq = datas[15];
	n_type = datas[0];
	if(n_type!=0x32){
		 printf("n_type RMP ZIP error \r\n" );
        return -1;
		}
#ifdef ET_DEBUG
	 printf("len = %d \r\n  ",data_len );
	 printf("allDataIndex = %d \r\n	",allDataIndex );


	 printf("type = %x \r\n  ",n_type );

	 printf("state = %x \r\n  ",datas[10] );
	 printf(" n_Samle = %d \r\n",n_Samle );
#endif
		
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        printf("n_Freq  error \r\n" );
        return -2;
    }

	index = 16;

    n_Freq--;
    *freq = 2500000 / n_Freq;

    printf("n_Freq = %d \r\n", 2500000 / n_Freq );
	t_crc = xCal_crc(&datas[16], data_len );
	printf("code crc  =%x \r\n",t_crc );

	if( n_Crc != t_crc){
		   printf("n_Crc  error \r\n" );
		   return -3;
	   }
	

	if(n_Samle>16){
		 printf("n_Index  error \r\n" );
        return -4;
		}

	for( sIndex=0; sIndex<n_Samle;sIndex++){
		nHighLevel[sIndex] = (u16) datas[index++];
		nHighLevel[sIndex] <<= 8;
		nHighLevel[sIndex] |= (u16) datas[index++];
		nLowLevel[sIndex] = (u16) datas[index++];
		nLowLevel[sIndex] <<= 8;
		nLowLevel[sIndex] |= (u16) datas[index++];
	//	 printf("sample[%d] nHighLevel = %d nLowLevel = %d  \r\n",sIndex, nHighLevel[sIndex],nLowLevel[sIndex] );
	//	nLowLevel[sIndex] = (nLowLevel[sIndex] * 8 / n_Freq)  +1;
		}
	data_len += 16;
    n = 0;

	partIndexCount = 0;

	
	while (unzip_end) {
		
			
			dIndex = datas[index];
			dIndex = (dIndex>>4)&0x0f;
		//	printf("index  = %d \r\n",index );
		//	printf("dIndex  = %d \r\n",dIndex );
            irpluse[n++] = nHighLevel[dIndex];
		
            irpluse[n++] =(nLowLevel[dIndex] * 8 / n_Freq)  +1;
			partIndexCount++;
			if(partIndexCount==allDataIndex){
				 printf("allDataIndex  out \r\n" );
				return n;
				}
			if (0x0000ffff == nLowLevel[dIndex]) {
				 printf("nLowLevel  out \r\n" );
				return n;
			}
			if(index>data_len){
				 printf("data_len  out \r\n" );
				return n;
				}
		
			dIndex = datas[index];
			dIndex = (dIndex)&0x0f;
		//	printf("dIndex  = %d \r\n",dIndex );
            irpluse[n++] = nHighLevel[dIndex];
            irpluse[n++] =(nLowLevel[dIndex] * 8 / n_Freq)  +1;
			partIndexCount++;
			if(partIndexCount==allDataIndex){
				 printf("allDataIndex  out \r\n" );
				return n;
				}
			if (0x0000ffff == nLowLevel[dIndex]) {
				 printf("nLowLevel  out \r\n" );
				return n;
			}
			if(index>data_len){
				 printf("data_len  out \r\n" );
				return n;
				}
			index++;
			
		
	}
	

	return n;
}

/******************************************************/
/*Funcation: et4207_UnCompress_REC_normal                        	        */
/*Input:  	u8 array datas			   */
/*Output: 	state < 0 error u16 freq u16 array irpluse	   */
/*Desc: 	translate original consumerit data to ET compress data	   */

/******************************************************/

int et4207_UnCompress_REC(u8 *datas, u16 *irpluse, u16 *freq) {
    u8  n_Crc;
    u8  n_Index;
    u8  n_Freq;
//    u8 n_flag;

	u8 n_type;
	u16 data_len;

    
	u8	t_crc;


    //u8 PartIndexCount;
  
	int index;
    int  n;

	u16 nHighLevel ;
	u16 nLowLevel ;

	data_len = (u16)(datas[8]*256+datas[9]);
	
//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
  
    n_Index = datas[3]; 
	n_Index++;
    n_Freq = 66;
	n_type = datas[0];
	if(n_type!=0x40){
		 printf("n_type REC NORMAL error \r\n" );
        return -1;
		}
#ifdef ET_DEBUG
	 printf("len = %d \r\n  ",data_len );


	 printf("state = %x \r\n  ",datas[10] );
	 printf(" n_Index = %d \r\n",n_Index );
#endif
		
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        printf("n_Freq  error \r\n" );
        return -2;
    }

	index = 16;

    n_Freq--;
    *freq = 2500000 / n_Freq;

    printf("n_Freq = %d \r\n", 2500000 / n_Freq );
	t_crc = xCal_crc(&datas[16], data_len );
	printf("code crc  =%x \r\n",t_crc );

	if( n_Crc != t_crc){
		   printf("n_Crc  error \r\n" );
		   return -3;
	   }
	



	
    n = 0;

	
	while (n_Index--) {
		
		if(index>=(data_len+16)){
			 printf("data_len  out \r\n" );
				return n;
			}
			
			nHighLevel = (u16) datas[index++];
			nHighLevel <<= 8;
			nHighLevel |= (u16) datas[index++];
			nLowLevel = (u16) datas[index++];
			nLowLevel <<= 8;
			nLowLevel |= (u16) datas[index++];
		// printf("nHighLevel = %d  nLowLevel = %d\r\n",nHighLevel,nLowLevel );
            irpluse[n] = (nHighLevel * 4 / n_Freq)  +1;
		//	 printf("irpluse = %d \r\n",irpluse[n] );
			n++;
            irpluse[n] =(nLowLevel * 4 / n_Freq)  +1;
		//	 printf("irpluse = %d \r\n",irpluse[n] );
			n++;
			
			if (0xffff == nLowLevel) {
				 printf("nLowLevel  end \r\n" );
				return n;
			}
			
		
	}
	
	printf("n_Index  end \r\n" );

	return n;
}



//��ʼ��IIC�ӿ�
void ET4207_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
//    RCC_ClocksTypeDef   rcc_clocks;

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

/*
*	�ɹ�:����code_buffer�ĳ���
*	ʧ��:����-1
*/
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
	printf("n_freq is %d ",n_Freq);
	if((n_Freq>0x3e)||(n_Freq<0x15))return -1;
	n_Freq++;
	*freq = 2500000 / n_Freq;

	learn_buffer = datas + 5;

	//	if( n_Crc != xCal_crc(learn_buffer, n_PartIndexCount + n_Sample + n_Index )) return false;

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
		n_Sample_p += 4;
	}
	return n;
}


u8 ET4207SendTest(void){
	
	u8 err=0;

	//	err =	ET4207SendCode(send_etcode,sizeof(send_etcode));
	//err =	ET4207SendCode(zip_learn,sizeof(zip_learn));
	err =	ET4207SendCode(rec_learn,sizeof(rec_learn));
	return err;

}

void ET4007LearnTest(void){
	
	u8 err=0;
	int i;
	int freq;
	int ircode[1024];
  char orig[512];
		for(i=0;i<sizeof(rec_learn1);i++){
			orig[i] = rec_learn1[i];
		
		}
	//	err =	ET4207SendCode(send_etcode,sizeof(send_etcode));
	//err =	ET4207SendCode(zip_learn,sizeof(zip_learn));
	err =	et4007_Learndata_UnCompress(orig,ircode,&freq);
	printf("err=%d",err);
}



u8 ET4207SendCode(u8 *etcode,int length){
	
	u8 err=0;
	if(length >440){
		return 0xf0;
		}
	err = Hard_IIC_WriteNByte(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_SEND_,length,etcode);
	if(err!=0){
		 printf("ET4207SendCode err1 = %d \r\n",err );
	//	return err;
		}
	delay_us(100);
	err = Hard_IIC_WriteNByte(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_SEND_END_,0,etcode);
	if(err!=0){
		 printf("ET4207SendCode err2 = %d \r\n",err );
		return err;
		}
	return err;

}


/**
  *****************************************************************************
  * @Name   : ET4207 ѧϰ��ʼ
  *
  * @Brief  : none
  *
  * @Input  : mode:       0Ϊ��rmt��ѧϰ������ Ϊ1��P14ѧϰ������
  *           algorithm:   �㷨 0 Ϊ��׼��ѹ�� 1Ϊ�߶�ѹ�� 2Ϊ��Լѹ��
  *
  * @Output : err:     ���صĴ���ֵ 0 Ϊ�ɹ� 
  *
  * @Return : none
  *****************************************************************************
**/


u8 ET4207StartLearn(u8 mode,u8 algorithm){
	
	u8 err=0;
	u8 cmd;
	cmd = _ET4207_CONTROL_START_LEARND_RMT_;
	if(mode!=0){
		cmd |= 0x08;
		}
	cmd += algorithm;
	err = Hard_IIC_WriteNByte(I2C1,ET4207_ADDRESS,cmd,0,NULL);
	if(err!=0){
		return err;
		}
	
	return err;

}


u8 ET4207StartLearnREC(){
	
	u8 err=0;
	u8 cmd;
	cmd = _ET4207_CONTROL_START_LEARND_REC_;
	
	err = Hard_IIC_WriteNByte(I2C1,ET4207_ADDRESS,cmd,0,NULL);
	if(err!=0){
		return err;
		}
	
	return err;

}


void printIrFormat(u16 *irpulse,int len){
	int i = 0;
	for(i=0;i<len;i++){
		if(i%2==1){
			printf("L%d\r\n",irpulse[i]);
			}else{
			printf("H%d\r\n",irpulse[i]);
				}
		}
	
}


u8 ET4207ReadCode(u8 *etcode){
	
	int len=0;
	int length = 440;
	int i;
	u16 freq;
	u16 ircode[1024];
	u32 irpulse;
	if(length >440){
		return 0xf0;
		}
	len = Hard_IIC_PageRead(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_READ_CODE_,length,etcode);
	if(len!=0){
		return len;
		}
	 printf("\r\n" );
#ifdef ET_DEBUG
	for(i=0;i<440;i++){
		//printf("	RETLW 0%02xH ; %d \r\n",etcode[i],i-16);
		
		printf("0x%02x,",etcode[i]);
		if(i%16==15){
			 printf("\r\n" );
			}
		}
	
#endif
	len =  et4207_UnCompress_zip(etcode, ircode, &freq) ;
	
	if(len>0){
		for(i=0;i<len;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 printf("%d,",irpulse);
			}
		 printf("\r\n" );
		}else {
		 printf("learn err =  %d",len);
			}
		

	len =  et4207_UnCompress_normal(etcode, ircode, &freq) ;
	
	if(len>0){
		for(i=0;i<len;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 printf("%d,",irpulse);
			}
		}else {
		 printf("learn err =  %d",len);
			}
		 printf("\r\n" );
		
	len =  et4207_UnCompress_ZIP2(etcode, ircode, &freq) ;
	
	if(len>0){
		for(i=0;i<len;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 if(i%2==1){
			//	printf("L%d\r\n",irpulse);
				}else{
			//	printf("H%d\r\n",irpulse);
				}
			}
		}else {
		 printf("learn err =  %d",len);
			}
		
	len =  et4207_UnCompress_REC(etcode, ircode, &freq) ;
	
	if(len>0){
		for(i=0;i<len;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 if(i%2==1){
			//	printf("L%d\r\n",irpulse);
				}else{
			//	printf("H%d\r\n",irpulse);
				}
			}
		}else {
		 printf("learn err =  %d",len);
			}

		 
	return len;

}





u8 ET4207ReadVersion(u8 *etcode){
	
	u8 err=0;
	
	err = Hard_IIC_PageRead(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_READ_VERSION_,8,etcode);
	if(err!=0){
		return err;
		}
	
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
  *           WriteAdd:   д���ڴ���ʼ��ַ
  *           NumToWrite: д��������
  *           *pBuffer:   д��������黺��
  *
  * @Output : *err:     ���صĴ���ֵ
  *
  * @Return : none
  *****************************************************************************
**/
u8  Hard_IIC_WriteNByte(I2C_TypeDef * IICx, u8 SlaveAdd, u8 WriteAdd, u16 NumToWrite, u8 * pBuffer)
{
	u16 sta = 0;
	u16 temp = 0;
	u8 err = 0;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //�ȴ�IIC
	{
		temp++;
		if (temp > 800)
		{
			err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
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
			err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
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
			err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
		}
	}
	delay_us(10);
	//
	//��ȡSR2״̬�Ĵ���
	//
	sta = IICx->SR2;  //������ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	I2C_SendData(IICx, WriteAdd);  
	temp = 0;
	//
	//EV8
	//
	while (!I2C_CheckEvent(IICx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		temp++;
		if (temp > 800)
		{
			err |= 1<<3;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
		}
	}
	delay_us(10);
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
				err |= 1<<4;
				 printf("send err =  %d",err);
				I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
				return err;
			}
		}
	}
	I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
		delay_us(100);
		return err;
}


/**
  *****************************************************************************
  * @Name   : Ӳ��IIC��ȡ����ֽ�����
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:      IIC��
  *           SlaveAdd:  ��Ϊ���豸ʱʶ���ַ
  *           ReadAdd:   ��ȡ���ڴ���ʼ��ַ
  *           NumToRead: ��ȡ����
  *
  * @Output : *pBuffer: �������������
  *           *err:     ���صĴ���ֵ
  *
  * @Return : ��ȡ��������
  *****************************************************************************
**/
u8 Hard_IIC_PageRead(I2C_TypeDef* IICx, u8 SlaveAdd, u8 ReadAdd, u16 NumToRead, u8 * pBuffer)
{
	u16 i = 0;
//	u16 temp = 0;
	u16 sta = 0;
	u8 err = 0;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //�ȴ�IIC
	{
		i++;
		if (i > 800)
		{
			err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
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
			err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
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
			err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
		}
	}
	sta = IICx->SR2;  //������ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	
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
			err |= 1<<3;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
		}
	}
	//I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
	delay_us(10);
	
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
			err |= 1<<4;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
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
			err |= 1<<5;
			I2C_GenerateSTOP(IICx, ENABLE);  //����ֹͣ�ź�
			return err;
		}
	}
	delay_us(10);
	sta = IICx->SR2;  //������ȡSR1�Ĵ�����,��SR2�Ĵ����Ķ����������ADDRλ�������٣�����������������
	//
	//������������
	//
	I2C_AcknowledgeConfig(IICx, ENABLE);

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
	
	return err;
}






