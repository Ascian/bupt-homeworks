#pragma once
#include<string.h>
#include<stdlib.h>
typedef struct Str {
	char* string;
	unsigned short length;
}Str;

//初始化字符串
void initStr(Str* str);

//设置字符串
void setStr(Str* dst, const char* src, const unsigned short length);

//返回字符串长度
int strLength(const Str* str);

//读取子串
void getSubstring(Str* dst, const Str* src, const unsigned short add, const unsigned short length);

//复制字符串
void copyStr(Str* dst, const Str* src);

//复制指定长度字符串
void copynStr(Str* dst, const Str* src, const unsigned short length);

//比较字符串，str1较大返回1，str1较小返回-1，相等返回0
int compareStr(const Str* str1, const Str* str2);

//连接字符串
void concatStr(Str* dst, const Str* src);

//在字符串尾部添加字符
void appendStr(Str* dst, const char ch);

//连接指定长度字符串
void concatnStr(Str* dst, const Str* src, const unsigned short length);

//删除字符串
void deleteStr(Str* dst);

