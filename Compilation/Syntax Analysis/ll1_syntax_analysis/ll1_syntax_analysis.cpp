#include "ll1_syntax_analysis.h"

string Grammar::to_string(grammar_production& production)
{
	string result;
	result += non_terminal_symbols[production.left];
	result += "->";
	if (production.right.size() == 0)
		result += "epsilon";
	else
	{
		for (int symbol : production.right)
		{
			if (symbol < 0)
				result += non_terminal_symbols[-symbol];
			else
				result += terminal_symbols[symbol];
		}
	}
	return result;
}

bool Grammar::add_non_terminal_symbol(const char non_terminal_symbol)
{
	//判断符号是否不在终结符集里
	for (int i = 1; i < non_terminal_symbols.size(); i++)
	{
		if (non_terminal_symbols[i] == non_terminal_symbol)
		{
			return false;
		}
	}
	non_terminal_symbols.push_back(non_terminal_symbol);
	return true;
}

bool Grammar::add_terminal_symbol(const char terminal_symbol)
{
	//判断符号是否不在非终结符里
	for (int i = 1; i < non_terminal_symbols.size(); i++)
	{
		if (non_terminal_symbols[i] == terminal_symbol)
		{
			return false;
		}
	}
	terminal_symbols.push_back(terminal_symbol);
	return true;
}

bool Grammar::set_initial_symbol(const char initial_symbol)
{
	//判断符号是否在非终结符集里
	for (int i = 1; i < non_terminal_symbols.size(); i++)
	{
		if (non_terminal_symbols[i] == initial_symbol)
		{
			this->initial_symbol_index = i;
			return true;
		}
	}
	return false;
}

bool Grammar::add_production(const char left, const string& right)
{
	grammar_production production;
	int index = 0;
	//判断生成式左边非终结符是否在非终结符集里
	for (int i = 1; i < non_terminal_symbols.size(); i++)
	{
		if (non_terminal_symbols[i] == left)
		{
			index = i;
			production.left = index;
			break;
		}
	}
	if (index == 0)
		return false;

	for (char symbol : right)
	{
		index = 0;
		//判断符号是否在非终结符集里
		for (int i = 1; i < non_terminal_symbols.size(); i++)
		{
			if (non_terminal_symbols[i] == symbol)
			{
				index = i;	//非终结符索引存为负数区别终结符
				production.right.push_back(-index);
				break;
			}
		}
		if (index == 0)
		{
			//判断符号是否在终结符集里
			for (int i = 1; i < terminal_symbols.size(); i++)
			{
				if (terminal_symbols[i] == symbol)
				{
					index = i;
					production.right.push_back(index);
					break;
				}
			}
			if (index == 0)	//该符号为未知符号
				return false;
		}
	}

	productions.push_back(production);
	return true;
}

//构建符号串的first集
void construct_symbol_string_first_set(set<int>& symbol_string_first_set, const vector<int>& symbol_string, const vector<set<int>>& symbol_first_set)
{
	int i = 0;
	for (; i < symbol_string.size(); i++)
	{
		if (symbol_string[i] > 0)	//终结符
		{
			symbol_string_first_set.insert(symbol_string[i]);
			break;
		}
		else	//非终结符
		{
			symbol_string_first_set.insert(symbol_first_set[-symbol_string[i]].begin(), symbol_first_set[-symbol_string[i]].end());
			if (symbol_first_set[-symbol_string[i]].find(EPSILON) == symbol_first_set[-symbol_string[i]].end())	//first集不含有epsilon
				break;
		}
	}
	if (i == symbol_string.size())	//生成式右边可以推导出epsilon
		symbol_string_first_set.insert(EPSILON);
}

void Grammar::construct_first_set()
{
	vector<set<int>> new_first_set(non_terminal_symbols.size());
	vector<set<int>> new_production_first_set(productions.size());

	bool is_modified = true;
	while (is_modified)
	{
		is_modified = false;
		for (int i = 0; i < productions.size(); i++)
		{
			int origin_size = new_production_first_set[i].size();

			construct_symbol_string_first_set(new_production_first_set[i], productions[i].right, new_first_set);

			if (origin_size != new_production_first_set[i].size())	//生成式first集有增加
			{
				new_first_set[productions[i].left].insert(new_production_first_set[i].begin(), new_production_first_set[i].end());
				is_modified = true;
			}
		}
	}

	production_first_set = new_production_first_set;
	first_set = new_first_set;
}

void Grammar::construct_follow_set()
{
	if (first_set.size() == 0)
		construct_first_set();

	vector<set<int>> new_follow_set(non_terminal_symbols.size());
	vector<set<int>> follow_symbol(non_terminal_symbols.size());	//若FOLLOW(A)需要加入FOLLOW(B)，则将A加入follow_symbol[B]
	new_follow_set[initial_symbol_index].insert(END_SYMBOL);	//加入结束符到初始符号的follow集
	for (grammar_production production : productions)
	{
		for (int i = 0; i < production.right.size(); i++)
		{
			if (production.right[i] < 0)	//非终结符
			{
				if (production.right.size() == i + 1 && -production.right[i] != production.left)	//右边无任何符号
				{
					new_follow_set[-production.right[i]].insert(new_follow_set[production.left].begin(), new_follow_set[production.left].end());
					follow_symbol[-production.right[i]].insert(production.left);
				}
				else	//右边为符号串
				{
					set<int> symbol_string_first_set;
					vector<int> symbol_string(production.right.begin() + i + 1, production.right.end());	//截取符号串
					construct_symbol_string_first_set(symbol_string_first_set, symbol_string, first_set);

					new_follow_set[-production.right[i]].insert(symbol_string_first_set.begin(), symbol_string_first_set.end());
					if (symbol_string_first_set.find(EPSILON) != symbol_string_first_set.end())	//右边符号串first集含有epsilon
					{
						new_follow_set[-production.right[i]].erase(EPSILON);	//删除follow集中epsilon
						new_follow_set[-production.right[i]].insert(new_follow_set[production.left].begin(), new_follow_set[production.left].end());
						follow_symbol[-production.right[i]].insert(production.left);
					}
				}
			}
		}
	}

	//循环遍历每个非终结符的follow_symbol集，将非终结符A的follow_symbol集内的每一个非终结符B的follow集加入A的follow集，直到follow集不再发生改变
	bool is_modified = true;
	while (is_modified)
	{
		is_modified = false;
		for (int i = 0; i < follow_symbol.size(); i++)
		{
			int origin_size = new_follow_set[i].size();
			for (int symbol : follow_symbol[i])
				new_follow_set[i].insert(new_follow_set[symbol].begin(), new_follow_set[symbol].end());
			if (origin_size != new_follow_set[i].size())
				is_modified = true;
		}
	}

	follow_set = new_follow_set;
}

bool Grammar::construct_analysis_table()
{
	if (production_first_set.size() == 0)
		construct_first_set();
	if (follow_set.size() == 0)
		construct_follow_set();


	vector<vector<int>> new_analysis_table(non_terminal_symbols.size(), vector<int>(terminal_symbols.size(), EMPTY));	//初始值为EMPTY
	for (int i = 0; i < production_first_set.size(); i++)
	{
		for (int symbol : production_first_set[i])
		{
			if (symbol == EPSILON)	//生成式右边的first集含有epsilon
			{
				for (int terminal_symbol : follow_set[productions[i].left])
				{
					if (new_analysis_table[productions[i].left][terminal_symbol] == EMPTY ||
						new_analysis_table[productions[i].left][terminal_symbol] == i)
						new_analysis_table[productions[i].left][terminal_symbol] = i;
					else
						return false;	//出现重复表项
				}
			}
			else
			{
				if (new_analysis_table[productions[i].left][symbol] == EMPTY ||
					new_analysis_table[productions[i].left][symbol] == i)
					new_analysis_table[productions[i].left][symbol] = i;
				else
					return false;	//出现重复表项
			}
		}
	}

	//添加错误处理符号
	for (int i = 1; i < non_terminal_symbols.size(); i++)
	{
		if (new_analysis_table[i][0] == EMPTY)	//遇到结束符出错始终弹出栈顶
			new_analysis_table[i][0] = SYNCH;
		for (int j = 1; j < terminal_symbols.size(); j++)
		{
			if (new_analysis_table[i][j] == EMPTY &&
				follow_set[i].find(j) != follow_set[i].end())	//j属于i的follow集
				new_analysis_table[i][j] = SYNCH;
		}
	}
	analyse_table = new_analysis_table;
}

int Grammar::forward_pointer(ifstream& token_stream, int& char_num)
{
	char c;
	while (true)
	{
		char_num++;
		c = token_stream.get();
		if (c == EOF)
			return END_SYMBOL;
		for (int i = 1; i < terminal_symbols.size(); i++)
		{
			if (c == terminal_symbols[i])
				return i;
		}
		cout << char_num << ": unknown token" << endl;
	}
}

void Grammar::error(ifstream& token_stream, int& current_token, int& char_num, vector<int>& stack, const int& cause)
{

	if (cause == EMPTY)
	{
		cout << char_num << ": skip input char \'" << terminal_symbols[current_token] << "\'" << endl;
		current_token = forward_pointer(token_stream, char_num);
	}
	else
	{
		cout << char_num << ": pop state \'" << non_terminal_symbols[-stack.back()] << "\'" << endl;
		stack.pop_back();
	}
}

void Grammar::syntax_analysis(ifstream& token_stream)
{
	if (analyse_table.size() == 0)
		construct_analysis_table();

	int char_num = 0;
	int current_token = forward_pointer(token_stream, char_num);
	vector<int> stack = { END_SYMBOL, -initial_symbol_index };

	while (stack.back() != END_SYMBOL)
	{
		if (stack.back() > 0)	//栈顶是终结符号
		{
			if (stack.back() == current_token)
			{
				stack.pop_back();
				current_token = forward_pointer(token_stream, char_num);;
			}
			else	//与输入符号不匹配
			{
				cout << char_num << ": want \'" << terminal_symbols[stack.back()] << "\' but get \'" << terminal_symbols[current_token] << endl;
				stack.pop_back();
			}
		}
		else	//栈顶是非终结符号
		{
			int production_index = analyse_table[-stack.back()][current_token];
			if (production_index == EMPTY || production_index == SYNCH)
				error(token_stream, current_token, char_num, stack, production_index);
			else
			{
				cout << to_string(productions[production_index]) << endl;
				stack.pop_back();
				for (int i = (int)productions[production_index].right.size() - 1; i >= 0; i--)
					stack.push_back(productions[production_index].right[i]);
			}
		}
	}
	if (current_token != END_SYMBOL)	//栈顶为结束符，但输入记号流还没有分析完，输出错误信息和剩余记号
	{
		cout << "The stack is empty , but there are still tokens not analysed" << endl;
		cout << "rest tokens: " << terminal_symbols[current_token];
		char c = token_stream.get();
		while (c != EOF)
		{
			cout << c;
			c = token_stream.get();
		}
	}
	else
		cout << "ACCEPT" << endl;
}


