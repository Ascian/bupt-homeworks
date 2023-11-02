#pragma once

#include <vector>

using namespace std;

class FrameTable
{
public:
	enum AllocateResult
	{
		MEMORY_FULL = -1,
	};

	explicit FrameTable(const int& page_count);

	//分配一个空闲帧
	//@param
	//    const int& process_id - 需要获取帧的进程ID
	//    const int& virtual_page_num - 该进程的虚拟页码
	//@return
	//    返回分配到的物理页码，若主存已满返回MEMORY_FULL
	const int Allocate(const int& process_id, const int& virtual_page_num);

	//将page_nums中的页加入空闲帧
	void Release(const vector<int>& page_nums);

	//交换页的所有者
	//@param
	//    int* process_out - 换出的进程
	//    int* page_num_out - 换出进程的虚拟页码
	//    const int& page_num - 主存物理页码
	//    const int& process_in - 换入的进程
	//    const int& page_num_in - 换入进程的虚拟页码
	inline void Swap(int* process_out, int* page_num_out, const int& page_num,
		const int& process_in, const int& page_num_in)
	{
		*process_out = frame_table_[page_num].process_id;
		*page_num_out = frame_table_[page_num].virtual_page_num;
		frame_table_[page_num].process_id = process_in;
		frame_table_[page_num].virtual_page_num = page_num_in;
	}

private:
	struct Frame
	{
		int process_id;  //占用了该帧的进程ID
		int virtual_page_num;   //进程的虚拟页码
		int next_empty;  //下一个空闲帧的序号，若该帧为最后一个空闲帧则设置为-1
	};

	int page_count_;  //帧数
	Frame* frame_table_;  //帧表
	int first_empty_;  //第一个空闲帧的序号，若主存无空闲帧设置为-1
};

