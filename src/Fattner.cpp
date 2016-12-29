# include "Fattner.h"


Fattner::Fattner(){

	equalizer = new Equalizer(6);
	init();
}
void Fattner::init(){

	equalizer->setBandFrequency(0, 30);
	equalizer->setBandFrequency(1, 97);
	equalizer->setBandFrequency(2, 200);
	equalizer->setBandFrequency(3, 522);
	equalizer->setBandFrequency(4, 1870);
	equalizer->setBandFrequency(5, 3900);

	mGainBase[0] = 0;
	mGainBase[1] = 2.6;
	mGainBase[2] = 2.1;
	mGainBase[3] = -2.3;
	mGainBase[4] = 0;
	mGainBase[5] = 2.1;

	equalizer->setBandReso(0, 1);
	equalizer->setBandReso(1, 0.27);
	equalizer->setBandReso(2, 2.5);
	equalizer->setBandReso(3, 1.3);
	equalizer->setBandReso(4, 0.30);
	equalizer->setBandReso(5, 0.47);

}

void Fattner::setSampleRate(double sampleRate){

	equalizer->setSampleRate(sampleRate);
}
void Fattner::setRatio(double ratio){
	
	for (int i = 0; i < 6; i++){

		equalizer->setBandGain(i, ratio*mGainBase[i]);
		
	}
}

Fattner::~Fattner(){

}
