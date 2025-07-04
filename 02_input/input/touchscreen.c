#include <input_manager.h>
#include <tslib.h>
#include <stdio.h>


static struct tsdev *g_ts;

static int TouchscreenDeviceInit(void)
{
	g_ts = ts_setup(NULL, 0);	
	if (!g_ts) {
		perror("ts_open");
		return -1;
	}
	return 0;
}

static int TouchscreenDeviceExit(void)
{
	ts_close(g_ts);
	return 0;
}

static int TouchscreenGetInputEvent(pInputEvent ptInputEvent)
{
	struct ts_sample samp;
	int ret;

	ret = ts_read(g_ts, &samp, 1);

	if (ret != 1)
		return -1;

	ptInputEvent->iType = INPUT_TYPE_TOUCH;
	ptInputEvent->iX        = samp.x;
	ptInputEvent->iY        = samp.y;
	ptInputEvent->iPressure = samp.pressure;
	ptInputEvent->tTime     = samp.tv;

	return 0;

}

static InputDeviceOpr g_tTouchscreenDev = {
	.name          = "touchscreen",
    .GetInputEvent = TouchscreenGetInputEvent, 
    .DeviceInit    = TouchscreenDeviceInit,
    .DeviceExit    = TouchscreenDeviceExit,
};
#if 1

int main(int argc, char **argv)
{
	InputEvent event;
	int ret;
	
	g_tTouchscreenDev.DeviceInit();

	while (1)
	{
		ret = g_tTouchscreenDev.GetInputEvent(&event);
		if (ret) {
			printf("GetInputEvent err!\n");
			return -1;
		}
		else
		{
			printf("Type      : %d\n", event.iType);
			printf("iX        : %d\n", event.iX);
			printf("iY        : %d\n", event.iY);
			printf("iPressure : %d\n", event.iPressure);
		}
	}
	return 0;
}

#endif
