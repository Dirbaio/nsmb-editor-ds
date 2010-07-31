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


bool smaller(NitroFile const* a, NitroFile const* b)
{
    if(!a) return true;
    if(!b) return false;
    
    if(a->begOffs == b->begOffs)
        return a->size < b->size;
    return a->begOffs < b->begOffs;
}

NitroFile::NitroFile(int id, int begOffs, int endOffs, NitroFilesystem* parent, NitroFile* allocFile, bool fixedBeg, bool fixedEnd, bool endsize, string name)
{
    this->fixedBeg = fixedBeg;
    this->fixedEnd = fixedEnd;
    this->parent = parent;
    this->name = name;
    this->id = id;
    this->allocFile = allocFile;
    cached = false;
    cacheModified = false;
    
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
        begOffs = allocFile->getUintAt(begPtrOffs);
    }
    if(!fixedEnd)
    {
        endOffs = allocFile->getUintAt(endPtrOffs);
        
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
        allocFile->setUintAt(begPtrOffs, begOffs);
    }
    
    if(!fixedEnd)
    {
        if(endsize)
            allocFile->setUintAt(endPtrOffs, size);
        else
            allocFile->setUintAt(endPtrOffs, endOffs);
    }
}

u8* NitroFile::getContents()
{
    if(endOffs < begOffs)
    {
        print();
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(parent->romFile, begOffs, SEEK_SET);
    
    u8* res = new u8[endOffs-begOffs];
    fread(res, 1, endOffs-begOffs, parent->romFile);
    return res;
}

void NitroFile::loadContentsInto(void* ptru)
{
    u8* ptr = (u8*) ptru;
    if(endOffs < begOffs)
    {
        print();
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(parent->romFile, begOffs, SEEK_SET);
    
    fread(ptr, 1, endOffs-begOffs, parent->romFile);
}

	
void NitroFile::loadCompressedContentsInto(void* destu)
{
    u16* dest = (u16*) destu;
    if(endOffs < begOffs)
    {
        print();
        iprintf("endOffs < begOffs\n");
        while(1);
    }
    
    fseek(parent->romFile, begOffs, SEEK_SET);
    
    u8* res = new u8[endOffs-begOffs];
    fread(res, 1, endOffs-begOffs, parent->romFile);

	uint32 decsize = res[1] | res[2]<<8 | res[3]<<16;
	uint16* buffer = new uint16[decsize/2];
	decompress(res, buffer, LZ77);
    
    for(int i = 0; i < decsize/2; i++)
        dest[i] = buffer[i];

    delete[] buffer;
    delete[] res;
}

void NitroFile::replaceContents(void* ptr, int nsize)
{
    iprintf("Replacing: ");
    print();
    int newOffs = begOffs;
    if(size < nsize)
    {
        newOffs = parent->findFreeSpace(nsize);
        iprintf("%x -> %x\n", nsize, newOffs);
    }

    begOffs = newOffs;
    size = nsize;
    endOffs = begOffs+size;
    
    fseek(parent->romFile, begOffs, SEEK_SET);
    fwrite(ptr, 1, nsize, parent->romFile);
    
    saveOffsets();
    if(!cached)
        parent->flushCache();
}

u8 NitroFile::getByteAt(int offs)
{
    if(cached)
        return cachedContents[offs];

    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    return readByte(parent->romFile);
}
void NitroFile::setByteAt(int offs, u8 val)
{
    if(cached)
    {
        cachedContents[offs] = val;
        cacheModified = true;
        return;
    }
    
    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    writeByte(parent->romFile, val);
}
u16 NitroFile::getUshortAt(int offs)
{
    if(cached)
        return *(u16*)(cachedContents+offs);
    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    return readUShort(parent->romFile);
}
void NitroFile::setUshortAt(int offs, u16 val)
{
    if(cached)
    {
        *(u16*)(cachedContents+offs) = val;
        cacheModified = true;
        return;
    }
    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    writeUShort(parent->romFile, val);
}
u32 NitroFile::getUintAt(int offs)
{
    if(cached)
        return *(u32*)(cachedContents+offs);
    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    return readUInt(parent->romFile);
}
void NitroFile::setUintAt(int offs, u32 val)
{
    if(cached)
    {
    iprintf("== %s: %x = %x\n", name.c_str(), offs, val);
        *(u32*)(cachedContents+offs) = val;
        cacheModified = true;
        return;
    }
    fseek(parent->romFile, begOffs+offs, SEEK_SET);
    writeUInt(parent->romFile, val);
}

void NitroFile::print()
{
    iprintf("%d: %x-%x %s %d %d\n", id, begOffs, endOffs, name.c_str(), cached, cacheModified);
}

void NitroFile::enableCache()
{
    iprintf("Caching: ");
    print();
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


void NitroFile::fixHeaderChecksum()
{
    if(!cached) return;
    u16 crc = swiCRC16(0, cachedContents, 0x15E);
    setUshortAt(0x15E, crc);
}

NitroFile::~NitroFile()
{
    flushCache();
    if(cached)
        delete[] cachedContents;
}

void NitroFilesystem::addFile(NitroFile *f)
{
//    f->print();
    filesByName[f->name] = f;
    filesById[f->id] = f;
    allFiles.push_back(f);
}

void NitroFilesystem::loadOvTable(NitroFile* f)
{
    NitroFile* fatFile = filesByName["fat.bin"];
    
    int size = f->size;
    size /= sizeof(OverlayEntry);
    
    OverlayEntry* ovs = (OverlayEntry*) f->getContents();
    
    for(int i = 0; i < size; i++)
    {
        u32 id = ovs[i].fileId;
  //      iprintf("%d ", id);
        NitroFile* tf = new NitroFile(id, id*8, 
        id*8+4, this, fatFile, false, false, false, "overlayfile");
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
    romPath = fn;
    iprintf("Loading NitroFS\n");
    NitroFile* headerFile = new NitroFile(-1, 0, 0x200, this, NULL, true, true, false, "header.bin");
    addFile(headerFile);
    headerFile->enableCache();
    addFile(new NitroFile(-1, 0x20, 0x2C, this, headerFile, false, false, true, "arm9.bin"));
    addFile(new NitroFile(-1, 0x30, 0x3C, this, headerFile, false, false, true, "arm7.bin"));
    addFile(new NitroFile(-1, 0x40, 0x44, this, headerFile, false, false, true, "fnt.bin"));
    addFile(new NitroFile(-1, 0x48, 0x4C, this, headerFile, false, false, true, "fat.bin"));
    addFile(new NitroFile(-1, 0x50, 0x54, this, headerFile, false, false, true, "arm9ovt.bin"));
    addFile(new NitroFile(-1, 0x58, 0x5C, this, headerFile, false, false, true, "arm7ovt.bin"));
    addFile(new NitroFile(-1, 0x58, 0x840, this, headerFile, false, true, true, "banner.bin"));
    
    NitroFile* fatFile = filesByName["fat.bin"];
    fatFile->enableCache();
    NitroFile* fntFile = filesByName["fnt.bin"];
    fntFile->enableCache();
 
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
                NitroFile* tf = new NitroFile(thisId, thisId*8, thisId*8+4, 
                    this, fatFile, false, false, false, fname);
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
    allFiles.sort(smaller); //sort by offset
    
    for (list<NitroFile*>::iterator it = allFiles.begin(); it != allFiles.end(); it++)
    {
        NitroFile* first = *it;
        list<NitroFile*>::iterator it2 = it;
        it2++;
        if(it2 == allFiles.end()) continue;
        NitroFile* second = *it2;
        
        int spBegin = first->endOffs;
        if (spBegin % 4 != 0)
            spBegin += 4 - spBegin % 4;
        if(spBegin<=0x1400000)
            spBegin = 0x1400000;
            
        int spEnd = second->begOffs - 1;
        if (spEnd % 4 != 0)
            spEnd -= spEnd % 4;
        
        int spSize = spEnd - spBegin;
        if (spSize >= len)
            if(spBegin >= 0x1400000)
                return spBegin;
    }
    iprintf("LAST FILE: ");
    allFiles.back()->print();
    int lastFile = allFiles.back()->endOffs + 1;
    if (lastFile % 4 != 0)
        lastFile += 4 - lastFile % 4;

    if(lastFile < 0x1400000)
        return 0x1400000;
    else
        return lastFile;
}

void NitroFilesystem::flushCache()
{
    allFiles.sort(smaller); //sort by offset
    NitroFile* lastFile = allFiles.back();
    uint lastOffs = lastFile->endOffs + 0x10;
    lastOffs &= ~3;
    getFileByName("header.bin")->setUintAt(0x80, lastOffs);
    getFileByName("header.bin")->fixHeaderChecksum();
    
    for(list<NitroFile*>::iterator it = allFiles.begin(); it != allFiles.end(); it++)
        (*it)->flushCache();

    fflush(romFile);
    fclose(romFile);
    romFile = fopen(romPath, "r+b");
    if(romFile == NULL)
    {
        iprintf("ERROR: Romfile = null. Halting!\n");
        while(1);
    }
    
}
NitroFilesystem::~NitroFilesystem()
{
    for(list<NitroFile*>::iterator it = allFiles.begin(); it != allFiles.end(); it++)
        delete *it;
    
    fflush(romFile);
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


