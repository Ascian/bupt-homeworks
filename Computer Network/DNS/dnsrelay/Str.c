#include"Str.h"

void initStr(Str* str)
{
	str->length = 0;
	str->string = NULL;
}

void setStr(Str* dst, const char* src, const unsigned short length) {
	free(dst->string);
	if (length != 0) {
		dst->string = (char*)malloc(sizeof(char) * length);
		if (dst->string == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	dst->length = length;
	for (int i = 0; i < length; i++)
		dst->string[i] = src[i];
}

int strLength(const Str* str)
{
	return str->length;
}

void getSubstring(Str* dst, const Str* src, const unsigned short add, const unsigned short length) {
	char* temp = NULL;
	if (length != 0) {
		temp = (char*)malloc(sizeof(char) * length);
		if (temp == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	for (int i = 0; i < length; i++)
		temp[i] = src->string[add + i];
	free(dst->string);
	dst->string = temp;
	dst->length = length;
}

void copyStr(Str* dst, const Str* src) {
	char* temp = NULL;
	if (src->length != 0) {
		temp = (char*)malloc(sizeof(char) * src->length);
		if (temp == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	for (int i = 0; i < src->length; i++) {
		temp[i] = src->string[i];
	}
	free(dst->string);
	dst->string = temp;
	dst->length = src->length;
}

void copynStr(Str* dst, const Str* src, const unsigned short length) {
	char* temp = NULL;
	if (length != 0) {
		temp = (char*)malloc(sizeof(char) * length);
		if (temp == NULL) {
			printf("Request memory failed\n");
			exit(-1);
		}
	}
	for (int i = 0; i < length; i++) {
		temp[i] = src->string[i];
	}
	free(dst->string);
	dst->string = temp;
	dst->length = length;
}

int compareStr(const Str* str1, const Str* str2) {
	for (int i = 0; i < str1->length && i < str2->length; i++) {
		if (str1->string[i] < str2->string[i])
			return -1;
		else if (str1->string[i] > str2->string[i])
			return 1;
	}
	if (str1->length < str2->length)
		return -1;
	else if (str1->length > str2->length)
		return 1;
	return 0;
}

void concatStr(Str* dst, const Str* src) {
	dst->string = (char*)realloc(dst->string, sizeof(char) * (dst->length + src->length));
	if (dst->string == NULL) {
		printf("Request memory failed\n");
		exit(-1);
	}
	for (int i = 0; i < src->length; i++)
		dst->string[i + dst->length] = src->string[i];
	dst->length = dst->length + src->length;
}

void appendStr(Str* dst, const char ch) {
	dst->string = (char*)realloc(dst->string, sizeof(char) * (dst->length + 1));
	if (dst->string == NULL) {
		printf("Request memory failed\n");
		exit(-1);
	}
	dst->string[dst->length] = ch;
	dst->length = dst->length + 1;
}

void concatnStr(Str* dst, const Str* src, const unsigned short length) {
	dst->string = (char*)realloc(dst->string, sizeof(char) * (dst->length + length));
	if (dst->string == NULL) {
		printf("Request memory failed\n");
		exit(-1);
	}
	for (int i = 0; i < length; i++)
		dst->string[i + dst->length] = src->string[i];
	dst->length = dst->length + length;
}

void deleteStr(Str* dst)
{
	free(dst->string);
	dst->string = NULL;
	dst->length = 0;
}