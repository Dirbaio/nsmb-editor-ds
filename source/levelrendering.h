#include "tilesets.h"
#include "level.h"
#include "oamUtil.h"

#include <vector>

using namespace std;

#ifndef _LEVELRENDERING_H
#define _LEVELRENDERING_H

void renderLevel(uint xMins, uint xMaxs, uint yMins, uint yMaxs);
void renderLevelSprites(uint xMins, uint xMaxs, uint yMins, uint yMaxs, uint xCam, uint yCam);

#endif