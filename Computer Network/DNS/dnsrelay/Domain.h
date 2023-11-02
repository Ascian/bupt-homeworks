#pragma once
#include"Str.h"

typedef struct Domain {
	Str name;//域名
	char isStatic;//是否属于静态表
	char isAvailable;//是否允许查询
	char numA;//A类型记录数量,值为-1表示未查询
	char num4A;//AAAA类型记录数量,值为-1表示未查询
	Str* pRdataA;//A类型记录,仅保存RDATA字段
	Str* pRdata4A;//AAAA类型记录，仅保存RDATA字段
}Domain;

//初始化域名结构体
void initDomain(Domain* pDomain);

//修改域名结构体
void setDomainName(Domain* pDomain, const Str* pName);

//复制域名结构体
void copyDomain(Domain* pDomain1, const Domain* pDomain2);

//添加指定type类型记录
void addRecord(Domain* pDomain, const Str* pRdata, const int type);

//获取指定type类型化数量
char getNumOf(const Domain* pDomain, const int type);

//删除域名结构体
void deleteDomain(Domain* pDomain);
