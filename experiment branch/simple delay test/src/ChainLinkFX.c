#include "Effects.h"
#include "portaudio.h"
#include <stdio.h>
#include <stdlib.h>
#define PA_SAMPLE_TYPE paFloat32

int newChain(PaDeviceIndex inputDeviceIndex, PaDeviceIndex outputDeviceIndex);
int removeChain(int chainIndex);
int newChainLink(int chainIndex);
int removeChainLink(int chainIndex, int chainLinkIndex);
int audioCallback(const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData);
int setParameter(int chainIndex,int effectIndex,int parameterIndex,int value);				
void delayEffect(SAMPLE *in, SAMPLE *out, void* userData);				

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
	PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(inputDeviceIndex);
	PaDeviceInfo* outputDeviceInfo = Pa_GetDeviceInfo(outputDeviceIndex);
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
	
	//registering first (empty) ChainLink:
	newChain->chainLink.numInputChannels = newChain->inputParameters.channelCount;
	newChain->chainLink.numOutputChannels = newChain->outputParameters.channelCount;
	newChain->chainLink.effectType = DELAY;
	newChain->chainLink.effectFunction = &delayEffect;
	//messy, but I'm gonna malloc 5 seconds for the delay buffer here
	int bytesToAllocate = sizeof(float)*inputDeviceInfo->defaultSampleRate*newChain->inputParameters.channelCount*5;
	newChain->chainLink.delayData.delayBuffer = malloc(bytesToAllocate);
	newChain->chainLink.delayData.writeIndex = 0;
	newChain->chainLink.delayData.writeIndex = 0;
	newChain->chainLink.delayData.bufferSize = inputDeviceInfo->defaultSampleRate*5;
	newChain->chainLink.delayData.delayTimeInMs = 1000;
	newChain->chainLink.delayData.feedback = 50;
	//...and initialize the buffer to zeros... uggh this is gross...
	float* theBuffer = newChain->chainLink.delayData.delayBuffer;
	int i;
	for(i = 0; i < inputDeviceInfo->defaultSampleRate * 5; i++){
		theBuffer[i] = 0.0;
	}
	err = Pa_OpenStream(
			&(newChain->stream),
			&(newChain->inputParameters),
			&(newChain->outputParameters),
			inputDeviceInfo->defaultSampleRate,	
			paFramesPerBufferUnspecified,
			paClipOff,
			audioCallback,
			&(newChain->chainLink));
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
	ChainLink* functionChain = (ChainLink*) userData;
	int currentChannel;
	if(inputBuffer == NULL)
	{
		for(i=0; i<framesPerBuffer; i++)
		{
			for(currentChannel = 0; currentChannel < functionChain->numOutputChannels; currentChannel++)
			{
			*out++ = 0;
			}
		}
	}
	else
	{
		for(i = 0; i<framesPerBuffer; i++)
		{
			for(currentChannel = 0; currentChannel < functionChain->numOutputChannels; currentChannel++)
			{
			(*functionChain).effectFunction(in,out,userData);
			*out++;
			*in++;
			}
		}
	}
	return paContinue;
}

int removeChain(int chainIndex)
{
	if(rootChain == NULL){
		//this totally shouldn't happen but I'll keep it here for debug purposes
		printf("rootChain is null!?\n");
		return -1;
	}
	Chain* chainToRemove = rootChain;
	int i;
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
	free(chainToRemove);
	return 0;
}

int newChainLink(int chainIndex)
{
	//is memory already allocated? How does that work with unions (just allocate for biggest?)
	//set new final node's next pointer to NULL
	//set final node's next node pointer to new node
}
int removeChainLink(int chainIndex, int chainLinkIndex)
{
	//if root chain: 
	//if next chain exists:
	//set root chain to next chain	
}

void delayEffect(SAMPLE *in, SAMPLE *out, void *functionChain)
{
	ChainLink* userData = (ChainLink*) functionChain;
	*out = *in;
	float actualDelayInSamples= (float)userData->delayData.delayTimeInMs * 44800.0 * 0.001;
	userData->delayData.delayTimeInSamples = (int)actualDelayInSamples;
	userData->delayData.readIndex = userData->delayData.writeIndex;
	userData->delayData.readIndex -= userData->delayData.delayTimeInSamples;
	while(userData->delayData.readIndex < 0){
		userData->delayData.readIndex += userData->delayData.bufferSize;
	}
	while(userData->delayData.readIndex > userData->delayData.bufferSize){
		userData->delayData.readIndex -= userData->delayData.bufferSize;
	}
	userData->delayData.writeIndex++;
	userData->delayData.writeIndex %= userData->delayData.bufferSize;
	//now set output to current value + delayed value
	*out += (userData->delayData.delayBuffer[userData->delayData.readIndex] * (userData->delayData.feedback*0.01));
	//delayBuffer gets set here rather than earlier so that it can feed back
	userData->delayData.delayBuffer[userData->delayData.writeIndex] = *out;
}

int setParameter(int chainIndex,int effectIndex,int parameterIndex,int value)
{
	int i;
	Chain* iterator = rootChain;
	for(i = 0; i < chainIndex; i++){
		iterator = iterator->nextChain;
	}
	ChainLink* effectIterator = &(iterator->chainLink);
	for(i = 0; i < effectIndex; i++){
		effectIterator = effectIterator->nextLink;
	}
	switch(effectIterator->effectType){
		case DELAY:
			if(parameterIndex == 0){
				effectIterator->delayData.delayTimeInMs = value;
			}
			else if(parameterIndex == 1){
				effectIterator->delayData.feedback = value;
			}
			break;
		default:
			return -1;
	}
	return 0;
}
