#define PLUG_MFR "DropSnorz"
#define PLUG_NAME "WobbleIzer"

#define PLUG_CLASS_NAME Wobbleizer

#define BUNDLE_MFR "DropSnorz"
#define BUNDLE_NAME "WobbleIzer"

#define PLUG_ENTRY Wobbleizer_Entry
#define PLUG_VIEW_ENTRY Wobbleizer_ViewEntry

#define PLUG_ENTRY_STR "Wobbleizer_Entry"
#define PLUG_VIEW_ENTRY_STR "Wobbleizer_ViewEntry"

#define VIEW_CLASS Wobbleizer_View
#define VIEW_CLASS_STR "Wobbleizer_View"


// Format        0xMAJR.MN.BG - in HEX! so version 10.1.5 would be 0x000A0105
#define PLUG_VER 0x00001600
#define VST3_VER_STR "0.22.0"

// http://service.steinberg.de/databases/plugin.nsf/plugIn?openForm
// 4 chars, single quotes. At least one capital letter
#define PLUG_UNIQUE_ID 'wobx'
// make sure this is not the same as BUNDLE_MFR
#define PLUG_MFR_ID 'DRSN'

// ProTools stuff

#if (defined(AAX_API) || defined(RTAS_API)) && !defined(_PIDS_)
  #define _PIDS_
  const int PLUG_TYPE_IDS[2] = {'EFN1', 'EFN2'};
  const int PLUG_TYPE_IDS_AS[2] = {'EFA1', 'EFA2'}; // AudioSuite
#endif

#define PLUG_MFR_PT "DropSnorz\nDropSnorz\nAcme"
#define PLUG_NAME_PT "Wobbleizer\nIPEF"
#define PLUG_TYPE_PT "Effect"
#define PLUG_DOES_AUDIOSUITE 1

/* PLUG_TYPE_PT can be "None", "EQ", "Dynamics", "PitchShift", "Reverb", "Delay", "Modulation", 
"Harmonic" "NoiseReduction" "Dither" "SoundField" "Effect" 
instrument determined by PLUG _IS _INST
*/

#define PLUG_CHANNEL_IO "1-1 2-2"

#define PLUG_LATENCY 64
#define PLUG_IS_INST 0

// if this is 0 RTAS can't get tempo info
#define PLUG_DOES_MIDI 0

#define PLUG_DOES_STATE_CHUNKS 0

// Unique IDs for each image resource.
#define KNOB_ID 101
#define SWITCH_FILTER_MODE_ID 102
#define SWITCH_WAVE_FORM_ID 103
#define METAL_SWITCH_ID 104
#define BACKGROUND_ID 105
#define DEBUG_BACKGROUND_ID 106
#define TAP_BUTTON_ID 107
#define KNOB_OFFSET_ID 108
#define KNOB_29F_ID 109
#define HORIZONTAL_SWITCH_ID 110
#define LED_FEEDBACK_ID 111
#define KNOB_FAT_ID 112

// Image resource locations for this plug.
#define KNOB_FN "resources/img/knob.png"
#define SWITCH_FILTER_MODE_FN "resources/img/filtermode.png"
#define SWITCH_WAVE_FORM_FN "resources/img/waveform.png"
#define METAL_SWITCH_FN "resources/img/metalswitch.png"
#define BACKGROUND_FN "resources/img/background.png"
#define DEBUG_BACKGROUND_FN "resources/img/debugbackground.png"
#define TAP_BUTTON_FN "resources/img/tap.png"
#define KNOB_OFFSET_FN "resources/img/knoboffset.png"
#define KNOB_29F_FN "resources/img/knob_29f.png"
#define HORIZONTAL_SWITCH_FN "resources/img/horizontal_switch.png"
#define LED_FEEDBACK_FN "resources/img/led_feedback.png"
#define KNOB_FAT_FN "resources/img/boosterknob.png"

// GUI default dimensions
#define GUI_WIDTH 600
#define GUI_HEIGHT 375

// on MSVC, you must define SA_API in the resource editor preprocessor macros as well as the c++ ones
#if defined(SA_API) && !defined(OS_IOS)
#include "app_wrapper/app_resource.h"
#endif

// vst3 stuff
#define MFR_URL "dropsnorz.com"
#define MFR_EMAIL "dropsnorz@gmail.com"
#define EFFECT_TYPE_VST3 "Fx"

/* "Fx|Analyzer"", "Fx|Delay", "Fx|Distortion", "Fx|Dynamics", "Fx|EQ", "Fx|Filter",
"Fx", "Fx|Instrument", "Fx|InstrumentExternal", "Fx|Spatial", "Fx|Generator",
"Fx|Mastering", "Fx|Modulation", "Fx|PitchShift", "Fx|Restoration", "Fx|Reverb",
"Fx|Surround", "Fx|Tools", "Instrument", "Instrument|Drum", "Instrument|Sampler",
"Instrument|Synth", "Instrument|Synth|Sampler", "Instrument|External", "Spatial",
"Spatial|Fx", "OnlyRT", "OnlyOfflineProcess", "Mono", "Stereo",
"Surround"
*/
