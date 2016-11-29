#pragma once
//#include "DspFilters/Dsp.h"

#include"DspFilters/RBJ.h"
#include "DspFilters/SmoothedFilter.h"
class Equalizer
{
public:
	Equalizer(int nBand);
	void process(double** inputs, int nFrames);
	void setBandFrequency(int nBand, double frequency);
	void setBandGain(int nBand, double GainDB);
	void setBandWidth(int nBand, double width);

	inline void setSampleRate(double sampleRate){ mSampleRate = sampleRate; }

	~Equalizer();

private:

	Dsp::Filter** filters;

	int numBands;
	double mSampleRate;
	double mMix;
};

