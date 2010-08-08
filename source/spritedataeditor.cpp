#include "spritedataeditor.h"
#include "text.h"
#include <nds.h>

void renderSpriteData(u8* ptr)
{
    textClearOpaque();
    
    int xstart = 5
    for(int i = 0; i < 6; i++)
    {
        renderHexChar(i*3+xstart, 12, 0, ptr[i] % 16);
        renderHexChar(i*3+xstart+1, 12, 0, ptr[i] >> 4);
    }
}

void editSpriteData(u8* ptr)
{
    textScroll(0);
    renderSpriteData(ptr);
    bool selecting = true;
    
    while(selecting)
    {        
        scanKeys();
        touchRead(&touch);
        keysNowPressed = keysDownRepeat();
        keysNowHeld = keysHeld();

        if(keysNowPressed & KEY_TOUCH)
        {
        }

        lastTouchPress = (keysNowHeld & KEY_TOUCH) != 0;
        if(keysNowPressed & KEY_RIGHT)
            selecting = false;
        swiWaitForVBlank();
    }
   
}