#include <config.h>
#include <stdio.h>
#include <string.h>

static int g_iItemCfgCount = 0;
static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];

//解析配置文件
int ParseConfigFile()
{	
	FILE *fp;
	char buf[100];
	char *p = buf;

	//打开congfig文件
	fp = fopen(CFG_FILE, "r");
	if(!fp)
	{
		printf("can not open cfg file %s\n", CFG_FILE);
		return -1;
	}	

	//一行一行处理
	while(fgets(buf, 100, fp))
	{
		buf[99] = '\0';
		//处理掉开头的空格或者TAB
		while(*p == ' ' || *p == '\t')
			p++;

		//遇到注释，直接跳过这一行
		if(*p == '#')
			continue;

		g_tItemCfgs[g_iItemCfgCount].common[0] = '\0';   //如果某一项没用command直接设置为空
		g_tItemCfgs[g_iItemCfgCount].index = g_iItemCfgCount;
		//config的格式是name canbetouch comman
		sscanf(p, "%s %d %s", g_tItemCfgs[g_iItemCfgCount].name, &g_tItemCfgs[g_iItemCfgCount].bCanBeTouched, g_tItemCfgs[g_iItemCfgCount].common);
		g_iItemCfgCount++;
	}
	return 0;
}


int GetItemCfgCount(void)
{
	return g_iItemCfgCount;
}

pItemCfg GetItemCfgByIndex(int index)
{
	if(index < g_iItemCfgCount)
		return &g_tItemCfgs[index];

	else
		return NULL;
}

pItemCfg GetItemCfgByName(char* name)
{
	int i;
	for(i = 0; i < g_iItemCfgCount; i++)
	{
		if(strcmp(name, g_tItemCfgs[i].name) == 0)
		{
			return &g_tItemCfgs[i];
		}
	}
	return NULL;
}




