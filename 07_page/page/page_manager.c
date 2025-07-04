#include <common.h>
#include <page_manager.h>
#include <string.h>


static pPageAction g_ptPages = NULL;

void PageRegister(pPageAction ptPageAction)
{
	ptPageAction->pNext = g_ptPages;
	g_ptPages = ptPageAction;
}

pPageAction Page(char   *name)
{
	pPageAction ptTmp = g_ptPages;
	while(ptTmp)
	{
		if(strcmp(ptTmp->name, name) == 0)
			return ptTmp;

		ptTmp = ptTmp->pNext;
	}
	return NULL;
}


void PagesRegister(void)
{
	extern void MainPageRegister(void);
	MainPageRegister();
}
