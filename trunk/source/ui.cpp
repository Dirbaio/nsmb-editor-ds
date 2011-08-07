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


#include "ui.h"
#include "uiGraphics.h"
#include "tilesets.h"

#include "rom.h"

bool uiOn = false;

#define BTN_SAVE 0
#define BTN_SCROLL 1
#define BTN_MOVE 2
#define BTN_RESIZE 6
#define BTN_DELETE 3
#define BTN_CLONE 4
#define BTN_PALETTE 5
#define BTN_PROPERTIES 8
#define BTN_VIEW 10
#define BTN_ZONE 11
#define BTN_ABOUT 9
#define BTN_SCROLLSPEED_UP 12
#define BTN_SCROLLSPEED_DOWN 13
#define BTN_EMPTY 17

bool saving = false;
bool paletteOn = false;

uint16 *bg1ptr;

inline void setTileXY(uint x, uint y, uint16 tile, bool sel)
{
//	iprintf("%d %d: %d\n", x, y, tile);
	if(sel)
		tile |= 1<<15;
	bg1ptr[(x%32) + (y%32)*32] = tile;
	
}

void renderButton(int pos, int btn, bool sel)
{
	btn++; //skip first empty
	setTileXY(pos*2+0, 0, btn*4+0, sel);
	setTileXY(pos*2+1, 0, btn*4+1, sel);
	setTileXY(pos*2+0, 1, btn*4+2, sel);
	setTileXY(pos*2+1, 1, btn*4+3, sel);
}


void renderUI()
{
	bg1ptr=  (uint16*)0x0601C000;
	for(int x = 0; x < 16; x++)
		renderButton(x, BTN_EMPTY, false);
	
	renderButton(0, BTN_SAVE, saving);
	
	renderButton(2, BTN_SCROLL, editor->editAction == EDITACTION_SCROLL);
	renderButton(3, BTN_MOVE, editor->editAction == EDITACTION_MOVE);
	renderButton(4, BTN_RESIZE, editor->editAction == EDITACTION_RESIZE);
	renderButton(5, BTN_CLONE, editor->editAction == EDITACTION_CLONE);
	
	renderButton(7, BTN_DELETE, false);
	
	renderButton(9, BTN_ABOUT, false);
	renderButton(10, BTN_SCROLLSPEED_UP, false);
	renderButton(11, BTN_SCROLLSPEED_DOWN, false);
	
	renderButton(13, BTN_PROPERTIES, false);
	renderButton(14, BTN_PALETTE, false);
}

void uiShow()
{
	uiOn = true;
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, 0x18, 0x4);
	cpuCopy16(&uiGraphicsTiles, bgGetGfxPtr(1) + 128, uiGraphicsTilesLen);
	
	vramSetBankE(VRAM_E_LCD);
	cpuCopy16(&uiGraphicsPal, VRAM_E_EXT_PALETTE[1][0], uiGraphicsPalLen);
	shadeExtPal(1);
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);

	bgShow(1);
	renderUI();
}

void uiHide()
{
	uiOn = false;
	bgHide(1);
}

void uiTouchDown(int x, int y)
{
	int bx = x / 16;
	
	switch(bx)
	{
		case 0:  editor->save(); break;
		case 2:  editor->editAction = EDITACTION_SCROLL; break;
		case 3:  editor->editAction = EDITACTION_MOVE  ; break;
		case 4:  editor->editAction = EDITACTION_RESIZE; break;
		case 5:  editor->editAction = EDITACTION_CLONE ; break;
       
		case 7:  editor->deleteObjects(); break;
		case 13:  editor->showProperties(); break;
		case 14:  editor->showPalette(); break;
		
		case 10: editor->increaseSpeed(); break;
		case 11: editor->decreaseSpeed(); break;
		
		case 9: editor->showAbout(); break;
	}
	
	renderUI();
}

