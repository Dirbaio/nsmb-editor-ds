
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
extern uint16 map16Overlays [256][4];
extern objPointer *objectIndex[3];
extern uint8 *objectDefinitions[3];

void loadTilesets(int tileset);
void unloadTilesets();

#endif