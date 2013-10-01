#include "Effects.h"
#include "portaudio.h"
#include <stdio.h>
#include <stdlib.h>
#define PA_SAMPLE_TYPE paFloat32

typedef float SAMPLE;

int newChain(PaDeviceIndex inputDeviceIndex, PaDeviceIndex outputDeviceIndex);
int audioCallback(const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData);
				
void emptyEffect(SAMPLE **in, SAMPLE **out);				

Chain* rootChain = NULL;
int newChain(PaDeviceIndex inputDeviceIndex, PaDeviceIndex outputDeviceIndex)
{
	Chain* chain;
	PaError err;
	if(rootChain == NULL){
		chain = rootChain;
	}
	else{
		chain = rootChain->nextChain;
		while(chain->nextChain != NULL){
			chain = chain->nextChain;
		}
		//set to first null chain in the list
		chain = chain->nextChain;
	}
	chain = malloc(sizeof(Chain));
	chain->inputParameters.device = inputDeviceIndex;
	if(chain->inputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Input device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(inputDeviceIndex);
	PaDeviceInfo* outputDeviceInfo = Pa_GetDeviceInfo(outputDeviceIndex);
	chain->inputParameters.channelCount = inputDeviceInfo->maxInputChannels;
	chain->inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	chain->inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;
	chain->inputParameters.hostApiSpecificStreamInfo = NULL;
	
	chain->outputParameters.device = outputDeviceIndex;
	if(chain->outputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Output device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	chain->outputParameters.channelCount = outputDeviceInfo->maxOutputChannels;
	chain->outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	chain->outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency;
	chain->outputParameters.hostApiSpecificStreamInfo = NULL;
	//registering an effect:
	chain->chainLink.effectFunction = &emptyEffect;
	err = Pa_OpenStream(
			&(chain->stream),
			&(chain->inputParameters),
			&(chain->outputParameters),
			inputDeviceInfo->defaultSampleRate,	
			paFramesPerBufferUnspecified,
			paClipOff,
			audioCallback,
			&(chain->chainLink));
	if(err != paNoError) goto error;
	err = Pa_StartStream(chain->stream);
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
	if(inputBuffer == NULL)
	{
		for(i=0; i<framesPerBuffer; i++)
		{
			//should alter to get channels right
			*out++ = 0;
			*out++ = 0;
		}
	}
	else
	{
		for(i = 0; i<framesPerBuffer; i++)
		{
			(*functionChain).effectFunction(&in,&out);
			(*functionChain).effectFunction(&in,&out);
		}
	}
	return paContinue;
}

void emptyEffect(SAMPLE **in, SAMPLE **out)
{
	*((*out)++) = *((*in)++);
}

int removeChain(int chainIndex)
{
	//should have error-checking
	
	return 0;
}
