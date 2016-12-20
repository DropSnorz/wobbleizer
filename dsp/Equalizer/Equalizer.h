#pragma once
#include "Biquad.h"
class Equalizer
{
public:
	Equalizer(int nBand);
	void process(double** inputs, int nFrames);
	void setBandFrequency(int nBand, double frequency);
	void setBandGain(int nBand, double GainDB);
	void setBandReso(int nBand, double Q);

	inline void setSampleRate(double sampleRate){ mSampleRate = sampleRate; }

	~Equalizer();

private:

	Biquad** FilterR;
	Biquad** FilterL;

	int numBands;
	double mSampleRate;
	double mMix;
};

