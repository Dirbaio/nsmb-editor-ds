//#include "editor.hpp"
#include <nds.h>

#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "editor.h"
#include "ui.h"

using namespace std;

#ifndef _CONTROL_H
#define _CONTROL_H
extern uint keysNowHeld;
extern uint keysNowPressed;
extern touchPosition touch;

void checkControls();
#endif 