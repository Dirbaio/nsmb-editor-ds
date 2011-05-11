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


#include <nds.h>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "rom.h"
#include "tilesets.h"
#include "level.h"
#include "levelrendering.h"
#include "ui.h"
#include "control.h"
#include "oamUtil.h"

#include "textlist.h"
#include "spritedataeditor.h"
#include "lists.h"
#include "text.h"
#include "objectlist.h"

#define mapxy(x, y) map[x+y*32]

void loadNewLevel()
{
    if(editor)
        delete editor;
        
    iprintf("Loading Editor!\n");
    int level = showTextList(levelList);
	editor = new LevelEditor(levelFileList[level]);
}
int main(void)  
{
	// install the default exception handler
 	defaultExceptionHandler();

	consoleDemoInit(); 
	iprintf("NSMB Editor DS!\n");
	iprintf("By Dirbaio\n\n");
	iprintf("Built %s %s\n\n", __DATE__, __TIME__);
    
    
	BG_PALETTE[0] = 0xEE0E;
	
	lcdMainOnBottom();
	
//	REG_POWERCNT = POWER_ALL_2D;
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG_EXT_PALETTE | DISPLAY_SPR_EXT_PALETTE | DISPLAY_SCREEN_BASE(1);

	vramSetBankD(VRAM_D_MAIN_BG_0x06000000);
	vramSetBankA(VRAM_A_MAIN_SPRITE_0x06400000);


	if (!fatInitDefault())
	{
        iprintf("Error loading FAT. Halting.");
        while(1);
	}
    
    
    textInit();
    
    loadLists();
    
    iprintf("Loading Rom!\n");
    loadROM();
		
	uiShow();
    iprintf("Loading OAM!\n");
	loadOAM();
	bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 0x10, 0);
	bgShow(2);
	bgSetPriority(2, 1);
	REG_BG2CNT |= BG_WRAP_ON;
	bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0x14, 0);
	bgShow(3);
	bgSetPriority(3, 0);
	REG_BG3CNT |= BG_WRAP_ON;
	bgUpdate();
		
	loadTilesets(0);
    loadNewLevel();
//	editor = new LevelEditor("A01_1");


	iprintf("Done loading everything!\n");
	
    /*
    NitroFile* f = fs->getFileByName("A01_1.bin");
    f->print();
    u8* cts = f->getContents();
    f->replaceContents(cts, f->size);
    iprintf("NitroFS test done\n");
    while(1);
     */
	iprintf("Going to main loop!\n");
	while(1)
	{
		checkControls();
		if(keysNowPressed & KEY_START)
		    loadNewLevel();
		    
		editor->renderSprites();
		oamFrame();
		swiWaitForVBlank();
//		renderSprite(10, 10, SpriteSize_16x16, 0, 0);
//		renderSprite(30, 10, SpriteSize_16x16, 2, 2);
//		renderRect(5, 5, 24, 56);
	}
}
