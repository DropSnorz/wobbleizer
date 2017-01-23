#include "Wobbleizer.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

#include <thread>

const int kNumPrograms = 1;

enum EParams
{
	kFilterMode = 0,
	kFilterCutoff,
	kGraphicLFOFeedback,
	kFilterResonance,

	kLFOWave,
	kLFOAmount,
	kLFOOffset,
	kLFOIsSynced,
	kLFOFrequency,
	kLFOSyncedFrequency,
	kRearmStep,
	kRearmTap,
	kRearmOnFrequencyUpdate,

	kBoosterRate,
	
  kNumParams
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kFilterModeX = 72,
	kFilterModeY = 95,
	kFilterCutoffX = 155,
	kFilterCutoffY = 89,
	kFilterResonanceX = 255,
	kFilterResonanceY = 90,

	kLFOWaveX = 69,
	kLFOWaveY = 210,
	kLFOAmountX = 145,
	kLFOAmountY = 200,
	kLFOOffsetX = 220,
	kLFOOffsetY = 200,
	kLFOIsSyncedX = 290,
	kLFOIsSyncedY = 193,
	kLFOFrequencyX = 410,
	kLFOFrequencyY = 200,
	kLFOSyncedFrequencyX = 410,
	kLFOSyncedFrequencyY = 200,

	kRearmStepX = 70,
	kRearmStepY = 300,
	kRearmTapX = 228,
	kRearmTapY = 293,
	kRearmOnFrequencyUpdateX = 384,
	kRearmOnFrequencyUpdateY = 300,

	kBoosterRateX = 508,
	kBoosterRateY = 245,

	kGraphicLFOFeedbackX = 147,
	kGraphicLFOFeedbackY = 82,
    kKnobFrames = 101
};

Wobbleizer::Wobbleizer(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mLFOFilterModAmount(0.0), mRearmOnFrequencyUpdate(false)
{
  TRACE;

  mFilter = FilterControler();
  mRearmClock = RearmClock();
  mFattner = Fattner();
  mEffectRack = EffectRack();

  SetLatency(PLUG_LATENCY);


  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kFilterMode)->InitEnum("FilterMode", FilterMode::FILTER_MODE_LOWPASS, FilterMode::kNumFilterModes);
  GetParam(kFilterCutoff)->InitDouble("FilterCutoff", 0.999, 0.001, 0.999, 0.001,"HZ");
  //GetParam(kFilterCutoff)->SetShape(2);
  GetParam(kFilterResonance)->InitDouble("FilterResonance", 0.7, 0.4, 3, 0.001);
  GetParam(kFilterResonance)->SetShape(3.1155);
  GetParam(kLFOWave)->InitEnum("LFOWave", OscillatorMode::OSCILLATOR_MODE_SINE, OscillatorMode::kNumOscillatorMode);
  GetParam(kLFOAmount)->InitDouble("LFOAmount",0.0, 0.0, 1.0, 0.001);
  GetParam(kLFOOffset)->InitInt("LFOOffset", 0, -360, 360);
  GetParam(kLFOIsSynced)->InitBool("LFOIsSynced", false);
  GetParam(kLFOFrequency)->InitDouble("LFOFrequency", 5.0, 0.1, 20.0, 0.01, "Hz");
  GetParam(kLFOSyncedFrequency)->InitEnum("LFOSyncedFrequency", SyncedQuantization::BAR_8D, SyncedQuantization::kNumSyncedQuantization);
  GetParam(kRearmStep)->InitEnum("LFORearmStep", RearmQuantization::R_NONE, RearmQuantization::kNumRearmQuantization);
  GetParam(kRearmTap)->InitBool("LFORearmTap", false);
  GetParam(kRearmOnFrequencyUpdate)->InitBool("RearmOnFrequencyUpdate", false);
  GetParam(kGraphicLFOFeedback)->InitDouble("FilterCutoffMod",20000,20,20000,1,"HZ");
  GetParam(kGraphicLFOFeedback)->SetShape(4.1634);
  //GetParam(kGraphicLFOFeedback)->SetShape(2);
  GetParam(kGraphicLFOFeedback)->SetCanAutomate(false);
  GetParam(kBoosterRate)->InitDouble("BoosterRate", 0.0, 0.0, 0.999, 0.001);

#pragma region SetDisplayText

  // FILTER MODE

  GetParam(kFilterMode)->SetDisplayText(FilterMode::FILTER_MODE_BANDPASS, "BandPass");
  GetParam(kFilterMode)->SetDisplayText(FilterMode::FILTER_MODE_HIGHPASS, "HighPass");
  GetParam(kFilterMode)->SetDisplayText(FilterMode::FILTER_MODE_LOWPASS, "LowPass");


  // WAVE

  GetParam(kLFOWave)->SetDisplayText(OscillatorMode::OSCILLATOR_MODE_SAW, "Saw");
  GetParam(kLFOWave)->SetDisplayText(OscillatorMode::OSCILLATOR_MODE_SINE, "Sine");
  GetParam(kLFOWave)->SetDisplayText(OscillatorMode::OSCILLATOR_MODE_SQUARE, "Square");
  GetParam(kLFOWave)->SetDisplayText(OscillatorMode::OSCILLATOR_MODE_TRIANGLE, "Triangle");

  // SYNCED_QUANTIZATION

  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_8, "8/1");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_8T, "8/1T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_8D, "8/1D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_4, "4/1");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_4T, "4/1T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_4D, "4/1D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_2, "2/1");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_2D, "2/1D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_2T, "2/1T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1, "1/1");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1T, "1/1T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1D, "1/1D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_2, "1/2");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_2T, "1/2T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_2D, "1/2D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_4, "1/4");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_4T, "1/4T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_4D, "1/4D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_8, "1/8");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_8T, "1/8T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_8D, "1/8D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_16, "1/16");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_16T, "1/16T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_16D, "1/16D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_32, "1/32");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_32T, "1/32T");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_32D, "1/32D");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_64, "1/64");
  GetParam(kLFOSyncedFrequency)->SetDisplayText(SyncedQuantization::BAR_1_64D, "1/64D");



  //REARM_QUANTIZATION

  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_NONE, "None");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_8, "8/1");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_8T, "8/1T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_4, "4/1");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_4T, "4/1T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_2, "2/1");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_2T, "2/1T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1, "1/1");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1T, "1/1T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_2, "1/2");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_2T, "1/2T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_4, "1/4");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_4T, "1/4T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_8, "1/8");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_8T, "1/8T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_16, "1/16");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_16T, "1/16T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_32, "1/32");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_32T, "1/32T");
  GetParam(kRearmStep)->SetDisplayText(RearmQuantization::R_BAR_1_64, "1/64");

  GetParam(kRearmTap)->SetDisplayText(0, "Ready to Rearm...");
  GetParam(kRearmTap)->SetDisplayText(1, "Rearmed !");


#pragma endregion

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  //pGraphics->AttachPanelBackground(&COLOR_BLACK);
  pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);

  IBitmap filtermodeBitmap = pGraphics->LoadIBitmap(SWITCH_FILTER_MODE_ID, SWITCH_FILTER_MODE_FN, 3);
  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  IBitmap knob_29f = pGraphics->LoadIBitmap(KNOB_29F_ID, KNOB_29F_FN, 101);
  IBitmap waveformBitmap = pGraphics->LoadIBitmap(SWITCH_WAVE_FORM_ID, SWITCH_FILTER_MODE_FN, 4);
  IBitmap metalSwitchBitmap = pGraphics->LoadIBitmap(METAL_SWITCH_ID, METAL_SWITCH_FN, 2);
  IBitmap tapButton = pGraphics->LoadIBitmap(TAP_BUTTON_ID, TAP_BUTTON_FN, 2);
  IBitmap knobOffset = pGraphics->LoadIBitmap(KNOB_OFFSET_ID, KNOB_OFFSET_FN, 101);
  IBitmap horizontalSwitch = pGraphics->LoadIBitmap(HORIZONTAL_SWITCH_ID, HORIZONTAL_SWITCH_FN, 2);
  IBitmap ledFeedback = pGraphics->LoadIBitmap(LED_FEEDBACK_ID, LED_FEEDBACK_FN, 65);
  IBitmap knob_fat = pGraphics->LoadIBitmap(KNOB_FAT_ID, KNOB_FAT_FN, 101);

  pGraphics->AttachControl(new ISwitchControl(this, kFilterModeX, kFilterModeY, kFilterMode, &filtermodeBitmap));
  pGraphics->AttachControl(new IKnobMultiControl(this, kGraphicLFOFeedbackX, kGraphicLFOFeedbackY, kGraphicLFOFeedback, &ledFeedback));
  pGraphics->AttachControl(new IKnobMultiControl(this, kFilterCutoffX, kFilterCutoffY, kFilterCutoff, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kFilterResonanceX, kFilterResonanceY, kFilterResonance, &knob));
  pGraphics->AttachControl(new ISwitchControl(this, kLFOWaveX, kLFOWaveY, kLFOWave, &waveformBitmap));
  pGraphics->AttachControl(new IKnobMultiControl(this, kLFOAmountX, kLFOAmountY, kLFOAmount, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kLFOOffsetX, kLFOOffsetY, kLFOOffset, &knobOffset));
  pGraphics->AttachControl(new ISwitchControl(this, kLFOIsSyncedX, kLFOIsSyncedY, kLFOIsSynced, &metalSwitchBitmap));
  pGraphics->AttachControl(new IKnobMultiControl(this, kLFOFrequencyX, kLFOFrequencyY, kLFOFrequency, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kLFOSyncedFrequencyX, kLFOSyncedFrequencyY, kLFOSyncedFrequency, &knob_29f));
  pGraphics->AttachControl(new IKnobMultiControl(this, kRearmStepX, kRearmStepY, kRearmStep, &knob));
  pGraphics->AttachControl(new IContactControl(this, kRearmTapX, kRearmTapY, kRearmTap, &tapButton));
  pGraphics->AttachControl(new ISwitchControl(this, kRearmOnFrequencyUpdateX, kRearmOnFrequencyUpdateY, kRearmOnFrequencyUpdate, &horizontalSwitch));
  pGraphics->AttachControl(new IKnobMultiControl(this, kBoosterRateX, kBoosterRateY, kBoosterRate, &knob_fat));
  pGraphics->GetControl(kLFOSyncedFrequency + 1)->Hide(true); 


  //BPM View;

  IRECT versionRect(500, 150, 580, 170);
  IText versionText(14, &COLOR_WHITE, "Arial", IText::kStyleNormal, IText::kAlignNear, 0, IText::kQualityDefault);
  ITextControl* versionTextControl = new ITextControl(this,versionRect,&versionText,VST3_VER_STR);
  pGraphics->AttachControl(versionTextControl);

  IRECT printerRect(385, 18, 585, 40);
  parameterPrinter = new ParameterPrinterControl(this, printerRect, &COLOR_TRANSPARENT);
  pGraphics->AttachControl(parameterPrinter);

#ifdef _DEBUG
  IRECT debugRect(480, 360, 600, 375);
  IBitmap debugBackground = pGraphics->LoadIBitmap(DEBUG_BACKGROUND_ID, DEBUG_BACKGROUND_FN);
  debugPanel = new DebugPanelControl(this, 260, 175, &debugBackground, debugRect);
  pGraphics->AttachControl(debugPanel);
#endif


  //pGraphics->ShowControlBounds(true);

  AttachGraphics(pGraphics);

}
 
Wobbleizer::~Wobbleizer() {}

 void Wobbleizer::ApplyFilter(double** inputs, double** outputs, int nFrames){

	outputs = mFilter.process(inputs, nFrames);

}

void Wobbleizer::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

	int const channelCount = 2;

	double* leftInput = inputs[0];
	double* rightInput = inputs[1];
	double* leftOutput = outputs[0];
	double* rightOutput = outputs[1];

	// LFO and rearm control update
	double currentTempo = GetTempo();
	if (currentTempo != mLFO.getTempo()){
		mLFO.setTempo(currentTempo);
	}

	ITimeInfo* ti = new ITimeInfo();
	GetTime(ti);
	double tivalue = ti->mPPQPos;
	
	mRearmClock.setTempo(currentTempo);
	if (mRearmClock.getNeedRearm(tivalue)){
#ifdef _DEBUG
		debugPanel->Print(">AUDIO CORE : LFO PHASE REARMED");
#endif
		mLFO.resetPhase();
	}
	
	//GUI update
	GetGUI()->SetParameterFromPlug(kGraphicLFOFeedback, mFilter.getCalculatedCutoffFrequency(), false);

	double lfoFilterModulation;


	audioLeftInputBuffer.Add(leftInput, nFrames * sizeof(double));
	audioRightInputBuffer.Add(rightInput, nFrames * sizeof(double));

	while (audioLeftInputBuffer.Available() >= sizeof(double)*BUFFER_SIZE) {
		double** data;
		
		data = (double**)malloc(2 * sizeof(double**));
		data[0] = (double*)malloc(BUFFER_SIZE * sizeof(double));
		data[1] = (double*)malloc(BUFFER_SIZE * sizeof(double));

		audioLeftInputBuffer.GetToBuf(0, data[0], BUFFER_SIZE * sizeof(double));
		audioLeftInputBuffer.Advance(BUFFER_SIZE * sizeof(double));

		audioRightInputBuffer.GetToBuf(0, data[1], BUFFER_SIZE * sizeof(double));
		audioRightInputBuffer.Advance(BUFFER_SIZE * sizeof(double));


		lfoFilterModulation = mLFO.reachSample(BUFFER_SIZE) * mLFOFilterModAmount;

		mFilter.setCutoffMod(lfoFilterModulation);
		mFilter.process(data, BUFFER_SIZE);
		mFattner.process(data, BUFFER_SIZE);

		audioLeftOutputBuffer.Add(data[0], BUFFER_SIZE * sizeof(double));
		audioRightOutputBuffer.Add(data[1], BUFFER_SIZE * sizeof(double));

		free(data[0]);
		free(data[1]);
		free(data);

	}

	audioLeftOutputBuffer.GetToBuf(0, leftOutput, nFrames * sizeof(double));
	audioLeftOutputBuffer.Advance(nFrames * sizeof(double));

	audioRightOutputBuffer.GetToBuf(0, rightOutput, nFrames * sizeof(double));
	audioRightOutputBuffer.Advance(nFrames * sizeof(double));

	free(ti);
}


void Wobbleizer::Reset()
{
	TRACE;
	IMutexLock lock(this);
	double sampleRate = GetSampleRate();
	mLFO.setSampleRate(sampleRate);
	mFilter.setSampleRate(sampleRate);
	mFattner.setSampleRate(sampleRate);

	
}

void Wobbleizer::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
	double mod;
	switch (paramIdx)
	{
	case kFilterMode:
		mFilter.setFilterMode(static_cast<FilterMode>(GetParam(kFilterMode)->Int()));
		break;
	case kFilterCutoff:
		mFilter.setCutoff(GetParam(kFilterCutoff)->Value());
		break;
	case kFilterResonance:
		mFilter.setResonance(GetParam(kFilterResonance)->Value());
		break;
	case kLFOWave:
		mLFO.setMode(static_cast<OscillatorMode>(GetParam(kLFOWave)->Int()));
		break;
	case kLFOAmount:
		mod = GetParam(kLFOAmount)->Value();

		if (mod == 0.0){
			mLFO.setMuted(true);
			mLFOFilterModAmount = 0;
		}
		else{
			mLFO.setMuted(false);
			mLFOFilterModAmount = mod;
		}
		break;
	case kLFOOffset:
		mLFO.setOffsetFromDeg(GetParam(kLFOOffset)->Int());
		break;
	case kLFOIsSynced:

		if (GetParam(kLFOIsSynced)->Value() == true){
			GetGUI()->GetControl(kLFOFrequency + 1)->Hide(true);
			GetGUI()->GetControl(kLFOSyncedFrequency + 1)->Hide(false);
			mLFO.setIsSynced(true);
			mLFO.setQuantization(static_cast<SyncedQuantization>(GetParam(kLFOSyncedFrequency)->Int()));
		}
		else{
			GetGUI()->GetControl(kLFOFrequency + 1)->Hide(false);
			GetGUI()->GetControl(kLFOSyncedFrequency + 1)->Hide(true);
			mLFO.setIsSynced(false);
			mLFO.setFrequency(GetParam(kLFOFrequency)->Value());

		}
		break;

	case kLFOFrequency:{
		double newDValue = GetParam(kLFOFrequency)->Value();
		if (newDValue != mLFO.getFrequency() && !mLFO.getIsSynced() && mRearmOnFrequencyUpdate){
#ifdef _DEBUG
			debugPanel->Print(">PARAMETER UPDATE > AUDIO CORE > LFO PHASE REARMED");
#endif
			mLFO.resetPhase();
		}
		mLFO.setFrequency(newDValue);
	}
		break;
	
	case kLFOSyncedFrequency:{

		SyncedQuantization newValue = static_cast<SyncedQuantization>(GetParam(kLFOSyncedFrequency)->Int());
		if (newValue != mLFO.getQuantization() && mLFO.getIsSynced() && mRearmOnFrequencyUpdate){

#ifdef _DEBUG
			debugPanel->Print(">PARAMETER UPDATE > AUDIO CORE > LFO PHASE REARMED");
#endif
			mLFO.resetPhase();
		}
		mLFO.setQuantization(newValue);
	}

		break;
	
	case kRearmStep:
		mRearmClock.setQuantization(static_cast<RearmQuantization>(GetParam(kRearmStep)->Int()));
		break;
	case kRearmTap:
		if (GetParam(kRearmTap)->Value() == true){

			mLFO.resetPhase();
		}
		break;
	case kRearmOnFrequencyUpdate:
		mRearmOnFrequencyUpdate = GetParam(kRearmOnFrequencyUpdate)->Bool();
		break;
	case kBoosterRate:
		mFattner.setRatio(GetParam(kBoosterRate)->Value());
		break;
	default:
      break;
  }


	// Parameter printer update
	char valueDisplay[50] = "";
	int lastClickedId = GetGUI()->GetLastClickedParam();

	if (lastClickedId >= 0){
		if (lastClickedId == kFilterCutoff){
			sprintf(valueDisplay, "%d", (int)mFilter.getCutoffFrequency());
		}
		else{
			GetParam(lastClickedId)->GetDisplayForHost(valueDisplay);
		}

		if (lastClickedId != kGraphicLFOFeedback){
			char nameDisplay[100];
			strcpy(nameDisplay, GetParam(lastClickedId)->GetNameForHost());
			parameterPrinter->SetParameterPairText(nameDisplay, valueDisplay);

		}
	}

	//DebugPrinter update
#ifdef _DEBUG
	char debugText[200];
	strcpy(debugText, ">PARAMETER UPDATE: ");
	strcat(debugText, GetParam(paramIdx)->GetNameForHost());
	strcat(debugText, " VALUE ");
	strcat(debugText, valueDisplay);
	debugPanel->Print(debugText);
#endif	

}
