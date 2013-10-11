#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "portaudio.h"
#include "ChainLinkFX_JNIBridge.h"
#include "ChainLinkFX.h"
#include "Effects.h"


JNIEXPORT void JNICALL Java_ChainLinkFX_JNIBridge_initPA(JNIEnv *env,
	jobject callingObject)
{
	Pa_Initialize();
	return;
}

JNIEXPORT void JNICALL Java_ChainLinkFX_JNIBridge_terminatePA(JNIEnv *env,
	jobject callingObject)
{
	Pa_Terminate();
	return;
}

JNIEXPORT jobject JNICALL Java_ChainLinkFX_JNIBridge_constructDevice(JNIEnv *env,
	jobject callingObject, jint deviceIndex)
{
	jclass customTypeClass = (*env)->FindClass(env,"ChainLinkFX/Device");
	jmethodID midConstructor = (*env)->GetMethodID(env,customTypeClass, "<init>", "(Ljava/lang/String;III)V");
	const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo((PaDeviceIndex)deviceIndex);
	jint maxInputChannels = deviceInfo->maxInputChannels;
	jint maxOutputChannels = deviceInfo->maxOutputChannels;;
	char* deviceText = (char*)malloc(strlen(deviceInfo->name));
	
	strcpy(deviceText, deviceInfo->name);
	jstring deviceString = (*env)->NewStringUTF(env, deviceText);
	jobject device = (*env)->NewObject(env, customTypeClass, midConstructor,deviceString,deviceIndex,maxInputChannels,maxOutputChannels);
	free(deviceText);
	return device;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_getDeviceCount(JNIEnv *env,
	jobject callingObject)
{
	jint i = (jint)Pa_GetDeviceCount();
	return i;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_addChain(JNIEnv *env,
	jobject callingObject, jint inputDeviceIndex, jint outputDeviceIndex)
{
	int err = newChain((int)inputDeviceIndex, (int)outputDeviceIndex);
	if(err != 0){
		return -1;
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_removeChain(JNIEnv *env,
	jobject callingObject, jint chainIndex)
{
	int err = removeChain((int)chainIndex);
	return err;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_addChainLink(JNIEnv *env,
	jobject callingObject, jint chainIndex, jint effectType)
{
	int effectTypeInt = (int)effectType;
	//hope this cast to enum will work
	//int err = newChainLink((int)chainIndex, (EffectType)effectTypeInt);
	int err = newChainLink((int)chainIndex, 1);
	return err;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_setParameter(JNIEnv *env,
	jobject callingObject, jint chainIndex,jint effectIndex,jint parameterIndex,jfloat value)
{
	return 0;
}
