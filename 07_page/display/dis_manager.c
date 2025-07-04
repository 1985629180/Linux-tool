#include <stdio.h>
#include <string.h>
#include <dis_manager.h>

static pDisOpr  g_DisDevs = NULL;
static pDisOpr  g_DisDafault = NULL;
static DisBuff g_DisBuff;
static int lind_width;
static int pixel_width;

int PutPixel(int x, int y, unsigned int dwColor)
{
	unsigned char *pen_8 = (unsigned char *)(g_DisBuff.buff + y*lind_width + x*pixel_width);  //指向目标像素的指针
	unsigned short *pen_16;
	unsigned int *pen_32;   //不同的位深
	unsigned int red, blue, green;

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int * )pen_8;

	switch(g_DisBuff.iBpp)  //根据位深绘制像素
	{
		case 8:	
		*pen_8 = dwColor;		
		break;

        //565,red|green|blue
		case 16:	
			red   = (dwColor >> 16) & 0xff; 
			green = (dwColor >> 8) & 0xff; 
			blue  = (dwColor >> 0) & 0xff; 
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue << 0);
			*pen_16 = dwColor;
		break;

		case 32:
			*pen_32 = dwColor;
		break;

		default:
			printf("can't surport %dbpp\n", g_DisBuff.iBpp);
			return -1;
		break;
	}

	return 0;
}


//绘制一片区域底色
void DrawRegionColor(pRegion ptRegion, unsigned int dwColor)
{
	int i,j;
	int x = ptRegion->iLeftUpX;
	int y = ptRegion->iLeftUpY;
	int width = ptRegion->iWidth;
	int heigh = ptRegion->iHeight;

	for(j = y; j < y + width; y++)
	{
		for(i = x; i < x + heigh; i++)
		{
			PutPixel(j, x, dwColor);
		}
	}
}

void DrawTextInRegionCentral(char* name, pRegion ptRegion, unsigned int dwColor)
{
	int i = 0;
	int n = strlen(name);
	int iFontSize = ptRegion->iWidth/n/2;
	int err;
	FontBitMap tFontBitMap;

	int iOriginX, iOriginY;

	if(iFontSize > ptRegion->iHeight)
		iFontSize = ptRegion->iHeight;

	iOriginX = (ptRegion->iWidth - n * iFontSize)/2 + ptRegion->iLeftUpX;
	iOriginY = (ptRegion->iHeight - iFontSize)/2 + iFontSize + ptRegion->iLeftUpY;

	SetFontSize(iFontSize);

	while(name[i])
	{
		tFontBitMap.iCurOriginX = iOriginX;
		tFontBitMap.iCurOriginY = iOriginY;
		err = GetFontBitMap(name[i], &tFontBitMap);
		if(err == -1)
		{	
			printf("GetFontBitMap Err");
			return;
		}
		//绘制bitmap到framebuffer上
		DrawFontBitMap(&tFontBitMap, dwColor);	
		iOriginX = tFontBitMap.iNextOriginX;
		iOriginY = tFontBitMap.iNextOriginY;
		i++;
	}
}


/************************************************************************************************
g_DispDevs -> [Device1] -> [Device2] -> NULL
	现在，我们要将一个新的设备操作结构 [NewDevice] 注册到链表中。执行这两行代码后，链表的状态如下：
g_DispDevs -> [NewDevice] -> [Device1] -> [Device2] -> NULL
*************************************************************************************************/
void RegisterDisplay(pDisOpr ptDisOpr)
{
    ptDisOpr->pNext = g_DisDevs;   //g_DisDevs的头节点赋值给ptDisOpr的next指针
                                    //所以ptDisOpr是头节点了                                  
    g_DisDevs = ptDisOpr;           //g_DisDevs指向ptDisOpr
}


//从设备链表中选择要操作的设备
int SelectDefaultDisplay(char *name)
{
	pDisOpr pTemp = g_DisDevs;
	while(pTemp)
	{
		if(strcmp(name, pTemp->name) == 0)
		{
			g_DisDafault = pTemp;
			return 0;
		}
		pTemp = pTemp->pNext;
	}
	return -1;
}


//初始化设备
int InitDefaultDisplay(void)
{
    int ret;
    ret = g_DisDafault->DeviceInit();
    if(ret)
    {
        printf("DeviceInit err\n");
		return -1;
    }
    ret = g_DisDafault->GetBuffer(&g_DisBuff);
    if(ret)
    {
        printf("GetBuffer err\n");
		return -1;
    }
	lind_width = g_DisBuff.iXres * g_DisBuff.iBpp / 8;
	pixel_width = g_DisBuff.iBpp / 8;

	return 0;
}

pDisBuff GetDisplayBuffer()
{
	return &g_DisBuff;
}


int FlushDisplayRegion(pRegion ptRegion, pDisBuff ptDispBuff)
{
	return g_DisDafault->FlushRegion(ptRegion, ptDispBuff);
}

void DisplayInit(void)
{
    extern void FramebufferInit(void);  //extern 声明：告诉编译器 FramebufferInit 函数在其他文件中定义。
	FramebufferInit();
}
