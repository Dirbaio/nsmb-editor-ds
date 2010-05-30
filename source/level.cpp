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

List<LevelObject> objects;
List<LevelSprite> sprites;

bool *selectedObjects;
uint selectedObjectsSize;

uint levelFileID, bgdatFileID;

uint8* levelFile;
blockPtr* blockPointers;

uint8 *levelBlocks[14];

bool loaded = false;

inline uint min(uint a, uint b)
{
	return a<b?a:b;
}
inline uint max(uint a, uint b)
{
	return a>b?a:b;
}

void loadLevel(uint levelFileIDp, uint bgdatFileIDp)
{
	loaded = true;
	
	levelFileID = levelFileIDp;
	bgdatFileID = bgdatFileIDp;
	
	uint8* bgdatFile = loadFileFromROM(bgdatFileIDp);
	uint  fileSize = getFileSizeFromROM(bgdatFileIDp);
	
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

		o.tilesetNum = objects[i].objNum >> 12;
		o.objNum &= 0x0FFF;
		o.selected = false;
		objects.addItem(o);
		filePos += 10;
	}
	
	uint8* levelFile = loadFileFromROM(levelFileIDp);
	blockPointers = (blockPtr*) levelFile;
	for(int i = 0; i < 14; i++)
	{
		levelBlocks[i] = &levelFile[blockPointers[i].offs];
	}
	loadTilesets(levelBlocks[0][0xC]);
	free(bgdatFile);
}

void unloadLevel()
{
	if(!loaded) return;
	
	free(levelFile);
	unloadTilesets();
	
	loaded = false;
}
