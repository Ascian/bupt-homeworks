#include"Domain.h"

void initDomain(Domain* pDomain)
{
	initStr(&pDomain->name);
	pDomain->isStatic = 0;
	pDomain->isAvailable = 1;
	pDomain->numA = -1;
	pDomain->num4A = -1;
	pDomain->pRdataA = NULL;
	pDomain->pRdata4A = NULL;
}

void setDomainName(Domain* pDomain, const Str* pName)
{
	copyStr(&(pDomain->name), pName);
}

void copyDomain(Domain* pDomain1, const Domain* pDomain2) {
	deleteDomain(pDomain1);
	pDomain1->isStatic = pDomain2->isStatic;
	pDomain1->isAvailable = pDomain2->isAvailable;
	copyStr(&pDomain1->name, &pDomain2->name);

	//复制A类型记录
	pDomain1->numA = pDomain2->numA;
	if (pDomain1->numA != 0 && pDomain1->numA != -1) {
		pDomain1->pRdataA = (Str*)malloc(sizeof(Str) * pDomain1->numA);
		if (pDomain1->pRdataA == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	for (int i = 0; i < pDomain1->numA; i++) {
		initStr(&pDomain1->pRdataA[i]);
		copyStr(&pDomain1->pRdataA[i], &pDomain2->pRdataA[i]);
	}

	//复制AAAA类型记录
	pDomain1->num4A = pDomain2->num4A;
	if (pDomain1->num4A != 0 && pDomain1->num4A != -1) {
		pDomain1->pRdata4A = (Str*)malloc(sizeof(Str) * pDomain1->num4A);
		if (pDomain1->pRdata4A == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	for (int i = 0; i < pDomain1->num4A; i++) {
		initStr(&pDomain1->pRdata4A[i]);
		copyStr(&pDomain1->pRdata4A[i], &pDomain2->pRdata4A[i]);
	}
}

void addRecord(Domain* pDomain, const Str* pRdata, const int type)
{
	if (type == 1) {
		//添加A类型记录
		if (pDomain->numA == 127)
			return;
		pDomain->pRdataA = (Str*)realloc(pDomain->pRdataA, sizeof(Str) * (pDomain->numA + 1));
		if (pDomain->pRdataA == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
		initStr(&pDomain->pRdataA[pDomain->numA]);
		copyStr(&pDomain->pRdataA[pDomain->numA], pRdata);
		pDomain->numA++;
	}
	else if (type == 28) {
		//添加AA类型记录
		if (pDomain->num4A == 127)
			return;
		pDomain->pRdata4A = (Str*)realloc(pDomain->pRdata4A, sizeof(Str) * (pDomain->num4A + 1));
		if (pDomain->pRdata4A == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
		initStr(&pDomain->pRdata4A[pDomain->num4A]);
		copyStr(&pDomain->pRdata4A[pDomain->num4A], pRdata);
		pDomain->num4A++;
	}
}

char getNumOf(const Domain* pDomain, const int type)
{
	if (type == 1)
		return pDomain->numA;
	else
		return pDomain->num4A;
}

void deleteDomain(Domain* pDomain) {
	deleteStr(&(pDomain->name));
	for (int i = 0; i < pDomain->numA; i++)
		deleteStr(&pDomain->pRdataA[i]);
	free(pDomain->pRdataA);
	pDomain->numA = -1;
	pDomain->pRdataA = NULL;
	for (int i = 0; i < pDomain->num4A; i++)
		deleteStr(&pDomain->pRdata4A[i]);
	free(pDomain->pRdata4A);
	pDomain->num4A = -1;
	pDomain->pRdata4A = NULL;
}