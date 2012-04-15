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


#include "oamUtil.h"
#include "oamGfx.h"
#include "rom.h"

void loadOAM()
{
	//initialize the sub sprite engine with 1D mapping 128 byte boundary
	//and no external palette support
	oamInit(&oamMain, SpriteMapping_1D_128 , true);
	
	cpuCopy16(&oamGfxTiles, SPRITE_GFX, oamGfxTilesLen);
    
	//Load the Palettes
	vramSetBankF(VRAM_F_LCD);
	cpuCopy16(&oamGfxPal, VRAM_F_EXT_PALETTE[0][0], oamGfxPalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

int oamId;

void renderSprite(int x, int y, SpriteSize size, int tile)
{
	renderSpriteEx(x, y, 0, size, tile, false, false);
}

void renderSpriteEx(int x, int y, int prio, SpriteSize size, int tile, bool hflip, bool vflip)
{
	if(oamId >= 128)
	{
		iprintf("ERROR: OAM IS FULL\n");
		return;
	}
	
	oamSet(&oamMain, oamId++, 
		x, y, prio, 0, 
		size, SpriteColorFormat_256Color, 
		oamGetGfxPtr(&oamMain, tile*2), 
		-1, false, false, hflip, vflip, false);
}

void oamFrame()
{
	oamId = 0;
	oamUpdate(&oamMain);
	oamClear(&oamMain, 0, 0);
}
