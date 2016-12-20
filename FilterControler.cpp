#include "FilterControler.h"

double FilterControler::process2(double inputValue) {

	if (inputValue == 0.0) return inputValue;
	double calculatedCutoff = getCalculatedCutoff();
	buf0 += calculatedCutoff * (inputValue - buf0 + feedbackAmount * (buf0 - buf1));
	buf1 += calculatedCutoff * (buf0 - buf1);
	buf2 += calculatedCutoff * (buf1 - buf2);
	buf3 += calculatedCutoff * (buf2 - buf3);

	switch (mode) {
	case FILTER_MODE_LOWPASS:
		return buf3;
	case FILTER_MODE_HIGHPASS:
		return inputValue - buf3;
	case FILTER_MODE_BANDPASS:
		return buf0 - buf3;
	default:
		return 0.0;
	}
}
double** FilterControler::process(double** inputs, int nframes){

	double currentCutoff = getCalculatedCutoffFrequency();
	lowPassFilter->setParam(1, currentCutoff);
	lowPassFilterSO->setParam(1, currentCutoff);
	highPassFilter->setParam(1, currentCutoff);
	bandPassFilter->setParam(1, currentCutoff);
	
	switch (mode){
	case FILTER_MODE_LOWPASS:
		lowPassFilter->process(nframes,inputs);
		//lowPassFilterSO->process(nframes, inputs);

		//lowPassBiquadFilter->process(inputs, nframes);

		break;
	case FILTER_MODE_HIGHPASS:
		highPassFilter->process(nframes, inputs);
		break;
	case FILTER_MODE_BANDPASS:
		bandPassFilter->process(nframes, inputs);
		break;
	default:
		break;
	}

	return inputs;

}

void FilterControler::setCutoff(double newCutoff){

	cutoff = newCutoff;
}

void FilterControler::setCutoffMod(double newCutoffMod){

	cutoffMod = newCutoffMod;

}

void FilterControler::setResonance(double newResonance){

	resonance = newResonance;

	lowPassFilter->setParam(2, resonance);
	lowPassFilterSO->setParam(2, resonance);
	highPassFilter->setParam(2, resonance);
	bandPassFilter->setParam(2, resonance);

}

void FilterControler::setSampleRate(double newSampleRate){
	sampleRate = newSampleRate;

	lowPassFilter->setParam(0, sampleRate);
	lowPassFilterSO->setParam(0, sampleRate);
	highPassFilter->setParam(0, sampleRate);
	bandPassFilter->setParam(0, sampleRate);

}

void FilterControler::setCutoffFromFrequency(double frequency){
	
	cutoff = ((frequency - 20) / (20000 - 20));
}

void FilterControler::initBiquadFilter(){


	/*lowPassFilter = new Dsp::SmoothedFilterDesign <Dsp::ChebyshevI::Design::LowPass<2>, 2 >(1024);
	highPassFilter = new Dsp::SmoothedFilterDesign <Dsp::ChebyshevI::Design::HighPass<2>, 2>(1024);
	bandPassFilter = new Dsp::SmoothedFilterDesign <Dsp::ChebyshevI::Design::BandPass<2>, 2>(1024);
	*/

	
	lowPassFilterSO = new Dsp::SmoothedFilterDesign < Dsp::RBJ::Design::LowPass, 2>(1024);
	highPassFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::HighPass, 2>(1024);
	bandPassFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandPass2, 2>(1024);
	lowPassFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2>(1024);
	
	Dsp::Params params;

	params[0] = sampleRate;
	params[1] = getCalculatedCutoffFrequency();
	params[2] = resonance;

	lowPassFilter->setParams(params);
	lowPassFilterSO->setParams(params);
	highPassFilter->setParams(params);
	bandPassFilter->setParams(params);


}
