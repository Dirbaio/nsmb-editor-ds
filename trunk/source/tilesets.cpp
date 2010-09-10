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
#include "rom.h"
#include "tilegfx.h"
#include "level.h"

const uint16 map16Overlays [256][4] = 
{

{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x03BC, 0x03BD, 0x03BE, 0x03BF},
{0x03C0, 0x03C1, 0x03C2, 0x03C3},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0390, 0x0391, 0x0392, 0x0393},
{0x0394, 0x0395, 0x0396, 0x0397},
{0x0398, 0x0399, 0x039A, 0x039B},
{0x039C, 0x039D, 0x039E, 0x039F},
{0x03A0, 0x03A1, 0x03A2, 0x03A3},
{0x03A4, 0x03A5, 0x03A6, 0x03A7},
{0x03A8, 0x03A9, 0x03AA, 0x03AB},
{0x03AC, 0x03AD, 0x03AE, 0x03AF},
{0x03B0, 0x03B1, 0x03B2, 0x03B3},
{0x03B4, 0x03B5, 0x03B6, 0x03B7},
{0x03B4, 0x03B5, 0x03B6, 0x03B7},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x03B8, 0x03B9, 0x03BA, 0x03BB},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0390, 0x0391, 0x0392, 0x0393},
{0x0394, 0x0395, 0x0396, 0x0397},
{0x039C, 0x039D, 0x039E, 0x039F},
{0x03AC, 0x03AD, 0x03AE, 0x03AF},
{0x03C4, 0x03C5, 0x03C6, 0x03C7},
{0x03B0, 0x03B1, 0x03B2, 0x03B3},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0390, 0x0391, 0x0392, 0x0393},
{0x0394, 0x0395, 0x0396, 0x0397},
{0x039C, 0x039D, 0x039E, 0x039F},
{0x03AC, 0x03AD, 0x03AE, 0x03AF},
{0x03B0, 0x03B1, 0x03B2, 0x03B3},
{0x03B4, 0x03B5, 0x03B6, 0x03B7},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
};

const uint16 map16ExtraData[256][4] = 
{

{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x43CC, 0x43CD, 0x43CE, 0x43CF},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4B4E, 0x4B4F, 0x4B4C, 0x4B4D},
{0x4B52, 0x4B53, 0x4B50, 0x4B51},
{0x4300, 0x4301, 0x4302, 0x4303},
{0x4701, 0x4700, 0x4703, 0x4702},
{0x43C8, 0x43C9, 0x43C8, 0x43C9},
{0x4308, 0x4308, 0x430A, 0x430A},
{0x430C, 0x430D, 0x430A, 0x430A},
{0x4310, 0x4311, 0x430A, 0x430A},
{0x4314, 0x43C9, 0x4316, 0x43C9},
{0x43CA, 0x4315, 0x43CA, 0x4317},
{0x43C8, 0x43C9, 0x4B2C, 0x4B2D},
{0x43CA, 0x43CB, 0x4B2E, 0x4B2F},
{0x431C, 0x431D, 0x431E, 0x431F},
{0x4320, 0x4321, 0x4322, 0x4323},
{0x4342, 0x4343, 0x4342, 0x4343},
{0x4344, 0x4344, 0x4346, 0x4346},
{0x434C, 0x434D, 0x434E, 0x434F},
{0x4350, 0x4351, 0x4352, 0x4353},
{0x4304, 0x4305, 0x4306, 0x4307},
{0x4705, 0x4704, 0x4707, 0x4706},
{0x43CA, 0x43CB, 0x43CA, 0x43CB},
{0x4309, 0x4309, 0x430B, 0x430B},
{0x4309, 0x4309, 0x430E, 0x430F},
{0x4309, 0x4309, 0x4312, 0x4313},
{0x4318, 0x43C9, 0x431A, 0x43C9},
{0x43CA, 0x4319, 0x43CA, 0x431B},
{0x432C, 0x432D, 0x43C8, 0x43C9},
{0x432E, 0x432F, 0x43CA, 0x43CB},
{0x4324, 0x4325, 0x4326, 0x4327},
{0x4328, 0x4329, 0x432A, 0x432B},
{0x4348, 0x4349, 0x4342, 0x4343},
{0x4342, 0x4343, 0x434A, 0x434B},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4B42, 0x4B43, 0x4B40, 0x4B41},
{0x4340, 0x4341, 0x4342, 0x4343},
{0x433C, 0x433C, 0x433D, 0x433D},
{0x433D, 0x433D, 0x433D, 0x433D},
{0x4330, 0x4331, 0x4332, 0x4333},
{0x4348, 0x4349, 0x4342, 0x4343},
{0x4342, 0x4343, 0x434A, 0x434B},
{0x4354, 0x4344, 0x4356, 0x4346},
{0x4344, 0x4355, 0x4346, 0x4357},
{0x4334, 0x4335, 0x4336, 0x4337},
{0x4338, 0x4339, 0x433A, 0x433B},
{0x4342, 0x4343, 0x4342, 0x4343},
{0x4344, 0x4344, 0x4346, 0x4346},
{0x43C8, 0x43C9, 0x43C8, 0x43C9},
{0x43CA, 0x43CB, 0x43CA, 0x43CB},
{0x4354, 0x4344, 0x4356, 0x4346},
{0x4344, 0x4355, 0x4346, 0x4357},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4300, 0x4301, 0x4302, 0x4303},
{0x4308, 0x4308, 0x430A, 0x430A},
{0x4701, 0x4700, 0x4703, 0x4702},
{0x4B4E, 0x4B4F, 0x4B4C, 0x4B4D},
{0x4B52, 0x4B53, 0x4B50, 0x4B51},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x43D8, 0x43D9, 0x43DA, 0x43DB},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4304, 0x4305, 0x4306, 0x4307},
{0x4309, 0x4309, 0x430B, 0x430B},
{0x4705, 0x4704, 0x4707, 0x4706},
{0x434C, 0x434D, 0x434E, 0x434F},
{0x4350, 0x4351, 0x4352, 0x4353},
{0x4384, 0x4385, 0x4386, 0x4387},
{0x438C, 0x438C, 0x438E, 0x438E},
{0x4785, 0x4784, 0x4787, 0x4786},
{0x436C, 0x436D, 0x436E, 0x436F},
{0x4370, 0x4370, 0x4372, 0x4372},
{0x476D, 0x476C, 0x476F, 0x476E},
{0x4360, 0x4361, 0x4B62, 0x4B63},
{0x4364, 0x4365, 0x4B66, 0x4B67},
{0x4358, 0x4359, 0x435A, 0x435B},
{0x435C, 0x435D, 0x435E, 0x435F},
{0x437C, 0x437D, 0x4B7E, 0x4B7F},
{0x4380, 0x4381, 0x4B82, 0x4B83},
{0x4374, 0x4375, 0x4376, 0x4377},
{0x4378, 0x4379, 0x437A, 0x437B},
{0x437C, 0x437D, 0x437C, 0x437D},
{0x4380, 0x4381, 0x4380, 0x4381},
{0x4388, 0x4389, 0x438A, 0x438B},
{0x438D, 0x438D, 0x438F, 0x438F},
{0x4789, 0x4788, 0x478B, 0x478A},
{0x4368, 0x4369, 0x436A, 0x436B},
{0x4371, 0x4371, 0x4373, 0x4373},
{0x4769, 0x4768, 0x476B, 0x476A},
{0x4362, 0x4363, 0x4360, 0x4361},
{0x4366, 0x4367, 0x4364, 0x4365},
{0x4B5A, 0x4B5B, 0x4B58, 0x4B59},
{0x4B5E, 0x4B5F, 0x4B5C, 0x4B5D},
{0x437E, 0x437F, 0x437C, 0x437D},
{0x4382, 0x4383, 0x4380, 0x4381},
{0x4B76, 0x4B77, 0x4B74, 0x4B75},
{0x4B7A, 0x4B7B, 0x4B78, 0x4B79},
{0x4360, 0x4361, 0x4360, 0x4361},
{0x4364, 0x4365, 0x4364, 0x4365},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4358, 0x4359, 0x435A, 0x435B},
{0x435C, 0x435D, 0x435E, 0x435F},
{0x4360, 0x4361, 0x4360, 0x4361},
{0x436C, 0x436D, 0x436E, 0x436F},
{0x4370, 0x4370, 0x4372, 0x4372},
{0x476D, 0x476C, 0x476F, 0x476E},
{0x4374, 0x4375, 0x4376, 0x4377},
{0x4378, 0x4379, 0x437A, 0x437B},
{0x4380, 0x4381, 0x4380, 0x4381},
{0x4384, 0x4385, 0x4386, 0x4387},
{0x438C, 0x438C, 0x438E, 0x438E},
{0x4785, 0x4784, 0x4787, 0x4786},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000},
{0x4B5A, 0x4B5B, 0x4B58, 0x4B59},
{0x4B5E, 0x4B5F, 0x4B5C, 0x4B5D},
{0x4364, 0x4365, 0x4364, 0x4365},
{0x4368, 0x4369, 0x436A, 0x436B},
{0x4371, 0x4371, 0x4373, 0x4373},
{0x4769, 0x4768, 0x476B, 0x476A},
{0x4B76, 0x4B77, 0x4B74, 0x4B75},
{0x4B7A, 0x4B7B, 0x4B78, 0x4B79},
{0x437C, 0x437D, 0x437C, 0x437D},
{0x4388, 0x4389, 0x438A, 0x438B},
{0x438D, 0x438D, 0x438F, 0x438F},
{0x4789, 0x4788, 0x478B, 0x478A},

};

objPointer *objectIndex[3];
int objectCount[3];
int objectOffs[3];
uint8 *objectDefinitions[3];
uint16 map16Data [TOTAL_MAP16][4];

void createOverrides()
{
	for(int i = 0; i < 256; i++)
	{
		if(map16ExtraData[i][0] != 0)
		{
			map16Data[i][0] = map16ExtraData[i][0];
			map16Data[i][1] = map16ExtraData[i][1];
			map16Data[i][2] = map16ExtraData[i][2];
			map16Data[i][3] = map16ExtraData[i][3];
		}
	}
}

void loadTilesets(int tileset)
{

    u16* bg2Ptr = bgGetGfxPtr(2);
	//Load graphics
    
    fs->getFileById(jyoytu_ncg_fileID[region])
        ->loadCompressedContentsInto(bg2Ptr);
    fs->getFileById(getFileIDFromTable(Table_TS_NCG, tileset))
        ->loadCompressedContentsInto(bg2Ptr+0x1800);
    fs->getFileById(subnohara_ncg_fileID[region])
        ->loadCompressedContentsInto(bg2Ptr+0x5000);

	cpuCopy16(&tilegfxTiles, bgGetGfxPtr(2)+0x6000, tilegfxTilesLen);

	//Load the Palettes
	vramSetBankE(VRAM_E_LCD);
    fs->getFileById(jyoytu_ncl_fileIDs[0][region])
        ->loadCompressedContentsInto(VRAM_E_EXT_PALETTE[2]);
    fs->getFileById(getFileIDFromTable(Table_TS_NCL, tileset))
        ->loadCompressedContentsInto(VRAM_E_EXT_PALETTE[2][2]);
    fs->getFileById(subnohara_ncl_fileID[region])
        ->loadCompressedContentsInto(VRAM_E_EXT_PALETTE[2][6]);
        
	cpuCopy16(&tilegfxPal, VRAM_E_EXT_PALETTE[2][4], tilegfxPalLen);
	cpuCopy16(&tilegfxPal, VRAM_E_EXT_PALETTE[3][0], tilegfxPalLen);
	shadeExtPal(2);
	shadeExtPal(3);
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	
	//Load the MAP16!
    fs->getFileById(jyoytu_pnl_fileID[region])
        ->loadContentsInto(&map16Data);
    fs->getFileById(getFileIDFromTable(Table_TS_PNL, tileset))
        ->loadContentsInto(&map16Data[MAP16_PAGE*1]);
    fs->getFileById(subnohara_pnl_fileID[region])
        ->loadContentsInto(&map16Data[MAP16_PAGE*4]);
	
	//Load the Object Indexes!!!
	NitroFile* indFile;
	
	indFile = fs->getFileById(jyoytu_unt_hd_fileID[region]);
	objectCount[0] = indFile->size / 4 - 1;
	objectIndex[0] = (objPointer*) indFile->getContents();
	
	indFile = fs->getFileById(getFileIDFromTable(Table_TS_UNT_HD, tileset));
	objectCount[1] = indFile->size / 4 - 1;
	objectIndex[1] = (objPointer*) indFile->getContents();
	
	indFile = fs->getFileById(subnohara_unt_hd_fileID[region]);
	objectCount[2] = indFile->size / 4 - 1;
	objectIndex[2] = (objPointer*) indFile->getContents();
	
	objectOffs[0] = 0;
	objectOffs[1] = objectCount[0];
	objectOffs[2] = objectCount[0] + objectCount[1];
	
	iprintf("Tilset sizes: %d %d %d\n", objectCount[0], objectCount[1], objectCount[2]);
	
	//Load the Object Files!!!
	objectDefinitions[0] = 
        fs->getFileById(jyoytu_unt_fileID[region])
            ->getContents();
	objectDefinitions[1] =
        fs->getFileById(getFileIDFromTable(Table_TS_UNT, tileset))
            ->getContents();
	objectDefinitions[2] = 
        fs->getFileById(subnohara_unt_fileID[region])
            ->getContents();
            
	createOverrides();
}

void unloadTilesets()
{
	delete[] objectIndex[0];
	delete[] objectIndex[1];
	delete[] objectIndex[2];
	delete[] objectDefinitions[0];
	delete[] objectDefinitions[1];
	delete[] objectDefinitions[2];
}

void shadeExtPal(uint p)
{

	//Create the "selected" blueish effect
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 256; j++)
		{
			uint16 col = VRAM_E_EXT_PALETTE[p][i][j];
			uint16 r = (col >> 0) & 0x1F;
			uint16 g = (col >> 5) & 0x1F;
			uint16 b = (col >> 10) & 0x1F;
			uint16 a = (col >> 15) & 0x1;
			r = (r + 0)/2;
			g = (g + 0)/2;
			b = (b + 31)/2;
			VRAM_E_EXT_PALETTE[p][i+8][j] = 
				r << 0 |
				g << 5 |
				b << 10 |
				a << 15;
		}
	}
}


