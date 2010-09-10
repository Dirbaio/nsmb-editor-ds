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
#include "oamUtil.h"


    
SelectableList::SelectableList()
{
    selection = 0;
    scrolly = 0;
    selecting = true;

    speedy = 0.1;
}

void SelectableList::select(int sel)
{
    int rownum = sel / getObjsPerRow();
    scrolly = rownum * getObjHeight() - 50;
    if(scrolly < -10) scrolly = -10;
    if(scrolly > getListHeight() + 10) scrolly = getListHeight() + 10;
    selection = sel;
    render();
}

void SelectableList::render()
{
    beginRender((int)scrolly);
    int yy = (int) scrolly;
    int startInd = yy / getObjHeight() * getObjsPerRow();
    
    for(int y = startInd; (y / getObjsPerRow()) * getObjHeight() <= yy + 192; y++)
    {
        if(y < 0) continue;
        if(y >= getObjCount()) continue;

        renderObj(y, y==selection);
    }
    endRender();
}

void SelectableList::show()
{
    bool touchvalid = false;
    while(selecting)
    {        
        scanKeys();
        touchRead(&touch);
        keysNowPressed = keysDownRepeat();
        keysNowHeld = keysHeld();

		bool mustRender = false;
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
                    int nselection = (int)(touchy/getObjHeight());
                    nselection *= getObjsPerRow();
                    int obw = 256 / getObjsPerRow();
                    nselection += touch.px / obw;

					bool inbounds = true;
                    if(nselection < 0)
                    {
                    	nselection = 0;
                    	inbounds = false;
                    }
                    if(nselection >= getObjCount())
                    {
                    	nselection = getObjCount()-1;
                    	inbounds = false;
                    }
                    
                    if(dblTouchTime < 20 && nselection == selection && inbounds)
                        selecting = false;
                    else
                        dblTouchTime = 0;
                        
                    if(selection != nselection)
                   		mustRender = true;
                    selection = nselection;
    //                iprintf("%d\n", selection);
                }
            }
        }
        else
            touchvalid = true;
        
        if(keysNowHeld & KEY_TOUCH)
            speedy *= 0.7f;
        else
            speedy *= 0.96f;
        if(scrolly < -300)
            scrolly = -300;

        bool out = false;
        if(scrolly < -60)
        {
            out = true;
            speedy += 1;
        }
//        iprintf("%d\n", lsts.size()*8 - 192);
        
        if(scrolly + 192 - 300 > (float)(getListHeight()))
            scrolly = (float)getListHeight() + 300 - 192;
            
        if(scrolly + 192 - 60 > (float)(getListHeight()))
        {
            out = true;
            speedy -= 1;
        }
           
        float speedyabs = speedy;
        if(speedy < 0) speedyabs = -speedy;
        if(speedyabs < 0.1) speedy = 0;
//        printf("%4.2f %4.2f %4.2f\n", speedy, scrolly, touchy);
        if(speedy != 0)
        {
            scrolly += speedy;
            mustRender = true;
        }
            
        if(mustRender)
            render();
            
        lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
        dblTouchTime++;
        oamFrame();
        swiWaitForVBlank();
    }
}

int SelectableList::getObjCount() {return 0;}
int SelectableList::getObjsPerRow() {return 0;}
int SelectableList::getObjHeight() {return 0;}
void SelectableList::renderObj(int ind, bool selected) {}
void SelectableList::beginRender(int y) {}
void SelectableList::endRender() {}

int SelectableList::getListHeight()
{
    int rowcount = getObjCount() / getObjsPerRow();
    if(getObjCount() % getObjsPerRow() != 0) rowcount++;
    return rowcount * getObjHeight();
}

void SelectableList::objSelected(int ind) {}
void SelectableList::keyPressed(u32 mask) {}


/*
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
//        iprintf("%d\n", yv);
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
}*/
