#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCH  1
#define INPUT_TYPE_KEY    2
#define INPUT_TYPE_NET    3

#ifndef NULL
#define NULL (void *)0
#endif

//输入事件的类型和各个数据
typedef struct InputEvent{
	struct timeval tTime;
	int  iType;
	int  iX;
	int  iY;
	int  iPressure;
	char str[1024];
}InputEvent, *pInputEvent;

typedef struct InputDeviceOpr{
	char *name;
	int  (*DeviceInit)(void);
	int  (*DeviceExit)(void);
 	int  (*GetInputEvent)(pInputEvent ptInputEvent);
	struct InputDeviceOpr *pNext;
}InputDeviceOpr, *pInputDeviceOpr;



#endif
