#include "Message.h"

//采用大端方式，将ch前两字节转换为short类型
unsigned short byteToShort(const char* ch) {
	return (unsigned short)((unsigned short)ch[1] + ((unsigned short)ch[0] << 8));
}

//采用大端方式，将short类型转换为ch前两字节
void shortToByte(char* ch, const short sh) {
	ch[0] = (char)(sh >> 8);
	ch[1] = (char)sh;
}

//采用大端方式，将int类型转换为ch前两字节
void intToByte(char* ch, const short sh) {
	ch[0] = (char)(sh >> 24);
	ch[1] = (char)(sh >> 16);
	ch[2] = (char)(sh >> 8);
	ch[3] = (char)sh;
}

//直接从套接字收到的message中add位置开始读取域名信息,若格式错误返回0，否则返回读取信息长度
int getName(Str* pName, const Str* pMessage, const short add) {
	deleteStr(pName);
	int result;
	int i;
	for (i = (int)add & 0xffff; i < strLength(pMessage) && pMessage->string[i] != 0 && ((pMessage->string[i] & (3 << 6)) != (3 << 6));) {
		int num = (int)pMessage->string[i] & 0xff;//读取字符个数
		i++;
		for (int j = 0; j < num && i < strLength(pMessage); i++, j++)
			appendStr(pName, pMessage->string[i]);
		appendStr(pName, '.');
	}
	result = i - add;

	if (i >= strLength(pMessage))//格式错误
		return 0;
	if ((pMessage->string[i] & 0xc0) == 0xc0) {
		//域名信息中存在偏移量
		Str temp;
		initStr(&temp);
		char ch[2];
		//读取偏移量
		ch[0] = pMessage->string[i];
		ch[0] &= 0x3f;
		ch[1] = pMessage->string[i + 1];
		getName(&temp, pMessage, byteToShort(ch));
		concatStr(pName, &temp);
		deleteStr(&temp);
		result += 2;
	}
	else {
		copynStr(pName, pName, strLength(pName) - 1);//去除最后一个'.'
		result += 1;
	}
	return result;
}

//将域名转换为可以植入报文的格式
void changeNameFormat(Str* pName, const Str* pDomainName) {
	deleteStr(pName);
	int temp = 0;
	for (int i = 0; i < strLength(pDomainName); i++) {
		appendStr(pName, 0);//预留保存字符串长度空间
		while (pDomainName->string[i] != '.' && i < strLength(pDomainName)) {
			appendStr(pName, pDomainName->string[i]);
			i++;
			temp++;
		}
		pName->string[i - temp] = temp;//字符串长度
		temp = 0;
	}
	appendStr(pName, 0);//加上'\0'标志结束
}

void setId(Str* pQuery, const short id)
{
	shortToByte(pQuery->string, id);
}

void setQr(Str* pQuery, const char qr)
{
	pQuery->string[2] |= qr << 7;
}

void setRcode(Str* pQuery, const char rcode)
{
	pQuery->string[3] |= rcode & 0x0F;
}

void setTc(Str* pQuery, const char tc)
{
	pQuery->string[2] |= (tc << 1) & 0x03;
}

void setAncount(Str* pQuery, const short ancount)
{
	shortToByte(pQuery->string + 6, ancount);
}

short getId(const Str* pMessage)
{
	return byteToShort(pMessage->string);
}

int getQr(const Str* pMessage)
{
	return (pMessage->string[2] & 0x80) >> 7;
}

int getOpcode(const Str* pMessage)
{
	return (pMessage->string[2] & 0x78) >> 3;
}

int getAa(const Str* pMessage)
{
	return (pMessage->string[2] & 0x78) >> 3;
}

int getTc(const Str* pMessage)
{
	return (pMessage->string[2] & 0x02) >> 1;
}

int getRd(const Str* pMessage)
{
	return pMessage->string[2] & 0x01;
}

int getRa(const Str* pMessage)
{
	return (pMessage->string[3] & 0x80) >> 7;
}

int getRcode(const Str* pMessage)
{
	return pMessage->string[3] & 0x0f;
}

short getQdcount(const Str* pMessage)
{
	return byteToShort(pMessage->string + 4);
}

short getAncount(const Str* pMessage)
{
	return byteToShort(pMessage->string + 6);
}

short getNscount(const Str* pMessage)
{
	return byteToShort(pMessage->string + 8);
}

short getArcount(const Str* pMessage)
{
	return byteToShort(pMessage->string + 10);
}

short getQnameAndType(Str* pName, const Str* pMessage)
{
	int nameLength = getName(pName, pMessage, 12);
	return byteToShort(pMessage->string + 12 + nameLength);
}

void mergeAnswer(Str* pMessage, const Domain* pDomain, const short type) {
	Str name;
	Str record;
	char temp[10];
	initStr(&record);
	initStr(&name);
	changeNameFormat(&name, &pDomain->name);//将域名转换为可以植入报文的格式

	if (type == 1) {
		//A类型查询
		for (int i = 0; i < pDomain->numA; i++) {
			concatStr(pMessage, &name);//连接NAME字段
			shortToByte(temp, type);//TYPE字段
			shortToByte(temp + 2, 1);//CLASS默认为IN
			intToByte(temp + 4, 10000);//TTL默认10000
			shortToByte(temp + 8, strLength(&pDomain->pRdataA[i]));//RDLENGTH字段
			setStr(&record, temp, 10);
			concatStr(pMessage, &record);
			concatStr(pMessage, &pDomain->pRdataA[i]);//连接RDATA字段
		}
	}
	else if (type == 28) {
		//AAAA类型查询
		for (int i = 0; i < pDomain->num4A; i++) {
			concatStr(pMessage, &name);//连接NAME字段
			shortToByte(temp, type);//TYPE字段
			shortToByte(temp + 2, 1);//CLASS默认为IN
			intToByte(temp + 4, 10000);//TTL默认10000
			shortToByte(temp + 8, (short)strLength(&pDomain->pRdata4A[i]));//RDLENGTH字段
			setStr(&record, temp, 10);
			concatStr(pMessage, &record);
			concatStr(pMessage, &pDomain->pRdata4A[i]);//连接RDATA字段
		}
	}
}

int saveRdata(Domain* pDomain, const Str* pMessage) {
	Str rdata;
	Str name;
	initStr(&rdata);
	initStr(&name);
	int type;
	int nameLength = getName(&name, pMessage, 12);//获取QNAME和长度
	type = byteToShort(pMessage->string + 12 + nameLength);
	if (nameLength == 0) {
		deleteStr(&rdata);
		deleteStr(&name);
		return 0;
	}

	if (type == 1) {
		if (pDomain->numA == -1)//第一次查询A记录
			pDomain->numA = 0;
	}
	else if (type == 28) {
		if (pDomain->num4A == -1)//第一次查询AAAA记录
			pDomain->num4A = 0;
	}
	else//其余类型记录不保存
		return 0;

	setDomainName(pDomain, &name);
	int add = 16 + nameLength;//第一个记录位置
	int recordNum = getAncount(pMessage);

	for (int i = 0; i < recordNum; i++) {
		if ((nameLength = getName(&name, pMessage, add)) == 0) {//获取NAME字段和长度
			deleteStr(&rdata);
			deleteStr(&name);
			return;
		}
		short rdLength = byteToShort(pMessage->string + add + nameLength + 8);//RDATA字段长度
		if (type == byteToShort(pMessage->string + add + nameLength)) {
			//符合type类型的记录
			getSubstring(&rdata, pMessage, add + nameLength + 10, rdLength);
			addRecord(pDomain, &rdata, type);
		}
		add += nameLength + 10 + rdLength;//add修改为下一个记录位置
	}

	deleteStr(&rdata);
	deleteStr(&name);
	return 1;
}