#include "ui.hpp"
#include "uiGraphics.h"
#include "tilesets.hpp"

#include "rom.hpp"

bool uiOn = false;

#define BTN_SAVE 0
#define BTN_SCROLL 1
#define BTN_MOVE 2
#define BTN_DELETE 3
#define BTN_CLONE 4
#define BTN_PALETTE 5
#define BTN_OBJ 6
#define BTN_ENTRANCE 7
#define BTN_PATH 8
#define BTN_PPATH 9
#define BTN_VIEW 10
#define BTN_ZONE 11
#define BTN_EMPTY 17

bool saving = false;
uint16 *bg0ptr;

inline void setTileXY(uint x, uint y, uint16 tile, bool sel)
{
	if(sel)
		tile |= 1<<15;
	bg0ptr[(x%32) + (y%32)*32] = tile;
	
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
	bg0ptr= bgGetMapPtr(1);
	for(int x = 0; x < 16; x++)
		renderButton(x, BTN_EMPTY, false);
		
	renderButton(0, BTN_SAVE, saving);
}

void uiShow()
{
	uiOn = true;
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, 0x18, 0x4);
	dmaCopySafe(&uiGraphicsTiles, bgGetGfxPtr(1) + 64, uiGraphicsTilesLen);
	
	vramSetBankE(VRAM_E_LCD);
	dmaCopySafe(&uiGraphicsPal, VRAM_E_EXT_PALETTE[1][0], uiGraphicsPalLen);
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
}

