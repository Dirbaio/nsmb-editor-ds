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


#include "editor.h"

uint editMode = EDITMODE_OBJECTS;
uint editAction = EDITACTION_SCROLL;

int levelx = 0;
int levely = 0;

bool multiSelecting = false;
int multiSelectX, multiSelectY;
int multiSelectXb, multiSelectYb;


uint tx, ty; //Position touched now, relative to level
uint ltx, lty; //Position touched on last frame.
uint lax, lay; //Last position relative to screen

bool resizeLeft = false;
bool resizeTop = false;


uint calcLevelFileID(uint levNum)
{
	uint res = 822 + levNum*2;
	if(levNum >= 14) res++;
	
	return res;
}


void loadEditor(uint lev)
{
	unloadLevel();
	iprintf("Loading Editor. Level %d\n", lev);
	int fid = calcLevelFileID(lev);
	loadLevel(fid, fid+1);
	iprintf("Level loaded. ");
	levelx = 0;
	levely = 0;
	
	bgSetScroll(2, levelx%512, levely%512);
	bgSetScroll(3, levelx%512, levely%512);
	renderLevel(levelx/16, levelx/16+16, levely/16, levely/16+12);
	iprintf("Level Rendered.\n");
}

uint touchx, touchy;

void repaintScreen()
{
	renderLevel(levelx/16, levelx/16+16, levely/16, levely/16+12);
}

void unselectAll()
{
	for(list<LevelObject>::iterator i = objects.begin(); i != objects.end(); ++i)
		i->selected = false;
	for(list<LevelSprite>::iterator i = sprites.begin(); i != sprites.end(); ++i)
		i->selected = false;
}
bool elementInMultiRect(LevelElement& obj)
{
	int xMin = multiSelectX < multiSelectXb ? multiSelectX : multiSelectXb;
	int xMax = multiSelectX > multiSelectXb ? multiSelectX : multiSelectXb;
	int yMin = multiSelectY < multiSelectYb ? multiSelectY : multiSelectYb;
	int yMax = multiSelectY > multiSelectYb ? multiSelectY : multiSelectYb;
	int mult = 16 / obj.getSizeMultiplier();
	
	if(obj.x > xMax*mult) return false;
	if(obj.y > yMax*mult) return false;
	if(obj.x + obj.tx < xMin*mult) return false;
	if(obj.y + obj.ty < yMin*mult) return false;
	return true;
}

bool elementAtPos(LevelElement& e, int x, int y)
{
	int sizeMult = e.getSizeMultiplier();
	if(e.x <= x/sizeMult && e.x + e.tx > x/sizeMult)
	if(e.y <= y/sizeMult && e.y + e.ty > y/sizeMult)
		return true;
		
	return false;
}

LevelElement* getElementAtPos(int x, int y)
{
	for(list<LevelSprite>::reverse_iterator i = sprites.rbegin(); i!=sprites.rend(); ++i)
		if(elementAtPos(*i, x, y))
			return &(*i);

	for(list<LevelObject>::reverse_iterator i = objects.rbegin(); i!=objects.rend(); ++i)
		if(elementAtPos(*i, x, y))
			return &(*i);
	
	return NULL;
}

void doSelection(uint x, uint y)
{
	LevelElement* elemAtCursor = getElementAtPos(x, y);
	multiSelecting = false;
	
	if(elemAtCursor != NULL)
		if(elemAtCursor->selected)
			return;
	
	unselectAll();
	
	if(elemAtCursor == NULL)
	{
		multiSelecting = true;
		multiSelectX = x / 16;
		multiSelectY = y / 16;
		multiSelectXb = x / 16;
		multiSelectYb = y / 16;
	}
	else
	{
		elemAtCursor->selected = true;
	}
}

void editorTouchDown(uint x, uint y)
{
	tx = x+levelx;
	ty = y+levely;

	if(editAction != EDITACTION_SCROLL)
	{
		doSelection(tx, ty);
		repaintScreen();
	}
	
	ltx = tx;
	lty = ty;
	lax = x;
	lay = y;
}

void doMultiSelection()
{
	unselectAll();
	for(list<LevelSprite>::reverse_iterator i = sprites.rbegin(); i!=sprites.rend(); ++i)
		if(elementInMultiRect(*i))
			i->selected = true;

	for(list<LevelObject>::reverse_iterator i = objects.rbegin(); i!=objects.rend(); ++i)
		if(elementInMultiRect(*i))
			i->selected = true;
}

void doAction(LevelElement& e)
{
	if(!e.selected) return;
	
	int mult = e.getSizeMultiplier();
	int dx = tx / mult;
	dx -= ltx / mult;
	int dy = ty / mult;
	dy -= lty / mult;
	
	if(editAction == EDITACTION_MOVE)
	{
		if(e.x + dx < 0) e.x = 0;
		else e.x += dx;
		
		if(e.y + dy < 0) e.y = 0;
		else e.y += dy;
	}
}

void editorTouchMoved(uint x, uint y)
{
	tx = x+levelx;
	ty = y+levely;
	
	if(editAction == EDITACTION_SCROLL)
	{
		
		levelx += lax;
		levelx -= x;
		levely += lay;
		levely -= y;

		if(levelx<0) levelx = 0;
		if(levely<0) levely = 0;

		bgSetScroll(2, levelx%512, levely%512);
		bgSetScroll(3, levelx%512, levely%512);

		bgUpdate();
	}
	else if(multiSelecting)
	{
		multiSelectXb = tx/16;
		multiSelectYb = ty/16;
		doMultiSelection();
	}
	else
	{
		for(list<LevelObject>::iterator i = objects.begin(); i != objects.end(); ++i)
			doAction(*i);
		for(list<LevelSprite>::iterator i = sprites.begin(); i != sprites.end(); ++i)
			doAction(*i);
	}
	
	ltx = tx;
	lty = ty;	
	lax = x;
	lay = y;
	repaintScreen();
}

void editorRenderSprites()
{
	renderLevelSprites(levelx/16, levelx/16+16, levely/16, levely/16+12, levelx, levely);
}

void saveEditor()
{
	
}
