#include <nds.h>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "rom.hpp"
#include "tilesets.hpp"
#include "level.hpp"
#include "levelrendering.hpp"
#include "ui.hpp"

#define mapxy(x, y) map[x+y*32]


uint calcLevelFileID(uint levNum)
{
	uint res = 822 + levNum*2;
	if(levNum >= 14) res++;
	
	return res;
}

int main(void)  
{
	consoleDemoInit(); 
	iprintf("Helloooooooo World!\n");
	
	BG_PALETTE[0] = 0xEE0E;
	
	lcdMainOnBottom();
	
//	REG_POWERCNT = POWER_ALL_2D;
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG_EXT_PALETTE | DISPLAY_SCREEN_BASE(1);

	vramSetBankA(VRAM_A_MAIN_BG);

	if (fatInitDefault())
	{
		iprintf("Loading Rom!\n");
		loadROM();
		
    	iprintf("Loading tilesets!\n");
	}
	uiShow();
	
	uint lev = 0;
	uint fid = calcLevelFileID(lev);
	loadLevel(fid, fid+1);
	bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
	bgShow(2);
	bgSetPriority(2, 1);
	REG_BG2CNT |= BG_WRAP_ON;
	bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0x14, 0);
	bgShow(3);
	bgSetPriority(3, 0);
	REG_BG3CNT |= BG_WRAP_ON;
	bgUpdate();
	iprintf("Done!\n");
	
	touchPosition touch;
	int levelx = 0;
	int levely = 0;
	int lastlx = 0;
	int lastly = 0;
	int lastx = 0;
	int lasty = 0;

	while(1)
	{
		
		// read the touchscreen coordinates
		touchRead(&touch);
		scanKeys();
		
		int pressed = keysDown();	// buttons pressed this loop
		int held = keysHeld();		// buttons currently held

		if(pressed & KEY_TOUCH)
		{
			lastx = touch.px;
			lasty = touch.py;
		}
		
		if(pressed & KEY_A)
			lev++;
		if(pressed & KEY_B)
			lev--;

		if((pressed & KEY_A)!=0 || (pressed & KEY_B)!=0)
		{
			unloadLevel();
			iprintf("Level %d\n", lev);
			fid = calcLevelFileID(lev);
			loadLevel(fid, fid+1);	
			bgSetScroll(2, levelx%512, levely%512);
			bgSetScroll(3, levelx%512, levely%512);
			renderLevel(levelx/16, levelx/16+16, levely/16, levely/16+12);
		}

		if(held & KEY_TOUCH)
		{
			levelx -= touch.px - lastx;
			levely -= touch.py - lasty;
			
			if(levelx<0) levelx = 0;
			if(levely<0) levely = 0;

			if(levelx/16 != lastlx/16)
			{
				uint xMin, xMax;
				if(levelx < lastlx)
				{
					xMin = levelx/16;
					xMax = lastlx/16;
				}
				else
				{
					xMin = lastlx/16+16;
					xMax = levelx/16+16;
				}
				
				renderLevel(xMin, xMax, levely/16, levely/16+12);
			}
			
			if(levely/16 != lastly/16)
			{
				uint yMin, yMax;
				if(levely < lastly)
				{
					yMin = levely/16;
					yMax = lastly/16;
				}
				else
				{
					yMin = lastly/16+12;
					yMax = levely/16+12;
				}
				
				renderLevel(levelx/16, levelx/16+16, yMin, yMax);
			}

			lastx = touch.px;
			lasty = touch.py;
			lastlx = levelx;
			lastly = levely;
			bgSetScroll(2, levelx%512, levely%512);
			bgSetScroll(3, levelx%512, levely%512);
		}
		
		
		bgUpdate();
		
	/*
		scanKeys();
		int keys = keysDown();
		if(keys & KEY_A)
			currPage++;
		if(keys & KEY_B)
			currPage--;
		if(currPage != lastPage)
		{
			iprintf("Page: %d\n", currPage);
			uint x, y;
			for(x = 0; x < 16; x++)
			{
				for(y = 0; y < 12; y++)
				{
					uint m16addr = (x + y*16 + currPage*12*16)*4;
//					iprintf("%x %x \n", mapaddr, m16addr);
					uint mapaddr = (x*2 + y*64);
					map[mapaddr] = map16Data[m16addr];
					map[mapaddr+1] = map16Data[m16addr+1];
					map[mapaddr+32] = map16Data[m16addr+2];
					map[mapaddr+33] = map16Data[m16addr+3];
				}
			}

			for(i = 0; i < 12*16; i++)
			{
				uint map16Offs = (i+currPage*12*16)*4;
//				uint vramOffs = (i % 16) * 2 + (i / 16) * 64;
				map[0] = map16Data[map16Offs];
				map[1] = map16Data[map16Offs+1];
				map[32] = map16Data[map16Offs+2];
				map[33] = map16Data[map16Offs+3];
			}	
			lastPage = currPage;
		}*/
		
		swiWaitForVBlank();
	}
}
