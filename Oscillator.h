#include <math.h>
#include "Enum.h"


enum OscillatorMode {
	OSCILLATOR_MODE_SINE,
	OSCILLATOR_MODE_SAW,
	OSCILLATOR_MODE_SQUARE,
	OSCILLATOR_MODE_TRIANGLE,
	kNumOscillatorMode
};


class Oscillator {
private:
	OscillatorMode mOscillatorMode;
	SyncedQuantization mQuantization;
	const double mPI;
	const double twoPI;
	double mFrequency;
	double mPhase;
	double mOffset;
	double mSampleRate;
	double mPhaseIncrement;
	double mTempo;
	bool isSynced;
	bool isMuted;
	void updateIncrement();
	double frequencyfromDotQuantization(double coef);
	double frequencyfromTierceQuantization(double coef);
	double frequencyfromQuantization(double coef);

public:
	void setMode(OscillatorMode mode);
	void setQuantization(SyncedQuantization quantization);
	SyncedQuantization getQuantization();
	void setFrequency(double frequency);
	double getFrequency();
	double getFrequencyFromQuantization();
	void setOffset(double offset);
	void setOffsetFromDeg(int offset);
	void setSampleRate(double sampleRate);
	void setTempo(double tempo);
	double getTempo();
	void setIsSynced(bool synced);
	bool getIsSynced();
	void setMuted(bool isMuted);
	void updateFrequency();
	void generate(double* buffer, int nFrames);
	double reachSample(int sampleCount);
	double nextSample();
	void resetPhase();

	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SINE),
		mQuantization(BAR_1),
		mPI(2 * acos(0.0)),
		twoPI(2 * mPI),
		mFrequency(440.0),
		mPhase(0.0),
		mOffset(0.0),
		mTempo(0),
		isSynced(false),
		isMuted(false),
		mSampleRate(44100.0) {
		updateIncrement();
	};
};

