#include "recursive_syntax_analysis.h"

int forward_pointer(ifstream& token_stream, int& char_num)
{
	char_num++;
	char c = token_stream.get();
	if (c == '+')
		return PLUS;
	if (c == '-')
		return MINUS;
	if (c == '*')
		return MULTIPLY;
	if (c == '/')
		return DIVIDE;
	if (c == '(')
		return LEFT_PARENTHESIS;
	if (c == ')')
		return RIGHT_PARENTHESIS;
	if (c == 'n')
		return NUM;
	if (c == EOF)
		return EOF;

	return error(token_stream, char_num, "unknown token", { PLUS, MINUS, MULTIPLY, DIVIDE, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, NUM });
}

int error(ifstream& token_stream, int& char_num, const string& description, const set<token>& expect_tokens)
{
	cout << char_num << ": " << description << endl;

	if (expect_tokens.size() == 0)
		return -1;

	int t = forward_pointer(token_stream, char_num);
	while (t != EOF && expect_tokens.find((token)t) == expect_tokens.end())
		t = forward_pointer(token_stream, char_num);
	return t;
}

void procE(int& current_token, ifstream& token_stream, int& char_num)
{
	cout << "E -> TE'" << endl;
	procT(current_token, token_stream, char_num);
	if (current_token == PLUS)
	{
		cout << "E' -> +E" << endl;
		current_token = forward_pointer(token_stream, char_num);
		procE(current_token, token_stream, char_num);
	}
	else if (current_token == MINUS)
	{
		cout << "E' -> -E" << endl;
		current_token = forward_pointer(token_stream, char_num);
		procE(current_token, token_stream, char_num);
	}
	else
		cout << "E' -> EPSILON" << endl;
}

void procT(int& current_token, ifstream& token_stream, int& char_num)
{
	cout << "T -> FT'" << endl;
	procF(current_token, token_stream, char_num);
	if (current_token == MULTIPLY)
	{
		cout << "T -> *T" << endl;
		current_token = forward_pointer(token_stream, char_num);
		procT(current_token, token_stream, char_num);
	}
	else if (current_token == DIVIDE)
	{
		cout << "T -> /T" << endl;
		current_token = forward_pointer(token_stream, char_num);
		procT(current_token, token_stream, char_num);
	}
	else
		cout << "T' -> EPSILON" << endl;
}

void procF(int& current_token, ifstream& token_stream, int& char_num)
{
	if (current_token == LEFT_PARENTHESIS)
	{
		cout << "F -> (E)" << endl;
		current_token = forward_pointer(token_stream, char_num);
		procE(current_token, token_stream, char_num);
		if (current_token != RIGHT_PARENTHESIS)
		{
			current_token = error(token_stream, char_num, "missing right parenthesis", { RIGHT_PARENTHESIS });
			if (current_token == EOF)	//找不到想要的字符
				/*
					判断EOF和error配套使用，error返回EOF时表示出错，找不到期望的字符
					由于存在epsilon表达式，forward返回EOF时将其当做一个记号
				*/
				return;
		}
		current_token = forward_pointer(token_stream, char_num);
	}
	else if (current_token == NUM)
	{
		cout << "F -> num" << endl;
		current_token = forward_pointer(token_stream, char_num);
	}
	else {
		current_token = error(token_stream, char_num, "missing operation object", { LEFT_PARENTHESIS, NUM });
		if (current_token == EOF)	//找不到想要的字符
			return;
		procF(current_token, token_stream, char_num);
	}
}

void syntax_analysis(ifstream& token_stream)
{
	int char_num = 0;
	int current_token = forward_pointer(token_stream, char_num);
	procE(current_token, token_stream, char_num);
}
