#include "level.h"

vector<LevelObject> objects;
vector<LevelSprite> sprites;
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
	objects = vector<LevelObject>(objCount);
	
	uint i;
	uint filePos = 0;
	for (i = 0; i < objCount; i++)
	{
		objects[i].objNum = bgdatFile[filePos]     | (bgdatFile[filePos + 1] << 8);
		objects[i].x =      bgdatFile[filePos + 2] | (bgdatFile[filePos + 3] << 8);
		objects[i].y =      bgdatFile[filePos + 4] | (bgdatFile[filePos + 5] << 8);
		objects[i].tx =     bgdatFile[filePos + 6] | (bgdatFile[filePos + 7] << 8);
		objects[i].ty =     bgdatFile[filePos + 8] | (bgdatFile[filePos + 9] << 8);

		objects[i].tilesetNum = objects[i].objNum >> 12;
		objects[i].objNum &= 0x0FFF;
		objects[i].selected = false;
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
