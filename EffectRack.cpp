#include "EffectRack.h"



EffectRack::EffectRack() :mEffectValue(0.0)
{


}

void EffectRack::setEffectValue(double value){
	mEffectValue = value;
}

double EffectRack::getEffectValue(){
	return mEffectValue;
}

double EffectRack::Fatter(double input){

	double value = input;
	if (mEffectValue <= 0.001){
		return value;
	}
	else{
		value = Saturator(value);
		//value = BassBooster(value);

		return value;
	}
}

double EffectRack::Saturator(double input){
	double saturatorCoef = mEffectValue;

	if (input < saturatorCoef){
		return input;
	}
	else if (input >= saturatorCoef){
		return saturatorCoef + ((input - saturatorCoef) / (1.0 + pow((input - saturatorCoef) / (1.0 - saturatorCoef), 2)));
	}
	else{
		return (saturatorCoef + 1) / 2.0;
	}
}

double EffectRack::BassBooster(double input){

	double selectivity, gain1, gain2,cap,ratio;
	cap = 1;
	ratio = 0.5*mEffectValue;
	selectivity = 80.0;
	gain1 = 1.0 / (selectivity + 1.0);
	gain2 = mEffectValue;

	cap = (input + cap*selectivity)*gain1;
	input = quickSaturate((input + cap*ratio)*gain2);

	return input;

}

double EffectRack::quickSaturate(double input){
	return fmin(fmax(-1.0, input), 1.0);

}
EffectRack::~EffectRack()
{
}
