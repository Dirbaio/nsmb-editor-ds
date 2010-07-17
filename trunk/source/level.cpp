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


#include "level.h"

list<LevelObject> objects;
list<LevelSprite> sprites;

uint levelFileID, bgdatFileID;

uint8* levelFile;
uint8 *levelBlocks[14];
uint levelBlocksLen[14];

bool loaded = false;

inline uint min(uint a, uint b)
{
	return a<b?a:b;
}
inline uint max(uint a, uint b)
{
	return a>b?a:b;
}

void loadObjects()
{
	uint8* bgdatFile = loadFileFromROM(bgdatFileID);
	uint  fileSize = getFileSizeFromROM(bgdatFileID);
	
	uint objCount = fileSize / 10;
	
	uint i;
	uint filePos = 0;
	for (i = 0; i < objCount; i++)
	{
		LevelObject o;
		o.objNum = bgdatFile[filePos]     | (bgdatFile[filePos + 1] << 8);
		o.x =      bgdatFile[filePos + 2] | (bgdatFile[filePos + 3] << 8);
		o.y =      bgdatFile[filePos + 4] | (bgdatFile[filePos + 5] << 8);
		o.tx =     bgdatFile[filePos + 6] | (bgdatFile[filePos + 7] << 8);
		o.ty =     bgdatFile[filePos + 8] | (bgdatFile[filePos + 9] << 8);

		o.tilesetNum = o.objNum >> 12;
		o.objNum &= 0x0FFF;
		o.selected = false;
		objects.push_back(o);
		filePos += 10;
	}
	
	delete[] bgdatFile;
}


struct blockPtr
{
	uint32 offs;
	uint32 size;
};

void loadBlocks()
{
	levelFile = loadFileFromROM(levelFileID);
    u32* levelFile32 = (u32*) levelFile;
    
	for(int i = 0; i < 14; i++)
	{
		levelBlocksLen[i] = levelFile32[i*2+1];
		levelBlocks[i] = levelFile + levelFile32[i*2];
        iprintf("%d -> %x %x\n", i, levelBlocksLen[i], levelBlocks[i]);
	}
	

}

void loadSprites()
{
	int filePos = 0;
	uint8* block = levelBlocks[6];
	uint16 spriteId = block[filePos] | block[filePos+1]<<8;
	filePos += 2;
	
	while(spriteId != 0xFFFF)
	{
		LevelSprite s;
        s.selected = false;
		s.spriteNum = spriteId;
		s.x = block[filePos] | block[filePos+1]<<8;
		filePos += 2;
		s.y = block[filePos] | block[filePos+1]<<8;
		filePos += 2;
		s.spriteData[0] = block[filePos++];
		s.spriteData[1] = block[filePos++];
		s.spriteData[2] = block[filePos++];
		s.spriteData[3] = block[filePos++];
		s.spriteData[4] = block[filePos++];
		s.spriteData[5] = block[filePos++];
		sprites.push_back(s);
		
		spriteId = block[filePos] | block[filePos+1]<<8;
		filePos += 2;
	}

}

void loadLevel(uint levelFileIDp, uint bgdatFileIDp)
{
	loaded = true;
	
	levelFileID = levelFileIDp;
	bgdatFileID = bgdatFileIDp;
	
    objects = list<LevelObject>();
    sprites = list<LevelSprite>();
	loadObjects();
	loadBlocks();
    
    u8 tilnum = levelBlocks[0][0xC];
    iprintf("Tileset Number: %d\n", tilnum);
	loadSprites();
	loadTilesets(tilnum);
}

void unloadLevel()
{
	if(!loaded) return;
	
	for(int i = 0; i < 14; i++)
		delete[] levelBlocks[i];
	
	unloadTilesets();
	
	loaded = false;
}



//========================

int LevelElement::getSizeMultiplier()
{
	return 16;
}

bool LevelElement::isResizable()
{
	return true;
}

bool LevelSprite::isResizable()
{
	return false;
}
LevelSprite::LevelSprite()
{
	tx = 1;
	ty = 1;
}