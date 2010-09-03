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

#include "textlist.h"
#include <cstdio>

TextList::TextList(vector<string>& ls)
{
    lst = ls;
}

int TextList::getObjCount()
{
    return lst.size();
}
int TextList::getObjsPerRow()
{
    return 1;
}
int TextList::getObjHeight()
{
    return 8;
}
void TextList::renderObj(int ind, bool selected)
{
//    iprintf("r%d ", ind);
    if(selected)
        renderText(0, ind%32, 32, 1, lst[ind]);
    else
        renderText(0, ind%32, 32, 0, lst[ind]);
}

void TextList::scrollList(int y)
{
//    iprintf("scroll: %d \n", y);
    textScroll(y);
    textClearOpaque();
}

int showTextList(vector<string>& lst)
{
    return showTextList(lst, 0);
}

int showTextList(vector<string>& lst, int inisel)
{
    TextList l (lst);
    l.select(inisel);
    l.show();
    textClearTransparent();
    return l.selection;
}

