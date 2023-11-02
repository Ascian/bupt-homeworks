#include "Message.h"

//���ô�˷�ʽ����chǰ���ֽ�ת��Ϊshort����
unsigned short byteToShort(const char* ch) {
	return (unsigned short)((unsigned short)ch[1] + ((unsigned short)ch[0] << 8));
}

//���ô�˷�ʽ����short����ת��Ϊchǰ���ֽ�
void shortToByte(char* ch, const short sh) {
	ch[0] = (char)(sh >> 8);
	ch[1] = (char)sh;
}

//���ô�˷�ʽ����int����ת��Ϊchǰ���ֽ�
void intToByte(char* ch, const short sh) {
	ch[0] = (char)(sh >> 24);
	ch[1] = (char)(sh >> 16);
	ch[2] = (char)(sh >> 8);
	ch[3] = (char)sh;
}

//ֱ�Ӵ��׽����յ���message��addλ�ÿ�ʼ��ȡ������Ϣ,����ʽ���󷵻�0�����򷵻ض�ȡ��Ϣ����
int getName(Str* pName, const Str* pMessage, const short add) {
	deleteStr(pName);
	int result;
	int i;
	for (i = (int)add & 0xffff; i < strLength(pMessage) && pMessage->string[i] != 0 && ((pMessage->string[i] & (3 << 6)) != (3 << 6));) {
		int num = (int)pMessage->string[i] & 0xff;//��ȡ�ַ�����
		i++;
		for (int j = 0; j < num && i < strLength(pMessage); i++, j++)
			appendStr(pName, pMessage->string[i]);
		appendStr(pName, '.');
	}
	result = i - add;

	if (i >= strLength(pMessage))//��ʽ����
		return 0;
	if ((pMessage->string[i] & 0xc0) == 0xc0) {
		//������Ϣ�д���ƫ����
		Str temp;
		initStr(&temp);
		char ch[2];
		//��ȡƫ����
		ch[0] = pMessage->string[i];
		ch[0] &= 0x3f;
		ch[1] = pMessage->string[i + 1];
		getName(&temp, pMessage, byteToShort(ch));
		concatStr(pName, &temp);
		deleteStr(&temp);
		result += 2;
	}
	else {
		copynStr(pName, pName, strLength(pName) - 1);//ȥ�����һ��'.'
		result += 1;
	}
	return result;
}

//������ת��Ϊ����ֲ�뱨�ĵĸ�ʽ
void changeNameFormat(Str* pName, const Str* pDomainName) {
	deleteStr(pName);
	int temp = 0;
	for (int i = 0; i < strLength(pDomainName); i++) {
		appendStr(pName, 0);//Ԥ�������ַ������ȿռ�
		while (pDomainName->string[i] != '.' && i < strLength(pDomainName)) {
			appendStr(pName, pDomainName->string[i]);
			i++;
			temp++;
		}
		pName->string[i - temp] = temp;//�ַ�������
		temp = 0;
	}
	appendStr(pName, 0);//����'\0'��־����
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
	changeNameFormat(&name, &pDomain->name);//������ת��Ϊ����ֲ�뱨�ĵĸ�ʽ

	if (type == 1) {
		//A���Ͳ�ѯ
		for (int i = 0; i < pDomain->numA; i++) {
			concatStr(pMessage, &name);//����NAME�ֶ�
			shortToByte(temp, type);//TYPE�ֶ�
			shortToByte(temp + 2, 1);//CLASSĬ��ΪIN
			intToByte(temp + 4, 10000);//TTLĬ��10000
			shortToByte(temp + 8, strLength(&pDomain->pRdataA[i]));//RDLENGTH�ֶ�
			setStr(&record, temp, 10);
			concatStr(pMessage, &record);
			concatStr(pMessage, &pDomain->pRdataA[i]);//����RDATA�ֶ�
		}
	}
	else if (type == 28) {
		//AAAA���Ͳ�ѯ
		for (int i = 0; i < pDomain->num4A; i++) {
			concatStr(pMessage, &name);//����NAME�ֶ�
			shortToByte(temp, type);//TYPE�ֶ�
			shortToByte(temp + 2, 1);//CLASSĬ��ΪIN
			intToByte(temp + 4, 10000);//TTLĬ��10000
			shortToByte(temp + 8, (short)strLength(&pDomain->pRdata4A[i]));//RDLENGTH�ֶ�
			setStr(&record, temp, 10);
			concatStr(pMessage, &record);
			concatStr(pMessage, &pDomain->pRdata4A[i]);//����RDATA�ֶ�
		}
	}
}

int saveRdata(Domain* pDomain, const Str* pMessage) {
	Str rdata;
	Str name;
	initStr(&rdata);
	initStr(&name);
	int type;
	int nameLength = getName(&name, pMessage, 12);//��ȡQNAME�ͳ���
	type = byteToShort(pMessage->string + 12 + nameLength);
	if (nameLength == 0) {
		deleteStr(&rdata);
		deleteStr(&name);
		return 0;
	}

	if (type == 1) {
		if (pDomain->numA == -1)//��һ�β�ѯA��¼
			pDomain->numA = 0;
	}
	else if (type == 28) {
		if (pDomain->num4A == -1)//��һ�β�ѯAAAA��¼
			pDomain->num4A = 0;
	}
	else//�������ͼ�¼������
		return 0;

	setDomainName(pDomain, &name);
	int add = 16 + nameLength;//��һ����¼λ��
	int recordNum = getAncount(pMessage);

	for (int i = 0; i < recordNum; i++) {
		if ((nameLength = getName(&name, pMessage, add)) == 0) {//��ȡNAME�ֶκͳ���
			deleteStr(&rdata);
			deleteStr(&name);
			return;
		}
		short rdLength = byteToShort(pMessage->string + add + nameLength + 8);//RDATA�ֶγ���
		if (type == byteToShort(pMessage->string + add + nameLength)) {
			//����type���͵ļ�¼
			getSubstring(&rdata, pMessage, add + nameLength + 10, rdLength);
			addRecord(pDomain, &rdata, type);
		}
		add += nameLength + 10 + rdLength;//add�޸�Ϊ��һ����¼λ��
	}

	deleteStr(&rdata);
	deleteStr(&name);
	return 1;
}