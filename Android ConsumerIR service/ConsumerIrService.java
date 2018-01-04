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

package com.android.server;

import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.IConsumerIrService;
import android.os.PowerManager;
import android.util.Slog;

import java.lang.RuntimeException;

public class ConsumerIrService extends IConsumerIrService.Stub {
	 int trig = 0;
    private static final String TAG = "ConsumerIrService";

    private static final int MAX_XMIT_TIME = 2000000; /* in microseconds */

    private static native long halOpen();
    private static native int halTransmit(long halObject, int carrierFrequency, int[] pattern);
    private static native int[] halGetCarrierFrequencies(long halObject);

	private static native int halGetIRDevice(long halObject);
	
	private static native int halGetIRState(long halObject);

	private static native int[] halGetIRCode(long halObject);

	private static native int halLearnCmd(long halObject, int cmd);
	

    private final Context mContext;
    private final PowerManager.WakeLock mWakeLock;
    private final long mNativeHal;
    private final Object mHalLock = new Object();

	private static int  learnState = 0;

    ConsumerIrService(Context context) {
        mContext = context;
        PowerManager pm = (PowerManager)context.getSystemService(
                Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
        mWakeLock.setReferenceCounted(true);

        mNativeHal = halOpen();
        if (mContext.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CONSUMER_IR)) {
            if (mNativeHal == 0) {
                throw new RuntimeException("FEATURE_CONSUMER_IR present, but no IR HAL loaded!");
            }
        } else if (mNativeHal != 0) {
            throw new RuntimeException("IR HAL present, but FEATURE_CONSUMER_IR is not set!");
        }
    }

    @Override
    public boolean hasIrEmitter() {
        return mNativeHal != 0;
    }

    private void throwIfNoIrEmitter() {
        if (mNativeHal == 0) {
            throw new UnsupportedOperationException("IR emitter not available");
        }
    }


    @Override
    public void transmit(String packageName, int carrierFrequency, int[] pattern) {
        if (mContext.checkCallingOrSelfPermission(android.Manifest.permission.TRANSMIT_IR)
                != PackageManager.PERMISSION_GRANTED) {
            throw new SecurityException("Requires TRANSMIT_IR permission");
        }

        long totalXmitTime = 0;

        for (int slice : pattern) {
            if (slice <= 0) {
                throw new IllegalArgumentException("Non-positive IR slice");
            }
            totalXmitTime += slice;
        }

        if (totalXmitTime > MAX_XMIT_TIME ) {
            throw new IllegalArgumentException("IR pattern too long");
        }

        throwIfNoIrEmitter();

        // Right now there is no mechanism to ensure fair queing of IR requests
        synchronized (mHalLock) {
            int err = halTransmit(mNativeHal, carrierFrequency, pattern);

            if (err < 0) {
                Slog.e(TAG, "Error transmitting: " + err);
            }
        }
    }



	@Override
	public String getIrInfo() {


	int ret =   halGetIRDevice(mNativeHal);
	
           
    String str = String.format("%08x",ret);

    
    return str;
    
	}


	@Override
	public int[] learnIR(int timeout) {
	//halLearnCmd(mNativeHal,1);
	learnState = 1;
	int[] value = null;
	int devCount = 5;
	int state = 0;
	boolean learnStart = false ;
	while(devCount>0){
		halLearnCmd(mNativeHal,1);
	try {
			Thread.sleep(100);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
	state = halGetIRState(mNativeHal);
	if(state>0){
		learnStart = true;
		break;
		}
	devCount--;
	}
	
//	Slog.d(TAG, "learnIR: devCount = "+devCount);	
	if(learnStart==false){
		value = new int[1];
		value[0] = 10;
		Slog.e(TAG, "learnIR: learnStart failed ");
		return value;
		}
	
//	Slog.d(TAG, "learnIR: learnStart  ");	
	while((timeout--)>0){
		state = halGetIRState(mNativeHal);
		if(learnState==0){
			value = new int[1];
			value[0] = 11;
			return value;
			}
		if(state==0){
			learnState = 0;
			value = halGetIRCode(mNativeHal);
			Slog.d(TAG, "learnIR: halGetIRCode finish ");
			return value;
			}
		
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		}
	return value;
	}

	
	@Override
	public void cancelLearning() {
	learnState = 0;
	/*if(trig>0){
		trig = 0;
		}else {
		trig = 1;
			}
			*/
	halLearnCmd(mNativeHal,0);
	
	}

	@Override
	public int getState() {
	int state = 0;
	state = halGetIRState(mNativeHal);
	return state;
	}

    @Override
    public int[] getCarrierFrequencies() {
        if (mContext.checkCallingOrSelfPermission(android.Manifest.permission.TRANSMIT_IR)
                != PackageManager.PERMISSION_GRANTED) {
            throw new SecurityException("Requires TRANSMIT_IR permission");
        }

        throwIfNoIrEmitter();

        synchronized(mHalLock) {
            return halGetCarrierFrequencies(mNativeHal);
        }
    }
}
