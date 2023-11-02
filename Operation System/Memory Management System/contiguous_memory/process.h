#pragma once

struct Process
{
	bool is_used;
	int id;
	int spend_time;
	int memory_begin_index;
	int memory_size;
};