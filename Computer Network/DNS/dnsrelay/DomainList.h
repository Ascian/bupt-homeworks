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
	struct LRUCacheNode* root;//��������ڵ㣬�洢��̬��;�̬���ṩ���ٲ�ѯ
	struct LRUCacheNode head;//˫�������ͷ,�洢��̬���ṩLRU����
	struct LRUCacheNode tail;//˫�������β
	int count;
	int capacity;
}DomainList;

//��ʼ�������б�
void initDomainList(DomainList* pDomainList, const int capacity);

//��������ṹ��,����ɹ�����1,���򷵻�0
int addDomain(DomainList* pDomainList, const Domain* pDomain);

//�ͷ�������
void deleteDomainList(DomainList* pDomainList);

//ˢ���������ж�̬��¼
void refreshDomianList(DomainList* pDomainList);

//���ҹؼ���ΪpKey�������ṹ�壬���ҵ�������ָ�룬���򷵻�NULL
Domain* findDomain(DomainList* pDomainList, const Str* pKey);

//���ļ��ж�ȡ�������ݣ�����debugLevelѡ���������
void loadDomainList(DomainList* pDomainList, const char* fileAddress, const int debugLevel);