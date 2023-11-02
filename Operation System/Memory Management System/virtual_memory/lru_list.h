#pragma once

#include <unordered_map>
#include <vector>

using namespace std;

class LruList
{
public:
	LruList()
	{
		first_node_.next = &last_node_;
		last_node_.prev = &first_node_;
	}

	//若键为id的结点不存在，插入id到LRU链表表头；否则将键为id的结点移至表头
	void Push(const int& id);

	//删除表尾并返回表尾的键
	inline const int Pop()
	{
		int result = last_node_.prev->id;
		last_node_.prev->prev->next = &last_node_;
		last_node_.prev = last_node_.prev->prev;
		list_.erase(result);
		return result;
	}

	//删除键为ids中的所有结点
	void Remove(const vector<int>& ids);

private:
	struct Node
	{
		int id;
		Node* prev;
		Node* next;
	};

	unordered_map<int, Node> list_;
	Node first_node_;
	Node last_node_;
};

