#include "control.hpp"

uint keysNowHeld;
uint keysNowPressed;
touchPosition touch;
uint lasttx, lastty;
bool penOverUI = false;

void checkControls()
{
	keysNowHeld = keysHeld();
	keysNowPressed = keysDown();
	touchRead(&touch);
	
	if(keysNowPressed & KEY_TOUCH)
	{
		penOverUI = touch.py <= 16;
//   		if(penOverUI)
//			uiTouchDown(touch.px, touch.py);
//		else
//			editorTouchDown(touch.px, touch.py);
		lasttx = touch.px;
		lastty = touch.py;
	}
	
	if(touch.px != lasttx || touch.py != lastty)
	{
//		if(!penOverUI)
//			editorTouchMoved(touch.px - lasttx, touch.py - lastty);
		lasttx = touch.px;
		lastty = touch.py;		
	}
}
