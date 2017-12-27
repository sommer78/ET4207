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

//ET4207 驱动代码	   
					  
////////////////////////////////////////////////////////////////////////////////// 	

u8	 send_etcode[]  ={0x54, 0x00 ,0x83 ,0x20 ,0x00 ,0x72 ,0x00 ,0x00 ,0x01 ,0xca ,0x00 ,0x71 ,0x00 ,0x43 ,0x00 ,0x10 ,0x00 ,0x2b ,0x00 ,0x10 ,0x00 ,0x0f ,0x00 ,0x10 ,0x05 ,0xf0,
		  0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
		  0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x12 ,0x22 ,0x12 ,0x21 ,0x12 ,0x12 ,0x21 ,0x12 ,0x11 ,0x22 ,0x12,
		  0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x12 ,0x22 ,0x22 ,0x12 ,0x22 ,0x21 ,0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x21 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22,
		  0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x12 ,0x22 ,0x22 ,0x13,0x00 };

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
/*Input:  	binary - 数值数组，输入要转换的数值； 
	      len - binary数组里的数值个数（以字节为单位）； 
	     buff - 字符数组，输出转换后的字符； 
	      size - buff数组的空间（以字节为单位）。 		*/
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
/*Input:  	binary - 数值数组，输入要转换的数值； 
	      len - binary数组里的数值个数（以字节为单位）； 
	     buff - 字符数组，输出转换后的字符； 
	      size - buff数组的空间（以字节为单位）。 		*/
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





char ET4003_Learndata_UnCompress(u8 *learn_buffer, u32 *IrBuffer, u16 *freq) {
    u32 i, m, n;
    u32 temp;
    u8 stauts = 1;
    u8 n_ADDRESS_INDEX_LIST;
    u8 v_SAMPLING_VALUE_Tmp;
    u8 v_INDEX_LIST_Tmp;
    u8 count_total = 200;
    u8 cycle = learn_buffer[120];
    u8 ADDRESS_SAMPLING_VALUE[200];

    //u8 FreqL = learn_buffer[121];
    u8 *ADDRESS_INDEX_VALUE = &learn_buffer[0];
    u8 *ADDRESS_INDEX_LIST = &learn_buffer[14];

    memset(ADDRESS_SAMPLING_VALUE, 0, 200);
    //memset(IrBuffer,0x00,sizeof(IrBuffer));
    for (i = 0; i < 256; i++) {
        IrBuffer[i] = 0x00;
    }
    cycle++;

    temp = (u32) cycle * 4 * 10 / 5;
    if (temp == 0) {
//		LOGE("ET4003_Learndata_UnCompress temp equal zero \n");
        return 0;
    }
    *freq = (1000000 * 5) / (cycle * 4);
    if (*freq == 0) {
        return 0;
    }
    if (count_total != 0xff) {
        n_ADDRESS_INDEX_LIST = 0;
        v_SAMPLING_VALUE_Tmp = 0;
        v_INDEX_LIST_Tmp = 0;

        for (n_ADDRESS_INDEX_LIST = 0; n_ADDRESS_INDEX_LIST != count_total;
             n_ADDRESS_INDEX_LIST++) {
            v_INDEX_LIST_Tmp = ADDRESS_INDEX_LIST[n_ADDRESS_INDEX_LIST / 2];
            if ((n_ADDRESS_INDEX_LIST & 0x01) == 0) {
                v_INDEX_LIST_Tmp >>= 4;
            } else {
                v_INDEX_LIST_Tmp &= 0x0f;
            }

            if (v_INDEX_LIST_Tmp == 0x0f) {
                v_SAMPLING_VALUE_Tmp = 0xff;
            } else {
                if (v_INDEX_LIST_Tmp == 0x0e) {
                    v_SAMPLING_VALUE_Tmp = 0x7f;
                } else {
                    v_SAMPLING_VALUE_Tmp =
                            ADDRESS_INDEX_VALUE[v_INDEX_LIST_Tmp];
                }
            }

            ADDRESS_SAMPLING_VALUE[n_ADDRESS_INDEX_LIST] = v_SAMPLING_VALUE_Tmp;
        }

    }

    if ((ADDRESS_SAMPLING_VALUE[0] == 0xff)
        && (ADDRESS_SAMPLING_VALUE[1] == 0xff)
        && (ADDRESS_SAMPLING_VALUE[2] == 0xff)
        && (ADDRESS_SAMPLING_VALUE[3] == 0xff)
        && (ADDRESS_SAMPLING_VALUE[4] == 0xff)) {
        return 0;
    }

    for (count_total = 200, i = 0, n = 0; count_total != 0;
         count_total--, i++) {
        if (0 == ADDRESS_SAMPLING_VALUE[i])
            return 0;
        m = ADDRESS_SAMPLING_VALUE[i] & 0x7f;
        if (0x80 == (ADDRESS_SAMPLING_VALUE[i] & 0x80)) {
            if (0 == stauts) {
                n++;
                stauts = 1;
            }

            IrBuffer[n] += m * 47 * 10 / temp;
        } else {
            if (1 == stauts) {
                n++;
                stauts = 0;
            }

            IrBuffer[n] += m * 45 * 10 / temp;
        }
    }

    return n + 1;
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
	data_len = datas[8]+datas[9];

//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
    n_PartIndexCount = datas[12];
    n_Sample = datas[13];
    n_Index = datas[14]; 
    n_Freq = datas[15];
	n_type = datas[1];
	if(n_type!=0x31){
		 printf("n_type  error \r\n" );
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
       printf("partIndex[%d] = 0x%02x \r\n", i,learn_buffer[i] );
    }
   
    for(i=0;i<n_Sample;i++){
        learn_buffer[n_PartIndexCount+i] =datas[i+16+n_PartIndexCount+n_Index];
       printf("sample[%d] = 0x%02x \r\n", i,learn_buffer[n_PartIndexCount+i] );
    }

	
	for(i=0;i<n_Index;i++){
		   learn_buffer[n_PartIndexCount+n_Sample+i] =datas[i+16+n_PartIndexCount];
		 printf("n_Index[%d] = 0x%02x \r\n", i,learn_buffer[n_PartIndexCount+n_Sample+i] );
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
    u8  n_PartIndexCount;

    u8  n_Index;
    u8  n_Freq;
//    u8 n_flag;

	u8 n_type;
	u16 data_len;

    


    u8 PartIndexCount;
  

    int  n;

	u16 nHighLevel ;
	u16 nLowLevel ;

	data_len = datas[8]+datas[9];

//    u8 crc;
//    n_flag = datas[10];
    n_Crc = datas[11];
    n_PartIndexCount = datas[12];
  
    n_Index = datas[14]; 
    n_Freq = datas[15];
	n_type = datas[1];
	if(n_type!=0x30){
		 printf("n_type  error \r\n" );
        return -1;
		}
#ifdef ET_DEBUG
	 printf("len = %d \r\n  ",data_len );

	 printf("type = %x \r\n  ",n_type );

	 printf("state = %x \r\n  ",datas[10] );
	 printf("n_PartIndexCount = %d   n_Index = %d \r\n",n_PartIndexCount,n_Index );
#endif
		
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        printf("n_Freq  error \r\n" );
        return -2;
    }

	n_Index = 16;

    n_Freq--;
    *freq = 2500000 / n_Freq;

    printf("n_Freq = %d \r\n", 2500000 / n_Freq );

	if( n_Crc != xCal_crc(&datas[16], data_len )){
		   printf("n_Crc  error \r\n" );
		   return -3;
	   }
	



	
    n = 0;


	
	while (PartIndexCount--) {
		
			
			nHighLevel = (u16) datas[n_Index++];
			nHighLevel <<= 8;
			nHighLevel |= (u16) datas[n_Index++];
			nLowLevel = (u16) datas[n_Index++];
			nLowLevel <<= 8;
			nLowLevel |= (u16) datas[n_Index++];
		
            irpluse[n] = nHighLevel;
			n++;
            irpluse[n] =(nLowLevel * 8 / n_Freq)  +1;
			n++;
			if (0x0000ffff == nLowLevel) {
				return n;
			}
			
		
	}
	

	return n;
}


//初始化IIC接口
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


u8 ET4207SendTest(void){
	
	u8 err=0;
	err =	ET4207SendCode(send_etcode,sizeof(send_etcode));
	return err;

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
  * @Name   : ET4207 学习开始
  *
  * @Brief  : none
  *
  * @Input  : mode:       0为从rmt脚学习红外码 为1从P14学习红外码
  *           algorithm:   算法 0 为标准不压缩 1为高度压缩 2为简约压缩
  *
  * @Output : err:     返回的错误值 0 为成功 
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



u8 ET4207ReadCode(u8 *etcode){
	
	u8 err=0;
	int length = 440;
	int i;
	u16 freq;
	u16 ircode[1024];
	u32 irpulse;
	if(length >440){
		return 0xf0;
		}
	err = Hard_IIC_PageRead(I2C1,ET4207_ADDRESS,_ET4207_CONTROL_READ_CODE_,length,etcode);
	if(err!=0){
		return err;
		}
	
#ifdef ET_DEBUG
	for(i=0;i<440;i++){
		printf("0x%02x,",etcode[i]);
		}
		printf("\r\n");
#endif
	err =  et4207_UnCompress_zip(etcode, ircode, &freq) ;
	
	if(err>0){
		for(i=0;i<err;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 printf("%d,",irpulse);
			}
		 printf("\r\n" );
		}else {
		 printf("learn err =  %d",err);
			}
		

	err =  et4207_UnCompress_normal(etcode, ircode, &freq) ;
	
	if(err>0){
		for(i=0;i<err;i++){
			 irpulse = (u32)ircode[i] *1000000/freq;
			 printf("%d,",irpulse);
			}
		}else {
		 printf("learn err =  %d",err);
			}
		 printf("\r\n" );
	return err;

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
  * @Name   : 硬件IIC发送多个字节数据
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:       IIC组
  *           SlaveAdd:   作为从设备时识别地址
  *           WriteAdd:   写入内存起始地址
  *           NumToWrite: 写入数据量
  *           *pBuffer:   写入的数据组缓存
  *
  * @Output : *err:     返回的错误值
  *
  * @Return : none
  *****************************************************************************
**/
u8  Hard_IIC_WriteNByte(I2C_TypeDef * IICx, u8 SlaveAdd, u8 WriteAdd, u16 NumToWrite, u8 * pBuffer)
{
	u16 sta = 0;
	u16 temp = 0;
	u8 err = 0;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //等待IIC
	{
		temp++;
		if (temp > 800)
		{
			err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
		}
	}
	delay_us(10);
	//
	//读取SR2状态寄存器
	//
	sta = IICx->SR2;  //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位，不可少！！！！！！！！！
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
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
		}
	}
	delay_us(10);
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
				err |= 1<<4;
				 printf("send err =  %d",err);
				I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
				return err;
			}
		}
	}
	I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
		delay_us(100);
		return err;
}


/**
  *****************************************************************************
  * @Name   : 硬件IIC读取多个字节数据
  *
  * @Brief  : none
  *
  * @Input  : I2Cx:      IIC组
  *           SlaveAdd:  作为从设备时识别地址
  *           ReadAdd:   读取的内存起始地址
  *           NumToRead: 读取数量
  *
  * @Output : *pBuffer: 数据输出缓冲区
  *           *err:     返回的错误值
  *
  * @Return : 读取到的数据
  *****************************************************************************
**/
u8 Hard_IIC_PageRead(I2C_TypeDef* IICx, u8 SlaveAdd, u8 ReadAdd, u16 NumToRead, u8 * pBuffer)
{
	u16 i = 0;
//	u16 temp = 0;
	u16 sta = 0;
	u8 err = 0;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY))  //等待IIC
	{
		i++;
		if (i > 800)
		{
			err |= 1<<0;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<1;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<2;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<3;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
		}
	}
	//I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
	delay_us(10);
	
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
			err |= 1<<4;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
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
			err |= 1<<5;
			I2C_GenerateSTOP(IICx, ENABLE);  //产生停止信号
			return err;
		}
	}
	delay_us(10);
	sta = IICx->SR2;  //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位，不可少！！！！！！！！！
	//
	//批量接收数据
	//
	I2C_AcknowledgeConfig(IICx, ENABLE);

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
	
	return err;
}







