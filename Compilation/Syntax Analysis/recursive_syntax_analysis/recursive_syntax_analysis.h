#pragma once

#include <iostream>
#include <fstream>
#include <set>

using namespace std;

//记号类型
enum token
{
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	NUM,
};

//读取一个记号，返回记号类型
int forward_pointer(ifstream& token_stream, int& char_num);

//错误处理，不断跳过输入符号直到遇到其中一个期望的终结符并返回
int error(ifstream& token_stream, int& char_num, const string& description, const set<token>& expect_tokens = {});

//非终结符E分析函数
void procE(int& current_token, ifstream& token_stream, int& char_num);

//非终结符T分析函数
void procT(int& current_token, ifstream& token_stream, int& char_num);

//非终结符F分析函数
void procF(int& current_token, ifstream& token_stream, int& char_num);

//语法分析
void syntax_analysis(ifstream& token_stream);