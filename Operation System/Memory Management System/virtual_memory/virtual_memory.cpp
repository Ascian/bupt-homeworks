#include "virtual_memory.h"

VirtualMemory::VirtualMemory(const int& page_count)
{
	for (int i = 0; i < page_count; i++)
		empty_pages_.push_back(i);
}

const bool VirtualMemory::Allocate(int* page_nums, const int& page_count)
{
	if (page_count > empty_pages_.size())
		return false;
	auto iter = empty_pages_.begin();
	for (int i = 0; i < page_count; i++)
	{
		page_nums[i] = *iter;
		iter = empty_pages_.erase(iter);
	}
	return true;
}