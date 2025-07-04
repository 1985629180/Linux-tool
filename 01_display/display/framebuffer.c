#include <dis_manager.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>


static int fd_fb;
static struct fb_var_screeninfo var;
static unsigned int lind_width;
static unsigned int pixel_width;
static unsigned int screen_size;
static unsigned char *fb_base;


//设备初始化
static int FbDeviceInit(void)
{
	
    fd_fb = open("/dev/fb0", O_RDWR);  //打开设备节点
    if(fd_fb < 0)
    {
        printf("can't open /dev/fb0\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))  //获得lcd信息
	{
		printf("can't get var\n");
		return -1;
	}
    lind_width = var.xres*var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;  //像素宽度
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    //mmap分配framebuffer内存
    fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	return 0;

}

static int FbDeviceExit(void)
{
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
} 

//将 framebuffer 的相关信息填充到传入的 ptDispBuff 结构中。
static int  FbGetBuffer(pDisBuff ptDisBuff) //从app处获得buffer
{
    ptDisBuff->iXres = var.xres;
	ptDisBuff->iYres = var.yres;
	ptDisBuff->iBpp  = var.bits_per_pixel;
	ptDisBuff->buff  = (char *)fb_base;  //把framebuffer内存赋给buff，上层可以直接调用就可以使用framebuffer
	return 0;
}

int  FbFlushRegion(pRegion ptRegion, pDisBuff ptDisBuff)   //将framebuffer的数据刷新到lcd
{
    return 0;
}

static DisOpr g_tFramebufferOpr = 
{
    .name        = "fb",
    .DeviceInit  = FbDeviceInit,
    .DeviceExit  = FbDeviceExit, 
    .GetBuffer   = FbGetBuffer,
    .FlushRegion = FbFlushRegion, 
};
void FramebufferInit(void)
{
	RegisterDisplay(&g_tFramebufferOpr);
}
