#include "Effects.h"
#include <stdlib.h>

void* initEmptyEffect()
{
	void* effectPtr = NULL;
	
	return effectPtr;
}

void emptyEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	//seriously do nothing here
}

void* initDelayEffect()
{
	void* effectPtr;
	DelayData* delayData = malloc(sizeof(DelayData));
	delayData->delayBuffer = malloc(sizeof(float)*224000); //placeholder
	delayData->writeIndex = 0;
	delayData->readIndex = 0;
	delayData->bufferSize = 224000;
	delayData->delayTimeInMs = 1000;
	delayData->feedback = 50;
	effectPtr = delayData;
	return effectPtr;
}

void delayEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	ChainLink* chainLink = (ChainLink*) functionChain;
	DelayData* delayData = (DelayData*) chainLink->effectData;
	//*out = *in;
	float actualDelayInSamples= (float) delayData->delayTimeInMs * 44800.0 * 0.001;
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
