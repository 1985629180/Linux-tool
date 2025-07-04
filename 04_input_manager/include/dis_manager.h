#ifndef _DIS_MANAGER_H
#define _DIS_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct DisBuff
{
    int   iXres;
    int   iYres;
    int   iBpp;  
    char *buff;
}DisBuff, *pDisBuff;

//得到buffer一些数据
typedef struct Region
{
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iHeight;
}Region, *pRegion;

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

#endif // 