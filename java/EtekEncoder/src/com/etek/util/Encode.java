package com.etek.util;

public class Encode {
	/**
	 * Logger for this class
	 */

	// static byte[] code;
	final static int MAX_SAMPLE_INDEX = 32;
	final static int MAX_DATA = 310;
	final static int MAX_SAMPLE = 64;
	final static String TAG = "ENCODE";

	public static String encodeToEtek(int freq, int[] data) {

		byte[] code = getEtCode((char) (1200000 / freq + 1), data);

		return Tools.bytesToHexString(code);
	}

	private static char calCrc(char[] data) {
		char crc = 0;
		char tmp = 0;
		int i, j;
		for (j = 0; j < data.length; j++) {

			crc ^= data[j];

			for (i = 0; i < 8; i++) {
				crc = (char) (crc & 0xff);
				tmp = (char) (crc & 0x01);

				if (tmp > 0) {
					crc = (char) ((crc >> 1) ^ 0x8C);
				} else {
					crc >>= 1;
				}

			}

		}

		return crc;
	}

	private static boolean compareTime(int origHigh, int origLow, int sampHigh,
			int sampLow) {
		if (compareCenter(origHigh, sampHigh, 3)) {
			if (compareCenter(origLow, sampLow, 3)) {
				return true;
			}
		}

		return false;
	}

	private static boolean compareAllData(int origHigh, int origLow, int index,
			int[] sample) {
		int i;
		int timeHigh, timeLow;

		for (i = 0; i < index; i += 2) {
			timeHigh = sample[i];
			timeLow = sample[i + 1];
			if (compareTime(origHigh, origLow, timeHigh, timeLow)) {
				return true;
			}
		}
		return false;

	}

	static void pushSampleData(int high, int low, int index, int[] sample) {
		// Log.d(TAG, "push high = " + high + "    low = " + low);
		sample[index] = high;
		sample[index + 1] = low;
	}

	private static int[] getSampleData(int[] irData) {
		int i, index = 0;
		int origHigh, origLow;
		int[] sample = new int[64];
		for (i = 0; i < irData.length; i += 2) {
			origHigh = irData[i];
			origLow = irData[i + 1];
			// Log.d(TAG, "origHigh = " + origHigh + "    origLow = " +
			// origLow);
			// logger.info("origHigh = "+origHigh+"    origLow = "+ origLow );
			if (index == 0) {
				/* first data send */
				pushSampleData(origHigh, origLow, index, sample);
				index += 2;

			} else {
				if (!compareAllData(origHigh, origLow, index, sample)) {

					pushSampleData(origHigh, origLow, index, sample);
					index += 2;
					if (index >= MAX_SAMPLE_INDEX) {

						return null;
					}
				}
			}
		}
		int[] newSample = new int[index];
		for (i = 0; i < index; i++) {
			newSample[i] = sample[i];
		}

		return newSample;
	}

	static char[] getCompressSample(int[] sample) {
		int i, j = 0;
		char[] zp_sample = new char[sample.length * 2];
		for (i = 0; i < sample.length; i++) {
			zp_sample[j++] = (char) ((sample[i] >> 8) & 0xff);
			zp_sample[j++] = (char) sample[i];
		}
		return zp_sample;
	}

	private static byte[] getEtCode(char freq, int[] original) {
		// logger.info(etc.toString());
		int actLength = 0;
		int[] sample = getSampleData(original);

		if (sample == null) {
			// Log.e(TAG, "get sample error");
			return null;
		}
		// for (int i=0;i<index;i++){
		// logger.info("sample["+i+"] = "+ sample[i] );
		// }
		int[] data = getAllDataIndex(original, sample);
		if (data == null) {
			// Log.e(TAG, "get data error");
			return null;
		}
		int[] compressData = getCompressData(data);
		char[] compressSample = getCompressSample(sample);
		actLength = compressData.length;

		int tempcount = actLength + MAX_SAMPLE;
		char[] dataTemp = new char[tempcount];
		for (int i = 0; i < compressSample.length; i++) {
			dataTemp[i] = compressSample[i];
		}
		int j = 0;
		for (int i = MAX_SAMPLE; i < tempcount; i++) {

			dataTemp[i] = (char) compressData[j++];
		}
		char crc = calCrc(dataTemp);
		int length = 10 + MAX_SAMPLE + actLength;
		byte[] code = new byte[length];
		code[0] = 0x54;
		code[1] = (byte) (length >> 8);
		code[2] = (byte) length;
		code[3] = (byte) freq;
		code[4] = (byte) (data.length >> 8);
		code[5] = (byte) (data.length);
		code[6] = 0x00; // reserve
		code[7] = 0x00; // reserve
		code[8] = 0x01;
		code[9] = (byte) crc;
		for (int i = 0; i < tempcount; i++) {
			code[i + 10] = (byte) dataTemp[i];
		}
		return code;

	}

	private static int getIndex(int origHigh, int origLow, int index,
			int[] sample) {
		int i = 0;
		int timeHigh, timeLow;

		for (i = 0; i < index; i += 2) {
			timeHigh = sample[i];
			timeLow = sample[i + 1];

			if (compareTime(origHigh, origLow, timeHigh, timeLow)) {
				return i;
			}
		}
		return 0;

	}

	private static int[] getAllDataIndex(int[] irData, int[] sample) {
		int i = 0, count = 0;
		int temp;
		int timeHigh, timeLow;

		int[] data = new int[400];
		int maxIndex = sample.length;
		for (i = 0; i < irData.length; i += 2) {
			timeHigh = irData[i];
			timeLow = irData[i + 1];
			temp = getIndex(timeHigh, timeLow, maxIndex, sample);
			data[count++] = (temp / 2);
			if (count > (MAX_DATA * 2)) {
				return null;
			}

		}
		int[] retData = new int[count];
		for (i = 0; i < count; i++) {
			retData[i] = data[i];
			// Log.d(TAG, "data[" + i + "]=" + data[i]);
		}

		return retData;
	}

	private static int[] getCompressData(int[] data) {
		int[] compress;
		if (data.length % 2 == 1) {
			compress = new int[data.length / 2 + 1];
		} else {
			compress = new int[data.length / 2];

		}
		// Log.d(TAG,"length = "+compress.length+"   data = "+data.length);
		int temp;

		int j = 0;
		int i = 0;
		while (i < data.length) {

			temp = (data[i++] << 4) & 0xf0;
			if (i != data.length) {
				temp |= (data[i++]) & 0x0f;
			}
			// printk("ir_data->data[%d] ----> 0x%x \n",j,temp);
			compress[j] = temp;
			// Log.d(TAG,"compress[ "+j +"   ] = "+ compress[j]);
			j++;

		}

		return compress;
	}

	public static boolean compareCenter(int data, int center, int offset) {

		if ((data > center - offset) && (data < center + offset)) {

			return true;
		}

		return false;

	}

}
