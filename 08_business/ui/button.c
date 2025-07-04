#include <ui.h>


//绘制一片region
static int DefaultOnDraw(struct Button *ptButton, pDisBuff ptDisBuff)
{
	//绘制底色
	DrawRegionColor(&ptButton->tRegion, BUTTON_DEFEAULT_COLOR);

	//居中显示文字
	DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

	//flush to lcd/web
	FlushDisplayRegion(&ptButton->tRegion, ptDisBuff);

	return 0;
}

//绘制按下后的区域样子
static int DefaultOnPressed(struct Button *ptButton, pDisBuff ptDisBuff, pInputEvent ptInputEvent)
{
	unsigned int dwColor = BUTTON_DEFEAULT_COLOR;
	ptButton->status = !ptButton->status;
	//按键按下
	if(ptButton->status)
	{
		dwColor = BUTTON_PRESSED_COLOR;

			//绘制底色
		DrawRegionColor(&ptButton->tRegion, dwColor);

		DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);
		//flush to lcd/web
		FlushDisplayRegion(&ptButton->tRegion, ptDisBuff);

	}	
	return 0;
}

void InitButton(pButton ptButton, char* name, pRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed)
{
	ptButton->name    = name; 
	ptButton->status  = 0;
	if(ptRegion)
		ptButton->tRegion = *ptRegion;
	ptButton->OnDraw  = OnDraw ? OnDraw : DefaultOnDraw; 
	ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}
