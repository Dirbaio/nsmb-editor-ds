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

#include "nitrofs.h"
#include<algorithm>


bool operator<(const NitroFile* a, const NitroFile* b)
{
    if(a->begOffs == b->begOffs)
        return a->size < b->size;
    return a.begOffs < b.begOffs;
}



NitroFile::NitroFile(FILE* romFile, int id, int begOffs, int endOffs, NitroFilesystem* parent, bool fixedBeg, bool fixedEnd, bool endsize, string name)
{
    this->fixedBeg = fixedBeg;
    this->fixedEnd = fixedEnd;
    this->romFile = romFile;
    this->parent = parent;
    this->name = name;
    this->id = id;
    
    if(fixedBeg)
        this -> begOffs = begOffs;
    else
        this -> begPtrOffs = begOffs;
        
    if(fixedEnd)
        this -> endOffs = endOffs;
    else
        this -> endPtrOffs = endOffs;

    this->endsize = endsize;
    loadOffsets();
}

void NitroFile::loadOffsets()
{
    if(!fixedBeg)
    {
        fseek(romFile, begPtrOffs, SEEK_SET);
        begOffs = readUInt(romFile);
    }
    if(!fixedEnd)
    {
        fseek(romFile, endPtrOffs, SEEK_SET);
        endOffs = readUInt(romFile);
        
        if(endsize)
        {
            endOffs += begOffs;
        }
    }
    
    size = endOffs-begOffs;
}

void NitroFile::saveOffsets()
{
    if(!fixedBeg)
    {
        fseek(romFile, begPtrOffs, SEEK_SET);
        writeUInt(romFile, begOffs);
    }
    
    if(!fixedEnd)
    {
        fseek(romFile, endPtrOffs, SEEK_SET);
        if(endsize)
            writeUInt(romFile, endOffs-begOffs);
        else
            writeUInt(romFile, endOffs);    
    }
}

u8* NitroFile::getContents()
{
    if(endOffs < begOffs)
    {
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(romFile, begOffs, SEEK_SET);
    
    u8* res = new u8[endOffs-begOffs];
    fread(res, 1, endOffs-begOffs, romFile);
    return res;
}

void NitroFile::loadContentsInto(void* ptru)
{
    u8* ptr = (u8*) ptru;
    if(endOffs < begOffs)
    {
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(romFile, begOffs, SEEK_SET);
    
    fread(ptr, 1, endOffs-begOffs, romFile);
}

	
void NitroFile::loadCompressedContentsInto(void* destu)
{
    u16* dest = (u16*) destu;
    if(endOffs < begOffs)
    {
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(romFile, begOffs, SEEK_SET);
    
    u8* res = new u8[endOffs-begOffs];
    fread(res, 1, endOffs-begOffs, romFile);

	uint32 decsize = res[1] | res[2]<<8 | res[3]<<16;
	uint16* buffer = new uint16[decsize/2];
	decompress(res, buffer, LZ77);
    
    for(int i = 0; i < decsize/2; i++)
        dest[i] = buffer[i];

    delete[] buffer;
    delete[] res;
}

void NitroFile::replaceContents(u8* ptr, int size)
{
    iprintf("Not implemented yet");
}

u8 NitroFile::getByteAt(int offs)
{
    if(cached)
        return cachedContents[offs];

    fseek(romFile, begOffs+offs, SEEK_SET);
    return readByte(romFile);
}
void NitroFile::setByteAt(int offs, u8 val)
{
    if(cached)
    {
        cachedContents[offs] = val;
        cacheModified = true;
        return;
    }
    
    fseek(romFile, begOffs+offs, SEEK_SET);
    writeByte(romFile, val);
}
u16 NitroFile::getUshortAt(int offs)
{
    if(cached)
        return *(u16*)(cachedContents+offs);
    fseek(romFile, begOffs+offs, SEEK_SET);
    return readUShort(romFile);
}
void NitroFile::setUshortAt(int offs, u16 val)
{
    if(cached)
    {
        *(u16*)(cachedContents+offs) = val;
        cacheModified = true;
        return;
    }
    fseek(romFile, begOffs+offs, SEEK_SET);
    writeUShort(romFile, val);
}
u32 NitroFile::getUintAt(int offs)
{
    if(cached)
        return *(u32*)(cachedContents+offs);
    fseek(romFile, begOffs+offs, SEEK_SET);
    return readUInt(romFile);
}
void NitroFile::setUintAt(int offs, u32 val)
{
    if(cached)
    {
        *(u32*)(cachedContents+offs) = val;
        cacheModified = true;
        return;
    }
    fseek(romFile, begOffs+offs, SEEK_SET);
    writeUInt(romFile, val);
}

void NitroFile::print()
{
    iprintf("%d: %x-%x %s\n", id, begOffs, endOffs, name.c_str());
}

void NitroFile::enableCache()
{
    cached = true;
    cacheModified = false;
    cachedContents = getContents();
}

void NitroFile::flushCache()
{
    if(!cached) return;
    if(!cacheModified) return;
    
    replaceContents(cachedContents, endOffs-begOffs);
}

NitroFile::~NitroFile()
{
    flushCache();
    if(cached)
        delete[] cachedContents;
}

void NitroFilesystem::addFile(NitroFile *f)
{
    filesByName[f->name] = f;
    filesById[f->id] = f;
    allFiles.push_back(f);
}

void NitroFilesystem::loadOvTable(NitroFile* f)
{
    NitroFile* fatFile = filesByName["fat.bin"];
    int fatOffset = fatFile->begOffs;
    
    u32 size = f->size;
    size /= sizeof(OverlayEntry);
    
    OverlayEntry* ovs = (OverlayEntry*) f->getContents();
    
    for(int i = 0; i < size; i++)
    {
        u32 id = ovs[i].fileId;
  //      iprintf("%d ", id);
        NitroFile* tf = new NitroFile(romFile, id, fatOffset+id*8, 
        fatOffset+id*8+4, this, false, false, false, "overlayfile");
        addFile(tf);
    }
    
    delete[] ovs;
//    iprintf("\n");
}

NitroFilesystem::NitroFilesystem(const char* fn)
{
	iprintf("Opening file %s\n", fn);
    romFile = fopen(fn, "r+b");
    if(romFile == NULL)
    {
        iprintf("ERROR: Romfile = null. Halting!\n");
        while(1);
    }
    
    iprintf("Loading NitroFS\n");
    addFile(new NitroFile(romFile, -1, 0, 0x200, this, true, true, false, "header.bin"));
    addFile(new NitroFile(romFile, -1, 0x20, 0x2C, this, false, false, true, "arm9.bin"));
    addFile(new NitroFile(romFile, -1, 0x30, 0x3C, this, false, false, true, "arm7.bin"));
    addFile(new NitroFile(romFile, -1, 0x40, 0x44, this, false, false, true, "fnt.bin"));
    addFile(new NitroFile(romFile, -1, 0x48, 0x4C, this, false, false, true, "fat.bin"));
    addFile(new NitroFile(romFile, -1, 0x50, 0x54, this, false, false, true, "arm9ovt.bin"));
    addFile(new NitroFile(romFile, -1, 0x58, 0x5C, this, false, false, true, "arm7ovt.bin"));
    addFile(new NitroFile(romFile, -1, 0x58, 0x840, this, false, true, true, "banner.bin"));
    
    NitroFile* fatFile = filesByName["fat.bin"];
    fatFile->enableCache();
    NitroFile* fntFile = filesByName["fnt.bin"];
    fntFile->enableCache();
    
    int fatOffset = fatFile->begOffs;
    int dirCount = fntFile->getUshortAt(0x06);
    for(int i = 0; i < dirCount; i++)
    {
//        iprintf(".");
        int subTableOffs = fntFile->getUintAt(i*8);
        int firstId = fntFile->getUshortAt(i*8+4);
        u8 len = 1;
        len = fntFile->getByteAt(subTableOffs);
        while(len != 0)
        {
//        iprintf(",");
            bool isdir = (len & 0x80) != 0;
            len &= 0x7F;
            subTableOffs++;
            string fname = "";
            for(int j = 0; j < len; j++)
                fname += fntFile -> getByteAt(subTableOffs++);
            
            if(!isdir)
            {
                int thisId = firstId;
                firstId++;
                NitroFile* tf = new NitroFile(romFile, thisId, fatOffset+thisId*8, fatOffset+thisId*8+4, 
                    this, false, false, false, fname);
                addFile(tf);
    //            tf->print();
            }
            len = fntFile->getByteAt(subTableOffs);
        }
    }
    
    loadOvTable(filesByName["arm9ovt.bin"]);
    loadOvTable(filesByName["arm7ovt.bin"]);

    
    iprintf("Done loading NitroFS\n");
}

int NitroFilesystem::findFreeSpace(int len)
{
    allFiles.Sort(); //sort by offset
    
    for (list<NitroFile>::iterator it = allFiles.begin; i != allFiles.end(); i++)
    {
        NitroFile* first = *it;
        list<NitroFile>::iterator it2 = it;
        it2++;
        if(it2 == allFiles.end()) continue;
        NitroFile* second = *it2;
        
        int spBegin = first->endOffs;
        if (spBegin % 4 != 0)
            spBegin += 4 - spBegin % 4;

        int spEnd = second->beginOffs - 1;
        if (spEnd % 4 != 0)
            spEnd -= spEnd % 4;
        
        int spSize = spEnd - spBegin;
        if (spSize >= len)
        {
            int spLeft = len - spSize;
            
            if(spBegin >= 0x1400000)
                return spBegin;
        }
    }

    int lastFile = allFiles.back()->endOffs + 1;
    if (lastFile % 4 != 0)
        lastFile += 4 - lastFile % 4;

    if(lastFile < 0x1400000)
        return 0x1400000;
    else
        return lastFile;
}

NitroFilesystem::~NitroFilesystem()
{
    for(list<NitroFile*>::iterator it = allFiles.begin(); it != allFiles.end(); it++)
        delete *it;
    
    fclose(romFile);
}

NitroFile* NitroFilesystem::getFileByName(string name)
{
    NitroFile* res = filesByName[name];
    if(res == NULL)
    {
        iprintf("No such file: %s\n", name.c_str());
        while(1);
    }
    return res;
}
NitroFile* NitroFilesystem::getFileById(int id)
{
    NitroFile* res = filesById[id];
    if(res == NULL)
    {
        iprintf("No such file: %d\n", id);
        while(1);
    }
    return res;
}

