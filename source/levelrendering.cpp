#include "levelrendering.hpp"

uint xMin, xMax, yMin, yMax;
bool renderingSelected;

bool onScreen(levelObject& obj)
{
	if(obj.x > xMax) return false;
	if(obj.y > yMax) return false;
	if(obj.x + obj.width < xMin) return false;
	if(obj.y + obj.height < yMin) return false;
	return true;
}

inline void setTileXY(uint x, uint y, uint16 tile)
{
//	if(tile != 0)
//		iprintf("-");
	uint16 *map = BG_MAP_RAM(0x20);
//	if(x >= 32) map += 32*32;
//	if(y >= 32) map += 32*32*2;
	if(renderingSelected)
		tile |= 1<<15;
	map[(x%64) + (y%64)*64] = tile;
	
}

inline void setTileXYb(uint x, uint y, uint16 tile)
{
	uint16 *map = BG_MAP_RAM(0x24);
	map[(x%64) + (y%64)*64] = tile;
	
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
//	iprintf("putarray %x %d %d\n", obj, x, y);
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
	
//	iprintf("ss %d %d, %d %d %d\n", slopeSectionWidth, slopeSectionHeight, subSectionWidth, subSectionHeight, hasSubSection);
	
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
//	iprintf("obj %d %d: %d %d, %d %d\n", tilesetNum, objNum, xp, yp, w, h);
	uint8* obj = objectDefinitions[tilesetNum] + objectIndex[tilesetNum][objNum].offs;
	if(obj[0] & 0x80)
		renderSlopeObject(obj, xp, yp, w, h);
	else
		renderStdObject(obj, xp, yp, w, h);
}



void renderLevel(uint xMins, uint xMaxs, uint yMins, uint yMaxs)
{
	
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
	
	for(uint i = 0; i < objects.size(); i++)
	{
		if(onScreen(objects[i]))
		{
			renderingSelected = selectedObjects[i];
			renderObject(objects[i].objNum, objects[i].tilesetNum, objects[i].x, objects[i].y, objects[i].width, objects[i].height);
		}
	}
}


void renderMinimap()
{
}

