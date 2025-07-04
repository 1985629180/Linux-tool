#include <config.h>
#include <stdio.h>
#include <ui.h>
#include <page_manager.h>
#include <math.h>
#include <string.h>


#define X_GAP 5
#define Y_GAP 5

static Button g_tButton[ITEMCFG_MAX_NUM];
static int g_iButtonCnt;

static int MainPageOnPressed(struct Button *ptButton, pDisBuff ptDispBuff, pInputEvent ptInputEvent)
{
	int dwColor = BUTTON_DEFEAULT_COLOR;
	char name[100];
	char status[100];
	char* strButton;

	//触摸屏事件
	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		//分辨能否被点击
		if(GetItemCfgByName(ptButton->name)->bCanBeTouched == 0)
			return -1;
  		//如果能按下，修改颜色
		ptButton->status = !ptButton->status;
		if(ptButton->status)
			dwColor = BUTTON_PRESSED_COLOR;
	}
    //网络事件
	/* 根据传入的字符串修改颜色 : wifi ok, wifi err, burn 70 */
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		sscanf(ptInputEvent->str, "%s %s", name, status);
		if(strcmp(status, "ok") == 0)			
			dwColor = BUTTON_PRESSED_COLOR;
		else if(strcmp(status, "err") == 0)			
			dwColor = BUTTON_DEFEAULT_COLOR;
		else if(status[0] > '0' || status[0] < '9')
		{
			dwColor = BUTTON_PERCENT_COLOR;
			strButton = status;	
		}
		else
			return -1;			
	}
	else
		return -1;

	
	/* 绘制底色 */
	DrawRegionColor(&ptButton->tRegion, dwColor);

	/* 居中写文字 */
	DrawTextInRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
	return 0;
}

static int isTouchPointInRegion(int iX, int iY, pRegion ptRegion)
{
	if(iX < ptRegion->iLeftUpX || iX >= ptRegion->iLeftUpX + ptRegion->iWidth)	
		return 0;
	if(iY < ptRegion->iLeftUpY || iX >= ptRegion->iLeftUpY + ptRegion->iHeight)	
		return 0;

	return 1;
}

//根据输入事件选中要配置的按键
static pButton GetButtonByName(char* name)
{
	int i;
	for(i = 0; i < g_iButtonCnt; i++)
	{
		if(strcmp(name, g_tButton[i].name) == 0)
		{
			return &g_tButton[i];
		}
	}
	return NULL;
}


//根据输入事件选中要配置的按键
static pButton GetButtonByInputEvent(pInputEvent ptInputEvent)
{
	int  i;
	char name[100];

	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		for(i = 0; i < g_iButtonCnt; i++)
		{
   			//触摸是否在这个按键的区域
			if(isTouchPointInRegion(ptInputEvent->iX, ptInputEvent->iY, &g_tButton[i].tRegion))
				return &g_tButton[i];
		}
	}
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		sscanf(ptInputEvent->str, "%s", name);
		return GetButtonByName(name);    
	}
	else
	{
		return NULL;
	}
	return NULL;
}



//依据配置文件生成按钮和界面
static void CreateButtons()
{
	int n; //按钮数量
	pDisBuff ptDisBuff;
	int xres;    //lcd x分辨率
	int yres;    //lcd y分辨率
	int width, height;
	int n_per_line;
	int start_x, start_y;  //起始按键坐标
	int rows;    //有多少列按钮
	pButton ptButton;
	int col, row;
	int i = 0;
	int per_start_x, per_start_y; 

	g_iButtonCnt = n = GetItemCfgCount();
	ptDisBuff =  GetDisplayBuffer();

	xres = ptDisBuff->iXres;
	yres = ptDisBuff->iYres;
	width = sqrt(1.0/0.618 *xres * yres / n);  //单个按钮面积=width*(width*0.618)=xres*yres/n
	n_per_line = xres / width + 1;
	width = xres / n_per_line;
	height = width * 0.618;

	//计算按键居中显示的坐标
	start_x = (xres - width * n_per_line) / 2;
	rows = n / n_per_line;
	if(rows * n_per_line < n)
		rows++;
	start_y = (yres - height * rows) / 2;
	
	//计算每个按钮的region坐标,然后通过初始化函数初始每个按钮
	for(row = 0; row < rows ; row++)
	{
		per_start_x = start_x - width; 
		per_start_y = start_y + row * height;
		for(col = 0; col < n_per_line; col++)
		{
			ptButton = &g_tButton[i];
			ptButton->tRegion.iLeftUpX = per_start_x + width;
			ptButton->tRegion.iLeftUpY = per_start_y;	
			ptButton->tRegion.iWidth   = width - X_GAP;
			ptButton->tRegion.iHeight  = height - Y_GAP;
			per_start_x = ptButton->tRegion.iLeftUpX; 
		
			InitButton(ptButton,GetItemCfgByIndex(i)->name , NULL, NULL, MainPageOnPressed);
			i++;
		}
	}
	for(i = 0; i < n; i++)
		g_tButton[i].OnDraw(&g_tButton[i], ptDisBuff);
}

static void MainPageRun(void *pParams)
{   
	int err;

	InputEvent tInputEvent;
	pButton ptButton;
	pDisBuff ptDisBuff;
	//主页面流程图

	ptDisBuff = GetDisplayBuffer();
	//1、读取配置文件
	err = ParseConfigFile();
	if(err)
	{
		printf("ParseConfigFile ERR!\n");
		return;
	}
    //2、根据配置文件生成按钮
	CreateButtons();

	//3、读取输入事件
	while(1)
	{
		err = GetInputEvent(&tInputEvent);
		if(err)
			continue;

		//4、根据输入事件找到按钮
		ptButton = GetButtonByInputEvent(&tInputEvent);
		if(ptButton == NULL)	
			continue;

		//调用onpressed函数处理函数
		ptButton->OnPressed(ptButton, ptDisBuff, &tInputEvent);
	}
	
}

static PageAction g_tMainPage = {
	.name = "main",
	.Run  = MainPageRun,
};

void MainPageRegister(void)
{
	PageRegister(&g_tMainPage);
}


