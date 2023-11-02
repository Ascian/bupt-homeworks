#pragma once

#include <vector>

using namespace std;

class PageTable
{
public:
	//GetPhysicalPageNum函数返回值
	enum class CheckResult
	{
		MAIN,  //物理页码指向主存空间
		SENCONDARY,  //物理页码指向虚存空间
		INVALID,  //虚拟页码非法
	};

	//@param
	//    const int* page_nums - 初始虚存物理页码数组
	//    const int& page_count - 页数
	PageTable(const int* page_nums, const int& page_count);

	~PageTable()
	{
		delete[] page_table_;
	}

	//获取物理页码
	//@param
	//    int* physical_page_num - 返回的物理页码
	//    const int& virtual_page_num - 虚拟页码
	//@return
	//    返回虚拟页码对应的物理页码是否有效
	const CheckResult GetPhysicalPageNum(int* physical_page_num,
		const int& virtual_page_num);

	//设置页表表项
	inline void SetPage(const int& virtual_page_num, const bool& is_valid, 
		const int& physical_page_num)
	{
		page_table_[virtual_page_num] = { is_valid, physical_page_num };
	}

	//释放进程，返回进程占用的主存页码和虚存页码
	//@param
	//    vector<int>* main_pages - 返回的主存页码集合
	//    vector<int>* secondary_pages - 返回的虚存页码集合
	void Release(vector<int>* main_pages,
		vector<int>* secondary_pages);

private:
	struct Page
	{
		//有效表示该页已经存放在主存内，物理页码指向主存空间的页；
		//无效表示该页存放在虚拟内存空间，物理页码指向虚存空间的页
		bool is_valid;  //无效/有效位
		int physical_page_num;  //物理页码
	};

	int page_count_;  //页数
	Page* page_table_; //页表
};

