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
#include "text.h"

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

void renderList(int y0)
{
    textClearOpaque();
    for(int y = y0; y <= y0 + 25; y++)
    {
        if(y < 0) continue;
        if(y >= (int)lsts.size()) continue;
//        iprintf("%d\n", y);
        if(y == selection)
            renderText(0, y, 32, 1, lsts[y]);
        else
            renderText(0, y, 32, 0, lsts[y]);
//        iprintf("%d\n", y);
    } 
}
int showList(vector<string>& lst)
{
    return showList(lst, 0);
}

int showList(vector<string>& lst, int inisel)
{
    selection = inisel;
    lsts = lst;
    scrolly = inisel * 8 - 50;
    if(scrolly < -10) scrolly = -10;
    if(scrolly > lst.size() * 8 + 10) scrolly = lst.size() * 8 + 10;
    speedy = 0;
    selection = inisel;
    selecting = true;

    speedy = 0.1;
//    renderText(1, 4, 8, 1, "HELLO WORLD");
//iprintf("aaaa");

    bool touchvalid = false;
    while(selecting)
    {        
        scanKeys();
        touchRead(&touch);
        keysNowPressed = keysDownRepeat();
        keysNowHeld = keysHeld();

        if(keysNowHeld & KEY_TOUCH)
        {
            if(touchvalid)
            {
                float ytouchnow = scrolly + (float)touch.py;
                if(lastTouchPress )
                {
                    float diff = ytouchnow - touchy;
               
    //                printf("%4.2f\n", diff);
                    speedy -= diff / 10;
                }
                else
                {
                    touchy = ytouchnow;
                    selection = (int)(touchy/8);
                    if(selection < 0) selection = 0;
                    if(selection >= lsts.size()) selection = lsts.size()-1;
    //                iprintf("%d\n", selection);
                }
            }
        }
        else
            touchvalid = true;
        
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
            textScroll((int)(scrolly));
            renderList((int)(scrolly/8));
        }
            
        lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
        if(keysNowPressed & KEY_RIGHT)
            selecting = false;
        swiWaitForVBlank();
    }
    textClearTransparent();
    return selection;
}