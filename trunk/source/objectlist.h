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


#ifndef _OBJECTLIST_H
#define _OBJECTLIST_H

#include "selectablelist.h"
#include <vector>
#include "text.h"

using namespace std;


class ObjectList : public SelectableList
{
    public:
    
    ObjectList(int tileset, int obj);
    virtual int getObjCount();
    virtual int getObjsPerRow();
    virtual int getObjHeight();
    virtual void renderObj(int ind, bool selected);
    virtual void beginRender(int y);
    virtual void endRender();
};

void showObjectList(int& tileset, int& obj);

#endif
