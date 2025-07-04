#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include <dis_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>

int main(int argc, char** argv)
{
	int err;

	if(argc != 2)
	{
		printf("Usage: %s <font_file>\n", argv[0]);
		return -1;
	}
    //业务系统程序流程图
	//1、初始化显示系统
	DisplayInit();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();	
    //2、初始化输入系统
	InputInit();         //注册所有输入设备
    InputDeviceInit();	 //初始化所有设备，为每个设备创建线程
	
	//3、初始化文字系统
	FontsRegister();

	err = SelectAndInitFont("freetype", argv[1]);
	if(err)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	//4、初始化页面系统
	PagesRegister();

	//5、运行业务系统的主页面
	Page("main")->Run(NULL);

}

