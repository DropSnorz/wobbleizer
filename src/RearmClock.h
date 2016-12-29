#pragma once
#include "Enum.h"

class RearmClock
{
public:
	RearmClock();
	bool getNeedRearm(double time);
	void setTempo(double tempo);
	void setQuantization(RearmQuantization quantization);
	void Reset();
	~RearmClock();

private:
	double mTempo;
	bool mEnabled;
	RearmQuantization mQuantization;
	double mBeatIncrement;
	double nextTime;
	double previousTime;

	void updateRearmRate();

};

