//
// Created by jiangs on 2015/11/10.
//
#include <jni.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include "logo.h"
#include "com_etek_ircore_IRCore.h"





#define  DEVICE_NAME  "/dev/irremote"  //device point
int remote_fd = 0;
struct timeval tv;

#define   ET_CMD_START_RMT_LEARN	5
#define   ET_CMD_START_REC_LEARN	7
#define   ET_CMD_STOP_LEARN		6
#define   ET_CMD_REPEAT			10
#define  ET_CMD_VERSION			4
#define  ET_CMD_SET_CURRENT		8


JNIEXPORT jint JNICALL Java_com_etek_ircore_IRCore_init
        (JNIEnv *env , jclass obj){
    remote_fd = open(DEVICE_NAME, O_RDWR);

    return remote_fd;
}

/*
 * Class:     com_etek_ircore_IRCore
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_etek_ircore_IRCore_close
        (JNIEnv *env , jclass obj){
                   LOGD("ircore finish ");
                   close(remote_fd);

                   return 1;
               }

/*
 * Class:     com_etek_ircore_IRCore
 * Method:    write
 * Signature: ([B)I
 */
 JNIEXPORT jint JNICALL Java_com_etek_ircore_IRCore_write
        (JNIEnv *env , jclass obj, jbyteArray data){

     int len;
     jbyte *newData = (*env)->GetByteArrayElements(env, data,NULL);
     len =  (*env)->GetArrayLength(env, data);
     LOGD("len =  %d   ", len);
     if(len<449&&len>10){
         len = write(remote_fd, newData, len);
     }else {
         LOGE("write len error =  %d  ", len);
         return 0;
     }

     return len;
 }

/*
 * Class:     com_etek_ircore_IRCore
 * Method:    read
 * Signature: (I)[B
 */
 JNIEXPORT jbyteArray JNICALL Java_com_etek_ircore_IRCore_read
        (JNIEnv *env , jclass obj, jint data){
     int length = 448;

     char buf[512];

     int len;
     len = read(remote_fd, buf, length);
     LOGD("len = %d ", len);
     if (len < 0) {
         LOGE("read code    %s error  ", DEVICE_NAME);
         return NULL;
     }
     jbyteArray byteArray = (*env)->NewByteArray(env, length);
     (*env)->SetByteArrayRegion(env, byteArray, 0, length, (jbyte *) buf);
     return byteArray;
 }




void Java_com_etek_ircore_IRCore_setLearnTimeout(JNIEnv* env, jobject jobj,
                                                     jint time) {
    tv.tv_sec = time;

}


jint Java_com_etek_ircore_IRCore_learnStop(JNIEnv* env) {
    int err;
    err = ioctl(remote_fd, ET_CMD_STOP_LEARN, 0x01);
    if (err == -1) {
        LOGE("ioctl learn IR code stop   %s error \n ", DEVICE_NAME);
        return 0;
    } else {
        return 1;
    }
}

jint Java_com_etek_ircore_IRCore_learnLoop(JNIEnv* env) {
    int rc;
    fd_set fds;
    int dev;
    FD_ZERO(&fds);

    FD_SET(remote_fd, &fds);
    tv.tv_sec = 50;
    switch (select(remote_fd + 1, &fds, NULL, NULL, &tv)) //select浣跨敤
    {
        LOGE(" learn IR code select loop   %s  \n ", DEVICE_NAME);
        case -1:
            LOGE(" learn IR code select error   %s error \n ", DEVICE_NAME);
            dev = -1;
            break;
        case 0:
            LOGE(" learn IR code select timeout   %s  \n ", DEVICE_NAME);
            dev = 0;

            break;
        default:
            if (FD_ISSET(remote_fd,&fds)) {
                dev = 1;
                LOGD(" learn IR code select   %s successful \n ", DEVICE_NAME);

            }
            break;
    } // end switch
    jint devStatus = dev;
    return devStatus;
}

jint Java_com_etek_ircore_IRCore_learnStart(JNIEnv* env) {
    int err;
    err = ioctl(remote_fd, ET_CMD_START_RMT_LEARN, 0x01);
    if (err == -1) {
        LOGE("ioctl learn IR code start   %s error  ", DEVICE_NAME);
        return 0;
    } else {
        return 1;
    }
}

jint Java_com_etek_ircore_IRCore_recLearnStart(JNIEnv* env) {
    int err;
    err = ioctl(remote_fd, ET_CMD_START_REC_LEARN, 0x01);
    if (err == -1) {
        LOGE("ioctl learn IR code start   %s error  ", DEVICE_NAME);
        return 0;
    } else {
        return 1;
    }
}


jint Java_com_etek_ircore_IRCore_readVersion(JNIEnv* env) {
    int err;

    err = ioctl(remote_fd, ET_CMD_VERSION, 0x01);
    if (err == -1) {
        LOGE("ioctl learn IR code start   %s error  ", DEVICE_NAME);
        return 0;
    } else {
        return err;
    }
}
jint Java_com_etek_ircore_IRCore_setCurrent(JNIEnv* env,jclass obj,jint d) {
    int err;
    int value = d;
    LOGD("  set current = %d  ", value);

    err = ioctl(remote_fd, ET_CMD_SET_CURRENT, value);
    if (err == -1) {
        LOGE("Java_com_etek_ircore_IRCore_setCurrent  %s error  ", DEVICE_NAME);
        return 0;
    } else {
        return err;
    }
}


