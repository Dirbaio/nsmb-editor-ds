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

class LevelElement
{
	public:
	uint x, y;
	uint tx, ty;
	bool selected;
};

class LevelObject : public LevelElement
{
	public:
	uint objNum;
	uint tilesetNum;
};

struct blockPtr
{
	uint32 offs;
	uint32 size;
};

class LevelSprite
{
	
};

extern vector<LevelObject> objects;
extern uint levelFileID, bgdatFileID;

void loadLevel(uint levelFileIDp, uint bgdatFileIDp);
void unloadLevel();

#endif