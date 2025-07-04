#ifndef _CONFIG_H
#define _CONFIG_H

#include <common.h>

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE "/etc/test_gui/gui.conf"

typedef struct  ItemCfg{
	int index;       //索引号
	char name[100];  //名字   后续可以通过索引号或者名字查找
	int bCanBeTouched;  //按钮是否能按下
	char common[100];
}ItemCfg, *pItemCfg;


int ParseConfigFile();
pItemCfg GetItemCfgByIndex(int index);
pItemCfg GetItemCfgByName(char* name);
int GetItemCfgCount(void);


#endif
