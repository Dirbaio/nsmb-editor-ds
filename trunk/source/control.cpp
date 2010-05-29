#include "control.h"

uint keysNowHeld;
uint keysNowPressed;
touchPosition touch;
uint lasttx, lastty;
bool penOverUI = false;

void checkControls()
{
	scanKeys();
	keysNowHeld = keysHeld();
	keysNowPressed = keysDown();
	touchRead(&touch);
	
	if(keysNowHeld & KEY_TOUCH)
	{

		if(keysNowPressed & KEY_TOUCH)
		{
			penOverUI = touch.py <= 16;

			if(penOverUI)
				uiTouchDown(touch.px, touch.py);
			else
				editorTouchDown(touch.px, touch.py);
		}

		if(touch.px != lasttx || touch.py != lastty)
			if(!penOverUI)
				editorTouchMoved(touch.px, touch.py);

		lasttx = touch.px;
		lastty = touch.py;
	}

}
