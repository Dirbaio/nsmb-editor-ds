#include "objectlist.h"
#include "levelrendering.h"
#include <nds.h>

using namespace std;

ObjectList::ObjectList(int tileset, int obj)
{
	select(objectOffs[tileset]+obj);
}

int ObjectList::getObjCount()
{
	return objectCount[0] + objectCount[1] + objectCount[2];
}

int ObjectList::getObjsPerRow()
{
	return 4;
}

int ObjectList::getObjHeight()
{
	return 48;
}

void ObjectList::renderObj(int ind, bool selected)
{
	setRenderingSelected(selected);
	int row = ind / 4;
	int col = ind % 4;
	
	int til, obj;
	if(ind >= objectOffs[2])
	{
		til = 2;
		obj = ind-objectOffs[2];
	}
	else if(ind >= objectOffs[1])
	{
		til = 1;
		obj = ind-objectOffs[1];
	}
	else
	{
		til = 0;
		obj = ind;
	}
//	iprintf("%d %d: %d %d\n", ind, selected, til, obj);
	renderObject(obj, til, col*4+1, row*3, 3, 2);
	if(selected)
		renderTileRect(col*4+1, row*3, 3, 2,  0x3e9);
}

void ObjectList::beginRender(int y)
{
	bgSetScroll(2, 8, y%512);
	bgSetScroll(3, 8, y%512);

	bgUpdate();
	beginRenderFrame(0, 272, y, y+192);
}

void ObjectList::endRender()
{
	endRenderFrame();
}

void showObjectList(int& tileset, int& obj)
{
    ObjectList l (tileset, obj);
    l.show();
    
    int ind = l.selection;
    
   	if(ind >= objectOffs[2])
	{
		tileset = 2;
		obj = ind-objectOffs[2];
	}
	else if(ind >= objectOffs[1])
	{
		tileset = 1;
		obj = ind-objectOffs[1];
	}
	else
	{
		tileset = 0;
		obj = ind;
	}
	iprintf("%d %d\n", tileset, obj);
}

