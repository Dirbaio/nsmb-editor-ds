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


#include<list>
#include "rom.h"
#include "tilesets.h"

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
	int x, y;
	int tx, ty;
	bool selected;
	
	virtual int getSizeMultiplier();
	virtual bool isResizable();
};

class LevelObject : public LevelElement
{
	public:
	int objNum;
	int tilesetNum;
};

class LevelSprite : public LevelElement
{
	public:
	LevelSprite();
	byte spriteData[6];
	int spriteNum;
	virtual bool isResizable();
};


extern list<LevelObject> objects;
extern list<LevelSprite> sprites;

extern uint levelFileID, bgdatFileID;

void loadLevel(uint levelFileIDp, uint bgdatFileIDp);
void unloadLevel();

#endif