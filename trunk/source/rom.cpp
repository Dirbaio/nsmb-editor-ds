
#include "rom.h"
#include <nds.h>

FILE* rom;
sNDSHeader romHeader;
uint8 *overlay0;
RomRegion region = US;

uint readUInt(FILE* file)
{
	return getc(file) | getc(file)<<8 | getc(file)<<16 | getc(file)<<24;
}

uint readUShort(FILE* file)
{
	return getc(file) | getc(file)<<8;
}



void SubS(uint& dest, uint v1, uint v2, bool& N, bool& V) {
	dest = v1 - v2;
	N = (dest & 0x80000000) != 0;
	V = (((v1 & 0x80000000) != 0) && ((v2 & 0x80000000) == 0) && ((dest & 0x80000000) == 0)) 
	 || (((v1 & 0x80000000) == 0) && ((v2 & 0x80000000) != 0) && ((dest & 0x80000000) != 0));
}

uint8* decompressOverlay(uint8* sourcedata, uint sourcedataLength)
{
	uint DataVar1, DataVar2;
	//uint last 8-5 bytes
	DataVar1 = (uint)(sourcedata[sourcedataLength - 8] | (sourcedata[sourcedataLength - 7] << 8) | (sourcedata[sourcedataLength - 6] << 16) | (sourcedata[sourcedataLength - 5] << 24));
	//uint last 4 bytes
	DataVar2 = (uint)(sourcedata[sourcedataLength - 4] | (sourcedata[sourcedataLength - 3] << 8) | (sourcedata[sourcedataLength - 2] << 16) | (sourcedata[sourcedataLength - 1] << 24));

	uint8* memory = (uint8*) malloc(sourcedataLength + DataVar2);
	dmaCopySafe(sourcedata, memory, sourcedataLength);
	
	uint r0, r1, r2, r3, r5, r6, r7, r12;
	bool N, V;
	r0 = (uint)sourcedataLength;

	r1 = DataVar1;
	r2 = DataVar2;
	r2 = r0 + r2; //length + datavar2 -> decompressed length
	r3 = r0 - (r1 >> 0x18); //delete the latest 3 bits??
	r1 &= 0xFFFFFF; //save the latest 3 bits
	r1 = r0 - r1;
a958:
	if (r3 <= r1) { //if r1 is 0 they will be equal
		goto a9B8; //return the memory buffer
	}
	r3 -= 1;
	r5 = memory[r3];
	r6 = 8;
a968:
	SubS(r6, r6, 1, N, V);
	if (N != V) {
		goto a958;
	}
	if ((r5 & 0x80) != 0) {
		goto a984;
	}
	r3 -= 1;
	r0 = memory[r3];
	r2 -= 1;
	memory[r2] = (uint8)r0;
	goto a9AC;
a984:
	r3 -= 1;
	r12 = memory[r3];
	r3 -= 1;
	r7 = memory[r3];
	r7 |= (r12 << 8);
	r7 &= 0xFFF;
	r7 += 2;
	r12 += 0x20;
a99C:
	r0 = memory[r2 + r7];
	r2 -= 1;
	memory[r2] = (uint8)r0;
	SubS(r12, r12, 0x10, N, V);
	if (N == V) {
		goto a99C;
	}
a9AC:
	r5 <<= 1;
	if (r3 > r1) {
		goto a968;
	}
a9B8:
	return memory;
}

void loadROM()
{
	iprintf(".");
	rom = fopen("nsmb.nds", "rb");
	iprintf(".");
	loadHeader();
	iprintf(".");
	uint8* ov = loadFileFromROM(0);
	iprintf(".");
	overlay0 = decompressOverlay(ov, getFileSizeFromROM(0));
	iprintf(".");
}

void closeROM()
{
	free(overlay0);
	fclose(rom);
}

/** FILE LOADING  **/

void loadHeader()
{
	loadDataInto(0, sizeof(romHeader), &romHeader);
}

uint8_t* loadData(uint offs, uint size)
{
	uint8_t* buffer = (uint8_t*)malloc(size);
	fseek(rom, offs, SEEK_SET);
	fread(buffer, size, sizeof(uint8_t), rom);
	return buffer;
}

void loadDataInto(uint offs, uint size, void* dest)
{
	fseek(rom, offs, SEEK_SET);
	fread(dest, size, sizeof(uint8_t), rom);
}

void loadCompressedData(uint offs, uint size, void* dest)
{
	uint8_t* ptr = loadData(offs, size);
	uint32 decsize = ptr[1] | ptr[2]<<8 | ptr[3]<<16;
	uint8_t* buffer = (uint8_t*)malloc(decsize);
	decompress(ptr, buffer, LZ77);
	dmaCopySafe(buffer, dest, decsize);
	free(ptr);
	free(buffer);
}

uint8_t* loadFileFromROM(uint fileID)
{
	fseek(rom, romHeader.fatOffset+fileID*8, SEEK_SET);
	uint32 offs = readUInt(rom);
	uint32 end =  readUInt(rom);
	iprintf(".");
	return loadData(offs, end-offs);
}

void loadFileFromROMInto(uint fileID, void* dest)
{
	fseek(rom, romHeader.fatOffset+fileID*8, SEEK_SET);
	uint32 offs = readUInt(rom);
	uint32 end =  readUInt(rom);
	iprintf(".");
	loadDataInto(offs, end-offs, dest);
}


uint32 getFileSizeFromROM(uint fileID)
{
	fseek(rom, romHeader.fatOffset+fileID*8, SEEK_SET);
	uint32 offs = readUInt(rom);
	uint32 end =  readUInt(rom);
	return end-offs;
}

void loadCompressedFileFromROM(uint fileID, void* dest)
{
	fseek(rom, romHeader.fatOffset+fileID*8, SEEK_SET);
	int offs = readUInt(rom);
	int end =  readUInt(rom);
	iprintf(".");
	loadCompressedData(offs, end-offs, dest);
}

int getFileIDFromTable(Overlay0OffsetName table, uint entry)
{
	uint off = overlay0Offsets[table][region]+entry*4;
	return (overlay0[off] | overlay0[off+1]<<8) + overlay0Offsets[0][region];
}


bool dmaCopySafe(const void *src, void *dst, u32 size)
{
    u32 srca= (u32)src, dsta= (u32)dst;

    // Check TCMs and BIOS (0x01000000, 0x0B000000, 0xFFFF0000).
    //# NOTE: probably incomplete checks.
    if((srca>>24)==0x01 || (srca>>24)==0x0B || (srca>>24)==0xFF)
        return false;
    if((dsta>>24)==0x01 || (dsta>>24)==0x0B || (dsta>>24)==0xFF)
        return false;

    if((srca|dsta) & 1)                 // Fail on byte copy.
        return false;
   
    while(dmaBusy(3)) ;

    if((srca>>24)==0x02)                // Write cache back to memory.
        DC_FlushRange(src, size);

    if((srca|dsta|size) & 3)
        dmaCopyHalfWords(3, src, dst, size);
    else
        dmaCopyWords(3, src, dst, size);

    if((dsta>>24)==0x02)                // Set cache of dst range to 'dirty'
        DC_InvalidateRange(dst, size);

    return true;
}


int overlay0Offsets[17][4] = {
								   {131, 135, 131, 131}, //File Offset (Overlay Count)
								   {0x2F8E4, 0x2F0F8, 0x2ECE4, 0x2EDA4}, //TS_UNT_HD
								   {0x2FA14, 0x2F228, 0x2EE14, 0x2EED4}, //TS_UNT
								   {0x2FB44, 0x2F358, 0x2EF44, 0x2F004}, //TS_CHK
								   {0x2FC74, 0x2F488, 0x2F074, 0x2F134}, //TS_ANIM_NCG
								   {0x30D74, 0x30588, 0x30174, 0x30234}, //BG_NCG
								   {0x30EA4, 0x306B8, 0x302A4, 0x30364}, //TS_NCG
								   {0x30FD4, 0x307E8, 0x303D4, 0x30494}, //FG_NCG
								   {0x31104, 0x30918, 0x30504, 0x305C4}, //FG_NSC
								   {0x31234, 0x30A48, 0x30634, 0x306F4}, //BG_NSC
								   {0x31364, 0x30B78, 0x30764, 0x30824}, //BG_NCL
								   {0x31494, 0x30CA8, 0x30894, 0x30954}, //TS_NCL
								   {0x315C4, 0x30DD8, 0x309C4, 0x30A84}, //FG_NCL
								   {0x316F4, 0x30F08, 0x30AF4, 0x30BB4}, //TS_PNL
								   {0x30CD8, 0x304EC, 0x300D8, 0x30198}, //Jyotyu_NCL
								   {0x2FDA4, 0x2F5B8, 0x2F1A4, 0x2FC74}, //Jyotyu_CHK
								   {0x2C930, 0x2BDF0, 0x2BD30, 0x2BDF0}, //Modifiers
							   };
							   
int jyoytu_ncg_fileID[4] = {235, 235, 235, 235};

int jyoytu_ncl_fileIDs[4][4] = {
							{408, 235, 235, 235}, // BROWN
							{406, 235, 235, 235}, // BLUE
							{409, 235, 235, 235}, // RED
							{410, 235, 235, 235} // GRAY
					}; 
					
int jyoytu_pnl_fileID[4] = {686, 235, 235, 235};
int jyoytu_unt_fileID[4] = {730, 235, 235, 235};
int jyoytu_unt_hd_fileID[4] = {731, 235, 235, 235};


int subnohara_ncg_fileID[4] = {330, 235, 235, 235};
int subnohara_ncl_fileID[4] = {510, 235, 235, 235};
int subnohara_pnl_fileID[4] = {711, 235, 235, 235};
int subnohara_unt_fileID[4] = {786, 235, 235, 235};
int subnohara_unt_hd_fileID[4] = {787, 235, 235, 235};
