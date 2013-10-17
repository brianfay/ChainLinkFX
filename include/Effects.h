#include <portaudio.h>
#ifndef EFFECTS_H
#define EFFECTS_H

typedef float SAMPLE;

typedef enum EffectType
{
	EMPTY=0,
	FEEDBACKDELAY=1,
	SINGLETAPDELAY=2
} EffectType;

typedef struct FeedbackDelayData
{
	float* delayBuffer;
	int writeIndex;
	int readIndex;
	int bufferSize;
	int delayTimeInMs;
	int delayTimeInSamples;
	float feedback;
} FeedbackDelayData;

typedef struct SingleTapDelayData
{
	float* delayBuffer;
	int writeIndex;
	int readIndex;
	int bufferSize;
	int delayTimeInMs;
	int delayTimeInSamples;
	float gain;
} SingleTapDelayData;

//A ChainLink contains a function pointer to an effect function, and data for that effect to use
//Each Chain will refer to a list of ChainLinks - its serial list of effects
//The first ChainLink in the list will be passed to the audio callback, which will increment through each ChainLink in the list
typedef struct ChainLink
{
	//function pointer to audio effect function
	EffectType effectType;
	void(*effectFunction)();
	void * effectData;
	struct ChainLink* nextLink;
	//we want to pass this data to the callback
	//should include pointer to data (fx parameters - union type?)
} ChainLink;

//A Chain is an audio stream. It contains a pointer to a ChainLink, which
//contains effect data and is passed to the audio callback for that stream.
//Chain is implemented as a list; multiple chains allows for parallel audio processes
typedef struct Chain
{
	//this is the root chainLink in the list
	ChainLink* chainLink;
	struct Chain* nextChain;
	PaStreamParameters inputParameters, outputParameters;
	PaStream* stream;
} Chain;


//this effect does nothing
void* initEmptyEffect();
void emptyEffect(SAMPLE *in, SAMPLE *out, void *functionChain);
void freeEmptyEffect(ChainLink *);

//a delay effect
void* initFeedbackDelayEffect();
void feedbackDelayEffect(SAMPLE *in, SAMPLE *out, void *functionChain);
void freeFeedbackDelayEffect(ChainLink *);

void* initSingleTapDelayEffect();
void singleTapDelayEffect(SAMPLE *in, SAMPLE *out, void *functionChain);
void freeSingleTapDelayEffect(ChainLink *);
#endif /* EFFECTS_H */
