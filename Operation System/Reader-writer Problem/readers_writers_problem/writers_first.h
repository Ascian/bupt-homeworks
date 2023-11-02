#pragma once

#pragma comment(lib, "pthreadVC2.lib")
#include <semaphore.h>

#include "logging.h"
#include "order.h"

//控制线程数量的信号量
extern sem_t kQueueNum;
//输出流
extern Log kOutputLog;

namespace writers_first
{

//共享变量
static int kShareData;
//读者数量
static int kReadNum = 0;
//写者数量
static int kWriteNum = 0;
//控制对缓冲区的访问，初始化为1
static sem_t kShareMutex;
//控制访问缓冲区排队权限，初始化为1
static sem_t kQueueMutex;
//控制write_num的互斥访问，初始化为1
static sem_t kWriteMutex;
//控制read_num的互斥访问，初始化为1
static sem_t kReadMutex;

void init()
{
	sem_init(&kShareMutex, 0, 1);
	sem_init(&kQueueMutex, 0, 1);
	sem_init(&kWriteMutex, 0, 1);
	sem_init(&kReadMutex, 0, 1);
}

void* writer(void* param)
{
	Order* order = (Order*)param;
	sem_wait(&kWriteMutex);  //互斥访问kWriteNum
	kWriteNum++;
	if (kWriteNum == 1)  //如果是第一个写进程，申请获取访问缓冲区排队权限
		sem_wait(&kQueueMutex);
	sem_post(&kWriteMutex);
	sem_wait(&kShareMutex);  //申请缓冲区访问权限

	//打印输出W、id、spend_time，并将id写入kShareData
	kOutputLog << "\t\t\tW" << order->id << order->spend_time << "\n";
	kShareData = order->id;
	Sleep(order->spend_time * 1000);

	sem_post(&kShareMutex);
	sem_wait(&kWriteMutex);
	kWriteNum--;
	if (kWriteNum == 0)
		sem_post(&kQueueMutex);  //如果是最后一个写进程，释放访问缓冲区排队权限，允许读线程排队访问
	sem_post(&kWriteMutex);

	//释放线程
	order->is_used = false;
	sem_post(&kQueueNum);
	return nullptr;
}

void* reader(void* param)
{
	Order* order = (Order*)param;
	sem_wait(&kQueueMutex);  //申请访问缓冲区排队权限
	sem_wait(&kReadMutex);   //互斥访问kReadNum
	kReadNum++;
	if (kReadNum == 1)  //如果是第一个读者，申请访问权限
		sem_wait(&kShareMutex);
	sem_post(&kReadMutex);
	sem_post(&kQueueMutex);  //释放排队权限

	//打印输出R、id、spend_time，读取kShareData，并打印输出
	kOutputLog << "\t\t\tR" << order->id << order->spend_time << kShareData << "\n";
	Sleep(order->spend_time * 1000);

	sem_wait(&kReadMutex);
	kReadNum--;
	if (kReadNum == 0)  //如果是最后一个读者，释放缓冲区访问权限
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
	sem_destroy(&kQueueMutex);
	sem_destroy(&kWriteMutex);
	sem_destroy(&kReadMutex);
}

}
