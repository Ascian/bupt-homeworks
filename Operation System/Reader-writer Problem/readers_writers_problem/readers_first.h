#pragma once

#pragma comment(lib, "pthreadVC2.lib")
#include <semaphore.h>
#include <Windows.h>

#include "logging.h"
#include "order.h"

//控制线程数量的信号量
extern sem_t kQueueNum;
//输出流
extern Log kOutputLog;

namespace readers_first
{

//共享变量
static int kShareData;
//读者数量
static int kReadNum = 0; 
//控制对缓冲区的访问，初始化为 1
static sem_t kShareMutex; 
//控制 read_num 的互斥访问，初始化为 1
static sem_t kReadMutex; 

void init()
{
	sem_init(&kShareMutex, 0, 1);
	sem_init(&kReadMutex, 0, 1);
}

void* writer(void* param) 
{
	Order* order = (Order*)param;
	sem_wait(&kShareMutex);  //等待访问权限

	//打印输出W、id、spend_time，并将id写入kShareData
	kOutputLog << "\t\t\tW" << order->id  << order->spend_time << "\n";
	kShareData = order->id;
	Sleep(order->spend_time * 1000);

	sem_post(&kShareMutex);  //释放访问权限

	//释放线程
	order->is_used = false;
	sem_post(&kQueueNum);
	return nullptr;
}

void* reader(void* param) 
{
	Order* order = (Order*)param;
	sem_wait(&kReadMutex);  //互斥访问kReadNum
	kReadNum++;
	if (kReadNum == 1)   //如果是第一个读进程，申请获取访问权限
		sem_wait(&kShareMutex);
	sem_post(&kReadMutex);

	//打印输出R、id、spend_time，读取kShareData并打印输出
	kOutputLog << "\t\t\tR" << order->id << order->spend_time << kShareData << "\n";
	Sleep(order->spend_time * 1000);

	sem_wait(&kReadMutex);
	kReadNum--;
	if (kReadNum == 0)   //如果是最后一个读进程，释放访问权限
		sem_post(&kShareMutex);
	sem_post(&kReadMutex);

	//释放线程
	order->is_used = false;
	sem_post(&kQueueNum);
	return nullptr;
}

void destroy()
{
	sem_destroy(&kShareMutex);
	sem_destroy(&kReadMutex);
}

}
