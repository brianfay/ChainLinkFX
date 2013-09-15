#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "portaudio.h"
#include "ChainLinkFX_JNIBridge.h"


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
	jmethodID midConstructor = (*env)->GetMethodID(env,customTypeClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;III)V");
	PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo((PaDeviceIndex)deviceIndex);
	jint maxInputChannels = deviceInfo->maxInputChannels;
	jint maxOutputChannels = deviceInfo->maxOutputChannels;;
	char* deviceText = (char*)malloc(strlen(deviceInfo->name));
	char* hostApiText = (char*)malloc(strlen("This is my string."));
	
	strcpy(deviceText, deviceInfo->name);
	strcpy(hostApiText, "This is my string.");
	jstring deviceString = (*env)->NewStringUTF(env, deviceText);
	jstring hostApiString = (*env)->NewStringUTF(env, hostApiText);
	jobject device = (*env)->NewObject(env, customTypeClass, midConstructor,deviceString,hostApiString,deviceIndex,maxInputChannels,maxOutputChannels);
	free(hostApiText);
	free(deviceText);
	return device;
}

JNIEXPORT jint JNICALL Java_ChainLinkFX_JNIBridge_getDeviceCount(JNIEnv *env,
	jobject callingObject)
{
	jint i = (jint)Pa_GetDeviceCount();
	return i;
}
