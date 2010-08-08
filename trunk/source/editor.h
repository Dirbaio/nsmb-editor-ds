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

#include "lists.h"
#include "selectableList.h"
#include "level.h"
#include "levelrendering.h"
#include "ui.h"

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

extern uint editMode;
extern uint editAction;

void loadEditor(string level);
void editorTouchDown(uint x, uint y);
void editorTouchMoved(uint x, uint y);
void saveEditor();
void editorRenderSprites();
void editorDeleteObjects();
void editorShowPalette();
void editorShowProperties();
#endif
