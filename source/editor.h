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


#ifndef _EDITOR_H
#define _EDITOR_H

#include<nds.h>

#include <string>
#include "level.h"


using namespace std;


#define EDITACTION_SCROLL (0)
#define EDITACTION_MOVE  (1)
#define EDITACTION_RESIZE (2)
#define EDITACTION_CLONE  (3)

#define EDITMODE_OBJECTS   (0)
#define EDITMODE_ENTRANCES (1)
#define EDITMODE_PATHS     (2)
#define EDITMODE_PROGPATHS (3)
#define EDITMODE_VIEWS     (4)
#define EDITMODE_ZONES     (5)

class LevelEditor;

extern LevelEditor* editor;

class LevelEditor
{
    public:
    u32 editAction;
    Level* l;

    uint touchx, touchy;
    int levelx;
    int levely;

    bool multiSelecting ;
    int multiSelectX, multiSelectY;
    int multiSelectXb, multiSelectYb;


    uint tx, ty; //Position touched now, relative to level
    uint ltx, lty; //Position touched on last frame.
    uint lax, lay; //Last position relative to screen
    int selMult; //Global sizeMultiplier of all the selected stuff

    bool resizeLeft;
    bool resizeTop;


    LevelEditor(string level);
    ~LevelEditor();
    void repaintScreen();
    void updateScroll();
    void touchDown(u32 x, u32 y);
    void touchMoved(u32 x, u32 y);
    void save();
    void renderSprites();
    void deleteObjects();
    void showPalette();
    void showProperties();
    
    void unselectAll();
    bool elementInMultiRect(LevelElement& obj);
    bool elementAtPos(LevelElement& e, int x, int y);
    LevelElement* getElementAtPos(int x, int y);
    void doSelection(uint x, uint y);
    void doMultiSelection();
    void doAction(LevelElement& e);
    int countSelectedObjects();
    template<class T>
    void doMultiSelectionInList(list<T>& lst);

};

#endif


