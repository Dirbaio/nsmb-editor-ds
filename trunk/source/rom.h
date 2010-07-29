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
#include "nitrofs.h"

#ifndef _ROM_H
#define _ROM_H

typedef enum {
	US = 0, EU = 1, JP = 2, KR = 3, CH = 4
} RomRegion;



typedef enum {
	Number_FileOffset = 0,
	Table_TS_UNT_HD = 1,
	Table_TS_UNT = 2,
	Table_TS_CHK = 3,
	Table_TS_ANIM_NCG = 4,
	Table_BG_NCG = 5,
	Table_TS_NCG = 6,
	Table_FG_NCG = 7,
	Table_FG_NSC = 8,
	Table_BG_NSC = 9,
	Table_BG_NCL = 10,
	Table_TS_NCL = 11,
	Table_FG_NCL = 12,
	Table_TS_PNL = 13,
	Table_Jyotyu_NCL = 14,
	File_Jyotyu_CHK = 15,
	File_Modifiers = 16
} Overlay0OffsetName;



extern NitroFilesystem* fs;
extern uint8 *overlay0;
extern RomRegion region;

void loadROM();
void closeROM();

int getFileIDFromTable(Overlay0OffsetName table, uint entry);
void cpuCopy8(const void* src, void* dest, u32 len);
void cpuCopy16(const void* src, void* dest, u32 len);
void cpuCopy32(const void* src, void* dest, u32 len);
uint8* decompressOverlay(uint8* sourcedata, uint sourcedataLength);

extern int overlay0Offsets[17][4];

extern int jyoytu_ncg_fileID[4];
extern int jyoytu_ncl_fileIDs[4][4] ;
extern int jyoytu_pnl_fileID[4];
extern int jyoytu_unt_fileID[4];
extern int jyoytu_unt_hd_fileID[4];


extern int subnohara_ncg_fileID[4];
extern int subnohara_ncl_fileID[4];
extern int subnohara_pnl_fileID[4];
extern int subnohara_unt_fileID[4];
extern int subnohara_unt_hd_fileID[4];


#endif