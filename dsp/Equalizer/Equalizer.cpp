#include "Equalizer.h"


Equalizer::Equalizer(int bands)
{
	numBands = bands;
	mSampleRate = 44100;
	FilterL = new Biquad*[6];
	FilterR = new Biquad*[6];

	for (int i = 0; i < numBands; i++){
		FilterL[i] = new Biquad();
		FilterR[i] = new Biquad();

		FilterL[i]->setBiquad(bq_type_peak, 0, 0, 0);
		FilterR[i]->setBiquad(bq_type_peak, 0, 0, 0);
	}
}

void Equalizer::setBandFrequency(int nBand, double frequency){

	FilterL[nBand]->setFc(frequency/mSampleRate);
	FilterR[nBand]->setFc(frequency/mSampleRate);
}

void Equalizer::setBandReso(int nBand, double reso){

	FilterL[nBand]->setQ(reso);
	FilterR[nBand]->setQ(reso);
}
void Equalizer::setBandGain(int nBand, double GainDB){

	FilterL[nBand]->setPeakGain(GainDB);
	FilterR[nBand]->setPeakGain(GainDB);
}

Equalizer::~Equalizer()
{

}

void Equalizer::process(double** inputs, int nFrames){
	
	for (int s = 0; s < nFrames; s++){

		for (int b = 0; b < numBands; b++){

			inputs[0][s] = FilterL[b]->process(inputs[0][s]);
			inputs[1][s] = FilterR[b]->process(inputs[1][s]);
		}
	}
}
