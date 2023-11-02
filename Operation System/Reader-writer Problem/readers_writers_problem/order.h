#pragma once

struct Order
{
	//表示该结构体是否正在被使用中
	bool is_used;
	//标识id
	int id;
	//读或写花费时间
	int spend_time;
};