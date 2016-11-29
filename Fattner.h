#pragma once
#include "Equalizer.h"
class Fattner
{
public:
	Fattner();
	~Fattner();

	void init();
	void setSampleRate(double sampleRate);
	void setRatio(double ratio);
	void process(double** inputs, int nFrames);

private:

	Equalizer* equalizer;
	double mGainBase[6];
	double mQBase[6];


};

