#include "memory_management_system.h"

const bool MemoryManagementSystem::AddProcess(const int& process_id, const int& page_count)
{
	int* allocated_pages = new int[page_count];
	//分配虚拟内存空间
	if (!virtual_memory_->Allocate(allocated_pages, page_count))
		return false;
	//设置进程页表
	PageTable* page_table = new PageTable(allocated_pages, page_count);
	processes_.insert({ process_id, page_table });
	return true;
}

const int MemoryManagementSystem::Access(const int& process_id,
	const int& virtual_page_num)
{
	auto process = processes_.find(process_id);
	int physical_page_num;
	PageTable::CheckResult result = process->second->GetPhysicalPageNum(
		&physical_page_num, virtual_page_num);
	if (result == PageTable::CheckResult::INVALID)
	{
		//虚拟页号非法
		kLog << "\t\t\tInvalid " << process_id << " " << virtual_page_num
			<< "\n";
		return INVALID;
	}
	if (result == PageTable::CheckResult::MAIN)
	{
		//虚拟页号对应的页在主存中
		kLog << "\t\t\tMain " << process_id << " " << virtual_page_num
			<< " " << physical_page_num << "\n";
		lru_list_.Push(physical_page_num);  //将该页置于LRU链表头部
		return physical_page_num;
	}
	else
	{
		//虚拟页号对应的页在虚存中
		int main_page_num = main_memory_->Allocate(process_id,
			virtual_page_num);  //分配主存空间
		if (main_page_num != FrameTable::MEMORY_FULL)
		{
			//主存空间未满，将虚存中的页调入主存
			kLog << "\t\t\tSecondary " << process_id << " "
				<< virtual_page_num << " " << main_page_num << "<-"
				<< physical_page_num << "\n";
			virtual_memory_->Release(physical_page_num);
		}
		else
		{
			//主存空间已满，将LRU链表尾部的页号去取出
			main_page_num = lru_list_.Pop();
			int replaced_process_id;
			int replaced_page_num;
			//修改帧表，交换页的所有者
			main_memory_->Swap(&replaced_process_id, &replaced_page_num,
				main_page_num, process_id, virtual_page_num);
			kLog << "\t\t\tSecondary " << process_id << " "
				<< virtual_page_num << " " << main_page_num << "<-("
				<< replaced_process_id << ", " << replaced_page_num
				<< ")\n";
			auto replaced_process = processes_.find(replaced_process_id);
			//设置原所有者的页表，将该页物理页号设为现所有者原虚存页号，并设置为无效
			replaced_process->second->SetPage(replaced_page_num, false,
				physical_page_num);
		}
		//修改现访问内存的进程的页表，将该页的物理页号设置为分配到的主存页号，并设置为有效
		process->second->SetPage(virtual_page_num, true, main_page_num);
		lru_list_.Push(main_page_num);  //将该页置于LRU链表头部
		return main_page_num;
	}
}

void MemoryManagementSystem::DeleteProcess(const int& process_id)
{
	vector<int> main_pages;
	vector<int> secondary_pages;
	auto process = processes_.find(process_id);
	process->second->Release(&main_pages, &secondary_pages);
	delete process->second;
	processes_.erase(process);
	main_memory_->Release(main_pages);
	virtual_memory_->Release(secondary_pages);
	lru_list_.Remove(main_pages);
}
