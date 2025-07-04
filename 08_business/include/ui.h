#ifndef _UI_H
#define _UI_H

#include <common.h>
#include <dis_manager.h>
#include <input_manager.h>

#define BUTTON_DEFEAULT_COLOR 0xff0000 //红色
#define BUTTON_PRESSED_COLOR  0X00ff00 //绿色
#define BUTTON_TEXT_COLOR	  0X000000 //黑色
#define BUTTON_PERCENT_COLOR  0x0000ff

struct Button;

//定义函数指针类型，使其他用这个更简洁
typedef int (*ONDRAW_FUNC)(struct Button *ptButton, pDisBuff ptDisBuff);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton, pDisBuff ptDisBuff, pInputEvent ptInputEvent);

typedef struct Button {
	char* name;
	int status; //按键状态
	Region tRegion;  //显示区域
	ONDRAW_FUNC OnDraw;
	ONPRESSED_FUNC OnPressed;
}Button, *pButton;

void InitButton(pButton ptButton, char* name, pRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed);


#endif
