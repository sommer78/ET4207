package com.etek.ircore;

/**
 * Created by jiangs on 2015/11/10.
 */
public class IRCore {



    public static native int init();
    public static native int close();
    public static native int write(byte[] data);
    public static native byte[] read(int command);
    public static native int readVersion();
    public static native int learnStart();
    public static native int learnStop();
    public static native int recLearnStart();

    public static native int setLearnTimeout(int time);
    public static native int learnLoop();
    //public static native int[] getET4207Learn(byte[] data);

    public static native int setCurrent(int data);
    static {
        System.loadLibrary("IRCore");
    }
}
