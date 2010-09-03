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


#include <nds.h>

using namespace std;

#ifndef _OAMUTIL_H
#define _OAMUTIL_H

void loadOAM();
void oamFrame();
void renderSprite(int x, int y, SpriteSize size, int tile);
void renderSpriteEx(int x, int y, int prio, SpriteSize size, int tile, bool hflip, bool vflip);

#endif
