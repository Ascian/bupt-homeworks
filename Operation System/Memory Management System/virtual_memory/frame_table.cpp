#include "frame_table.h"

FrameTable::FrameTable(const int& page_count)
	:page_count_(page_count)
{
	frame_table_ = new Frame[page_count];
	frame_table_[page_count - 1].next_empty = MEMORY_FULL;
	for (int i = page_count - 2; i >= 0; i--)
		frame_table_[i].next_empty = i + 1;
	first_empty_ = 0;
}

const int FrameTable::Allocate(const int& process_id, 
	const int& virtual_page_num)
{
	if (first_empty_ == MEMORY_FULL)
		return MEMORY_FULL;
	int result = first_empty_;
	first_empty_ = frame_table_[first_empty_].next_empty;
	frame_table_[result].process_id = process_id;
	frame_table_[result].virtual_page_num = virtual_page_num;
	frame_table_[result].next_empty = MEMORY_FULL;
	return result;
}

void FrameTable::Release(const vector<int>& page_nums)
{
	for (int page_num : page_nums)
	{
		frame_table_[page_num].next_empty = first_empty_;
		first_empty_ = page_num;
	}
}
