#include "lru_list.h"

void LruList::Push(const int& id)
{
	auto iter = list_.find(id);
	if (iter != list_.end())
	{
		//若键为`id`的结点不存在，插入键为`id`的结点到LRU链表表头
		iter->second.prev->next = iter->second.next;
		iter->second.next->prev = iter->second.prev;
		iter->second.next = first_node_.next;
		first_node_.next->prev = &iter->second;
		first_node_.next = &iter->second;
		iter->second.prev = &first_node_;
	}
	else
	{
		//将键为`id`的结点移至表头
		Node node = { id, &first_node_, first_node_.next };
		auto inserted_iter = list_.insert({ id, node });
		first_node_.next->prev = &inserted_iter.first->second;
		first_node_.next = &inserted_iter.first->second;
	}
}

void LruList::Remove(const vector<int>& ids)
{
	for (int id : ids)
	{
		auto iter = list_.find(id);
		iter->second.prev->next = iter->second.next;
		iter->second.next->prev = iter->second.prev;
		list_.erase(iter);
	}
}
