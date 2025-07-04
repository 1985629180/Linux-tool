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
#include <ui.h>

int main(int argc, char **argv)
{
	pDisBuff ptBuffer;
	int error;
	Button tButton;
	Region tRegion;

	if(argc != 2)
	{
		printf("Usage: %s <font_size>\n", argv[0]);
		return -1;
	}

		DisplayInit();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	ptBuffer = GetDisplayBuffer();

	FontsRegister();
	
	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	tRegion.iLeftUpX = 200;
	tRegion.iLeftUpY = 200;
	tRegion.iWidth   = 300;
	tRegion.iHeight   = 100;
	
	InitButton(&tButton, "test", &tRegion, NULL, NULL);
	tButton.OnDraw(&tButton, ptBuffer);
	while (1)
	{
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);
	}
	
	return 0;
}

