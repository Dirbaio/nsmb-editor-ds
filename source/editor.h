#include<nds.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "level.h"
#include "levelrendering.h"

using namespace std;

#ifndef _EDITOR_H
#define _EDITOR_H

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

void loadEditor(uint level);
void editorTouchDown(uint x, uint y);
void editorTouchMoved(uint x, uint y);
void saveEditor();
void editorRenderSprites();

#endif
