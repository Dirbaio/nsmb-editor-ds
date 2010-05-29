#include "oamUtil.h"
#include "oamGfx.h"
#include "rom.h"

void loadOAM()
{
	iprintf("INITOAM");
	//initialize the sub sprite engine with 1D mapping 128 byte boundary
	//and no external palette support
	oamInit(&oamMain, SpriteMapping_2D , true);
	
	dmaCopySafe(&oamGfxTiles, SPRITE_GFX, oamGfxTilesLen);

	//Load the Palettes
	vramSetBankF(VRAM_F_LCD);
	dmaCopySafe(&oamGfxPal, VRAM_F_EXT_PALETTE[0][0], oamGfxPalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

int oamId;

void renderSprite(int x, int y, SpriteSize size, int tilex, int tiley)
{
	renderSpriteEx(x, y, 0, size, tilex, tiley, false, false);
}

void renderSpriteEx(int x, int y, int prio, SpriteSize size, int tilex, int tiley, bool hflip, bool vflip)
{
	if(oamId >= 128)
	{
		iprintf("ERROR: OAM IS FULL\n");
		return;
	}
		
	if(x > 256) return;
	if(y > 192) return;
	
	oamSet(&oamMain, oamId++, 
		x, y, prio, 0, 
		size, SpriteColorFormat_256Color, 
		oamGetGfxPtr(&oamMain, (tilex+tiley*16)*2), 
		-1, false, false, hflip, vflip, false);
}

void renderRect(int x, int y, int w, int h)
{
	int xx = 0;
	while(xx < w)
	{
		if(w-xx >= 32)
		{
			renderSprite(x+xx, y, SpriteSize_32x8, 0, 4);
			renderSprite(x+xx, y+h-1, SpriteSize_32x8, 0, 4);
			xx += 32;
		}
		else if(w-xx  >= 16)
		{
			renderSprite(x+xx, y, SpriteSize_16x8, 0, 4);
			renderSprite(x+xx, y+h-1, SpriteSize_16x8, 0, 4);
			xx += 16;
		}
		else if(w-xx  >= 8)
		{
			renderSprite(x+xx, y, SpriteSize_8x8, 0, 4);
			renderSprite(x+xx, y+h-1, SpriteSize_8x8, 0, 4);
			xx += 8;
		}
		else break;
	}


	int yy = 0;
	while(yy < h)
	{
		if(h-yy >= 32)
		{
			renderSprite(x, y+yy, SpriteSize_8x32, 0, 5);
			renderSprite(x+w-1, y+yy, SpriteSize_8x32, 0, 5);
			yy += 32;
		}
		else if(h-yy >= 16)
		{
			renderSprite(x, y+yy, SpriteSize_8x16, 0, 5);
			renderSprite(x+w-1, y+yy, SpriteSize_8x16, 0, 5);
			yy += 16;
		}
		else if(h-yy >= 8)
		{
			renderSprite(x, y+yy, SpriteSize_8x8, 0, 5);
			renderSprite(x+w-1, y+yy, SpriteSize_8x8, 0, 5);
			yy += 8;
		}
		else break;
	}
}

void oamFrame()
{
	oamId = 0;
	oamUpdate(&oamMain);
}