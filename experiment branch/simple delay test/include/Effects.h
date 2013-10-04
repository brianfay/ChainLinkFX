#include "portaudio.h"
#ifndef EFFECTS_H
#define EFFECTS_H

typedef float SAMPLE;

typedef enum EffectType
{
	EMPTY,
	DELAY,
} EffectType;

typedef struct DelayData
{
	float* delayBuffer;
	int writeIndex;
	int readIndex;
	int bufferSize;
	int delayTimeInMs;
	int delayTimeInSamples;
	float feedback;
} DelayData;

//A ChainLink contains a function pointer to an effect function, and data for that effect to use
//Each Chain will refer to a list of ChainLinks - its serial list of effects
//The first ChainLink in the list will be passed to the audio callback, which will increment through each ChainLink in the list
typedef struct ChainLink
{
	//function pointer to audio effect function
	EffectType effectType;
	void(*effectFunction)();
	DelayData delayData;
	struct ChainLink* nextLink;
	//we want to pass this data to the callback
	int numInputChannels, numOutputChannels;
	//should include pointer to data (fx parameters - union type?)
} ChainLink;

//A Chain is an audio stream. It contains a pointer to a ChainLink, which
//contains effect data and is passed to the audio callback for that stream.
//Chain is implemented as a list; multiple chains allows for parallel audio processes
typedef struct Chain
{
	ChainLink chainLink;
	struct Chain* nextChain;
	PaStreamParameters inputParameters, outputParameters;
	PaStream* stream;
} Chain;



#endif /* EFFECTS_H */
