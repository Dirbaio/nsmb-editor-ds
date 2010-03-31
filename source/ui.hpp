#include "editor.hpp"
#include <nds.h>

using namespace std;

#ifndef _UI_H
#define _UI_H

extern bool uiOn;

void uiShow();
void uiHide();
void uiTouchDown(int x, int y);

#endif