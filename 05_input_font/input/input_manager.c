#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include <input_manager.h>

static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;  //条件变量

static pInputDeviceOpr g_InputDevs  = NULL; //管理所有的全局链表



/***************************************实现环形buffer***********************************************/
#define BUFFER_LEN  20 //环形buffer
static int g_iRead = 0;  //读指针
static int g_iWrite = 0; //写指针
static InputEvent g_atInputEvent[BUFFER_LEN]; 

static int isInputBufferFull(void)
{
	return ((g_iWrite + 1) % BUFFER_LEN == g_iRead);
}

static int isInputBufferEmpty(void)
{
	return (g_iWrite == g_iRead);
}

static void PutInputEventToBuffer(pInputEvent ptInputEvent)
{
	if(!isInputBufferFull())
	{
		g_atInputEvent[g_iWrite] = *ptInputEvent;
		g_iWrite = (g_iWrite + 1) % BUFFER_LEN;
	}
}

static int GetInputEventFromBuffer(pInputEvent ptInputEvent)
{
	if(!isInputBufferEmpty())
	{
		*ptInputEvent = g_atInputEvent[g_iRead];
		g_iRead = (g_iRead + 1) % BUFFER_LEN;
		return 1;
	}	
	else
		return 0;
}
/****************************************************************************************************/

//注册设备函数
void RegisterInputDevice(pInputDeviceOpr ptInputDev)
{
	ptInputDev->pNext = g_InputDevs;
	g_InputDevs = ptInputDev;
}

//上层APP调用这个函数即可注册所有函数
void InputInit(void)
{
   //注册屏幕
	extern void TouchScreenRegister(void);
	TouchScreenRegister();

	//注册网络输入
	extern void NetinputRegister(void);
	NetinputRegister();
}

static void *input_recv_thread_func(void *data)
{
	pInputDeviceOpr ptInputDev = (pInputDeviceOpr)data;
	InputEvent tEvent;
	int ret;
	
	while(1)
	{
        //读数据存入tEvent中
		ret = ptInputDev->GetInputEvent(&tEvent);
		if(!ret)
		{
			pthread_mutex_lock(&g_tMutex);
			//把接收到的消息保存到环形buffer
			PutInputEventToBuffer(&tEvent);
			pthread_cond_signal(&g_tConVar);   //通知接收线程
			pthread_mutex_unlock(&g_tMutex);
		}

	}
	return NULL;
}


//初始化所有设备                                     
void InputDeviceInit(void)
 {
	int ret;
	pthread_t tid;   

	pInputDeviceOpr ptTmp  = g_InputDevs;

    while(ptTmp)
    {
		ret = ptTmp->DeviceInit();
		if(!ret)
		{
            //创建接收消息处理线程
			ret = pthread_create(&tid, NULL,input_recv_thread_func, ptTmp); //input_recv_thread_func是线程执行函数，ptTemp是要传入的参数
		}
		ptTmp = ptTmp->pNext;
	}
}

//提供给APP直接可以获取数据
int GetInputEvent(pInputEvent ptInputEvent)
{
	InputEvent tEvent;	
	int ret;

	pthread_mutex_lock(&g_tMutex);
	if(GetInputEventFromBuffer(&tEvent))
	{
	  //如果获得数据不用休眠，直接存入ptInputEvent
		*ptInputEvent = tEvent;
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	else
	{
		//没有获得数据则休眠,等待线程条件变量
		pthread_cond_wait(&g_tConVar, &g_tMutex);
		//有数据唤醒则开始获得数据
		if(GetInputEventFromBuffer(&tEvent))
		{
			*ptInputEvent = tEvent;
			 ret = 0;
		}
		else
			ret = -1;

		pthread_mutex_unlock(&g_tMutex);
	}
	
	return ret;
}	

