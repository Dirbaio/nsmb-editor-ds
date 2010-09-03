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

#ifndef _SELECTABLELIST_H
#define _SELECTABLELIST_H

#include<nds.h>
#include<vector>
#include<string>

using namespace std;

class SelectableList
{
    public: 
    
    bool selecting;
    float scrolly, speedy;
    int selection;
    touchPosition touch;
    u32 keysNowPressed, keysNowHeld;
    u32 dblTouchTime; //time since last touch
    float touchy; //y position being touched
    bool lastTouchPress;

    SelectableList();
    void render();
    void show();
    void select(int ind);
    
    virtual int getObjCount();
    virtual int getObjsPerRow();
    virtual int getObjHeight();
    virtual void renderObj(int ind, bool selected);
    virtual void scrollList(int y);
    virtual void objSelected(int ind);
    virtual void keyPressed(u32 mask);
    
    int getListHeight();
};

#endif
