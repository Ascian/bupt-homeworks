#include "lr1_syntax_analysis.h"

int forward_pointer(ifstream& token_stream, int& char_num)
{
	char c;
	while (true)
	{
		char_num++;
		c = token_stream.get();
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
			return END;
		cout << char_num << ": unknown token" << endl;
	}
}

void error(ifstream& token_stream, int& current_token, int& char_num, vector<int>& stack, const char& error_type)
{
	switch (error_type)
	{
	case MISS_OP_OBJECT:
		cout << char_num << ": missing operation object" << endl;
		switch (stack.back())
		{
		case 0: case 6: case 7: case 8: case 9:
			stack.push_back(1);
			break;
		case 5: case 14: case 20: case 21: case 22: case 23:
			stack.push_back(13);
			break;
		}
		break;
	case UNMATCHED_PARENTHESES:
		cout << char_num << ": unmatched parentheses" << endl;
		current_token = forward_pointer(token_stream, char_num);
		break;
	case MISS_OP_SYMBLE:
		cout << char_num << ": missing operation symble" << endl;
		switch (stack.back())
		{
		case 3:
			stack.push_back(6);
			break;
		case 10: case 24:
			stack.push_back(20);
			break;
		}
		break;
	case MISS_RIGHT_PARENTHESIS:
		cout << char_num << ": missing right parenthesis" << endl;
		switch (stack.back())
		{
		case 10:
			stack.push_back(19);
			break;
		case 24:
			stack.push_back(29);
			break;
		}
		break;
	}
}

void syntax_analysis(ifstream& token_stream) {
	int char_num = 0;
	int current_token = forward_pointer(token_stream, char_num);
	vector<int> state_stack = { END, 0 };
	while (state_stack.back() != END)
	{	
		if (ACTION[state_stack.back()][current_token].operation == SHIFT)
		{
			state_stack.push_back(ACTION[state_stack.back()][current_token].index);
			current_token = forward_pointer(token_stream, char_num);
		}
		else if (ACTION[state_stack.back()][current_token].operation == REDUCE)
		{
			cout << productions[ACTION[state_stack.back()][current_token].index] << endl;
			int reduce_symble;
			//依据规约生成式右边符号串的符号数量，将栈中相同数量的状态弹出
			switch (ACTION[state_stack.back()][current_token].index)
			{
			case 2: case 3:
				state_stack.pop_back(); state_stack.pop_back(); state_stack.pop_back();
				reduce_symble = E;
				break;
			case 4:
				state_stack.pop_back();
				reduce_symble = E;
				break;
			case 5: case 6:
				state_stack.pop_back(); state_stack.pop_back(); state_stack.pop_back();
				reduce_symble = T;
				break;
			case 7:
				state_stack.pop_back();
				reduce_symble = T;
				break;
			case 8:
				state_stack.pop_back(); state_stack.pop_back(); state_stack.pop_back();
				reduce_symble = F;
				break;
			case 9:
				state_stack.pop_back();
				reduce_symble = F;
				break;
			}
			state_stack.push_back(ACTION[state_stack.back()][reduce_symble].index);
		}
		else if (ACTION[state_stack.back()][current_token].operation == ERROR)
			error(token_stream, current_token, char_num, state_stack, ACTION[state_stack.back()][current_token].index);
		else
		{
			cout << "ACCEPT" << endl;
			break;
		}
	}
}
