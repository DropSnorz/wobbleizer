# include "Fattner.h"


Fattner::Fattner(){

	equalizer = new Equalizer(6);
	init();
}
void Fattner::init(){

	equalizer->setBandFrequency(0, 30);
	equalizer->setBandFrequency(1, 130);
	equalizer->setBandFrequency(2, 700);
	equalizer->setBandFrequency(3, 1800);
	equalizer->setBandFrequency(4, 3600);
	equalizer->setBandFrequency(5, 5500);


	mGainBase[0] = 2.5;
	mGainBase[1] = 1.2;
	mGainBase[2] = 1;
	mGainBase[3] = -2.3;
	mGainBase[4] = -0.6;
	mGainBase[5] = 2.1;

	//double mQBase[6] = { 1, 0.27, 2.5, 1.3, 0.30, 0.47 };

	//setRatio(0);

}

void Fattner::setSampleRate(double sampleRate){

	equalizer->setSampleRate(sampleRate);
}
void Fattner::setRatio(double ratio){

	
	for (int i = 0; i < 6; i++){

		equalizer->setBandGain(i, ratio*mGainBase[i]);
		
	}
}


void Fattner::process(double** inputs, int nFrames){

	equalizer->process(inputs, nFrames);

}

Fattner::~Fattner(){

}
