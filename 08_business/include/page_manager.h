#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

typedef struct PageAction {
	char *name;                  //
	void (*Run)(void* pParams);  //当前页面正在运行的函数
	struct PageAction *pNext;
}PageAction, *pPageAction;

void PageRegister(pPageAction ptPageAction);
pPageAction Page(char *name);
void PagesRegister(void);

#endif
