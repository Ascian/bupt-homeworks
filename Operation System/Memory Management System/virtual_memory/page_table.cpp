#include "page_table.h"

PageTable::PageTable(const int* page_nums, const int& page_count)
	:page_count_(page_count)
{
	page_table_ = new Page[page_count];
	for (int i = 0; i < page_count; i++)
		page_table_[i] = { false, page_nums[i] };
}

const PageTable::CheckResult PageTable::GetPhysicalPageNum(
	int* physical_page_num, const int& virtual_page_num)
{
	if (virtual_page_num >= page_count_)
		return CheckResult::INVALID;
	*physical_page_num = page_table_[virtual_page_num].physical_page_num;
	if (page_table_[virtual_page_num].is_valid)
		return CheckResult::MAIN;
	else
		return CheckResult::SENCONDARY;
}

void PageTable::Release(vector<int>* main_pages,
	vector<int>* secondary_pages)
{
	for (int i = 0; i < page_count_; i++)
	{
		if (page_table_[i].is_valid)
			main_pages->push_back(page_table_[i].physical_page_num);
		else
			secondary_pages->push_back(page_table_[i].physical_page_num);
	}
}