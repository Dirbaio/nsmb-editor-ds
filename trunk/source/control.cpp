/*
*   This file is part of NSMB Editor DS
*
*   NSMB Editor DS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NSMB Editor DS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NSMB Editor DS.  If not, see <http://www.gnu.org/licenses/>.
*/


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
