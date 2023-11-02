#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

const char EPSILON = 0;
const char END_SYMBOL = 0;

class Grammar
{
private:
	typedef struct grammar_production
	{
		int left;	//non_terminal_symbols索引
		vector<int> right;	//符号索引序列, 负数为非终结符索引
	}production;

	string to_string(grammar_production& production);

	vector<char> non_terminal_symbols;	//0号位不使用
	vector<char> terminal_symbols;		//0号位不使用
	vector<production> productions;
	int initial_symbol_index;

	//以下数据结构使用数组索引代替具体数据
	vector<set<int>> first_set;	//索引为非终结符，内容为终结符集合，0号位不使用
	vector<set<int>> follow_set;	//索引为非终结符，内容为终结符集合，0号位不使用
	vector<set<int>> production_first_set;	//索引为生成式，内容为终结符集合
	vector<vector<int>> analyse_table;	//索引为[非终结符(0号位不使用)][终结符(0号位表示END_SYMBOL)]，内容为生成式
public:
	static const int EMPTY = -1;
	static const int SYNCH = -2;

	Grammar()
	{
		initial_symbol_index = 0;
		non_terminal_symbols.push_back(-1);
		terminal_symbols.push_back('$');
	}

	bool add_non_terminal_symbol(const char non_terminal_symbol);

	bool add_terminal_symbol(const char terminal_symbol);

	bool set_initial_symbol(const char initial_symbol);

	bool add_production(const char left, const string& right);

	//生成非终结符和生成式右边的first集
	void construct_first_set();

	//生成非终结符的follow集
	void construct_follow_set();

	//生成分析表
	bool construct_analysis_table();

private:
	//读取一个记号，返回非终结符索引
	int forward_pointer(ifstream& token_stream, int& char_num);

	//错误处理
	void error(ifstream& token_stream, int& current_token, int& char_num, vector<int>& stack, const int& cause);

public:
	//词法分析记号流
	void syntax_analysis(ifstream& token_stream);
};

