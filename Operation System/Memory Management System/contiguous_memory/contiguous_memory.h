#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <Windows.h>

#include <set>
#include <vector>
#include <string>

#include "process.h"
#include "logging.h"

using namespace std;

//控制进程数量的信号量
extern sem_t kQueueNum;
//输出流
extern Log kLog;

class ContiguousMemory
{
public:
	//内存适配方案
	enum class Method
	{
		FIRST_FIT,
		BEST_FIT,
		WORST_FIT,
	};

	//@param
	//    const int& size - 内存大小
	// 	  const int& process_max - 进程最大数量
	//    const bool& is_preemptive - 是否允许抢占
	//    const Method& method - 分配方案
	ContiguousMemory(const int& size, const int& process_max,
		const bool& is_preemptive, const Method& method, const int& output_interval);

	~ContiguousMemory();

	//执行进程
	void Execute(Process* process);

private:
	struct Hole
	{
		bool operator<(const Hole& hole) const { return size < hole.size; }

		//起始地址
		int begin_index;
		//大小
		int size;
	};

	struct ThreadParam
	{
		bool is_used;
		ContiguousMemory* memory;
		Process* process;
	};

	class HoleList
	{
	public:
		//将孔加入孔序列
		virtual void AddHole(const Hole& hole) = 0;
		//分配大小为size的空间，返回该空间的起始地址
		virtual const int Allocate(const int& size) = 0;
		//向孔序列中添加起始地址为begin_index，大小为size的空间，孔序列会将该空间相邻的孔合并成一个更大的孔
		virtual const Hole MergeHole(const int& begin_index, const int& size) = 0;
		//输出孔队列状态，用于程序调试
		virtual const string ToString() = 0;
	};

	//首次适配孔序列
	class FirstFitHoleList : public HoleList
	{
	public:
		inline virtual void AddHole(const Hole& hole) { holes_.push_back(hole); }

		virtual const int Allocate(const int& size);

		virtual const Hole MergeHole(const int& begin_index, const int& size);

		virtual const string ToString();

	private:
		//无序孔序列
		vector<Hole> holes_;
	};

	//有序孔序列
	class OrderHoleList : public HoleList
	{
	public:
		inline virtual void AddHole(const Hole& hole) { holes_.insert(hole); }

		virtual const int Allocate(const int& size) = 0;

		virtual const Hole MergeHole(const int& begin_index, const int& size);

		virtual const string ToString();

	protected:
		//有序孔序列
		multiset<Hole> holes_;
	};

	//最优适配孔序列
	class BestFitHoleList : public OrderHoleList
	{
	public:
		virtual const int Allocate(const int& size);
	};

	//最差适配孔序列
	class WorstFitHoleList : public OrderHoleList
	{
	public:
		virtual const int Allocate(const int& size);
	};

	//运行程序，并等待程序运行结束
	//@param
	//    ThreadParam* param
	friend void* Run(void* param);

	//输出内存状态，包括孔集合和进程队列
	//@param
	//    ContiguousMemory* param
	friend void* OutputState(void* param);

	//释放进程空间
	void Release(Process* process);

	//执行进程队列中可以在hole的空间内执行的进程
	void TestQueue(const Hole& hole);

	//测试进程是否允许继续执行
	const bool TestProcess(Process* process);

	//是否允许抢占
	bool is_preemptive_;

	//孔序列
	HoleList* hole_list_;
	//进程队列
	vector<Process*> process_queue_;

	//内存中进程最大数量
	int process_max_;
	pthread_t* threads_;
	ThreadParam* thread_params_;
	pthread_mutex_t hole_list_mutex_;
	pthread_mutex_t process_queue_mutex_;

	//内存状态输出间隔
	int output_interval_;
	pthread_t output_thread_;
};

void* OutputState(void* param);

void* Run(void* param);