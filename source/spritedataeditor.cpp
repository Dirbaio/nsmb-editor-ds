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
	int format;
	typedef struct{
		char heading[8][40];
		int headingsize[8];
		int take[8]; 
		int add[8];//Typo this is add will fix later...
		int nybble[8];
		int type[8];
		bool twonybbles[8];
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
#define L_PC 8


void chformat(){
	if (fscanf(f,"%d",&format)==EOF)
		isd=true;
}
void chboxread(int snum){
	int mask=1;
	int nybble;

	char heading[40];
	if (fscanf(f,"%d",&nybble)!=EOF){
		spritedatastruct[snum].nybble[curamount]=nybble;
			printf("Nybble: %d\n",nybble);
	}
	else {
		printf ("ERROR with chboxread()\n");
		iprintf("\nPress A to continue!\n");
		while(!(keysHeld() & KEY_A)) scanKeys();
		isd=true;
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

		if(fgets(&heading[0],40,f)!=NULL){
			char tmp[40];
			strcpy(tmp,heading);
			int i=0;
			for (i=0;i<=39;i++) // do not check the 40th because it will be '\0'... I think...
				if (tmp[i]==';'){
					tmp[i]='\0';
					spritedatastruct[snum].headingsize[curamount]=i;
					i=40;
				}
			strcpy(heading,tmp);
		}
		else isd=true;

	strcpy(spritedatastruct[snum].heading[curamount],heading);
	printf("Heading: %s\n",heading);
	spritedatastruct[snum].type[curamount]=L_checkbox;	
	curamount++;
	spritedatastruct[snum].gsd=true;
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
void numberread(int snum){
	spritedatastruct[snum].nybble[curamount]=GetSpriteNum();
	if (spritedatastruct[snum].nybble[curamount]==-1)
		isd=true;
	iprintf("Nybble:%d\n",spritedatastruct[snum].nybble[curamount]);
	spritedatastruct[snum].add[curamount]=GetSpriteNum();
	if (spritedatastruct[snum].add[curamount]==-1)
		isd=true;
	iprintf("Add:%d\n",spritedatastruct[snum].add[curamount]);
	spritedatastruct[snum].take[curamount]=GetSpriteNum();
	if (spritedatastruct[snum].take[curamount]==-1)
		isd=true;
	iprintf("Take:%d\n",spritedatastruct[snum].take[curamount]);
	int c=fgetc(f);
	if (c!=EOF)
		if (c)
			spritedatastruct[snum].twonybbles[curamount]=true;
	char heading[40];
		if(fgets(&heading[0],40,f)!=NULL){
			char tmp[40];
			strcpy(tmp,heading);
			int i=0;
			for (i=0;i<=39;i++) // do not check the 40th because it will be '\0'... I think...
				if (tmp[i]==';'){
					tmp[i]='\0';
					spritedatastruct[snum].headingsize[curamount]=i;
					i=40;
				}
			strcpy(heading,tmp);
		}
		else isd=true;
	strcpy(spritedatastruct[snum].heading[curamount],&heading[0]);
	printf("Heading: %s\n",heading);
	spritedatastruct[snum].type[curamount]=L_number;	
	curamount++;
	spritedatastruct[snum].gsd=true;
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
	chformat();
		while (ReadForSprite()){
			if (DEBUG) iprintf("Found Sprite\n");
			snum=GetSpriteNum();
			if (DEBUG) iprintf("Sprite number is: %d\n",snum);
			while((type=GetType())!=L_end || isd){
				if (type==0){
					isd=true;
				}
				else if (DEBUG) iprintf("GetType() returned: %d\n",type);
				if (type==L_checkbox){
					chboxread(snum);

				}
				else if (type==L_number){
					numberread(snum);
				}
			}
		}
		stop=true;
		if (DEBUG) iprintf("DEBUG has beed set, Please press A.");
		if (DEBUG) while(!(keysHeld() & KEY_A)) scanKeys();
		if (isd){
			iprintf("\n\nAn entry in the sprdata.txt is invalid\nYou can continue but must run\nthe editor in a hex-only sprite data editor.");
			iprintf("\nPress A to continue!\n");
			iprintf("DEBUG: %d\n",type);
			while(!(keysHeld() & KEY_A)) scanKeys();
		}
}
void renderSpriteData(int snum)
{
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

u8 getnybble(int i)
{
    u8 n = ptr[i/2];
    if(i % 2 == 0) return n >> 4;
    else return n & 0xF;
}

void setnybble(int i, u8 nval)
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

void editSpriteData(u8* sptr, string sa, string sb, int snum)
{
	if (!spritedatastruct[snum].gsd || isd)
	{
		ptr = sptr;
		msg1 = sa;
		msg2 = sb;
	    
		textScroll(0);
		bool selecting = true;
		selection = -1;
		while(selecting)
		{        
			renderSpriteData(snum);
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
					origval = getnybble(selection);
			}
			else if(keysNowHeld & KEY_TOUCH && selection != -1)
			{
				int y = ystart - touch.py / 8;
				setnybble(selection, origval+y);
			}
	        

			lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
			oamFrame();
			swiWaitForVBlank();
		}
		textClearTransparent();
	}
	else if (spritedatastruct[snum].gsd && !isd)
	{	//Render the new way
		iprintf("Rendering with sprdata.txt support :)\n");
		ptr = sptr;
		msg1 = sa;
		msg2 = sb;
		string tmp;
		int values[8];
		int oldvalue[8];
	    	int i=0;
		textScroll(0);
		bool selecting = true;
		for(i=0;i<=7;i++){
			values[i]=getnybble(spritedatastruct[snum].nybble[i]);
			iprintf("Current value for nybble %d, %s: %d\n",spritedatastruct[snum].nybble[i],&spritedatastruct[snum].heading[i][0],values[i]);
		}
		for(i=0;i<=7;i++){
			oldvalue[i]=getnybble(spritedatastruct[snum].nybble[i]);
		}
		while(selecting){
			textClearOpaque();
			renderText(0, 0, 32, 1, msg1);
			renderText(0, 1, 32, 0, msg2);
			renderText(3, 22, 5, 0, "OK");
			scanKeys();
			if (keysDown() & KEY_TOUCH){
				touchRead(&touch);	
				int y = touch.py / 8;
				if(y > 21) selecting = false;
				else if (y>=2){
					y-=2;
					if (spritedatastruct[snum].type[y]==L_checkbox)
						values[y]= !values[y];
					y+=2;
				}
			}
			for(i=0;i<=7;i++){
				if (spritedatastruct[snum].type[i]==L_checkbox){
					renderText(0,i+2,spritedatastruct[snum].headingsize[i]+1,values[i],&spritedatastruct[snum].heading[i][0]);
				}
			}
			oamFrame();
			swiWaitForVBlank();
		}
		for (i=0;i<=7;i++){
			if(values[i]!=oldvalue[i]){
				setnybble(spritedatastruct[snum].nybble[i],values[i]);
				iprintf("Setting nybble %d to  %d\n",spritedatastruct[snum].nybble[i],values[i]);
			}
		}
		textClearTransparent();
	}
}

