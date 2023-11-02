#pragma once
#include"Str.h"

typedef struct Domain {
	Str name;//����
	char isStatic;//�Ƿ����ھ�̬��
	char isAvailable;//�Ƿ������ѯ
	char numA;//A���ͼ�¼����,ֵΪ-1��ʾδ��ѯ
	char num4A;//AAAA���ͼ�¼����,ֵΪ-1��ʾδ��ѯ
	Str* pRdataA;//A���ͼ�¼,������RDATA�ֶ�
	Str* pRdata4A;//AAAA���ͼ�¼��������RDATA�ֶ�
}Domain;

//��ʼ�������ṹ��
void initDomain(Domain* pDomain);

//�޸������ṹ��
void setDomainName(Domain* pDomain, const Str* pName);

//���������ṹ��
void copyDomain(Domain* pDomain1, const Domain* pDomain2);

//���ָ��type���ͼ�¼
void addRecord(Domain* pDomain, const Str* pRdata, const int type);

//��ȡָ��type���ͻ�����
char getNumOf(const Domain* pDomain, const int type);

//ɾ�������ṹ��
void deleteDomain(Domain* pDomain);
