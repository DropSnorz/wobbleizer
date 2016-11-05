#pragma once
#include "IControl.h"
class ParameterPrinterControl : public IControl
{
public:
	ParameterPrinterControl(IPlugBase *pPlug, IRECT pR, const IColor* pColor);
	~ParameterPrinterControl();
	bool Draw(IGraphics* pGraphics);
	void SetParameterPairText(char* parameterName, char* parameterValue);
	void SetParameterNameText( char* parameterName);
	void SetParameterValueText( char* parameterValue);


private:

	WDL_String mDisplayName;
	WDL_String mDisplayValue;
	IText *mTextName;
	IText *mTextValue;


	IRECT *mRECTName;
	IRECT *mRECTValue;

	char mParameterName[50];
	char mParameterValue [50];


};

