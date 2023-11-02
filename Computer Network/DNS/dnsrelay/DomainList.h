#pragma once
#include<stdio.h>
#include<stdlib.h>
#include"Domain.h"
#include"Str.h"
#define RED 1
#define BLACK 0

typedef struct LRUCacheNode {
	Domain data;
	Str key;
	char color;
	struct LRUCacheNode* lchild;
	struct LRUCacheNode* rchild;
	struct LRUCacheNode* next;
	struct LRUCacheNode* pre;
}CacheNode, * RBTree;

typedef struct DomainList {
	struct LRUCacheNode* root;//红黑树根节点，存储动态表和静态表，提供快速查询
	struct LRUCacheNode head;//双向链表表头,存储动态表，提供LRU机制
	struct LRUCacheNode tail;//双向链表表尾
	int count;
	int capacity;
}DomainList;

//初始化域名列表
void initDomainList(DomainList* pDomainList, const int capacity);

//添加域名结构体,加入成功返回1,否则返回0
int addDomain(DomainList* pDomainList, const Domain* pDomain);

//释放域名表
void deleteDomainList(DomainList* pDomainList);

//刷新域名表中动态记录
void refreshDomianList(DomainList* pDomainList);

//查找关键字为pKey的域名结构体，若找到返回域指针，否则返回NULL
Domain* findDomain(DomainList* pDomainList, const Str* pKey);

//从文件中读取域名数据，根据debugLevel选择输出内容
void loadDomainList(DomainList* pDomainList, const char* fileAddress, const int debugLevel);