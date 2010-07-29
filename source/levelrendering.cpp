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


#include "levelrendering.h"

uint xMin, xMax, yMin, yMax;
bool renderingSelected;

uint16 *bg2ptr;
uint16 *bg3ptr;

bool onScreen(const LevelElement& obj)
{
	if(obj.x > xMax) return false;
	if(obj.y > yMax) return false;
	if(obj.x + obj.tx < xMin) return false;
	if(obj.y + obj.ty < yMin) return false;
	return true;
}

inline void setTileXY(uint x, uint y, uint16 tile)
{
	if(renderingSelected)
		tile |= 1<<15;
	bg2ptr[(x%64) + (y%64)*64] = tile;
	
}

inline void setTileXYb(uint x, uint y, uint16 tile)
{
	if(renderingSelected)
		tile |= 1<<15;
	bg3ptr[(x%64) + (y%64)*64] = tile;
}

inline uint16 getTileXY(uint x, uint y)
{
	return bg2ptr[(x%64) + (y%64)*64];	
}

inline uint16 getTileXYb(uint x, uint y)
{
	return bg3ptr[(x%64) + (y%64)*64];
}

inline void setMap16TileXY(uint x, uint y, uint16 tile)
{
	if(x < xMin || x > xMax) return;
	if(y < yMin || y > yMax) return;
	setTileXY(x*2,   y*2,   map16Data[tile][0]);
	setTileXY(x*2+1, y*2,   map16Data[tile][1]);
	setTileXY(x*2,   y*2+1, map16Data[tile][2]);
	setTileXY(x*2+1, y*2+1, map16Data[tile][3]);
	
	if(tile < 256)
	{
		setTileXYb(x*2,   y*2,   map16Overlays[tile][0]);
		setTileXYb(x*2+1, y*2,   map16Overlays[tile][1]);
		setTileXYb(x*2,   y*2+1, map16Overlays[tile][2]);
		setTileXYb(x*2+1, y*2+1, map16Overlays[tile][3]);
	}
	else
	{
		setTileXYb(x*2,   y*2,   0);
		setTileXYb(x*2+1, y*2,   0);
		setTileXYb(x*2,   y*2+1, 0);
		setTileXYb(x*2+1, y*2+1, 0);
	}
}

uint16 readTile(uint8* obj, uint& pos, uint8& controlByte)
{
	controlByte = obj[pos];
	pos++;
	uint16 res = 0;
	
	if(!(controlByte & 0x80))
	{
		res = obj[pos] | obj[pos+1]<<8;
		pos += 2;
	}
	
	return res;
}

//returns true if end of obj
bool skipRow(uint8* obj, uint& pos)
{
	uint8 controlByte = 0;
	
	while(controlByte != 0xFF && controlByte != 0xFE)
		readTile(obj, pos, controlByte);
		
	return obj[pos] == 0xFF;
}

void renderTile(uint8* obj, uint& pos, uint xp, uint yp)
{
	uint8 trash;
	setMap16TileXY(xp, yp, readTile(obj, pos, trash));
}

void putArray(uint8 *obj, uint xp, uint yp, uint w, uint h, uint x, uint y, bool& put)
{

	uint xo = x;
	uint pos = 0;
	uint8 trash;
	
	while(obj[pos] != 0xFF && obj[pos] != 0x85)
	{
		uint16 tile = readTile(obj, pos, trash);
		if(trash == 0xFE)
		{
			x = xo;
			y++;
		}
		else
		{
			if(x < w && y < h)
			{
				setMap16TileXY(x+xp, y+yp, tile);
				put = true;
			}
			x++;
		}
	}
}

void renderSlopeObject(uint8* obj, uint xp, uint yp, uint w, uint h)
{
	uint pos = 1; // Skip the slope control
	uint slopeSectionWidth = 0;
	uint8 controlByte = 0;
	while(obj[pos] != 0xFE)
	{
		readTile(obj, pos, controlByte);
		slopeSectionWidth++;
	}
	pos++;
	
	uint slopeSectionHeight = 1;
	
	while(!(obj[pos] & 0x80))
	{
		skipRow(obj, pos);
		slopeSectionHeight++;
	}
	
	bool hasSubSection = obj[pos] == 0x85;
	uint subSectionPos = pos+1;
	uint subSectionHeight = 1;
	uint subSectionWidth = 0;
	if(hasSubSection)
	{
		pos++; // Skip the slope control
		while(obj[pos] != 0xFE)
		{
			readTile(obj, pos, controlByte);
			subSectionWidth++;
		}
		pos++; //skip the newline
		while(obj[pos] != 0xFF)
		{
			skipRow(obj, pos);
			subSectionHeight++;
		}
	}
	

	
	controlByte = obj[0];

	//get direction
	bool goLeft = (controlByte & 1) != 0;
	bool goDown = (controlByte & 2) != 0;

	//get starting point

	int x = 0;
	int y = 0;
	if (!goDown)
		y = h - slopeSectionHeight;
	if (goLeft)
		x = w - slopeSectionWidth;

		//get increments
	int xi = slopeSectionWidth;
	if (goLeft)
		xi = -xi;

	int yi = slopeSectionHeight;
	if (!goDown)
		yi = -yi;
		
	//this is a strange stop condition.
	//Put tells if we have put a tile in the destination
	//When we don't put any tile in destination we are completely
	//out of bounds, so stop.
	bool put = true;
	while (put)
	{
		put = false;
		putArray(&obj[1], xp, yp, w, h, x, y, put);
		if (hasSubSection)
		{
			int xb = x;
			if (goLeft) // right align
				xb = x + slopeSectionWidth - subSectionWidth;
			if(goDown)
				putArray(&obj[subSectionPos], xp, yp, w, h, xb, y - subSectionHeight, put);
			else
				putArray(&obj[subSectionPos], xp, yp, w, h, xb, y + slopeSectionHeight, put);
		}
		x += xi;
		y += yi;
	}	
}

void renderRow(uint8* obj, uint& pos, uint xp, uint yp, uint w)
{

	uint posBegin = pos;
	//First off, let's see what repeats we have...
	uint tilesBeforeRepeat = 0;
	uint tilesInRepeat = 0;
	uint posBeforeRepeat = pos;
	uint posAfterRepeat = pos;
	uint tilesAfterRepeat = 0;
	uint totalTiles;
	
	uint8 trash;
	
	//Section Before Repeat / Or the whole object if it has no repeats
	while(!(obj[pos] & 0x01) && obj[pos] != 0xFE)  //While we dont see the VRepeat
	{
		tilesBeforeRepeat++;
		readTile(obj, pos, trash);
	}
	
	if(obj[pos] == 0xFE)  //If object has ended there's no repeat
	{
		tilesInRepeat = totalTiles = tilesBeforeRepeat;
		tilesAfterRepeat = tilesBeforeRepeat = 0;
	}
	else
	{
		posBeforeRepeat = pos;
		//Section In Repeat
		while(obj[pos] & 0x01 && obj[pos] != 0xFE) 
		{
			tilesInRepeat++;
			readTile(obj, pos, trash);
		}
		posAfterRepeat = pos;
		//Section After Repeat
		while(!(obj[pos] & 0x01) && obj[pos] != 0xFE)
		{
			tilesAfterRepeat++;
			readTile(obj, pos, trash);
		}
		totalTiles = tilesInRepeat + tilesBeforeRepeat + tilesAfterRepeat;
	}
	
	//Now draw the object!!
	
	pos = posBegin;
	
	for(uint i = 0; i < min(tilesBeforeRepeat, w); i++)
	{
		renderTile(obj, pos, xp+i, yp);
	}
	
	int tilesUntilRepeat = 0;
	for(uint i = 0; i + tilesBeforeRepeat + tilesAfterRepeat < w; i++)
	{
		if(tilesUntilRepeat == 0)
		{
			pos = posBeforeRepeat;
			tilesUntilRepeat = tilesInRepeat;
		}
		renderTile(obj, pos, xp+tilesBeforeRepeat+i, yp);
		tilesUntilRepeat--;
	}
	
	pos = posAfterRepeat;
	for(uint i = 0; i < min(tilesAfterRepeat, w); i++)
	{
		renderTile(obj, pos, xp+w-tilesAfterRepeat+i, yp);
	}
	
	pos = posBegin;
	skipRow(obj, pos);
}

void renderStdObject(uint8* obj, uint xp, uint yp, uint w, uint h)
{
	uint pos = 0;
	//First off, let's see what repeats we have...
	uint tilesBeforeRepeat = 0;
	uint tilesInRepeat = 0;
	uint posBeforeRepeat = 0;
	uint posAfterRepeat = 0;
	uint tilesAfterRepeat = 0;
	uint totalTiles;
	
	
	//Section Before Repeat / Or the whole object if it has no repeats
	while(!(obj[pos] & 0x02) && obj[pos] != 0xFF)  //While we dont see the VRepeat
	{
		tilesBeforeRepeat++;
		skipRow(obj, pos);
	}
	
	if(obj[pos] == 0xFF)  //If object has ended there's no repeat
	{
		tilesInRepeat = totalTiles = tilesBeforeRepeat;
		tilesAfterRepeat = tilesBeforeRepeat = 0;
	} 
	else
	{
		posBeforeRepeat = pos;
		//Section In Repeat
		while(obj[pos] & 0x02 && obj[pos] != 0xFF) 
		{
			tilesInRepeat++;
			skipRow(obj, pos);
		}
		posAfterRepeat = pos;
		//Section After Repeat
		while(!(obj[pos] & 0x02) && obj[pos] != 0xFF)
		{
			tilesAfterRepeat++;
			skipRow(obj, pos);
		}
		totalTiles = tilesInRepeat + tilesBeforeRepeat + tilesAfterRepeat;
	}
	
	//Now draw the object!!
	
	pos = 0;
	
	for(uint i = 0; i < min(tilesBeforeRepeat, h); i++)
	{
		renderRow(obj, pos, xp, yp+i, w);
	}
	
	int tilesUntilRepeat = 0;
	for(uint i = 0; i + tilesBeforeRepeat + tilesAfterRepeat < h; i++)
	{
		if(tilesUntilRepeat == 0)
		{
			pos = posBeforeRepeat;
			tilesUntilRepeat = tilesInRepeat;
		}
		renderRow(obj, pos, xp, yp+tilesBeforeRepeat+i, w);
		tilesUntilRepeat--;
	}
	
	pos = posAfterRepeat;
	for(uint i = 0; i < min(tilesAfterRepeat, h); i++)
	{
		renderRow(obj, pos, xp, yp+h-tilesAfterRepeat+i, w);
	}
	
}

void renderObject(uint objNum, uint tilesetNum, uint xp, uint yp, uint w, uint h)
{

	uint8* obj = objectDefinitions[tilesetNum] + objectIndex[tilesetNum][objNum].offs;
	if(obj[0] & 0x80)
    {
		renderSlopeObject(obj, xp, yp, w, h);
    }
	else
    {
		renderStdObject(obj, xp, yp, w, h);
    }
}


void renderLevelSprites(uint xMins, uint xMaxs, uint yMins, uint yMaxs, uint xCam, uint yCam)
{
	return;
	
	xMin = xMins;
	yMin = yMins;
	xMax = xMaxs;
	yMax = yMaxs;
	
	for(list<LevelObject>::reverse_iterator i = objects.rbegin(); i != objects.rend(); ++i)
	{
		if(onScreen(*i))
		{
			renderRect((i->x-xCam)*16, (i->y-yCam)*16, 
				i->tx*16, i->ty*16);
		}
	}
}

void renderTileLineVert(int x, int y, int linetile)
{
	uint16 cur = getTileXYb(x, y);
	if(cur == linetile) //hor
		cur = linetile+2; //vert+hor
	else if(cur != linetile+2) //if nothing
		cur = linetile+1; //vert
		
	setTileXYb(x, y, cur);
}

void renderTileLineHor(int x, int y, int linetile)
{
	uint16 cur = getTileXYb(x, y);
	if(cur == linetile+1) //vert
		cur = linetile+2; //vert+hor
	else if(cur != linetile+2) //if nothing
		cur = linetile; //hor
		
	setTileXYb(x, y, cur);
}

void renderTileRect(int xx, int yy, int tx, int ty, int linetile)
{
	xx *= 2;
	yy *= 2;
	tx *= 2;
	ty *= 2;
	
	for(int x = 0; x < tx; x++)
	{
		renderTileLineHor(x+xx, yy, linetile);
		renderTileLineHor(x+xx, yy+ty, linetile);
	}
	for(int y = 0; y < ty; y++)
	{
		renderTileLineVert(xx, y+yy, linetile);
		renderTileLineVert(xx+tx, y+yy, linetile);
	}
}

void renderSprites()
{
	for(list<LevelSprite>::iterator i = sprites.begin(); i != sprites.end(); ++i)
	{
		if(!onScreen(*i)) continue;
		
		renderingSelected = i->selected;
		
		setTileXY(i->x*2,   i->y*2,   0x3D4 | 1<<14);
		setTileXY(i->x*2+1, i->y*2,   0x3D5 | 1<<14);
		setTileXY(i->x*2,   i->y*2+1, 0x3D6 | 1<<14);
		setTileXY(i->x*2+1, i->y*2+1, 0x3D7 | 1<<14);
		
		int sprNum = i->spriteNum;
		setTileXYb(i->x*2,   i->y*2,   0x3DC + sprNum / 1000 % 10);
		setTileXYb(i->x*2+1, i->y*2,   0x3DC + sprNum / 100 % 10);
		setTileXYb(i->x*2,   i->y*2+1, 0x3DC + sprNum / 10 % 10);
		setTileXYb(i->x*2+1, i->y*2+1, 0x3DC + sprNum / 1 % 10);
	}

}

void renderLevel(uint xMins, uint xMaxs, uint yMins, uint yMaxs)
{
    
	bg2ptr = (uint16*)0x06018000;
	bg3ptr = (uint16*)0x0601A000;
		
	xMin = xMins;
	yMin = yMins;
	xMax = xMaxs;
	yMax = yMaxs;
		
    
	//Zero out the zone we're going to redraw...
	for(uint x = xMin; x <= xMax; x++)
		for(uint y = yMin; y <= yMax; y++)
			setMap16TileXY(x, y, 0);

/*
	for(uint x = 0; x < 16; x++)
	for(uint y = 0; y < 16; y++)
		setMap16TileXY(x, y, x+y*16);
	*/
	
    

	for(list<LevelObject>::iterator i = objects.begin(); i != objects.end(); ++i)
	{

		if(onScreen(*i))
		{

			renderingSelected = i->selected;
			renderObject(i->objNum, i->tilesetNum, 
				i->x, i->y, 
				i->tx, i->ty);

		}
	}

	
	renderingSelected = false;
	
	for(list<LevelObject>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		if(onScreen(*i))
		{
			if(i->selected)
				renderTileRect(i->x, i->y,
					i->tx, i->ty, 0x3e9);
			
		}
	}

	
	renderSprites();

}


void renderMinimap()
{
}

void load()
{
	
}