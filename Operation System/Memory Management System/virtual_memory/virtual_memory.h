#pragma once

#include <vector>

using namespace std;

class VirtualMemory
{
public:
	explicit VirtualMemory(const int& page_count);

	//分配虚拟内存空间
	//@parma
	//    int* page_nums - 返回的虚拟内存物理页码
	//    const int& page_count - 所需分配的页数
	//@return
	//    若虚拟内存空间虚拟内存空间不能满足要求，返回false，否则返回true
	const bool Allocate(int* page_nums, const int& page_count);

	//将page_num加入空闲帧序列
	inline void Release(const int& page_num)
	{
		empty_pages_.push_back(page_num);
	}

	//将page_nums加入空闲帧序列
	inline void Release(const vector<int>& page_nums)
	{
		empty_pages_.insert(empty_pages_.end(), page_nums.begin(), page_nums.end());
	}

private:
	vector<int> empty_pages_;  //空闲帧序列
};

