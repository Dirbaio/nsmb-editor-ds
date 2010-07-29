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

#include "selectablelist.h"
#include "font.h"
#include "rom.h"

vector<string> lsts;

namespace selectablelist
{
    bool selecting;
    float scrolly, speedy;
    int selection;
    touchPosition touch;
    uint keysNowPressed, keysNowHeld;
    uint dblTouchTime; //time since last touch
    float touchy; //y position being touched
    bool lastTouchPress;
}

using namespace selectablelist;

void desmumePrint(const char* s)
{
    return;
    *((vu32*)0x04FFF010) = (u32)s;
}
void renderText(int x, int y, uint max, int col, string& s)
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    x %= 32;
    y %= 32;
    bg0ptr += x + y*32;
    for(uint i = 0; i < s.size() && i < max; i++)
    {
        *bg0ptr = s[i] | col << 12;
                    desmumePrint("l");

        bg0ptr++;
    }
}

void renderList(int y0)
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    for(uint i = 0; i < 32*32; i++)
        bg0ptr[i] = 0;
    
    for(int y = y0; y <= y0 + 25; y++)
    {
        if(y < 0) continue;
        if(y >= (int)lsts.size()) continue;
//        iprintf("%d\n", y);
        
        renderText(0, y, 32, 0, lsts[y]);
//        iprintf("%d\n", y);
    } 
}

int showList(vector<string>& lst)
{
    lsts = lst;
    scrolly = 0;
    speedy = 0;
    selection = 0;
    selecting = true;
	bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0x1C, 5);
    bgShow(0);
	cpuCopy16(&fontTiles, (uint16*)0x06014000, fontTilesLen);

	vramSetBankE(VRAM_E_LCD);
    
    //Normal text
    VRAM_E_EXT_PALETTE[0][0][1] = RGB15(0, 0, 0); //FG
    VRAM_E_EXT_PALETTE[0][0][2] = RGB15(31, 31, 31); //BG

    //selected text
    VRAM_E_EXT_PALETTE[0][1][1] = RGB15(31, 31, 31); //FG
    VRAM_E_EXT_PALETTE[0][1][2] = RGB15(0, 0, 31); //BG
    
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
    renderList(0);
//    renderText(1, 4, 8, 1, "HELLO WORLD");
//iprintf("aaaa");
    while(selecting)
    {        
        if(keysNowHeld & KEY_TOUCH)
        {
            float ytouchnow = scrolly + (float)touch.py;
            if(lastTouchPress )
            {
                float diff = ytouchnow - touchy;
           
//                printf("%4.2f\n", diff);
                speedy -= diff / 10;
            }
            else
                touchy = ytouchnow;
        }
        
        if(keysNowHeld & KEY_TOUCH)
            speedy *= 0.7d;
        else
            speedy *= 0.93d;
        if(scrolly < -300)
            scrolly = -300;

        bool out = false;
        if(scrolly < -60)
        {
            out = true;
            speedy += 1;
        }
//        iprintf("%d\n", lsts.size()*8 - 192);
        
        if(scrolly + 192 - 300 > (float)(lsts.size()*8))
            scrolly = (float)(lsts.size()*8) + 300 - 192;
            
        if(scrolly + 192 - 60 > (float)(lsts.size()*8))
        {
            out = true;
            speedy -= 1;
        }
           
        float speedyabs = speedy;
        if(speedy < 0) speedyabs = -speedy;
        if(speedyabs < 0.1) speedyabs = 0;
//        printf("%4.2f %4.2f %4.2f\n", speedy, scrolly, touchy);
        if(speedy != 0)
        {
            scrolly += speedy;
            bgSetScroll(0, 0, (int)(scrolly));
            bgUpdate();
            desmumePrint("a");
            renderList((int)(scrolly/8));
            
            desmumePrint("b");
        }
            
        lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
        scanKeys();
        touchRead(&touch);
        keysNowPressed = keysDownRepeat();
        keysNowHeld = keysHeld();
        swiWaitForVBlank();
    }
}