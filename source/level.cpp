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
#include "levelrendering.h"

inline u32 min(u32 a, u32 b)
{
	return a<b?a:b;
}
inline u32 max(u32 a, u32 b)
{
	return a>b?a:b;
}

void Level::loadObjects()
{
    NitroFile* bgdatFilePtr = fs->getFileByName(levelFilePrefix+"_bgdat.bin");
	uint8* bgdatFile = bgdatFilePtr->getContents();
	uint  fileSize = bgdatFilePtr->size;
	
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

void Level::saveObjects()
{
    int objCount = objects.size();
    iprintf("Saving... %d\n", objCount);
    u16* bgdatFile = new u16[objCount * 5 + 1];
    int filePos = 0;
    
    for(list<LevelObject>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        bgdatFile[filePos + 0] = (it->objNum & 0xFFF) | ((it->tilesetNum & 0xF)<<12);
        bgdatFile[filePos + 1] = it->x;
        bgdatFile[filePos + 2] = it->y;
        bgdatFile[filePos + 3] = it->tx;
        bgdatFile[filePos + 4] = it->ty;
        filePos += 5;
    }
    
    bgdatFile[objCount*5] = 0xFFFF; // WHY??????

    NitroFile* fp = fs->getFileByName(levelFilePrefix+"_bgdat.bin");
    fp->replaceContents(bgdatFile, objCount*10+2);
}


struct blockPtr
{
	uint32 offs;
	uint32 size;
};

void Level::loadBlocks()
{
    NitroFile* levFil = fs->getFileByName(levelFilePrefix+".bin");
	u8* levelFile = levFil->getContents();
    u32* levelFile32 = (u32*) levelFile;
    
	for(int i = 0; i < 14; i++)
	{
		levelBlocksLen[i] = levelFile32[i*2+1];
        
		levelBlocks[i] = new u8[levelBlocksLen[i]];
        cpuCopy8(levelFile + levelFile32[i*2], levelBlocks[i], levelBlocksLen[i]);
	}
	delete[] levelFile;
}

void Level::saveBlocks()
{
    int levelFileLen = 14*8;
    for(int i = 0; i<14; i++)
        levelFileLen += levelBlocksLen[i];
        
    u8* levelFile = new u8[levelFileLen];
    u32* levelFile32 = (u32*) levelFile;
    
    int filePos = 14*8;
    for(int i = 0; i < 14; i++)
    {
        levelFile32[i*2] = filePos;
        levelFile32[i*2+1] = levelBlocksLen[i];
        cpuCopy8(levelBlocks[i], levelFile+filePos, levelBlocksLen[i]);
        filePos += levelBlocksLen[i];
    }
    
    NitroFile* fp = fs->getFileByName(levelFilePrefix+".bin");
    
    fp->replaceContents(levelFile, levelFileLen);
    
}

void Level::loadSprites()
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

void Level::saveSprites()
{
    uint8* block = new u8[sprites.size()*12 + 2];
    int filePos = 0;
    
    for(list<LevelSprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
    {
        LevelSprite& s = *it;
        block[filePos++] = (u8) s.spriteNum;
        block[filePos++] = (u8) (s.spriteNum >> 8);
        block[filePos++] = (u8) s.x;
        block[filePos++] = (u8) (s.x >> 8);
        block[filePos++] = (u8) s.y;
        block[filePos++] = (u8) (s.y >> 8);
        block[filePos++] = s.spriteData[0];
        block[filePos++] = s.spriteData[1];
        block[filePos++] = s.spriteData[2];
        block[filePos++] = s.spriteData[3];
        block[filePos++] = s.spriteData[4];
        block[filePos++] = s.spriteData[5];
    }
    
    block[filePos++] = 0xFF;
    block[filePos++] = 0xFF;
    
    if(levelBlocks[6])
        delete[] levelBlocks[6];
    levelBlocks[6] = block;
    levelBlocksLen[6] = sprites.size()*12 + 2;
}

void Level::loadEntrances()
{
    uint8* block = levelBlocks[5];
    int filePos = 0;
    for(int i = 0; i < levelBlocksLen[5] / 20; i++)
    {
        LevelEntrance e;
        e.x = block[filePos] | (block[filePos + 1] << 8);
        e.y = block[filePos + 2] | (block[filePos + 3] << 8);
        e.cameraX = block[filePos + 4] | (block[filePos + 5] << 8);
        e.cameraY = block[filePos + 6] | (block[filePos + 7] << 8);
        e.number = block[filePos + 8];
        e.destArea = block[filePos + 9];
        e.pipeID = block[filePos + 10];
        e.destEntrance = block[filePos + 12];
        e.type = block[filePos + 14];
        //e.settings = block[filePos + 15];
        //e.Unknown1 = block[filePos + 16];
        e.destView = block[filePos + 18];
        //e.Unknown2 = block[filePos + 19];
        filePos += 20;
		iprintf("%d %d\n", e.x, e.y);
		entrances.push_back(e);
    }
}
void Level::saveEntrances()
{
    uint8* block = new u8[entrances.size()*12 + 2];
    int filePos = 0;
    
    for(list<LevelEntrance>::iterator it = entrances.begin(); it != entrances.end(); it++)
    {
        LevelEntrance& e = *it;
        block[filePos] = (u8) (e.x & 0xFF);
		block[filePos+1] = (u8) (e.x >> 8);
		block[filePos+2] = (u8) (e.y & 0xFF);
		block[filePos+3] = (u8) (e.y >> 8);
		block[filePos+4] = (u8) (e.cameraX & 0xFF);
		block[filePos+5] = (u8) (e.cameraX >> 8);
		block[filePos+6] = (u8) (e.cameraY & 0xFF);
		block[filePos+7] = (u8) (e.cameraY >> 8);
        block[filePos+8] = e.number;
        block[filePos+9] = e.destArea;
        block[filePos+10] = e.pipeID;
        block[filePos+12] = e.destEntrance;
        block[filePos+14] = e.type;
        //block[filePos + 15] = e.settings;
        //block[filePos + 16] = e.Unknown1;
        block[filePos+18] = e.destView;
        //block[filePos + 19] = e.Unknown2;
		filePos+=20;
		iprintf("%d %d %d ",e.x,block[filePos],block[filePos+1]);
    }
    
    block[filePos++] = 0xFF;
    block[filePos++] = 0xFF;
    
    if(levelBlocks[5])
        delete[] levelBlocks[5];
    levelBlocks[5] = block;
    levelBlocksLen[5] = entrances.size()*12 + 2;
}

Level::Level(string pf)
{
    levelFilePrefix = pf;
	
    objects = list<LevelObject>();
    sprites = list<LevelSprite>();
	loadObjects();
	loadBlocks();
	loadSprites();
    loadEntrances();
    
    u8 tilnum = levelBlocks[0][0xC];
    iprintf("Tileset Number: %d\n", tilnum);
	loadTilesets(tilnum);
}
void Level::save()
{
    saveObjects();
    saveSprites();
    saveEntrances();
    
    saveBlocks();
}

Level::~Level()
{
	for(int i = 0; i < 14; i++)
		delete[] levelBlocks[i];
	
	unloadTilesets();
}



//========================

int LevelElement::getSizeMultiplier() const
{
	return 16;
}

bool LevelElement::isResizable() const
{
	return true;
}

bool LevelSprite::isResizable() const
{
	return false;
}
LevelSprite::LevelSprite()
{
	tx = 1;
	ty = 1;
}

LevelEntrance::LevelEntrance()
{
    tx = 16;
    ty = 16;
}

int LevelEntrance::getSizeMultiplier() const
{
    return 1;
}

bool LevelEntrance::isResizable() const
{
    return false;
}
