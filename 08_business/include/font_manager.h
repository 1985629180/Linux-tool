#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include <common.h>
#include <dis_manager.h>


typedef struct FontBitMap{
	Region tRegion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
    int iNextOriginY;
	unsigned char *pucBuffer;  //存放位图信息
}FontBitMap, *pFontBitMap;

typedef struct FontOpr {
	char *name;
	int (*FontInit)(char *aFineName);
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, pFontBitMap ptFontBitMap);
	struct FontOpr *pNext;
}FontOpr, *pFontOpr;

void FontsRegister(void);
void RegisterFont(pFontOpr ptFontOpr);


int SelectAndInitFont(char *aFontOprName, char *aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode, pFontBitMap ptFontBitMap);
void DrawFontBitMap(pFontBitMap ptFontbBitMap, unsigned int dwColor);


#endif 
