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

#include "spritedataeditor.h"
#include "text.h"
#include <nds.h>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include "oamUtil.h"
#include "lists.h"
#define DEBUG true
namespace spritedataeditor
{
    touchPosition touch;
    u32 keysNowPressed, keysNowHeld;
    bool lastTouchPress = false;
    int selection = -1;
    u8* ptr;
    u8 origval;
	bool SprDtaLoaded=false;
    string msg1, msg2;
	FILE* f;
	unsigned int addr;
	bool isd=false; //impossible sprite data
	int curamount;
	typedef struct{
		char heading[8][20];
		int take[8];
		int subs[8];
		int nibble[8];
		int type[8];
		bool gsd;
	}strdtal;
	strdtal spritedatastruct[323];
}

using namespace spritedataeditor;
#define xstart 7
#define ystart 11
#define L_list 1
#define L_checkbox 2
#define L_number 3
#define L_label 4
#define L_snumber 5
#define L_binary 6
#define L_end 7
void chboxread(int snum){
	int mask=1;
	int nibble;

	char heading[20];
		if (fscanf(f,"%d",&nibble)!=EOF){
		spritedatastruct[snum].nibble[curamount]=nibble;
		printf("Nibble: %d\n",nibble);
	}
	if (fscanf(f,"%d",&mask)!=EOF){
		spritedatastruct[snum].take[curamount]=mask;
		printf("Mask: %d\n",mask);

	}
	else{
		printf ("ERROR with chboxread()\n");
		iprintf("\nPress A to continue!\n");
		while(!(keysHeld() & KEY_A)) scanKeys();
		isd=true;
	}
	fgets(&heading[0],20,f);
	if (!(ferror(f) || feof(f))){
		strcpy(spritedatastruct[snum].heading[curamount],heading);
	}
	else{
		printf ("ERROR with chboxread()\n");
		iprintf("\nPress A to continue!\n");
		while(!(keysHeld() & KEY_A)) scanKeys();
		isd=true;
	}
	printf("Heading: %s\n",heading);
	spritedatastruct[snum].type[curamount]=L_checkbox;
	curamount++;
}
int GetType(){
	char type[25];
	if (fscanf(f,"%s",type)!=EOF){
		if (!strncmp(type,"list",4)) return L_list;
		else if (!strncmp(type,"checkbox",8)) return L_checkbox;
		else if (!strncmp(type,"number",6)) return L_number;
		else if (!strncmp(type,"snumber",7)) return L_snumber;
		else if (!strncmp(type,"label",5)) return L_label;
		else if (!strncmp(type,"binary",6)) return L_binary;
		else if (!strncmp(type,"end",3)) return L_end;
	}
	else return 0;
	return 0;
}
int GetSpriteNum(){
	int spritenum=255;
	if (fscanf(f,"%d",&spritenum)!=EOF) return spritenum;
	else return -1;
}
bool ReadForSprite(){
		char sprite[10];
		if(fscanf(f,"%s",sprite)!=EOF){
			if (strncmp (sprite,"sprite",6)==0) return true;
			else return false;
		}else return false;
}	
void readSpriteData(const char* fname){
	int type,snum;
	f=fopen("sprdata.txt", "rb");
	bool stop=false;
		while (ReadForSprite()){
			if (DEBUG) iprintf("Found Sprite\n");
			snum=GetSpriteNum();
			if (DEBUG) iprintf("Sprite number is: %d\n",snum);
			while((type=GetType())!=L_end){
				if (type==0){
					iprintf("\n\nAn entry in the sprdata.txt has an invalid type\nYou can continue but must run\nthe editor in a hex-only sprite data editor.");
					iprintf("\nPress A to continue!\n");
					while(!(keysHeld() & KEY_A)) scanKeys();
					isd=true;
				}
				else if (DEBUG) iprintf("GetType() returned: %d\n",type);
				if (type==L_checkbox){
					chboxread(snum);
			spritedatastruct[snum].gsd=true;

				}
			}
		}
		stop=true;
		if (DEBUG) iprintf("DEBUG has beed set, Please press A.");
		if (DEBUG) while(!(keysHeld() & KEY_A)) scanKeys();

}
void renderSpriteData(int snum)
{
	if (!spritedatastruct[snum].gsd && !isd){
		textClearOpaque();
		renderText(0, 0, 32, 1, msg1);
		renderText(0, 1, 32, 0, msg2);
		for(int i = 0; i < 6; i++)
		{
			renderChar(i*3+xstart, ystart-1, 0, (char)30);
			renderChar(i*3+xstart+1, ystart-1, 0, (char)30);
			renderHexChar(i*3+xstart, ystart, selection==i*2?1:0, ptr[i] >> 4);
			renderHexChar(i*3+xstart+1, ystart, selection==i*2+1?1:0, ptr[i] & 0xF);
			renderChar(i*3+xstart, ystart+1, 0, (char)31);
			renderChar(i*3+xstart+1, ystart+1, 0, (char)31);
		}
		renderText(3, 22, 5, 0, "OK");
	}
	else {
		int i=0;
		string temps;
		
		int selected[8];
		for (i=0;i<=8;i++){
			selected[8]=0;
		}
		textClearOpaque();
		renderText(0, 0, 32, 1, msg1);
		renderText(0, 1, 32, 0, msg2);
		// Render the new way....
		for (i=0;i<=8;i++){
			if (spritedatastruct[snum].type[i]==L_checkbox){
				strcpy(&temps[0],spritedatastruct[snum].heading[i]);
				renderText(0,2,20,selected[i],temps);
			}
		}
	}
}	

u8 getNibble(int i)
{
    u8 n = ptr[i/2];
    if(i % 2 == 0) return n >> 4;
    else return n & 0xF;
}

void setNibble(int i, u8 nval)
{
    nval &= 0xF;
    
    u8 val = ptr[i/2];
    if(i % 2 == 0)
    {
        val &= 0xF;
        val |= nval << 4;
    }
    else
    {
        val &= 0xF0;
        val |= nval;
    }
    ptr[i/2] = val;
}

void editSpriteData(u8* sptr, string sa, string sb, int spritenum)
{
    ptr = sptr;
    msg1 = sa;
    msg2 = sb;
    
    textScroll(0);
    bool selecting = true;
    iprintf("YAY YAY\n");
    selection = -1;
    while(selecting)
    {        
        renderSpriteData(spritenum);
        scanKeys();
        touchRead(&touch);
        keysNowPressed = keysDown();
        keysNowHeld = keysHeld();

        if(keysNowPressed & KEY_TOUCH)
        {
            int y = touch.py / 8;
            if(y > 21) selecting = false;
            selection = -1;
            if(y > ystart-2 && y < ystart+2)
            {
                int x = touch.px / 8;
                x -= xstart;
                int bx = x / 3;
                if(x % 3 != 2)
                    selection = bx * 2 + x % 3;
            }
            if(selection != -1)
                origval = getNibble(selection);
        }
        else if(keysNowHeld & KEY_TOUCH && selection != -1)
        {
            int y = ystart - touch.py / 8;
            setNibble(selection, origval+y);
        }
        

        lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
        oamFrame();
        swiWaitForVBlank();
    }
    textClearTransparent();
}
