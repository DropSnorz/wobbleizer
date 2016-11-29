#include "Equalizer.h"


Equalizer::Equalizer(int bands)
{
	numBands = bands;
	mSampleRate = 44100;
	filters = new Dsp::Filter*[numBands];
	
	for (int i = 0; i < numBands; i++){
		filters[i] = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandShelf, 2>(512);
		
		Dsp::Params params;

		params[0] = mSampleRate;
		params[1] = 1; // Frequency
		params[2] = 0; // Gain
		params[3] = 3; //Bandwidth

		filters[i]->setParams(params);
	}
	
}

void Equalizer::setBandFrequency(int nBand, double frequency){

	filters[nBand]->setParam(1, frequency);
}

void Equalizer::setBandWidth(int nBand, double width){

	filters[nBand]->setParam(3,  width);

}
void Equalizer::setBandGain(int nBand, double GainDB){

	filters[nBand]->setParam(2, GainDB);
}

void Equalizer::process(double** inputs, int nFrames){
	
		for (int b = 0; b < numBands; b++){

			filters[b]->process(nFrames, inputs);
		}
	
}


Equalizer::~Equalizer()
{

}
