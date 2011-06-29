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

#include "lists.h"
#include "spritedataeditor.h"
#include <stdio.h>

vector<string> jyotyuAnnotationList;
vector<string> spriteList;
vector<string> tilesetList;
vector<string> entranceList;
vector<string> topBGList;
vector<string> bottomBGList;
vector<string> musicList;
vector<int> spriteDataList;
vector<string> levelList;
vector<string> levelFileList;

void loadList(const char* fname, vector<string>& v)
{
    v.clear();
    
    char mystring[100];
    
    iprintf("%s\n", fname);
    FILE* f = fopen (fname, "rb");
    
    if (f == NULL)
    {
        perror ("Error opening file");
        while(1);
    }
    else
    {
        while(!feof(f))
        {
            fgets (mystring , 100 ,f);
            string s = mystring;
            s = s.substr(0, s.size()-2);
            v.push_back(s);
        }
        fclose (f);
    }
}

void loadLevelList(const char* fname)
{
    char mystring[100];
    
    iprintf("%s\n", fname);
    FILE* f = fopen (fname, "rb");
    
    if (f == NULL)
    {
        perror ("Error opening file");
        while(1);
    }
    else
    {
        while(!feof(f))
        {
            fgets (mystring , 100 ,f);
            string s = mystring;
            s = s.substr(0, s.size()-2);
            int ind = s.find('|');
            
            string levname = s.substr(0, ind);
            string fname = s.substr(ind+1, s.size() - ind - 3);
            char areacount = s[s.size()-1];
            
//            iprintf("%s %s %c\n", levname.c_str(), fname.c_str(), areacount);
           
            if(areacount == '1' )
            {
                levelList.push_back(levname);
                levelFileList.push_back(fname+"_1");
            }
            else
            {
                for(char c = '1'; c <= areacount; c++)
                { 
                levelList.push_back(levname+" Area "+c);
                levelFileList.push_back(fname+"_"+c);
                }
            }
//            levelList.push_back());
        }
        fclose (f);
    }
}
void loadSpriteData(const char* fname){
	readSpriteData(fname);
}
void loadLists()
{
    loadList("sprlist.txt", spriteList);
    loadList("tbglist.txt", topBGList);
    loadList("bbglist.txt", bottomBGList);
    loadList("muslist.txt", musicList);
    loadList("entlist.txt", entranceList);
	loadSpriteData("sprdata.txt");
    loadLevelList("levellist.txt");
}
