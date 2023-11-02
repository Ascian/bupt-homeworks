#include "DomainList.h"

int taller = 0;
int shorter = 0;

//在红黑树中寻找关键字为key的节点，若找到返回节点数据指针，否则返回NULL
CacheNode* searchRBTree(const RBTree T, const Str* pKey) {
	RBTree root = T;
	while (root) {
		if (!compareStr(pKey, &root->key))//找到关键字为key的节点
			return root;
		else if (compareStr(pKey, &root->key) < 0)//在左子树中继续寻找
			return searchRBTree(root->lchild, pKey);
		root = root->rchild;
	}
	return NULL;
}

void clearRBTree(RBTree T) {
	//释放整个红黑树
	clearRBTree(T->lchild);
	clearRBTree(T->rchild);
	deleteDomain(&T->data);
	deleteStr(&T->key);
	free(T);
}

void r_Rotate(RBTree* pT) {
	//对pT指向的红黑树作右旋处理
	RBTree lc = (*pT)->lchild;
	(*pT)->lchild = lc->rchild;
	lc->rchild = (*pT);
	(*pT) = lc;
}//r_Rotate

void l_Rotate(RBTree* pT) {
	//对pT指向的红黑树作左旋处理
	RBTree rc = (*pT)->rchild;
	(*pT)->rchild = rc->lchild;
	rc->lchild = (*pT);
	(*pT) = rc;
}//l_Rotate

CacheNode* insertRBTree(RBTree* pT, const Str* pKey, const Domain* pData) {
	//若在红黑树pT中不存在和key相同关键字的结点，则插入一个数据元素
	//为data的新节点,若因插入而使红黑树失去平衡，则作平衡旋转处理
	CacheNode* result;
	if (!(*pT)) {
		(*pT) = (RBTree)malloc(sizeof(CacheNode));
		if ((*pT) == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
		initDomain(&(*pT)->data);
		initStr(&(*pT)->key);
		copyDomain(&(*pT)->data, pData);
		copyStr(&(*pT)->key, pKey);
		(*pT)->lchild = NULL;
		(*pT)->rchild = NULL;
		(*pT)->next = NULL;
		(*pT)->pre = NULL;
		(*pT)->color = RED;
		taller = 1;
		return *pT;
	}
	else {
		if (!compareStr(pKey, &(*pT)->key)) {
			//树中已存在和key相同关键字的节点
			taller = 0;
			return NULL;
		}
		else if (compareStr(pKey, &(*pT)->key) < 0) {
			//应在左子树中继续寻找
			result = insertRBTree(&((*pT)->lchild), pKey, pData);
			if (!result) return NULL;
			if (taller)
				if ((*pT)->color == BLACK) {
					if ((*pT)->lchild->color == RED && (*pT)->lchild->lchild != NULL && (*pT)->lchild->lchild->color == RED) {
						//左孩子和左孩子的左孩子都为红色
						if ((*pT)->rchild == NULL || (*pT)->rchild->color == BLACK) {
							//右孩子为黑，作右旋处理，改变taller状态
							(*pT)->lchild->color = BLACK;
							(*pT)->color = RED;
							r_Rotate(pT);
							taller = 0;
						}//if
						else {
							//右孩子为红色，仅作颜色改变，不改变taller状态
							(*pT)->lchild->color = BLACK;
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
						}
					}//if
					else if ((*pT)->lchild->color == RED && (*pT)->lchild->rchild != NULL && (*pT)->lchild->rchild->color == RED) {
						//左孩子和左孩子的右孩子都为红色
						if ((*pT)->rchild == NULL || (*pT)->rchild->color == BLACK) {
							//右孩子为黑色，作双旋处理
							l_Rotate(&(*pT)->lchild);
							(*pT)->lchild->color = BLACK;
							(*pT)->color = RED;
							r_Rotate(pT);
							taller = 0;
						}
						else {
							//右孩子为红色，仅作颜色改变，不改变taller状态
							(*pT)->lchild->color = BLACK;
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
						}//if
					}//else if
				}//if
		}//else if
		else {
			//应在右子树中继续寻找
			result = insertRBTree(&((*pT)->rchild), pKey, pData);
			if (!result) return NULL;
			if (taller)
				if ((*pT)->color == BLACK) {
					if ((*pT)->rchild->color == RED && (*pT)->rchild->rchild != NULL && (*pT)->rchild->rchild->color == RED) {
						//右孩子和右孩子的右孩子都为红色
						if ((*pT)->lchild == NULL || (*pT)->lchild->color == BLACK) {
							//左孩子为黑，作左旋处理，改变taller状态
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
							l_Rotate(pT);
							taller = 0;
						}//if
						else {
							//左孩子为红色，仅作颜色改变，不改变taller状态
							(*pT)->lchild->color = BLACK;
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
						}//else
					}//if
					else if ((*pT)->rchild->color == RED && (*pT)->rchild->lchild != NULL && (*pT)->rchild->lchild->color == RED) {
						//右孩子和右孩子的左孩子都为红色
						if ((*pT)->lchild == NULL || (*pT)->lchild->color == BLACK) {
							//左孩子为黑色，作双旋处理
							r_Rotate(&(*pT)->rchild);
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
							l_Rotate(pT);
							taller = 0;
						}
						else {
							//左孩子为红色，仅作颜色改变，不改变taller状态
							(*pT)->lchild->color = BLACK;
							(*pT)->rchild->color = BLACK;
							(*pT)->color = RED;
						}//if
					}//else if
				}//if
		}//else
	}//else
	return result;
}

CacheNode* addRBTree(RBTree* pT, const Str* pKey, const Domain* pData) {
	CacheNode* result = insertRBTree(pT, pKey, pData);
	(*pT)->color = BLACK;//修改根结点颜色为黑
	return result;
}

CacheNode* deleteRBTree(RBTree* pT, RBTree parent, const Str* pKey) {
	// 若在以T为根的红黑树中不存在和key有相同关键字的节点，返回false;否则删除该元素，并true；
	// 若因删除而使红黑树失去平衡，则作平衡旋转处理
	// pr为T双亲
	CacheNode* result;
	if (!(*pT))  return NULL;//不存在该元素
	else if (!compareStr(pKey, &(*pT)->key)) {
		//找到元素节点
		if (!(*pT)->lchild) {
			//左子树为空，将右子树接到双亲上			
			if (!parent || (*pT)->color == RED || ((*pT)->rchild != NULL && (*pT)->rchild->color == RED)) {
				//*pT为根节点，或者*pT和其右孩子中有一个为红色，不做平衡调整
				if ((*pT)->rchild != NULL)
					(*pT)->rchild->color = BLACK;
				shorter = 0;
			}
			else {
				//*pT不为根节点，且*pT和其右孩子都为黑色，作平衡调整
				shorter = 1;
			}
			result = *pT;
			*pT = (*pT)->rchild;
			return result;//直接返回结果
		}
		else if (!(*pT)->rchild) {
			//右子树为空，将左子树接到双亲上	
			if (!parent || (*pT)->color == RED || ((*pT)->lchild != NULL && (*pT)->lchild->color == RED)) {
				//*pT为根节点，或者*pT和其左孩子中有一个为红色，不做平衡调整
				shorter = 0;
				if ((*pT)->lchild != NULL)
					(*pT)->lchild->color = BLACK;
			}
			else {
				//*pT不为根节点，且*pT和其左孩子都为黑色，作平衡调整
				shorter = 1;
			}
			result = *pT;
			*pT = (*pT)->lchild;
			return result;//直接返回结果
		}
		else {
			//左右子树都存在，将左子树的的尽头右子树值赋给T
			RBTree temp = (*pT)->lchild;
			RBTree tempParent = *pT;
			//替换左子树中关键字最大的节点
			while (temp->rchild != NULL) {
				tempParent = temp;
				temp = temp->rchild;
			}
			//颜色不作替换
			char color = (*pT)->color;
			(*pT)->color = temp->color;
			temp->color = color;
			CacheNode* tempLchild = temp->lchild;
			if (tempParent != *pT) {
				temp->lchild = (*pT)->lchild;
				temp->rchild = (*pT)->rchild;
				tempParent->rchild = *pT;
				*pT = temp;
				tempParent->rchild->lchild = tempLchild;
				tempParent->rchild->rchild = NULL;
			}
			else {
				temp->rchild = (*pT)->rchild;
				temp->lchild = *pT;
				(*pT)->rchild = NULL;
				(*pT)->lchild = tempLchild;
				*pT = temp;
			}
			/*跳转至在左子树中寻找*/
		}//else
	}//else if
	else if (compareStr(pKey, &(*pT)->key) > 0) {
		//在右子中寻找
		result = deleteRBTree(&((*pT)->rchild), *pT, pKey);
		if (!result)  return NULL;
		if (shorter) {
			//右孩子为双黑节点
			if ((*pT)->lchild->color == BLACK) {
				//左孩子为黑色
				if ((*pT)->lchild->lchild != NULL && (*pT)->lchild->lchild->color == RED) {
					//左孩子的左孩子为红色
					(*pT)->lchild->lchild->color = BLACK;
					(*pT)->lchild->color = (*pT)->color;
					r_Rotate(pT);
					(*pT)->rchild->color = BLACK;
					shorter = 0;
				}//if
				else if ((*pT)->lchild->rchild != NULL && (*pT)->lchild->rchild->color == RED) {
					//左孩子的右孩子为红色
					(*pT)->lchild->rchild->color = (*pT)->color;
					l_Rotate(&(*pT)->lchild);
					r_Rotate(pT);
					(*pT)->rchild->color = BLACK;
					shorter = 0;
				}//else if
				else {
					//兄弟节点的左右孩子都为黑色
					if ((*pT)->color == RED) {
						//*pT为红色
						(*pT)->color = BLACK;
						(*pT)->lchild->color = RED;
						shorter = 0;
					}
					else {
						//*pT为黑色
						(*pT)->lchild->color = RED;
					}
				}//else
			}//if
			else {
				//左孩子为红色
				r_Rotate(pT);
				(*pT)->color = BLACK;
				(*pT)->rchild->color = RED;
				RBTree* pRc = &(*pT)->rchild;
				if ((*pRc)->lchild->lchild != NULL && (*pRc)->lchild->lchild->color == RED) {
					//pRc左孩子的左孩子为红色
					(*pRc)->lchild->lchild->color = BLACK;
					(*pRc)->lchild->color = (*pRc)->color;
					r_Rotate(pRc);
					(*pRc)->rchild->color = BLACK;
				}//if
				else if ((*pRc)->lchild->rchild != NULL && (*pRc)->lchild->rchild->color == RED) {
					//pRc左孩子的右孩子为红色
					(*pRc)->lchild->rchild->color = (*pRc)->color;
					l_Rotate(&(*pRc)->lchild);
					r_Rotate(pRc);
					(*pRc)->rchild->color = BLACK;
					shorter = 0;
				}//else if
				else {
					(*pRc)->color = BLACK;
					(*pRc)->lchild->color = RED;
				}//else
				shorter = 0;
			}//else
		}//if
		return result;//直接返回结果
	}//else if

	//在左子树中寻找
	result = deleteRBTree(&((*pT)->lchild), *pT, pKey);
	if (!result)  return NULL;
	if (shorter) {
		//左孩子为双黑节点
		if ((*pT)->rchild->color == BLACK) {
			//右孩子为黑色
			if ((*pT)->rchild->rchild != NULL && (*pT)->rchild->rchild->color == RED) {
				//右孩子的右孩子为红色
				(*pT)->rchild->rchild->color = BLACK;
				(*pT)->rchild->color = (*pT)->color;
				l_Rotate(pT);
				(*pT)->lchild->color = BLACK;
				shorter = 0;
			}//if
			else if ((*pT)->rchild->lchild != NULL && (*pT)->rchild->lchild->color == RED) {
				//右孩子的左孩子为红色
				(*pT)->rchild->lchild->color = (*pT)->color;
				r_Rotate(&(*pT)->rchild);
				l_Rotate(pT);
				(*pT)->lchild->color = BLACK;
				shorter = 0;
			}//else if
			else {
				//兄弟节点的左右孩子都为黑色
				if ((*pT)->color == RED) {
					//*pT为红色
					(*pT)->color = BLACK;
					(*pT)->rchild->color = RED;
					shorter = 0;
				}
				else {
					//*pT为黑色
					(*pT)->rchild->color = RED;
				}
			}//else
		}//if
		else {
			//右孩子为红色
			l_Rotate(pT);
			(*pT)->color = BLACK;
			(*pT)->lchild->color = RED;
			RBTree* pLc = &(*pT)->lchild;
			if ((*pLc)->rchild->rchild != NULL && (*pLc)->rchild->rchild->color == RED) {
				//pLc右孩子的右孩子为红色
				(*pLc)->rchild->rchild->color = BLACK;
				(*pLc)->rchild->color = (*pLc)->color;
				l_Rotate(pLc);
				(*pLc)->lchild->color = BLACK;
			}//if
			else if ((*pLc)->rchild->lchild != NULL && (*pLc)->rchild->lchild->color == RED) {
				//pLc右孩子的左孩子为红色
				(*pLc)->rchild->lchild->color = (*pLc)->color;
				r_Rotate(&(*pLc)->rchild);
				l_Rotate(pLc);
				(*pLc)->lchild->color = BLACK;
			}//else if
			else {
				//pLc右孩子的左右孩子都为黑色
				(*pLc)->color = BLACK;
				(*pLc)->rchild->color = RED;
			}//else
			shorter = 0;
		}//else
	}//if

	return result;
}

void addNode(CacheNode* pHead, CacheNode* pNode) {
	//将节点加入链表
	pNode->pre = pHead;
	pNode->next = pHead->next;
	pHead->next->pre = pNode;
	pHead->next = pNode;
}

void removeNode(CacheNode* pNode) {
	//删除节点
	pNode->next->pre = pNode->pre;
	pNode->pre->next = pNode->next;
}

void moveToHead(CacheNode* pHead, CacheNode* pNode) {
	//将节点移动至表头
	removeNode(pNode);
	addNode(pHead, pNode);
}

CacheNode* popTail(CacheNode* pTail)
//将表尾删除，并返回表尾指针
{
	CacheNode* result = pTail->pre;
	removeNode(result);
	return result;
}

void initDomainList(DomainList* pDomainList, const int capacity)
{
	pDomainList->root = NULL;
	pDomainList->head.next = &pDomainList->tail;
	pDomainList->head.pre = NULL;
	pDomainList->tail.next = NULL;
	pDomainList->tail.pre = &pDomainList->head;
	pDomainList->count = 0;
	pDomainList->capacity = capacity;
}


void loadDomainList(DomainList* pDomainList, const char* filePath, const int debugLevel) {
	printf("Try to load table \"%s\"", filePath);
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("...Ignored\n");
		return;
	}
	else {
		printf("...OK\n");
	}

	int count = 0;
	int id[4] = { 0 };
	char readBuf[254] = { 0 };
	Str domainName;
	Str rdata;
	Domain domain;
	initStr(&domainName);
	initStr(&rdata);
	initDomain(&domain);

	while (!feof(file)) {
		//读取ID和域名	
		if (fscanf(file, "%d.%d.%d.%d %254s", &id[0], &id[1], &id[2], &id[3], readBuf) < 5 ||
			id[0] > 255 || id[1] > 255 || id[2] > 255 || id[3] > 255 || readBuf[253] != '\0') {
			//格式错误
			deleteStr(&rdata);
			deleteDomain(&domain);
			deleteStr(&domainName);
			printf("Format error.\n");
			printf("Successfully load %d records\n", count);
			return;
		}

		//输出调试信息
		if (debugLevel == 2) {
			char temp[17] = { 0 };
			sprintf(temp, "%d.%d.%d.%d", id[0], id[1], id[2], id[3]);
			printf("%8d: %-16s%s\n", count, temp, readBuf);
		}

		//添加记录到domainList
		setStr(&domainName, readBuf, strlen(readBuf));
		deleteDomain(&domain);//删除上一条记录
		initDomain(&domain);
		setDomainName(&domain, &domainName);
		domain.isStatic = 1;//设置为静态记录
		if (id[0] == 0 && id[1] == 0 && id[2] == 0 && id[3] == 0) {
			//id为0.0.0.0
			domain.isAvailable = 0;//设置为不可访问
			if (addDomain(pDomainList, &domain) == 0) {
				//域名信息已存在
				Domain* pDomain = findDomain(pDomainList, &domain.name);
				pDomain->isAvailable = 0;
			}
		}
		else {
			char temp[4];
			temp[0] = (char)id[0];
			temp[1] = (char)id[1];
			temp[2] = (char)id[2];
			temp[3] = (char)id[3];
			setStr(&rdata, temp, 4);
			domain.numA = 0;
			addRecord(&domain, &rdata, 1);
			if (addDomain(pDomainList, &domain) == 0) {
				//域名信息已存在
				Domain* pDomain = findDomain(pDomainList, &domain.name);
				if (pDomain->numA == -1)//第一次添加A类型记录
					domain.numA = 0;
				addRecord(pDomain, &rdata, 1);
			}
		}
		count++;
	}
	deleteStr(&rdata);
	deleteDomain(&domain);
	deleteStr(&domainName);
	printf("Successfully load %d records\n", count);
}

int addDomain(DomainList* pDomainList, const Domain* pData) {
	CacheNode* result = addRBTree(&pDomainList->root, &pData->name, pData);
	if (result == NULL)
		return 0;

	//若非静态表数据，将最新插入数据插入链表头部
	//若缓存大小超过容量，在表尾删除最近最少使用数据
	if (result->data.isStatic != 1) {
		addNode(&pDomainList->head, result);
		pDomainList->count++;
		if (pDomainList->count > pDomainList->capacity) {
			CacheNode* tail = popTail(&pDomainList->tail);
			CacheNode* needToDelete = deleteRBTree(&pDomainList->root, NULL, &tail->key);
			deleteDomain(&needToDelete->data);
			deleteStr(&needToDelete->key);
			free(needToDelete);
			pDomainList->count--;
		}
	}
	return 1;
}


Domain* findDomain(DomainList* pDomainList, const Str* pKey) {
	CacheNode* result = searchRBTree(pDomainList->root, pKey);
	if (result == NULL)
		return NULL;

	//若非静态表数据，将最新访问数据插入链表头部
	if (result->data.isStatic != 1)
		moveToHead(&pDomainList->head, result);
	return &result->data;
}

void refreshDomianList(DomainList* pDomainList) {
	while (pDomainList->count != 0) {
		CacheNode* tail = popTail(&pDomainList->tail);
		CacheNode* needToDelete = deleteRBTree(&pDomainList->root, NULL, &tail->key);
		deleteDomain(&needToDelete->data);
		deleteStr(&needToDelete->key);
		free(needToDelete);
		pDomainList->count--;
	}
}


void deleteDomainList(DomainList* pDomainList) {
	clearRBTree(pDomainList->root);
}