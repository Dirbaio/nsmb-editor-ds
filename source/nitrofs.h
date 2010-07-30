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

#ifndef _NITROFS_H
#define _NITROFS_H

#include<vector>
#include<map>
#include<string>
#include<nds.h>
#include<list>
#include<stdio.h>

using namespace std;



inline uint readUInt(FILE* file)
{
    uint res = getc(file);
    res |= getc(file) << 8;
    res |= getc(file) << 16;
    res |= getc(file) << 24;
	return res;
}

inline uint readUShort(FILE* file)
{
    uint res = getc(file);
    res |= getc(file) << 8;
	return res;
}
inline uint readByte(FILE* file)
{
	return getc(file);
}

inline void writeByte(FILE* file, u8 val)
{   
    putc(val, file);
}

inline void writeUShort(FILE* file, u16 val)
{
    putc((char)val, file);
    val >>= 8;
    putc((char)val, file);
}

inline void writeUInt(FILE* file, u32 val)
{
    putc((char)val, file);
    val >>= 8;
    putc((char)val, file);
    val >>= 8;
    putc((char)val, file);
    val >>= 8;
    putc((char)val, file);
}

class NitroFilesystem;


struct OverlayEntry
{
    u32 ovId;
    u32 loadRamAddr;
    u32 loadRamSize;
    u32 bssSize;
    u32 staticInitStart;
    u32 staticInitEnd;
    u32 fileId;
    u32 reserved;
};

class NitroFile
{
    public:
    
    int begPtrOffs, endPtrOffs;
    NitroFile* allocFile;
    int begOffs, endOffs, size;
    int id;
    bool fixedBeg, fixedEnd, endsize;
    string name;
    NitroFilesystem* parent;

    
    NitroFile(int id, int begOffs, int endOffs, NitroFilesystem* parent, NitroFile* allocFile, bool fixedBeg, bool fixedEnd, bool endsize, string name);
    void loadOffsets();
    void saveOffsets();
    
    u8* getContents();
    void loadContentsInto(void* ptru);
    void loadCompressedContentsInto(void* ptru);
    void replaceContents(void* ptr, int size);
    
    u8 getByteAt(int offs);
    void setByteAt(int offs, u8 val);
    
    u16 getUshortAt(int offs);
    void setUshortAt(int offs, u16 val);
    
    u32 getUintAt(int offs);
    void setUintAt(int offs, u32 val);
    void print();
   
    u8* cachedContents;
    bool cached;
    bool cacheModified;
    void enableCache();
    void flushCache();
    
    
    ~NitroFile();
};

class NitroFilesystem
{   
    private:
    
    map<string, NitroFile*> filesByName;
    map<int, NitroFile*> filesById;
    list<NitroFile*> allFiles;
    const char* romPath;
    
    public:
    FILE* romFile;
    
    NitroFilesystem(const char* fn);
    int findFreeSpace(int len);
    ~NitroFilesystem();
    void addFile(NitroFile* f);
    void flushCache();
    void loadOvTable(NitroFile* f);
    
    NitroFile* getFileByName(string name);
    NitroFile* getFileById(int id);
};

#endif