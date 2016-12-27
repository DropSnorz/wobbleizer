#pragma once
#include "dsp/Equalizer/Equalizer.h"
class Fattner
{
public:
	Fattner();
	~Fattner();

	void init();
	void setSampleRate(double sampleRate);
	void setRatio(double ratio);
	inline void process(double** inputs, int nFrames){
		equalizer->process(inputs, nFrames);
	};

private:

	Equalizer* equalizer;
	double mGainBase[6];

};

