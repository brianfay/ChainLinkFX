#include "Effects.h"
#include <stdlib.h>

extern int sampleRate;

void* initEmptyEffect()
{
	void* effectPtr = NULL;
	
	return effectPtr;
}

void emptyEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	//seriously do nothing here
}

void freeEmptyEffect(ChainLink* chainLink)
{
	
}

void* initFeedbackDelayEffect()
{
	void* effectPtr;
	FeedbackDelayData* delayData = malloc(sizeof(FeedbackDelayData));
	//quickly checking absurd memory use
	delayData->delayBuffer = malloc(sizeof(float)*sampleRate*6); //6 second delay buffer
	delayData->writeIndex = 0;
	delayData->readIndex = 0;
	delayData->bufferSize = sampleRate*6;
	delayData->delayTimeInMs = 1000;
	delayData->feedback = 50;
	effectPtr = delayData;
	return effectPtr;
}

void feedbackDelayEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	ChainLink* chainLink = (ChainLink*) functionChain;
	FeedbackDelayData* delayData = (FeedbackDelayData*) chainLink->effectData;
	float actualDelayInSamples= (float) delayData->delayTimeInMs * (float)sampleRate * 0.001;
	delayData->delayTimeInSamples = (int)actualDelayInSamples;
	delayData->readIndex = delayData->writeIndex;
	delayData->readIndex -= delayData->delayTimeInSamples;
	while(delayData->readIndex < 0){
		delayData->readIndex += delayData->bufferSize;
	}
	while(delayData->readIndex > delayData->bufferSize){
		delayData->readIndex -= delayData->bufferSize;
	}
	delayData->writeIndex++;
	delayData->writeIndex %= delayData->bufferSize;
	//now set output to current value + delayed value
	*out += (delayData->delayBuffer[delayData->readIndex] * (delayData->feedback*0.01));
	//delayBuffer gets set here rather than earlier so that it can feed back
	delayData->delayBuffer[delayData->writeIndex] = *out;
}

void freeFeedbackDelayEffect(ChainLink * chainLink)
{
	FeedbackDelayData * data;
	data = chainLink->effectData;
	free(data->delayBuffer);
	free(data);
}

void* initSingleTapDelayEffect()
{
	void* effectPtr;
	SingleTapDelayData* delayData = malloc(sizeof(SingleTapDelayData));
	delayData->delayBuffer = malloc(sizeof(float)*sampleRate*6); //6 seconds for buffer
	delayData->writeIndex = 0;
	delayData->readIndex = 0;
	delayData->bufferSize = sampleRate*6;
	delayData->delayTimeInMs = 1000;
	delayData->gain = 50;
	effectPtr = delayData;
	return effectPtr;
}

void singleTapDelayEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	ChainLink* chainLink = (ChainLink*) functionChain;
	SingleTapDelayData* delayData = (SingleTapDelayData*) chainLink->effectData;
	float actualDelayInSamples= (float) delayData->delayTimeInMs * (float)sampleRate * 0.001;
	delayData->delayTimeInSamples = (int)actualDelayInSamples;
	delayData->readIndex = delayData->writeIndex;
	delayData->readIndex -= delayData->delayTimeInSamples;
	while(delayData->readIndex < 0){
		delayData->readIndex += delayData->bufferSize;
	}
	while(delayData->readIndex > delayData->bufferSize){
		delayData->readIndex -= delayData->bufferSize;
	}
	delayData->writeIndex++;
	delayData->writeIndex %= delayData->bufferSize;
	delayData->delayBuffer[delayData->writeIndex] = *out;
	//now set output to current value + delayed value
	*out += (delayData->delayBuffer[delayData->readIndex] * (delayData->gain*0.01));
}

void freeSingleTapDelayEffect(ChainLink * chainLink)
{
	SingleTapDelayData * data;
	data = chainLink->effectData;
	free(data->delayBuffer);
	free(data);
}
