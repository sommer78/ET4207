//
// Created by 应用 on 2015/11/13.
//
#include "com_etek_ircore_LearnDecode.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "logo.h"



/******************************************************/
/*Funcation: et_xCal_crc                      	        				*/
/*Input:  	uint8_t *ptr	uint32_t len						*/
/*Output: 	uint8_t crc 	  								 */
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
/*Funcation: et_compare_time                      	        				*/
/*Input:  	emote_data data, uint32_t high_level,
		uint32_t low_level					*/
/*Output: 	true or false									 */
/*Desc: 		compare  remote data time 				*/

/******************************************************/
jboolean compareCenter(uint32_t data, uint32_t center) {

    if ((data > center - COMPARE_OFFSET) && (data < center + COMPARE_OFFSET)) {

        return JNI_TRUE;
    }

    return JNI_FALSE;

}

jboolean compare_time(uint32_t origHigh, uint32_t origLow, uint32_t sampHigh, uint32_t sampLow) {
    if (compareCenter(origHigh, sampHigh)) {
        if (compareCenter(origLow, sampLow)) {
            return JNI_TRUE;
        }
    }

    return JNI_FALSE;
}


/******************************************************/
/*Funcation: et_compare_alldata                      	        			*/
/*Input:  	emote_data data, uint32_t *sample int index		*/
/*Output: 	true or false						 			*/
/*Desc: 		compare  remote data to all sample  				*/

/******************************************************/
jboolean et_compare_alldata(uint32_t high_level,uint32_t low_level, uint32_t *sample, int index) {
    int i;
    uint32_t timeHigh, timeLow;

    for (i = 0; i < index; i += 2) {
        timeHigh = sample[i];
        timeLow = sample[i + 1];

        if (compare_time( high_level, low_level, timeHigh, timeLow)) {
            return JNI_TRUE;
        }
    }
    return JNI_FALSE;
}


void et_push_sample_time_data(uint32_t high_level,uint32_t low_level, uint32_t *sample, int index) {
    sample[index] = high_level;
    sample[index + 1] = low_level;
}

int et_sample_time_selection(uint32_t *original,uint32_t orig_count,uint32_t *sample) {
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
		uint32_t *sample, int index	   					*/
/*Output: 	index 	  								 */
/*Desc: 	data compare sample to get sample index  */

/******************************************************/

int et_get_index(uint32_t high_level, uint32_t low_level,uint32_t *sample, int index) {
    int i = 0;
    uint32_t timeHigh, timeLow;

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
		uint32_t *sample, int index	   					*/
/*Output: 	index 	  								 */
/*Desc: 	original data to get sample index to compress data     */

/******************************************************/
int et_get_data_index(uint32_t *original, uint32_t *sample,int index,int orig_count,uint8_t *data) {
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
/*Input:  	uint32_t *in int index			   */
/*Output: 	char *out  	   */
/*Desc: 	change uint32_t sample to double char sample  */

/******************************************************/
int et_compress_sample(uint32_t *sample,uint8_t *zp_sample,int sample_len) {
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

int et_compress_original_data(uint32_t *original, uint8_t freq,int orig_count ,uint8_t *et_data) {

    uint8_t temp[MAX_SEND_DATA];
    uint8_t data[MAX_ORIGINAL_DATA];
    uint32_t sample[MAX_INDEX];
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
        LOGD(" et_sample_time_selection  error \n\r");
        return sample_len;
    }


    couple_len = et_get_data_index(original, sample,sample_len,orig_count,data);

    if (couple_len<0){
        LOGD( "  et_get_data_index  error \n\r");
        return couple_len;
    }


    data_len = et_compress_data(data,zp_data,couple_len);
    if (data_len<0){
        LOGD( "  et_compress_data  error \n\r");
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
#if 0

    for (i = 0; i < ir_data->length; i++) {
        printk("remoteData[%d] ----> 0x%02x \n", i, ir_data->signal[i]);
        }
    printk(KERN_INFO  "remote couple is %d \n", ir_data->couple);
    printk(KERN_INFO  "remote freq is %d \n", ir_data->freq);
    printk(KERN_INFO  "remote length is %d \n", ir_data->length);
    printk(KERN_INFO  "remote index is %d \n", ir_data->index);
    printk(KERN_INFO  "remote data_count is %d \n", ir_data->data_count);
    printk(KERN_INFO  "remote count is %d \n", ir_data->count);
for (i = 0; i < MAX_INDEX; i++) {
        printk("sample [%d] ----> 0x%02x \n", i, ir_data->zp_sample[i]);
        }
#endif
    return length;

}

/*
*	成功:返回code_buffer的长度
*	失败:返回-1
*/
int et4007_Learndata_UnCompress(char *datas, int *code_buffer, int *freq) {
    uint8_t static n_Crc;
    uint8_t static n_PartIndexCount;
    uint8_t static n_Sample;
    uint8_t static n_Index;
    uint8_t static n_Freq;


    int temp;
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
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        return -1;
    }
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



char ET4003_Learndata_UnCompress(char *learn_buffer, int *IrBuffer, int *freq) {
    uint32_t i, m, n;
    uint32_t temp;
    uint8_t stauts = 1;
    uint8_t n_ADDRESS_INDEX_LIST;
    uint8_t v_SAMPLING_VALUE_Tmp;
    uint8_t v_INDEX_LIST_Tmp;
    uint8_t count_total = 200;
    uint8_t cycle = learn_buffer[120];
    uint8_t ADDRESS_SAMPLING_VALUE[200];

    //uint8_t FreqL = learn_buffer[121];
    uint8_t *ADDRESS_INDEX_VALUE = &learn_buffer[0];
    uint8_t *ADDRESS_INDEX_LIST = &learn_buffer[14];

    memset(ADDRESS_SAMPLING_VALUE, 0, 200);
    //memset(IrBuffer,0x00,sizeof(IrBuffer));
    for (i = 0; i < 256; i++) {
        IrBuffer[i] = 0x00;
    }
    cycle++;

    temp = (uint32_t) cycle * 4 * 10 / 5;
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
JNIEXPORT jbyteArray JNICALL Java_com_etek_ircore_LearnDecode_getET4007LearnCode(JNIEnv *env, jclass class, jbyteArray code) {
    int length;
    int i;
    int couple[2048];
    char signal[512];
    int freq;

    char *newCode = (char *) (*env)->GetByteArrayElements(env, code, NULL);
    length = (*env)->GetArrayLength(env, code);
    if (length < 10) {
        LOGE("et input data error = %d", length);
        return NULL;
    }
//    for (i = 0; i < length; i++) {
//        LOGD("data[%d] = 0x%x", i, newCode[i]);
//    }
    length = et4007_Learndata_UnCompress(newCode, couple, &freq);
    if (length < 5) {
    LOGE("et UnCompress error = %d", length);
    return NULL;
    }


    freq= 1200000/freq+1 ;

    length =  et_compress_original_data( couple, (char)freq, length ,signal);
    if(length<10){
        LOGE("et compress error = %d", length);
        return NULL;
    }

    jbyteArray byteArray = (*env)->NewByteArray(env, length);

    (*env)->SetByteArrayRegion(env, byteArray, 0, length, (jbyte*) signal);

    return byteArray;
}


JNIEXPORT jbyteArray JNICALL Java_com_etek_ircore_LearnDecode_getET4003LearnCode(JNIEnv *env, jclass class, jbyteArray code) {
    int length;
    int i;
    int couple[2048];
    char signal[512];
    int freq;

    char *newCode = (char *) (*env)->GetByteArrayElements(env, code, NULL);
    length = (*env)->GetArrayLength(env, code);
    if (length < 10) {
        LOGE("et input data error = %d", length);
        return NULL;
    }
//    for (i = 0; i < length; i++) {
//        LOGD("data[%d] = 0x%x", i, newCode[i]);
//    }
    length = ET4003_Learndata_UnCompress(newCode, couple, &freq);
    if (length < 5) {
        LOGE("et UnCompress error = %d", length);
        return NULL;
    }


    freq= 1200000/freq+1 ;

    length =  et_compress_original_data( couple, (char)freq, length ,signal);
    if(length<10){
        LOGE("et compress error = %d", length);
        return NULL;
    }

    jbyteArray byteArray = (*env)->NewByteArray(env, length);

    (*env)->SetByteArrayRegion(env, byteArray, 0, length, (jbyte*) signal);

    return byteArray;
}

int et4207_UnCompress(char *datas, int *ircode, int *freq) {
    unsigned char  n_Crc;
    unsigned char  n_PartIndexCount;
    unsigned char  n_Sample;
    unsigned char  n_Index;
    unsigned char  n_Freq;
    unsigned char n_flag;
    int i;

    int temp;
    unsigned char learn_buffer[512];
    
    unsigned char unzip_end = 1;
    unsigned char dat_temp;

    unsigned char PartIndexCount;
    int Sample0_nHighLevel;
    int Sample0_nLowLevel;
    int Sample1_nHighLevel;
    int Sample1_nLowLevel;

    unsigned char n_PartIndexCount_p;
    unsigned char n_Sample_p;
    unsigned char n_Index_p;

    int m, n;
    unsigned char shift = 0x80;
    unsigned char crc;
    n_flag = datas[10];
    n_Crc = datas[11];
    n_PartIndexCount = datas[12];
    n_Sample = datas[13];
    n_Index = datas[14];
    n_Freq = datas[15];
    if((n_Freq>0x3e)&&(n_Freq<0x15)){
        LOGE("n_Freq  error" );
        return -1;
    }

//    n_Freq++;
    n_Freq--;
    *freq = 2500000 / n_Freq;
  //  *freq = n_Freq;
    LOGD("n_Freq = %d", 2500000 / n_Freq );

    for(i=0;i<n_PartIndexCount;i++){
        learn_buffer[i] =datas[i+16];
     //   LOGD("partIndex[%d] = 0x%x", i,learn_buffer[i] );
    }
    for(i=0;i<n_Sample;i++){
        learn_buffer[n_PartIndexCount+i] =datas[i+192];
      //  LOGD("n_Sample[%d] = 0x%x", i,learn_buffer[n_PartIndexCount+i] );
    }

    for(i=0;i<n_Index;i++){
        learn_buffer[n_PartIndexCount+n_Sample+i] =datas[i+128];
     //   LOGD("index[%d] = 0x%x", i,learn_buffer[n_PartIndexCount+n_Sample+i] );
    }
    //	if( n_Crc != xCal_crc(learn_buffer, n_PartIndexCount + n_Sample + n_Index )) return false;
//    crc = xCal_crc(learn_buffer, n_PartIndexCount + n_Sample + n_Index );
//    LOGD("CRC VALUE = %x   %x",crc,n_Crc);
    if( n_Crc != xCal_crc(learn_buffer, n_PartIndexCount + n_Sample + n_Index )){
        LOGE("n_Crc  error" );
        return -1;
    }
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
            ircode[n] = Sample1_nHighLevel;
			n++;
            ircode[n] = Sample1_nLowLevel * 8 / n_Freq;
            ircode[n]++;
			n++;
			if (0x0000ffff == Sample1_nLowLevel) {
				unzip_end = 0;
				break;
			}
		} else {
            ircode[n] = Sample0_nHighLevel;
			n++;
            ircode[n] = Sample0_nLowLevel * 8 / n_Freq;
            ircode[n]++;
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
                ircode[n] = Sample1_nHighLevel;
				n++;
                ircode[n] = Sample1_nLowLevel * 8 / n_Freq;
                ircode[n]++;
				n++;
				if (0x0000ffff == Sample1_nLowLevel) {
					unzip_end = 0;
					break;
				}
			} else {
                ircode[n] = Sample0_nHighLevel;
				n++;
                ircode[n] = Sample0_nLowLevel * 8 / n_Freq;
                ircode[n]++;
				n++;
			}
			shift >>= 1;
		}
		n_PartIndexCount_p++;
		n_Sample_p += 4;
	}
	return n;
}



JNIEXPORT jbyteArray JNICALL Java_com_etek_ircore_LearnDecode_getET4207LearnCode(JNIEnv *env, jclass class, jbyteArray code) {
    int length;
    int i;
    int couple[2048];
    char signal[512];
    int freq;

    char *newCode = (char *) (*env)->GetByteArrayElements(env, code, NULL);
    length = (*env)->GetArrayLength(env, code);
    if (length < 10) {
        LOGE("et input data error = %d", length);
        return NULL;
    }
//    for (i = 0; i < length; i++) {
//        LOGD("data[%d] = 0x%x", i, newCode[i]);
//    }
    length = et4207_UnCompress(newCode, couple, &freq);
    if (length < 5) {
        LOGE("et UnCompress error = %d", length);
        return NULL;
    }


    freq= 1200000/freq+1 ;

    length =  et_compress_original_data( couple, (char)freq, length ,signal);
    if(length<10){
        LOGE("et compress error = %d", length);
        return NULL;
    }

    jbyteArray byteArray = (*env)->NewByteArray(env, length);

    (*env)->SetByteArrayRegion(env, byteArray, 0, length, (jbyte*) signal);

    return byteArray;
}