
#ifndef _TEXT_H
#define _TEXT_H

#include<nds.h>
#include "text.h"
#include <string>

using namespace std;


void renderText(int x, int y, uint max, int col, string& s);
void textInit();
void textShow();
void textClearTransparent();
void textClearOpaque();
void textScroll(int y);


#endif