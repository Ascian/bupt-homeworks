#pragma once
#include"Domain.h"
#include"Query.h"
#include"Str.h"
#include<stdlib.h>


//修改ID字段
void setId(Str* pQuery, const short id);

//修改QR字段
void setQr(Str* pQuery, const char qr);

//修改RCODE字段
void setRcode(Str* pQuery, const char rcode);

//修改TC字段
void setTc(Str* pQuery, const char tc);

//修改ANCOUNT字段
void setAncount(Str* pQuery, const short ancount);

//读取ID字段
short getId(const Str* pMessage);

//读取QR字段
int getQr(const Str* pMessage);

//读取OPCODE字段
int getOpcode(const Str* pMessage);

//读取AA字段
int getAa(const Str* pMessage);

//读取TC字段
int getTc(const Str* pMessage);

//读取RD字段
int getRd(const Str* pMessage);

//读取RA字段
int getRa(const Str* pMessage);

//读取RDCODE字段
int getRcode(const Str* pMessage);

//读取QDCOUNT字段
short getQdcount(const Str* pMessage);

//读取ANCOUNT字段
short getAncount(const Str* pMessage);

//读取NSCOUNT字段
short getNscount(const Str* pMessage);

//读取ARCOUNT字段
short getArcount(const Str* pMessage);

//读取QNAME字段,若格式错误返回0，否则返回查询地址类型
short getQnameAndType(Str* pName, const Str* pMessage);

//连接类型为type的记录
void mergeAnswer(Str* pMessage, const Domain* pDomain, const short type);

//保存RDATA记录到pDomain,若记录类型为A或AAAA类型加入域名并返回1，否则返回0
int saveRdata(Domain* pDomain, const Str* pMessage);