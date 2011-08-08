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
#include "text.h"
#include "textlist.h"
#include "spritedataeditor.h"
#include "lists.h"
#include "selectablelist.h"
#include "level.h"
#include "levelrendering.h"
#include "ui.h"
#include "objectlist.h"

int speed;



LevelEditor* editor;


LevelEditor::LevelEditor(string lev)
{
	iprintf("Loading Editor. Level %s\n", lev.c_str());
	l = new Level(lev);
	iprintf("Level loaded. ");
	levelx = 0;
	levely = 0;
	editAction = EDITACTION_SCROLL;
	multiSelecting = false;
	bgSetScroll(2, levelx%512, levely%512);
	bgSetScroll(3, levelx%512, levely%512);
	repaintScreen();
	updateScroll();
	speed = 2;
}

LevelEditor::~LevelEditor()
{
    delete l;
}

void LevelEditor::updateScroll()
{
	bgSetScroll(2, levelx%512, levely%512);
	bgSetScroll(3, levelx%512, levely%512);

	bgUpdate();
}
void LevelEditor::repaintScreen()
{
	renderLevel(l, levelx, levelx+256, levely, levely+192);
}

void LevelEditor::unselectAll()
{
	for(list<LevelObject>::iterator i = l->objects.begin(); i != l->objects.end(); ++i)
		i->selected = false;
	for(list<LevelSprite>::iterator i = l->sprites.begin(); i != l->sprites.end(); ++i)
		i->selected = false;
	for(list<LevelEntrance>::iterator i = l->entrances.begin(); i != l->entrances.end(); ++i)
		i->selected = false;
}

bool LevelEditor::elementInMultiRect(LevelElement& obj)
{
	int xMin = multiSelectX < multiSelectXb ? multiSelectX : multiSelectXb;
	int xMax = multiSelectX > multiSelectXb ? multiSelectX : multiSelectXb;
	int yMin = multiSelectY < multiSelectYb ? multiSelectY : multiSelectYb;
	int yMax = multiSelectY > multiSelectYb ? multiSelectY : multiSelectYb;
	int mult = 16 / obj.getSizeMultiplier();
	
	if(obj.x > xMax*mult) return false;
	if(obj.y > yMax*mult) return false;
	if(obj.x + obj.tx <= xMin*mult) return false;
	if(obj.y + obj.ty <= yMin*mult) return false;
	return true;
}

bool LevelEditor::elementAtPos(LevelElement& e, int x, int y)
{
	int sizeMult = e.getSizeMultiplier();
	if(e.x <= x/sizeMult && e.x + e.tx > x/sizeMult)
	if(e.y <= y/sizeMult && e.y + e.ty > y/sizeMult)
		return true;
		
	return false;
}

LevelElement* LevelEditor::getElementAtPos(int x, int y)
{
	for(list<LevelEntrance>::reverse_iterator i = l->entrances.rbegin(); i!=l->entrances.rend(); ++i)
		if(elementAtPos(*i, x, y))
			return &(*i);

	for(list<LevelSprite>::reverse_iterator i = l->sprites.rbegin(); i!=l->sprites.rend(); ++i)
		if(elementAtPos(*i, x, y))
			return &(*i);

	for(list<LevelObject>::reverse_iterator i = l->objects.rbegin(); i!=l->objects.rend(); ++i)
		if(elementAtPos(*i, x, y))
			return &(*i);
	
	return NULL;
}

void LevelEditor::doSelection(uint x, uint y)
{
	LevelElement* elemAtCursor = getElementAtPos(x, y);
	multiSelecting = false;
	
	if(elemAtCursor != NULL)
		if(elemAtCursor->selected)
			return;
	
	unselectAll();
	
    textClearTransparent();
    textScroll(0);
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
		selMult = elemAtCursor->getSizeMultiplier();
		
        for(list<LevelSprite>::iterator it = l->sprites.begin(); it != l->sprites.end(); it++)
            if(it->selected)
            {
                renderText(0, 2, 32, 0, spriteList[it->spriteNum]);
            }
            
	}
}

void LevelEditor::touchDown(uint x, uint y)
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

template<class T>
void LevelEditor::doMultiSelectionInList(list<T>& lst)
{
	for(typename list<T>::reverse_iterator i = lst.rbegin(); i!=lst.rend(); ++i)
		if(elementInMultiRect(*i))
		{
			i->selected = true;
			int mult = i->getSizeMultiplier();
			if(mult > selMult)
			    selMult = mult;
	    }
}

void LevelEditor::doMultiSelection()
{
	unselectAll();
	selMult = 1;
	doMultiSelectionInList(l->objects);
	doMultiSelectionInList(l->sprites);
	doMultiSelectionInList(l->entrances);
}


void LevelEditor::doAction(LevelElement& e)
{
	if(!e.selected) return;
	
	int dx = tx / selMult;
	dx -= ltx / selMult;
	int dy = ty / selMult;
	dy -= lty / selMult;
	
	int mult = e.getSizeMultiplier();
	
	if(editAction == EDITACTION_MOVE)
	{
	    e.x += dx * selMult / mult;
	    if(e.x < 0) e.x = 0;

	    e.y += dy * selMult / mult;
		if(e.y < 0) e.y = 0;
	}
    
    if(editAction == EDITACTION_RESIZE)
    {
        if(e.isResizable())
        {
            if(e.tx + dx <= 0)
                e.tx = 1;
            else
                e.tx += dx;
                
            if(e.ty + dy <= 0)
                e.ty = 1;
            else
                e.ty += dy;
        }
    }
}

template<class T>
void cloneSelected(list<T>& lst)
{
    int ocount = lst.size();
    typename list<T>::iterator it = lst.begin();
    for(int i = 0; i < ocount; i++)
    {
        if(it->selected)
        {
            lst.push_back(*it);
            it->selected = false;
        }
        it++;
    }
}

void LevelEditor::touchMoved(uint x, uint y)
{

	tx = x+levelx;
	ty = y+levely;
	textClearTransparent();
	textScroll(0);
	if(editAction == EDITACTION_SCROLL)
	{
		levelx += lax*speed;
		levelx -= x*speed;
		levely += lay*speed;
		levely -= y*speed;
		if(levelx<0) levelx = 0;
		if(levely<0) levely = 0;
        updateScroll();
	}
	else if(multiSelecting)
	{
		multiSelectXb = tx/16;
		multiSelectYb = ty/16;
		doMultiSelection();
	}
	else if(editAction == EDITACTION_CLONE)
    {
        cloneSelected(l->objects);
        cloneSelected(l->sprites);
        cloneSelected(l->entrances);
        
        editAction = EDITACTION_MOVE;
        renderUI();
    }
    else
	{
		for(list<LevelObject>::iterator i = l->objects.begin(); i != l->objects.end(); ++i)
			doAction(*i);
		for(list<LevelSprite>::iterator i = l->sprites.begin(); i != l->sprites.end(); ++i)
			doAction(*i);
		for(list<LevelEntrance>::iterator i = l->entrances.begin(); i != l->entrances.end(); ++i)
			doAction(*i);
	}
	
	ltx = tx;
	lty = ty;	
	lax = x;
	lay = y;
	repaintScreen();
}

void LevelEditor::renderSprites()
{
	renderLevelSprites(l, levelx, levelx+256, levely, levely+192, levelx, levely);
}

void LevelEditor::save()
{
	l->save();
}

template<class T>
void deleteSelected(list<T>& lst)
{
    
    typename list<T>::iterator i = lst.begin();
    
    while(i != lst.end())
    {
        if(i->selected)
        {
            typename list<T>::iterator ni = i;
            ni++;
            lst.erase(i);
            i = ni;
        }
        else
            i++;
    }
}

void LevelEditor::deleteObjects()
{
    deleteSelected(l->objects);
    deleteSelected(l->sprites);
    deleteSelected(l->entrances);
    
	repaintScreen();
}


int LevelEditor::countSelectedObjects()
{
    int r = 0;
    for(list<LevelObject>::iterator i = l->objects.begin(); i != l->objects.end(); i++)
        if(i->selected)
            r++;
    for(list<LevelSprite>::iterator i = l->sprites.begin(); i != l->sprites.end(); i++)
        if(i->selected)
            r++;
            
    return r;

}

void LevelEditor::showPalette()
{
    if(countSelectedObjects() != 1) return;
    for(list<LevelObject>::iterator i = l->objects.begin(); i != l->objects.end(); i++)
        if(i->selected)
        {
			showObjectList(i->tilesetNum, i->objNum);
			repaintScreen();
			updateScroll();
            return;
        }
        
    for(list<LevelSprite>::iterator i = l->sprites.begin(); i != l->sprites.end(); i++)
        if(i->selected)
        {
            i->spriteNum = showTextList(spriteList, i->spriteNum);
			repaintScreen();
			updateScroll();
            return;
        }
}

void LevelEditor::showProperties()
{
    if(countSelectedObjects() != 1) return;
    for(list<LevelSprite>::iterator i = l->sprites.begin(); i != l->sprites.end(); i++)
        if(i->selected)
        {
            editSpriteData(i->spriteData, "Editing Sprite Data", spriteList[i->spriteNum],i->spriteNum);
            return;
        }
    
}

void LevelEditor::increaseSpeed()
{
    if (speed < 8)
	{
    speed = speed + 1;
	}
	else
	{
		textClearTransparent();
		textScroll(0);
		renderText(0, 2, 32, 0,"The camera is too fast!");
		//iprintf("The camera is too fast!\n");
	}
	
}

void LevelEditor::decreaseSpeed()
{
    if (speed > 2)
	{
    speed = speed - 1;
	}
	else
	{
		textClearTransparent();
		textScroll(0);
		renderText(0, 2, 32, 0,"The camera is too slow!");
	//iprintf("The camers is too slow!\n");
    }
	
}
void LevelEditor::showAbout()
{
    consoleClear();
    iprintf("*********************\n");
	iprintf("NSMBe for DS. Early developement version by Dirbaio\n");
	iprintf("Press X for User Guide\n");	
	iprintf("*********************\n");
	
}

