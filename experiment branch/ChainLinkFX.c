#include <stdio.h>
#include <stdlib.h>
#include "Effects.h"
#define SAMPLE_RATE         (48000)
#define PA_SAMPLE_TYPE      paFloat32
#define FRAMES_PER_BUFFER   (2048)

typedef float SAMPLE;

void processAllElements(ChainNode* root, void* callback);
void newNode(ChainNode** lastNode);
int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData);
void doNothingEffect(SAMPLE **in, SAMPLE **out);

int init()
{
	ChainNode* root;
	int err = Pa_Initialize();
	if(err != paNoError) return -1;
	newNode(&root);
	printf("Hit ENTER to stop program.\n");
    getchar();
    err = Pa_CloseStream(root->stream);
    //if( err != paNoError ) goto error;
    //if( err != paNoError ) goto error;
	/*
	ChainNode* iterator = root;
	int i;
	for(i = 0; i < 20; i++){
		addNode(&iterator);
		iterator = iterator->next;
	}
	processAllElements(root, *print);
	while(root->next != NULL){
		removeLastNode(root);
	}	
	processAllElements(root, *print);
	*/
	Pa_Terminate();
	return 0;
}

void newNode(ChainNode** node)
{
	PaError err;
	
	PaHostApiIndex hostApiIndex = Pa_HostApiTypeIdToHostApiIndex(paJACK);
    if( err != paNoError ) goto error;
    
	(*node) = malloc(sizeof(ChainNode));
	(*node)->inputParameters.device = Pa_GetHostApiInfo(hostApiIndex)->defaultInputDevice;
	if((*node)->inputParameters.device == paNoDevice){
		fprintf(stderr,"Error: No default inpy device.\n");
		goto error;
	}
	(*node)->inputParameters.channelCount = 2;
	(*node)->inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	(*node)->inputParameters.suggestedLatency = Pa_GetDeviceInfo((*node)->inputParameters.device)->defaultLowInputLatency;
    (*node)->inputParameters.hostApiSpecificStreamInfo = NULL;
    (*node)->outputParameters.device = Pa_GetHostApiInfo(hostApiIndex)->defaultOutputDevice;
    printf("Input Device: %d\n", Pa_GetHostApiInfo(hostApiIndex)->defaultInputDevice);
    printf("Output Device: %d\n", Pa_GetHostApiInfo(hostApiIndex)->defaultOutputDevice);
    if ((*node)->outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		goto error;
    }
    (*node)->outputParameters.channelCount = 2;
    (*node)->outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    (*node)->outputParameters.suggestedLatency = Pa_GetDeviceInfo((*node)->outputParameters.device)->defaultLowOutputLatency;
    (*node)->outputParameters.hostApiSpecificStreamInfo = NULL;
    //registering an effect to the chain:
    (*node)->chain.effect = &doNothingEffect;
    err = Pa_OpenStream(
             &((*node)->stream),
              &((*node)->inputParameters),
              &((*node)->outputParameters),
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              audioCallback,
              &((*node)->chain));
    if( err != paNoError ) goto error;

    err = Pa_StartStream((*node)->stream);
    if( err != paNoError ) goto error;
    
    //Pa_Terminate();
    return;

error:
    //Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return;
}

void removeLastNode(ChainNode* root)
{
	ChainNode* iterator = root;
	ChainNode* tmp;
	do
	{
		tmp = iterator;
		iterator = iterator->next;
	} while(iterator->next != NULL);
	free(iterator);
	tmp->next = NULL;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int audioCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    Chain* functionChain = (Chain *) userData;

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
			//do once per channel
			(*functionChain).effect(&in,&out);
			(*functionChain).effect(&in,&out);
        }
    }
    
    return paContinue;
}

void doNothingEffect(SAMPLE **in, SAMPLE **out)
{
	        *((*out)++) = *((*in)++);  /* left  */
}
