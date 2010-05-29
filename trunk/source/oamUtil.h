#include <nds.h>

using namespace std;

#ifndef _OAMUTIL_H
#define _OAMUTIL_H

void loadOAM();
void oamFrame();
void renderSprite(int x, int y, SpriteSize size, int tilex, int tiley);
void renderSpriteEx(int x, int y, int prio, SpriteSize size, int tilex, int tiley, bool hflip, bool vflip);
void renderRect(int x, int y, int w, int h);

#endif