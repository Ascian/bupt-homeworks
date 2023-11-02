#pragma once
#include<string.h>
#include<stdlib.h>
typedef struct Str {
	char* string;
	unsigned short length;
}Str;

//��ʼ���ַ���
void initStr(Str* str);

//�����ַ���
void setStr(Str* dst, const char* src, const unsigned short length);

//�����ַ�������
int strLength(const Str* str);

//��ȡ�Ӵ�
void getSubstring(Str* dst, const Str* src, const unsigned short add, const unsigned short length);

//�����ַ���
void copyStr(Str* dst, const Str* src);

//����ָ�������ַ���
void copynStr(Str* dst, const Str* src, const unsigned short length);

//�Ƚ��ַ�����str1�ϴ󷵻�1��str1��С����-1����ȷ���0
int compareStr(const Str* str1, const Str* str2);

//�����ַ���
void concatStr(Str* dst, const Str* src);

//���ַ���β������ַ�
void appendStr(Str* dst, const char ch);

//����ָ�������ַ���
void concatnStr(Str* dst, const Str* src, const unsigned short length);

//ɾ���ַ���
void deleteStr(Str* dst);

