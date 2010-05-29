#include "oamUtil.h"
#include "oamGfx.h"
#include "rom.h"

void loadOAM()
{
	iprintf("INITOAM");
	//initialize the sub sprite engine with 1D mapping 128 byte boundary
	//and no external palette support
	oamInit(&oamMain, SpriteMapping_Bmp_1D_128, true);
	
	dmaCopySafe(&oamGfxTiles, SPRITE_GFX, oamGfxTilesLen);

	//Load the Palettes
	vramSetBankF(VRAM_F_LCD);
	dmaCopySafe(&oamGfxPal, VRAM_F_EXT_PALETTE[0][0], oamGfxPalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);


}