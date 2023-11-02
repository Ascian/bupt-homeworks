#include "contiguous_memory.h"

ContiguousMemory::ContiguousMemory(const int& size, const int& process_max, 
	const bool& is_preemptive, const Method& method, const int& output_interval)
	:is_preemptive_(is_preemptive), process_max_(process_max)
	, output_interval_(output_interval)
{
	switch (method)
	{
	case Method::FIRST_FIT:
		hole_list_ = new FirstFitHoleList();
		break;
	case Method::BEST_FIT:
		hole_list_ = new BestFitHoleList();
		break;
	case Method::WORST_FIT:
		hole_list_ = new WorstFitHoleList();
		break;
	}
	hole_list_->AddHole({ 1, size });  //初始孔

	threads_ = new pthread_t[process_max];
	thread_params_ = new ThreadParam[process_max];
	pthread_mutex_init(&hole_list_mutex_, nullptr);
	pthread_mutex_init(&process_queue_mutex_, nullptr);

	for (int i = 0; i < process_max; i++)
	{
		thread_params_[i].is_used = false;
		thread_params_[i].memory = this;
	}

	pthread_create(&output_thread_, nullptr, OutputState, this);
}

ContiguousMemory::~ContiguousMemory()
{
	delete hole_list_;
	delete[] threads_;
	delete[] thread_params_;
	pthread_mutex_destroy(&hole_list_mutex_);
	pthread_mutex_destroy(&process_queue_mutex_);
}

void ContiguousMemory::Execute(Process* process)
{
	//若不允许抢占，有进程在等待时，将该进程加入队列
	if (!is_preemptive_ && process_queue_.size() > 0)
	{
		pthread_mutex_lock(&process_queue_mutex_);
		process_queue_.push_back(process);
		pthread_mutex_unlock(&process_queue_mutex_);
		return;
	}

	//测试进程是否允许继续执行，若不行将进程加入进程队列
	if (!TestProcess(process))
	{
		pthread_mutex_lock(&process_queue_mutex_);
		process_queue_.push_back(process);
		pthread_mutex_unlock(&process_queue_mutex_);
	}
}

void* OutputState(void* param)
{
	ContiguousMemory* memory = (ContiguousMemory*)param;

	//每隔一段时间输出内存状态
	while (true)
	{
		Sleep(memory->output_interval_ * 1000);
		pthread_mutex_lock(&memory->hole_list_mutex_);
		string holes = memory->hole_list_->ToString();
		pthread_mutex_unlock(&memory->hole_list_mutex_);

		pthread_mutex_lock(&memory->process_queue_mutex_);
		string queue;
		for (int i = 0; i < memory->process_queue_.size(); i++)
			queue += " -> " + to_string(memory->process_queue_[i]->id);
		pthread_mutex_unlock(&memory->process_queue_mutex_);

		string delimeter(holes.size(), '-');
		kLog << "\t\t\t\t\t\t\t" << delimeter << "\n\t\t\t\t\t\t\t" << holes
			<< "\n\t\t\t\t\t\t\t" << delimeter << "\n\t\t\t\t\t\t\t" << queue 
			<< "\n";
	}
	return nullptr;
}

void ContiguousMemory::Release(Process* process)
{
	//将进程释放的空间加入空集合，并返回合并后的更大的孔
	pthread_mutex_lock(&hole_list_mutex_);
	Hole new_hole = hole_list_->MergeHole(process->memory_begin_index,
		process->memory_size);
	pthread_mutex_unlock(&hole_list_mutex_);

	kLog << "\t\t\tReclaim " << process->id << " (" << process->memory_begin_index
		<< ", " << process->memory_begin_index + process->memory_size - 1 << ") " << "\n";
	process->is_used = false;

	//依据合并后的大孔，取出进程队列可以在这一大孔中执行的进程
	TestQueue(new_hole);
}

void ContiguousMemory::TestQueue(const Hole& hole)
{
	Hole rest_hole = hole;
	pthread_mutex_lock(&process_queue_mutex_);
	if (is_preemptive_)
	{	
		//允许抢占情况下，测试整个进程队列
		for (int i = 0; i < process_queue_.size(); i++)
		{
			if (rest_hole.size >= process_queue_[i]->memory_size
				&& TestProcess(process_queue_[i]))
			{
				rest_hole.size -= process_queue_[i]->memory_size;
				process_queue_.erase(process_queue_.begin() + i);
				i--;
			}
		}
	}
	else
	{
		//不允许抢占情况下，从队首开始测试进程队列，测试到不满足的进程就退出
		for (int i = 0; i < process_queue_.size(); i++)
		{
			if (rest_hole.size < process_queue_[i]->memory_size
				|| !TestProcess(process_queue_[i]))
				break;
			rest_hole.size -= process_queue_[i]->memory_size;
			process_queue_.erase(process_queue_.begin() + i);
			i--;
		}	
	}
	pthread_mutex_unlock(&process_queue_mutex_);

}

const bool ContiguousMemory::TestProcess(Process* process)
{
	//为进程分配内存空间
	pthread_mutex_lock(&hole_list_mutex_);
	process->memory_begin_index = hole_list_->Allocate(process->memory_size);
	pthread_mutex_unlock(&hole_list_mutex_);

	if (process->memory_begin_index == 0)  //内存分配失败
		return false;
		
	//内存分配成功
	kLog << "\t\t\tAllocate " << process->id << " (" << process->memory_begin_index
		<< ", " << process->memory_begin_index + process->memory_size - 1 << ") " 
		<< process->spend_time << "\n";

	//寻找可用线程
	int i = 0;
	for (; i < process_max_; i++)
	{
		if (!thread_params_[i].is_used)
			break;
	}
	thread_params_[i].is_used = true;
	thread_params_[i].process = process;
	pthread_create(&threads_[i], nullptr, Run, &thread_params_[i]);
	return true;
}

void* Run(void* param)
{
	ContiguousMemory::ThreadParam* tp = (ContiguousMemory::ThreadParam*)param;
	Sleep(tp->process->spend_time * 1000);  //等待进程执行结束
	tp->memory->Release(tp->process);  //释放进程使用内存空间
	tp->is_used = false;
	sem_post(&kQueueNum);
	return nullptr;
}

const int ContiguousMemory::FirstFitHoleList::Allocate(const int& size)
{
	//从头查找第一个满足要求的孔
	for (int i = 0; i < holes_.size(); i++)
	{
		if (holes_[i].size > size)
		{
			holes_[i].size -= size;
			return holes_[i].begin_index + holes_[i].size;
		}
		if (holes_[i].size == size)
		{
			int result = holes_[i].begin_index;
			holes_.erase(holes_.begin() + i);
			return result;
		}
	}
	return 0;
}

const ContiguousMemory::Hole ContiguousMemory::FirstFitHoleList::MergeHole(
	const int& begin_index, const int& size)
{
	Hole new_hole = { begin_index, size };
	int merge_times = 0;  //最多只可能合并两次，即前后相邻的孔
	for (int i = 0; i < holes_.size() && merge_times < 2; )
	{
		if (holes_[i].begin_index == new_hole.begin_index + new_hole.size)
		{
			//后相邻的孔
			new_hole.size = holes_[i].size + new_hole.size;
			holes_.erase(holes_.begin() + i);
			merge_times++;
		}
		else if (holes_[i].begin_index + holes_[i].size == new_hole.begin_index)
		{
			//前相邻的孔
			new_hole.begin_index = holes_[i].begin_index;
			new_hole.size = holes_[i].size + new_hole.size;
			holes_.erase(holes_.begin() + i);
			merge_times++;
		}
		else
			i++;
	}
	holes_.push_back(new_hole);  //将新孔加入孔序列
	return new_hole;
}

const string ContiguousMemory::FirstFitHoleList::ToString()
{
	string holes;
	for (Hole hole : holes_)
		holes += "(" + to_string(hole.begin_index) + ", "
		+ to_string(hole.begin_index + hole.size - 1) + ") ";
	return holes;
}


const int ContiguousMemory::BestFitHoleList::Allocate(const int& size)
{
	//查找最小的满足要求的孔
	auto smallest_hole = holes_.lower_bound({ 0, size });
	if (smallest_hole == holes_.end())
		return 0;
	if (smallest_hole->size > size)
	{
		int result = smallest_hole->begin_index + smallest_hole->size - size;
		holes_.insert({ smallest_hole->begin_index, smallest_hole->size - size });
		holes_.erase(smallest_hole);
		return result;
	}
	if (smallest_hole->size == size)
	{
		int result = smallest_hole->begin_index;
		holes_.erase(smallest_hole);
		return result;
	}
}

const int ContiguousMemory::WorstFitHoleList::Allocate(const int& size)
{
	//查找最大的满足要求的孔
	auto biggest_hole = prev(holes_.end());
	if (biggest_hole->size > size)
	{
		int result = biggest_hole->begin_index + biggest_hole->size - size;
		holes_.insert({ biggest_hole->begin_index, biggest_hole->size - size });
		holes_.erase(biggest_hole);
		return result;
	}
	if (biggest_hole->size == size)
	{
		int result = biggest_hole->begin_index;
		holes_.erase(biggest_hole);
		return result;
	}
	return 0;
}

const ContiguousMemory::Hole ContiguousMemory::OrderHoleList::MergeHole(
	const int& begin_index, const int& size)
{
	Hole new_hole = { begin_index, size };
	int merge_times = 0;  //最多只可能合并两次，即前后相邻的孔
	auto iter = holes_.begin();
	while (iter != holes_.end() && merge_times < 2) 
	{
		if (iter->begin_index == new_hole.begin_index + new_hole.size)
		{
			//后相邻的孔
			new_hole.size = iter->size + new_hole.size;
			iter = holes_.erase(iter);
			merge_times++;
		}
		else if (iter->begin_index + iter->size == new_hole.begin_index)
		{
			//前相邻的孔
			new_hole.begin_index = iter->begin_index;
			new_hole.size = iter->size + new_hole.size;
			iter = holes_.erase(iter);
			merge_times++;
		}
		else
			iter++;
	}
	holes_.insert(new_hole);  //将新孔加入孔序列
	return new_hole;
}

const string ContiguousMemory::OrderHoleList::ToString()
{
	string holes;
	for (Hole hole : holes_)
		holes += "(" + to_string(hole.begin_index) + ", "
		+ to_string(hole.begin_index + hole.size - 1) + ") ";
	return holes;
}
