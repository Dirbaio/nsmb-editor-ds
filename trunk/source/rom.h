
#include<nds.h>
#include<fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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



extern FILE* rom;
extern sNDSHeader romHeader;
extern uint8 *overlay0;
extern RomRegion region;

uint readUInt(FILE* file);
uint readUShort(FILE* file);
void loadROM();
void closeROM();
void loadHeader();
uint8_t* loadData(uint offs, uint size);
void loadDataInto(uint offs, uint size, void* dest);
void loadCompressedData(uint offs, uint size, void* dest);
uint8_t* loadFileFromROM(uint fileID);
void loadFileFromROMInto(uint fileID, void* dest);
uint32 getFileSizeFromROM(uint fileID);
void loadCompressedFileFromROM(uint fileID, void* dest);
int getFileIDFromTable(Overlay0OffsetName table, uint entry);
bool dmaCopySafe(const void *src, void *dst, u32 size);

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