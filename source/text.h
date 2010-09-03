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


#ifndef _TEXT_H
#define _TEXT_H

#include<nds.h>
#include "text.h"
#include <string>

using namespace std;


void renderHexChar(int x, int y, int col, u8 c);
void renderChar(int x, int y, int col, char c);
void renderText(int x, int y, u32 max, int col, string& s);
void renderText(int x, int y, u32 max, int col, const char* s);
void textInit();
void textShow();
void textClearTransparent();
void textClearOpaque();
void textScroll(int y);


#endif
