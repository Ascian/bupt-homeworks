#pragma once

#include <vector>
#include <unordered_map>

#include "logging.h"
#include "page_table.h"
#include "virtual_memory.h"
#include "lru_list.h"
#include "frame_table.h"

using namespace std;

extern Log kLog;

class MemoryManagementSystem
{
public:
	enum AccessResult
	{
		INVALID = -1,
	};

	MemoryManagementSystem(const int& main_page_count, 
		const int& secondary_page_count)
	{
		main_memory_ = new FrameTable(main_page_count);
		virtual_memory_ = new VirtualMemory(secondary_page_count);
	}

	~MemoryManagementSystem()
	{
		delete main_memory_;
		delete virtual_memory_;
	}

	//添加进程到内存中
	const bool AddProcess(const int& process_id, const int& page_count);

	//依据进程虚拟页号获取物理页号
	const int Access(const int& process_id, const int& virtual_page_num);

	//释放进程空间
	void DeleteProcess(const int& process_id);

private:
	unordered_map<int, PageTable*> processes_;  //每个进程的页表，进程ID->页表
	FrameTable* main_memory_;  //主存帧表
	VirtualMemory* virtual_memory_;  //虚存
	LruList lru_list_;  //LRU链表，用于页面置换
};
