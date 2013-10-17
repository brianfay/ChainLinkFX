#include "Effects.h"
#include "portaudio.h"
#include "ChainLinkFX.h"
#include <stdio.h>
#include <stdlib.h>
#define PA_SAMPLE_TYPE paFloat32

Chain* rootChain = NULL;
int newChain(PaDeviceIndex inputDeviceIndex, PaDeviceIndex outputDeviceIndex)
{
	PaError err;
	//malloc memory for new node
	Chain* newChain = malloc(sizeof(Chain));
	//set new final node's next pointer to NULL
	newChain->nextChain = NULL;
	if(rootChain == NULL){
		rootChain = newChain;
	}
	else{
		Chain* iterator = rootChain;
		while(iterator->nextChain != NULL){
			iterator = iterator->nextChain;
		}
		//set final node's next node pointer to new node
		iterator->nextChain = newChain;
	}
	newChain->inputParameters.device = inputDeviceIndex;
	if(newChain->inputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Input device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	const PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(inputDeviceIndex);
	const PaDeviceInfo* outputDeviceInfo = Pa_GetDeviceInfo(outputDeviceIndex);
	newChain->inputParameters.channelCount = inputDeviceInfo->maxInputChannels;
	newChain->inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	newChain->inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;
	newChain->inputParameters.hostApiSpecificStreamInfo = NULL;
	newChain->outputParameters.device = outputDeviceIndex;
	if(newChain->outputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Output device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	newChain->outputParameters.channelCount = outputDeviceInfo->maxOutputChannels;
	newChain->outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	newChain->outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency;
	newChain->outputParameters.hostApiSpecificStreamInfo = NULL;
	
	//set root ChainLink to empty effect that does nothing, because it needs to be initialized
	//to something in order to be passed to the callback
	//should probably set this to be a gain adjustment; control output level and input level
	newChain->chainLink = malloc(sizeof(ChainLink));
	newChain->chainLink->effectType = IO;
	newChain->chainLink->effectData = initIOEffect();
	newChain->chainLink->effectFunction = &IOEffect;
	newChain->chainLink->nextLink = NULL;
	
	//set global number of input and output channels
	numInputChannels = newChain->inputParameters.channelCount;
	numOutputChannels = newChain->outputParameters.channelCount;
	
	//set the global sampleRate here
	sampleRate = inputDeviceInfo->defaultSampleRate;
	
	err = Pa_OpenStream(
			&(newChain->stream),
			&(newChain->inputParameters),
			&(newChain->outputParameters),
			inputDeviceInfo->defaultSampleRate,	
			paFramesPerBufferUnspecified,
			paClipOff,
			audioCallback,
			newChain->chainLink);
	if(err != paNoError) goto error;
	err = Pa_StartStream(newChain->stream);
	if( err != paNoError ) goto error;
	return 0;
	error:
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}

int audioCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
	SAMPLE *out = (SAMPLE*)outputBuffer;
	const SAMPLE *in = (const SAMPLE*)inputBuffer;
	unsigned int i;
	(void) timeInfo;
	(void) statusFlags;
	ChainLink* functionIterator = (ChainLink*) userData;
	IOData* ioData = functionIterator->effectData;
	int currentChannel;
	if(inputBuffer == NULL)
	{
		for(i=0; i<framesPerBuffer; i++)
		{
			for(currentChannel = 0; currentChannel < numOutputChannels; currentChannel++)
			{
			*out++ = 0;
			}
		}
	}
	else
	{
		//mono input, any number of output channels
		if(numInputChannels == 1)
		{
			for(i = 0; i<framesPerBuffer; i++)
			{
				for(currentChannel = 0; currentChannel < numOutputChannels; currentChannel++)
				{
				//scale input
				*out = (*in * (ioData->inputGain * 0.01));
				functionIterator->effectFunction(in,out,functionIterator);
					while(functionIterator->nextLink != NULL){
						functionIterator = functionIterator->nextLink;
						functionIterator->effectFunction(in,out,functionIterator);
					}
				functionIterator = (ChainLink*) userData;
				//scale output
				*out *= (ioData->outputGain * 0.01);
				out++;
				}
				in++;
			}
		}
		
		//equal number of input and output channels eg. stereo in, stereo out
		else if(numInputChannels == numOutputChannels)
		{
			for(i = 0; i<framesPerBuffer; i++)
			{
				for(currentChannel = 0; currentChannel < numOutputChannels; currentChannel++)
				{
				//scale input
				*out = (*in * (ioData->inputGain * 0.01));
				functionIterator->effectFunction(in,out,functionIterator);
					while(functionIterator->nextLink != NULL){
						functionIterator = functionIterator->nextLink;
						functionIterator->effectFunction(in,out,functionIterator);
					}
				functionIterator = (ChainLink*) userData;
				//scale output
				*out *= (ioData->outputGain * 0.01);
				*out++;
				*in++;
				}
			}
		}
	}
	return paContinue;
}


//check for mem leaks - will this free all chainlinks associated with a chain?
int removeChain(int chainIndex)
{
	if(rootChain == NULL){
		//this totally shouldn't happen but I'll keep it here for debug purposes
		printf("rootChain is null!?\n");
		return -1;
	}
	Chain* chainToRemove = rootChain;
	int i;
	
	//start by removing all chainLinks from the chain to free memory
	ChainLink * chainLinkIterator = chainToRemove->chainLink;
	i = 0;
	while(chainLinkIterator->nextLink != NULL){
		chainLinkIterator = chainLinkIterator->nextLink;
		i++;
	}
	int j;
	for(j = i; j >= 0; j--){
		fprintf(stderr, "in for loop, removing link %d \n", j);
		removeChainLink(chainIndex, j);
	}
	
	//if root chain: 
	if(chainIndex == 0){
		//if next chain exists:
		if(chainToRemove->nextChain != NULL){
		//set root chain to next chain
			rootChain = chainToRemove->nextChain;
		}
		else{
			rootChain = NULL;
		}
	}
	else{
		Chain* preChainToRemove;
		for(i = 0; i < chainIndex; i++){
			preChainToRemove = chainToRemove;
			chainToRemove = chainToRemove->nextChain;
		}
		preChainToRemove->nextChain = chainToRemove->nextChain;
	}
	if(chainToRemove == NULL){
		//this totally shouldn't happen but I'll keep it here for debug purposes
		printf("chainToRemove was null!\n");
		return -1;
	}
	
	Pa_StopStream(chainToRemove->stream);
	//this returns 1 when stream is still active
	while(Pa_IsStreamStopped(chainToRemove->stream) != 1){
		//do nothing; block until stream is finished
	}
	Pa_CloseStream(chainToRemove->stream);
	
	free(chainToRemove);
	return 0;
}

int newChainLink(int chainIndex, int effectType)
{
	//malloc the chainLink
	ChainLink* newChainLink = malloc(sizeof(ChainLink));
	//set new final node's next pointer to NULL
	newChainLink->nextLink = NULL;
	//init members
	
	switch(effectType){
		case IO:
			newChainLink->effectData = initIOEffect;
			newChainLink->effectFunction = &IOEffect;
			break;
		case FEEDBACKDELAY:
			newChainLink->effectData = initFeedbackDelayEffect();
			newChainLink->effectFunction = &feedbackDelayEffect;
			break;
		case SINGLETAPDELAY:
			newChainLink->effectData = initSingleTapDelayEffect();
			newChainLink->effectFunction = &singleTapDelayEffect;
			break;
		default: 
			return -1;
	}
	
	newChainLink->effectType = effectType;
	//newChainLink->effectData = initSingleTapDelayEffect();
	//newChainLink->effectFunction = &singleTapDelayEffect;
	//increment to chain index
	int i;
	Chain* iterator = rootChain;
	
	for(i = 0; i < chainIndex; i++){
		iterator = iterator->nextChain;
	}
	if(iterator == NULL){
		//this is unfortunate
		return -1;
	}
	//append new chainlink to end of chainLink list:
	ChainLink* linkIterator = iterator->chainLink;
	while(linkIterator->nextLink != NULL){
		linkIterator = linkIterator->nextLink;
	}
	//set final node's next node pointer to new node
	linkIterator->nextLink = newChainLink;
	
	return 0;
}

int removeChainLink(int chainIndex, int chainLinkIndex)
{
	Chain* chainIterator = rootChain;
	
	int i;
	for(i = 0; i < chainIndex; i++)
	{
		chainIterator = chainIterator->nextChain;
	}
	
	ChainLink* linkToRemove = chainIterator->chainLink;
	
	//if we're removing the root node: (should not happen because root node is reserved)
	/*
	if(chainLinkIndex == 0){
		chainIterator->chainLink = chainIterator->chainLink->nextLink;
		linkToRemove = chainIterator->chainLink;
	}
	*/
	//if it is not the root node:
	ChainLink* prevLink = linkToRemove;
	for(i = 0; i < chainLinkIndex; i++)
	{
		prevLink = linkToRemove;
		linkToRemove = linkToRemove->nextLink;
	}
	prevLink->nextLink = linkToRemove->nextLink;
	
	if(linkToRemove == NULL){
			//that's bad...
			return -1;
	}
	
	//
	switch(linkToRemove->effectType){
		case IO:
			freeIOEffect(linkToRemove);
			break;
		case FEEDBACKDELAY:
			freeFeedbackDelayEffect(linkToRemove);
			break;
		case SINGLETAPDELAY:
			freeSingleTapDelayEffect(linkToRemove);
			break;
		default:
			fprintf( stderr, "Could not determine the type of effect to remove.\n" );
	}
	//here's a potential pitfall... what if the memory is freed while the processing is ocurring?
	//This has not occurred yet in testing, but I'm still somewhat concerned.	
	free(linkToRemove);
	return 0;
}

int setParameter(int chainIndex, int linkIndex, int parameterIndex, int value)
{
	int i;
	Chain* iterator = rootChain;
	for(i = 0; i < chainIndex; i++){
		iterator = iterator->nextChain;
	}
	ChainLink* effectIterator = iterator->chainLink;
	linkIndex += 1; //the first link is reserved for input/output gain.
	for(i = 0; i < linkIndex; i++){
		effectIterator = effectIterator->nextLink;
	}
	switch(effectIterator->effectType){
		//using brackets in the switch block creates a scope and allows variables to be created
		//this is necessary because to get a usable pointer from the void ptr, effectData
		case IO:
		{
			IOData* ioData = (IOData*) effectIterator->effectData;
			if(parameterIndex == 0){
				ioData->inputGain = value;
			}
			else if(parameterIndex == 1){
				ioData->outputGain = value;
			}
		}
		case FEEDBACKDELAY:
		{ 
			FeedbackDelayData* delayData = (FeedbackDelayData*) effectIterator->effectData;
			if(parameterIndex == 0){
				delayData->delayTimeInMs = value;
			}
			else if(parameterIndex == 1){
				delayData->feedback = value;
			}
			break;
		}
		case SINGLETAPDELAY:
		{ 
			SingleTapDelayData* delayData = (SingleTapDelayData*) effectIterator->effectData;
			if(parameterIndex == 0){
				delayData->delayTimeInMs = value;
			}
			else if(parameterIndex == 1){
				delayData->gain = value;
			}
			break;
		}		
		default:
			return -1;
	}
	return 0;
}
