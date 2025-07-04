#ifndef _DIS_MANAGER_H
#define _DIS_MANAGER_H

#include <common.h>
#include <font_manager.h>

typedef struct DisBuff
{
    int   iXres;
    int   iYres;
    int   iBpp;  
    char *buff;
}DisBuff, *pDisBuff;



typedef struct DisOpr{
    char *name;
    int  (*DeviceInit)(void);
    int  (*DeviceExit)(void);
    int  (*GetBuffer)(pDisBuff ptDisBuff);  //从app处获得buffer
    int  (*FlushRegion)(pRegion ptRegion, pDisBuff ptDisBuff);   //将framebuffer的数据刷新到lcd
    struct DisOpr *pNext;   //构造链表，可以链接多个设备
}DisOpr, *pDisOpr;


void RegisterDisplay(pDisOpr ptDispOpr);

void DisplayInit(void);
int SelectDefaultDisplay(char *name);
int InitDefaultDisplay(void);
int PutPixel(int x, int y, unsigned int dwColor);
int FlushDisplayRegion(pRegion ptRegion, pDisBuff ptDispBuff);
pDisBuff GetDisplayBuffer(void);
void DrawRegionColor(pRegion ptRegion, unsigned int dwColor);
void DrawTextInRegionCentral(char* name, pRegion ptRegion, unsigned int dwColor);


#endif // 