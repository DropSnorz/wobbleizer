#include "DebugPanelControl.h"


DebugPanelControl::DebugPanelControl(IPlugBase* pPlug, int x, int y, IBitmap* pBitmap, IRECT pTargetArea) : IBitmapOverlayControl(pPlug, x, y, pBitmap, pTargetArea)
{
	IRECT textRect(x+10, y+ 10 , x+100, y+200);
	IText textProperty(12, &COLOR_BLACK, "Arial", IText::kStyleNormal, IText::kAlignNear, 0, IText::kQualityDefault);

	text = new ITextControl(pPlug, textRect, &textProperty, "");
	strcpy(strDebug, "\n\0");
}
DebugPanelControl::~DebugPanelControl()
{
	//remove text
}

void DebugPanelControl::Print(char* str){

	strcat(strDebug, "\n");
	strcat(strDebug, str);
	updateStr();
	text->SetTextFromPlug(strDebug);

}

void DebugPanelControl::updateStr(){

	int c, nbLine = 0;
	char newDebug[5000];
	char *token;
	int size = strlen(strDebug);    // taille de la chaine

	for (c = 0; c < size; c++)
	{
		if (strDebug[c] == '\n') // si le caract�re n� c est bien le caract�re
			nbLine++;         // on incr�mente le compteur
	}

	nbLine = nbLine + 1;

	if (nbLine >= 15){
		strcpy(newDebug, "\0");
		const char s[2] = "\n";
		token = strtok(strDebug, s);
		token = strtok(NULL, s);

		while (token != NULL)
		{
			strcat(newDebug, "\n");
			strcat(newDebug, token);
			token = strtok(NULL, s);

		}

		strcpy(strDebug, newDebug);

	}

}

bool DebugPanelControl::IsDirty(){
	 return (text->IsDirty()); 
	//return false;
}

bool DebugPanelControl::Draw(IGraphics* pGraphics){

	 __super::Draw(pGraphics);

	 if (mValue < 0.5){
		 //text->Draw(pGraphics);
	 }
	 else{
		 text->Draw(pGraphics);
		 
	 }

	 return true;
}


