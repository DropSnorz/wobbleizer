#include "Oscillator.h"


void Oscillator::setMode(OscillatorMode mode) {
	mOscillatorMode = mode;
}
void Oscillator::setQuantization(SyncedQuantization quantization){
	mQuantization = quantization;
	updateFrequency();
}
SyncedQuantization Oscillator::getQuantization(){

	return mQuantization;
}

void Oscillator::setFrequency(double frequency) {
	mFrequency = frequency;
	updateIncrement();
}

double Oscillator::getFrequency(){
	return mFrequency;
}

double Oscillator::getFrequencyFromQuantization(){

	double frequency = 0;
	SyncedQuantization quantization = mQuantization;
	
	switch (quantization){
	case BAR_8:
		frequency = frequencyfromQuantization(1.0 / 8.0);
		break;
	case BAR_8T:
		frequency = frequencyfromTierceQuantization(1.0 / 8.0);
		break;
	case BAR_8D:
		frequency = frequencyfromDotQuantization((1.0 / 8.0));
		break;
	case BAR_4: 
		frequency = frequencyfromQuantization(1. / 4.);
		break;
	case BAR_4T:
		frequency = frequencyfromTierceQuantization(1. / 4.);
		break;
	case BAR_4D:
		frequency = frequencyfromDotQuantization(1. / 4.);
		break;
	case BAR_2:
		frequency = frequencyfromQuantization(1. / 2.);
		break;
	case BAR_2T:
		frequency = frequencyfromTierceQuantization(1. / 2.);
		break;
	case BAR_2D:
		frequency = frequencyfromDotQuantization(1. / 2.);
		break;
	case BAR_1:
		frequency = frequencyfromQuantization(1);
		break;
	case BAR_1T:
		frequency = frequencyfromTierceQuantization(1);
		break;
	case BAR_1D:
		frequency = frequencyfromDotQuantization(1);
		break;
	case BAR_1_2:
		frequency = frequencyfromQuantization(2);
		break;
	case BAR_1_2T:
		frequency = frequencyfromTierceQuantization(2);
		break;
	case BAR_1_2D:
		frequency = frequencyfromDotQuantization(2);
		break;
	case BAR_1_4:
		frequency = frequencyfromQuantization(4);
		break;
	case BAR_1_4T:
		frequency = frequencyfromTierceQuantization(4);
		break;
	case BAR_1_4D:
		frequency = frequencyfromDotQuantization(4);
		break;
	case BAR_1_8:
		frequency = frequencyfromQuantization(8);
		break;
	case BAR_1_8T:
		frequency = frequencyfromTierceQuantization(8);
		break;
	case BAR_1_8D:
		frequency = frequencyfromDotQuantization(8);
		break;
	case BAR_1_16:
		frequency = frequencyfromQuantization(16);
		break;
	case BAR_1_16T:
		frequency = frequencyfromTierceQuantization(16);
		break;
	case BAR_1_16D:
		frequency = frequencyfromDotQuantization(16);
		break;
	case BAR_1_32:
		frequency = frequencyfromQuantization(32);
		break;
	case BAR_1_32T:
		frequency = frequencyfromTierceQuantization(32);
		break;
	case BAR_1_32D:
		frequency = frequencyfromDotQuantization(32);
		break;
	case BAR_1_64:
		frequency = frequencyfromQuantization(64);
		break;
	case BAR_1_64D:
		frequency = frequencyfromDotQuantization(64);
		break;
	default:

		frequency = 1;
		break;

	}
	return frequency;
}

void Oscillator::setSampleRate(double sampleRate) {
	mSampleRate = sampleRate;
	updateIncrement();
}

void Oscillator::setTempo(double tempo){
	mTempo = tempo;
	updateFrequency();
}

double Oscillator::getTempo(){
	return mTempo;
}
void Oscillator::setIsSynced(bool synced){
	isSynced = synced;
}

bool Oscillator::getIsSynced(){
	return isSynced;
}

void Oscillator::setMuted(bool muted){
	isMuted = muted;
}

void Oscillator::setOffset(double offset){
	mOffset = offset;

}

void Oscillator::setOffsetFromDeg(int offset){
	setOffset(offset*mPI / 180.0);
}

void Oscillator::updateIncrement() {
	mPhaseIncrement = mFrequency * 2 * mPI / mSampleRate;
}

void Oscillator::updateFrequency(){

	if (isSynced){
		setFrequency(getFrequencyFromQuantization());
	}

}

void Oscillator::generate(double* buffer, int nFrames) {
	const double twoPI = 2 * mPI;
	switch (mOscillatorMode) {
	case OSCILLATOR_MODE_SINE:
		for (int i = 0; i < nFrames; i++) {
			buffer[i] = sin(mPhase);
			mPhase += mPhaseIncrement;
			while (mPhase >= twoPI) {
				mPhase -= twoPI;
			}
		}
		break;
	case OSCILLATOR_MODE_SAW:
		for (int i = 0; i < nFrames; i++) {
			buffer[i] = 1.0 - (2.0 * mPhase / twoPI);
			mPhase += mPhaseIncrement;
			while (mPhase >= twoPI) {
				mPhase -= twoPI;
			}
		}
		break;

	case OSCILLATOR_MODE_SQUARE:
		for (int i = 0; i < nFrames; i++) {
			if (mPhase <= mPI) {
				buffer[i] = 1.0;
			}
			else {
				buffer[i] = -1.0;
			}
			mPhase += mPhaseIncrement;
			while (mPhase >= twoPI) {
				mPhase -= twoPI;
			}
		}
		break;
	case OSCILLATOR_MODE_TRIANGLE:
		for (int i = 0; i < nFrames; i++) {
			double value = -1.0 + (2.0 * mPhase / twoPI);
			buffer[i] = 2.0 * (fabs(value) - 0.5);
			mPhase += mPhaseIncrement;
			while (mPhase >= twoPI) {
				mPhase -= twoPI;
			}
		}
		break;

	}
}

double Oscillator::reachSample(int sampleCount){

	for (int i = 0; i < sampleCount - 1; i++){
		nextSample();
	}
	return nextSample();

}


double Oscillator::nextSample() {

		double value = 0.0;

		if (isMuted) return value;
		double phase = mPhase + mOffset;

		switch (mOscillatorMode) {
		case OSCILLATOR_MODE_SINE:

			value = sin(phase);
			break;
		case OSCILLATOR_MODE_SAW:

			value = 1.0 - (2.0 * mPhase / twoPI);
			break;
		case OSCILLATOR_MODE_SQUARE:
			if (phase <= mPI) {
				value = 1.0;
			}
			else {
				value = - 1.0;
			}
			break;
		case OSCILLATOR_MODE_TRIANGLE:

			value = -1.0 + (2.0 * mPhase / twoPI);
			value = 2.0 * (fabs(value) - 0.5);

			break;
		}
		mPhase += mPhaseIncrement;
		while (mPhase + mOffset >= twoPI) {
			mPhase -= twoPI;
		}
		return value;
	}
double Oscillator::frequencyfromQuantization(double coef){
	return ((mTempo / 60.0) / 4.0) * coef;
}

double Oscillator::frequencyfromTierceQuantization(double coef){
	return (((mTempo / 60.0) / 4.0) * coef) * (3.0 / 2.0);
}
double Oscillator::frequencyfromDotQuantization(double coef){
	return  ((mTempo / 60.0 / 4.0) * coef) * (2.0 / 3.0);

}

void Oscillator::resetPhase(){
	mPhase = 0.0;
}