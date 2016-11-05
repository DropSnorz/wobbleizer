#pragma once
#include"IControl.h"
#include "IBitmapMonoText.h"

class DebugPanelControl : public IBitmapOverlayControl
{
public:
	DebugPanelControl(IPlugBase* pPlug, int x, int y, IBitmap* pBitmap, IRECT pTargetArea);
	~DebugPanelControl();
	bool Draw(IGraphics* pGraphics);
	bool IsDirty();
	void Print(char* str);

private:

	ITextControl * text;
	char strDebug[5000];
	void updateStr();
};

