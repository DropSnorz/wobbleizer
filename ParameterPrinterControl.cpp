#include "ParameterPrinterControl.h"
#include <iostream>

ParameterPrinterControl::ParameterPrinterControl(IPlugBase *pPlug, IRECT pR, const IColor* pColor) : IControl(pPlug,pR)
{
	strcpy(mParameterName,"Hi there !\0");
	strcpy(mParameterValue, "\0");

	mRECTName = new IRECT(pR.L, pR.T, pR.R, pR.B);
	mRECTValue = new IRECT(pR.L + 120, pR.T, pR.R, pR.B);
	IColor textColor = IColor(210,15,15,15);

	mTextName = new IText(16, &textColor, "Courier New", IText::kStyleNormal, IText::kAlignNear, 0, IText::kQualityDefault);
	mDisplayName = new WDL_String();

	mTextValue = new IText(16, &textColor, "Courier New", IText::kStyleNormal, IText::kAlignFar, 0, IText::kQualityDefault);
	mDisplayValue = new WDL_String();

}

void ParameterPrinterControl::SetParameterPairText(char* parameterName, char* parameterValue){
	SetParameterNameText(parameterName);
	SetParameterValueText(parameterValue);
	SetDirty();

	
}

void ParameterPrinterControl::SetParameterNameText(char* parameterName){

	strcpy(mParameterName, parameterName);

}

void ParameterPrinterControl::SetParameterValueText(char* parameterValue){

	strcpy(mParameterValue,  parameterValue);

}

bool ParameterPrinterControl::Draw(IGraphics* pGraphics)
{
	DBGMSG("DRAWING...");
	mDisplayName.SetFormatted(80, "%s", mParameterName);
	mDisplayValue.SetFormatted(80, "%s", mParameterValue);

	return (pGraphics->DrawIText(mTextName, mDisplayName.Get(), mRECTName) && pGraphics->DrawIText(mTextValue, mDisplayValue.Get(), mRECTValue));

	return true;
}

ParameterPrinterControl::~ParameterPrinterControl()
{
	//TODO delete text control;
}
