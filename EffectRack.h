#pragma once
#include <math.h>

class EffectRack
{
public:
	EffectRack();
	double Fatter(double input);
	double Saturator(double input);
	double BassBooster(double input);
	double getEffectValue();
	void setEffectValue(double value);
	~EffectRack();
private:

	double mEffectValue;
	
	double quickSaturate(double input);

};

