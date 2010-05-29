#include "editor.h"

uint editMode = EDITMODE_OBJECTS;
uint editAction = EDITACTION_SCROLL;

int levelx = 0;
int levely = 0;
int lastlx = 0;
int lastly = 0;


uint calcLevelFileID(uint levNum)
{
	uint res = 822 + levNum*2;
	if(levNum >= 14) res++;
	
	return res;
}


void loadEditor(uint lev)
{
	unloadLevel();
	iprintf("Level %d\n", lev);
	int fid = calcLevelFileID(lev);
	loadLevel(fid, fid+1);
	
	levelx = 0;
	levely = 0;
	lastlx = 0;
	lastly = 0;
	
	bgSetScroll(2, levelx%512, levely%512);
	bgSetScroll(3, levelx%512, levely%512);
	renderLevel(levelx/16, levelx/16+16, levely/16, levely/16+12);
}

uint touchx, touchy;

void repaintScreen()
{
	renderLevel(levelx/16, levelx/16+16, levely/16, levely/16+12);
}

void unselectAll()
{
   	for(uint i = 0; i < objects.size(); i++)
		objects[i].selected = false;
}

void doSelection(uint x, uint y)
{
	unselectAll();
	uint selNum = 0xFFFFFFFF;
	for(uint i = 0; i < objects.size(); i++)
	{
		if(objects[i].x <= x/16 && objects[i].x + objects[i].tx > x/16)
		if(objects[i].y <= y/16 && objects[i].y + objects[i].ty > y/16)
			selNum = i;
	}
	
	if(selNum == 0xFFFFFFFF)
	{
	}
	else
	{
		objects[selNum].selected = true;
	}
}


uint lastx, lasty;
bool resizeLeft = false;
bool resizeTop = false;

void editorTouchDown(uint x, uint y)
{
	touchx = x;
	touchy = y;
	
	if(editAction != EDITACTION_SCROLL)
	{
		doSelection(x + levelx, y + levely);
		repaintScreen();
	}
	lastx = x;
	lasty = y;
	if(editMode == EDITMODE_OBJECTS)
	{
		lastx /= 16;
		lasty /= 16;
	}
}


void editorTouchMoved(uint x, uint y)
{
	if(editAction == EDITACTION_SCROLL)
	{
		
		levelx -= x - touchx;
		levely -= y - touchy;

		if(levelx<0) levelx = 0;
		if(levely<0) levely = 0;

		lastlx = levelx;
		lastly = levely;

		bgSetScroll(2, levelx%512, levely%512);
		bgSetScroll(3, levelx%512, levely%512);

		bgUpdate();
		touchx = x;
		touchy = y;
	}
	else
	{
		
		if(editMode == EDITMODE_OBJECTS)
		{
			x /= 16;
			y /= 16;
		}
		if(x == lastx && y == lasty)
			return;
			
		for(uint i = 0; i < objects.size(); i++)
			if(objects[i].selected)
			{
				objects[i].x += x - lastx;
				objects[i].y += y - lasty;
			}
			
		lastx = x;
		lasty = y;
		
	}
	
	
	repaintScreen();
}

void saveEditor()
{
	
}
