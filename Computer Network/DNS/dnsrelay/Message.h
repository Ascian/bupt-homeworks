#pragma once
#include"Domain.h"
#include"Query.h"
#include"Str.h"
#include<stdlib.h>


//�޸�ID�ֶ�
void setId(Str* pQuery, const short id);

//�޸�QR�ֶ�
void setQr(Str* pQuery, const char qr);

//�޸�RCODE�ֶ�
void setRcode(Str* pQuery, const char rcode);

//�޸�TC�ֶ�
void setTc(Str* pQuery, const char tc);

//�޸�ANCOUNT�ֶ�
void setAncount(Str* pQuery, const short ancount);

//��ȡID�ֶ�
short getId(const Str* pMessage);

//��ȡQR�ֶ�
int getQr(const Str* pMessage);

//��ȡOPCODE�ֶ�
int getOpcode(const Str* pMessage);

//��ȡAA�ֶ�
int getAa(const Str* pMessage);

//��ȡTC�ֶ�
int getTc(const Str* pMessage);

//��ȡRD�ֶ�
int getRd(const Str* pMessage);

//��ȡRA�ֶ�
int getRa(const Str* pMessage);

//��ȡRDCODE�ֶ�
int getRcode(const Str* pMessage);

//��ȡQDCOUNT�ֶ�
short getQdcount(const Str* pMessage);

//��ȡANCOUNT�ֶ�
short getAncount(const Str* pMessage);

//��ȡNSCOUNT�ֶ�
short getNscount(const Str* pMessage);

//��ȡARCOUNT�ֶ�
short getArcount(const Str* pMessage);

//��ȡQNAME�ֶ�,����ʽ���󷵻�0�����򷵻ز�ѯ��ַ����
short getQnameAndType(Str* pName, const Str* pMessage);

//��������Ϊtype�ļ�¼
void mergeAnswer(Str* pMessage, const Domain* pDomain, const short type);

//����RDATA��¼��pDomain,����¼����ΪA��AAAA���ͼ�������������1�����򷵻�0
int saveRdata(Domain* pDomain, const Str* pMessage);