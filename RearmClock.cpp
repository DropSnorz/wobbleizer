#include "RearmClock.h"
#include <math.h>


RearmClock::RearmClock() : nextTime(0.0), previousTime(0.0), mBeatIncrement(1),mEnabled(false)
{
}

bool RearmClock::getNeedRearm(double time){


	if (mEnabled == false){
		return false;
	}
	

	/*if (time > nextTime){
		nextTime = time + mBeatIncrement;
		return true;
	}
	*/

	double result = fmod(time, mBeatIncrement);
	if (result < 0.05 && !(previousTime<0.05))
		{
			previousTime = result;
		return true;
	}
	else{
		previousTime = result;
		return false;
		 
	}	
}

void RearmClock::setTempo(double tempo){

	if (tempo != mTempo){
		mTempo = tempo;
		updateRearmRate();
	}
}

void RearmClock::setQuantization(RearmQuantization quantization){
	mQuantization = quantization;
	updateRearmRate();
}

void RearmClock::Reset(){

	nextTime = 0;
	updateRearmRate();
}

void RearmClock::updateRearmRate(){

	double coef = 1.0;
	mEnabled = true;

	switch (mQuantization){
		case R_NONE:
			coef = 1;
			mEnabled = false;
			break;
		case R_BAR_8:
			coef = 32;
			break;
		case R_BAR_8T:
			//TODO Impl Coef;
			coef = 32 * (2. / 3.);
			break;
		case R_BAR_4:
			coef = 16;
			break;
		case R_BAR_4T:
			coef = 16 * (2. / 3.);
			break;
		case R_BAR_2:
			coef = 8;
			break;
		case R_BAR_2T:
			coef = 8 * (2. / 3.);
			break;
		case R_BAR_1:
			coef = 4;
			break;
		case R_BAR_1T:
			coef = 4 * (2. / 3.);
			break;
		case R_BAR_1_2:
			coef = 2;
			break;
		case R_BAR_1_2T:
			coef = 2*(2. / 3.);
			break;
		case R_BAR_1_4:
			coef = 1;
			break;
		case R_BAR_1_4T:
			coef = (2. / 3.);
			break;
		case R_BAR_1_8:
			coef = 0.5;
			break;
		case R_BAR_1_8T:
			coef = 0.5 * (2. / 3.);
			break;
		case R_BAR_1_16:
			coef = 0.25;
			break;
		case R_BAR_1_16T:
			coef = 0.25 * (2. / 3.);
			break;
		case R_BAR_1_32:
			coef = (1.0/32.0) * 4;
			break;
		case R_BAR_1_32T:
			coef = (1.0 / 32.0) * 4 * (2. / 3.);
			break;
		case R_BAR_1_64:
			coef = (1.0/64.0) * 4;
	}

	mBeatIncrement = coef;

}


RearmClock::~RearmClock()
{
}
