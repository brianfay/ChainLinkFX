#include "portaudio.h"
#ifndef EFFECTS_H
#define EFFECTS_H

typedef struct Chain
{
	//should include a function pointer to audio effect function
	//should include pointer to data (fx parameters - union type?)
	void(*effect)();
} Chain;


typedef struct ChainNode //kinda want to rename this to ChainLink but I'm already confused...
{
	Chain chain;
	PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
	struct ChainNode* next;
} ChainNode;

#endif /* EFFECTS_H */
