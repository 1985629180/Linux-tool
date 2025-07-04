
#include <input_manager.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* socket
 * bind
 * sendto/recvfrom
 */

#define SERVER_PORT 8888

static int g_iSocketServer;

static int NetinputDeviceInit(void)
{
	struct sockaddr_in tSocketServerAddr;
	int iRet;

	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if(g_iSocketServer == -1)
	{
		printf("socket error\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}

	return 0;
}

static int NetinputDeviceExit(void)
{
	close(g_iSocketServer);
	return 0;
}


//UDP接收到的数据拷贝到结构体的str里
int NetinputGetInputEvent (pInputEvent ptInputEvent)
{
	struct sockaddr_in tSocketClientAddr;
    int  iRecLenBytes;
	char aRecvBuf[1000];

	unsigned int iAddrLen = sizeof(struct sockaddr);
	
	iRecLenBytes = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);

	if(iRecLenBytes > 0)
	{
		aRecvBuf[iRecLenBytes] = '\0';
		ptInputEvent->iType    = INPUT_TYPE_NET;
		gettimeofday(&ptInputEvent->tTime, NULL);
		strncpy(ptInputEvent->str, (const char *) aRecvBuf, 1000);
		ptInputEvent->str[999] = '\0';
		return 0;
	}
	else
		return -1;
}

static InputDeviceOpr g_tNetinputDev = {
	.name          = "netinput",
    .GetInputEvent  = NetinputGetInputEvent,
	.DeviceInit     = NetinputDeviceInit,
	.DeviceExit     = NetinputDeviceExit,
};


#if 1
int main(int argc, char** argv)
{
	InputEvent event;
	int ret;
	
	g_tNetinputDev.DeviceInit();

	while(1)
	{
		ret = g_tNetinputDev.GetInputEvent(&event);
		if(ret)
		{
			printf("get net input err!\n");
			return -1;
		}
		else
		{
			printf("Type: %d\n", event.iType);
			printf("Str:  %s\n", event.str);
		}

	}
	return 0;
}

#endif
