#include <vector>
#include "rom.hpp"
#include "tilesets.hpp"



#define MSCREEN_WIDTH 64
#define MSCREEN_HEIGHT 64


#define VSCREEN_WIDTH 16
#define VSCREEN_HEIGHT 12

using namespace std;

#ifndef _LEVEL_H
#define _LEVEL_H

class levelObject
{
	public:
	uint objNum;
	uint tilesetNum;
	uint x, y;
	uint width, height;
};

struct blockPtr
{
	uint32 offs;
	uint32 size;
};

class levelSprite
{
	
};

extern vector<levelObject> objects;
extern uint levelFileID, bgdatFileID;

void loadLevel(uint levelFileIDp, uint bgdatFileIDp);
void unloadLevel();

#endif