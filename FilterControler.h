#include <math.h>
#include "DspFilters/SmoothedFilter.h"
#include "DspFilters/RBJ.h"
enum FilterMode {
	FILTER_MODE_LOWPASS = 0,
	FILTER_MODE_HIGHPASS,
	FILTER_MODE_BANDPASS,
	kNumFilterModes
};

class FilterControler {
public:

	FilterControler() :
		cutoff(0.999),
		resonance(0.0),
		cutoffMod(0.0),
		sampleRate(44100),
		mode(FILTER_MODE_LOWPASS),
		buf0(0.0),
		buf1(0.0),
		buf2(0.0),
		buf3(0.0)
	{
		initBiquadFilter();
	};
	double** process(double** inputs, int nframes);
	double process2(double inputValue);
	void initBiquadFilter();
	void updateBiquadParameters();
	void setSampleRate(double newSampleRate);
	void setCutoff(double newCutoff);
	void setCutoffFromFrequency(double frequency);
	void setResonance(double newResonance);
	inline void setFilterMode(FilterMode newMode) { mode = newMode; };
	inline double getCutoffMod(){ return cutoffMod; };

	void setCutoffMod(double newCutoffMod);

	double getCalculatedCutoff() {
		double relatedCutoff = cutoff;
		double calculatedCutoff;
		calculatedCutoff = fmax(fmin(relatedCutoff + cutoffMod, 0.99999), 0.00001);
		calculatedCutoff = calculatedCutoff;
		return calculatedCutoff;
		//return 0.0001 + pow(calculatedCutoff, 4.1634) * (0.9999 - 0.0001);
		//return fmin(0.5*pow(calculatedCutoff, 20000 + 1)*sampleRate + 20, 0.5*sampleRate);
	};

	 double getCalculatedCutoffFrequency() {

		double calculatedCutoff = getCalculatedCutoff();
		double curve = pow(calculatedCutoff, 4.1634);

		return 20 + (curve * (20000 - 20));
		//return 19 + floor(pow(4, calculatedCutoff / 14.15005)) + calculatedCutoff * 20;
	};

	 double getCutoffFrequency(){

		 double curve = pow(cutoff, 4.1634);
		 return 20 + (curve * (20000 - 20));

	 }
private:
	double cutoff;
	double resonance;
	FilterMode mode;
	double feedbackAmount;
	double cutoffMod;
	double buf0;
	double buf1;
	double buf2;
	double buf3;
	double sampleRate;

	Dsp::Filter* lowPassFilter;
	Dsp::Filter* lowPassFilterSO;
	Dsp::Filter* highPassFilter;
	Dsp::Filter* bandPassFilter;


};