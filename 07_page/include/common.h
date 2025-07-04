#ifndef _COMMON_H
#define _COMMON_H

#ifndef NULL
#define NULL (void*)0
#endif

//显示区域的信息
typedef struct Region
{
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iHeight;
}Region, *pRegion;


#endif


