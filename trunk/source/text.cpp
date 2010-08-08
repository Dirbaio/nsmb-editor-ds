
#include "text.h"
#include "font.h"
#include "rom.h"

void renderText(int x, int y, uint max, int col, string& s)
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    x %= 32;
    y %= 32;
    bg0ptr += x + y*32;
    for(uint i = 0; i < max; i++)
    {
        char c = ' ';
        if(i < s.size()) c = s[i];
        *bg0ptr = c | col << 12;

        bg0ptr++;
    }
}

void renderHexChar(int x, int y, int col, u8 c)
{
    if(c < 10)
        renderChar(x, y, col, c+'0');
    else
        renderChar(x, y, col, c+'A'-10);
}

void renderChar(int x, int y, int col, char c)
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    x %= 32;
    y %= 32;
    bg0ptr += x + y*32;
    *bg0ptr = c | col << 12;
}

void textInit()
{
	bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0x1C, 5);
	cpuCopy16(&fontTiles, (uint16*)0x06014000, fontTilesLen);

	vramSetBankE(VRAM_E_LCD);
    
    //Normal text
    VRAM_E_EXT_PALETTE[0][0][1] = RGB15(0, 0, 0); //FG
    VRAM_E_EXT_PALETTE[0][0][2] = RGB15(63, 31, 31); //BG

    //selected text
    VRAM_E_EXT_PALETTE[0][1][1] = RGB15(31, 31, 31); //FG
    VRAM_E_EXT_PALETTE[0][1][2] = RGB15(0, 0, 31); //BG
    
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
    bgShow(0);
    textClearTransparent();
}

void textClearTransparent()
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    for(uint i = 0; i < 32*32; i++)
        bg0ptr[i] = 1;
        
//    textScroll(0);
}

void textClearOpaque()
{
   	uint16* bg0ptr=  (uint16*)0x0601E000;
    for(uint i = 0; i < 32*32; i++)
        bg0ptr[i] = 0;
//    textScroll(0);
}

void textScroll(int y)
{
    bgSetScroll(0, 0, y);
    bgUpdate();
}