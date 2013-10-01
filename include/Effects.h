#include "portaudio.h"
#ifndef EFFECTS_H
#define EFFECTS_H

typedef union EffectData
{
	
} EffectData;

typedef struct ChainLink
{
	//function pointer to audio effect function
	void(*effectFunction)();
	EffectData effectData;
	struct ChainLink* nextLink;
	//should include pointer to data (fx parameters - union type?)
} ChainLink;


typedef struct Chain //kinda want to rename this to ChainLink but I'm already confused...
{
	ChainLink chainLink;
	struct Chain* nextChain;
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream;
} Chain;



#endif /* EFFECTS_H */
