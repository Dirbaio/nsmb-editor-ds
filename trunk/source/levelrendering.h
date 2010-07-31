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