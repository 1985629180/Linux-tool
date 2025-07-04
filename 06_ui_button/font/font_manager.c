#include <font_manager.h>
#include <string.h>


static pFontOpr g_ptFonts = NULL;
static pFontOpr g_ptDefaultFontOpr = NULL;
static DisBuff g_tDisBuff;



void DrawFontBitMap(pFontBitMap ptFontbBitMap, unsigned int dwColor)
{ 
	int i ,j;
    int x_point,y_point;
	int x = ptFontbBitMap->iCurOriginX;
	int y = ptFontbBitMap->iCurOriginY;
	int x_max = x + ptFontbBitMap->tRegion.iWidth;
	int y_max = y + ptFontbBitMap->tRegion.iHeight;
	int width = ptFontbBitMap->tRegion.iWidth;
	unsigned char* buffer = ptFontbBitMap->pucBuffer;  //bitmap的buffer信息

	for(j = y, x_point = 0;j < y_max; j++, x_point++)
	{
		for(i = x, y_point = 0; i < x_max; i++, y_point++)
		{
			if(i < 0 || j < 0 || i >= g_tDisBuff.iXres || j >= g_tDisBuff.iYres)
				continue;

			if(buffer[x_point*width + y_point])
				PutPixel(i, j, dwColor);
		}
		
	}
}



//注册到全局链表
void RegisterFont(pFontOpr ptFontOpr)
{
	ptFontOpr->pNext = g_ptFonts;
 	g_ptFonts = ptFontOpr; 
}

void FontsRegister(void)
{
	extern void FreetypeRegister(void);
	FreetypeRegister();
}

int SelectAndInitFont(char *aFontOprName, char *aFontFileName)
{
	pFontOpr ptTmp = g_ptFonts;

    while(ptTmp)
	{
		if(strcmp(ptTmp->name, aFontOprName) == 0)
		{
		 	break;
		}

		ptTmp =  ptTmp->pNext;
	}
	if(!ptTmp)
	{
	 	return -1;
	}
    g_ptDefaultFontOpr = ptTmp;
	
	return ptTmp->FontInit(aFontFileName);
}

int SetFontSize(int iFontSize)
{
	return g_ptDefaultFontOpr->SetFontSize(iFontSize);
}

int GetFontBitMap(unsigned int dwCode, pFontBitMap ptFontBitMap)
{
	return g_ptDefaultFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}


