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



#include<nds.h>
#include<fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#ifndef _TILESETS_H
#define _TILESETS_H


//1 page = 256 map16 tiles
//Jyoytu: 1 page (256 tiles)
//Tileset: 3 pages (768 tiles)
//Subnohara: 2 pages (512 tiles)
const int MAP16_PAGE = (256);
const int TOTAL_MAP16 = (MAP16_PAGE * 6);

struct objPointer
{
	uint16 offs;
	uint8 w, h;
};

#define overlayTileOffset 0x6000
extern uint16 map16Data [TOTAL_MAP16][4];
extern const uint16 map16Overlays [256][4];
extern objPointer *objectIndex[3];
extern int objectCount[3];
extern int objectOffs[3];
extern uint8 *objectDefinitions[3];

void loadTilesets(int tileset);
void unloadTilesets();

void shadeExtPal(uint p);
#endif
