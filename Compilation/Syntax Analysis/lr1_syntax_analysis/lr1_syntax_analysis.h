#pragma once

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//非终结符和终结符
enum symble
{
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	NUM,
	END,
	E,
	T,
	F,
};

//分析动作类型
enum operation
{
	EMPTY,
	SHIFT,
	REDUCE,
	ACCEPT,
	ERROR
};

//错误类型
enum error_type
{
	MISS_OP_OBJECT = 1,
	UNMATCHED_PARENTHESES,
	MISS_OP_SYMBLE,
	MISS_RIGHT_PARENTHESIS,
};

//分析表中分析动作
typedef struct action
{
	char operation;
	int index;
}action;

//LR(1)分析表，将go表并入ACTION表中，动作均为SHIFT
const action ACTION[30][11] = {
	//0
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 5},{ERROR, 2},{SHIFT, 1},{ERROR, 1},{SHIFT, 3},{SHIFT, 4},{SHIFT, 2}},
	//1
	{{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{REDUCE, 9},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//2
	{{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//3
	{{SHIFT, 6},{SHIFT, 7},{EMPTY, 0},{EMPTY, 0},{ERROR, 3},{ERROR, 2},{ERROR, 3},{ACCEPT, 0},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//4
	{{REDUCE, 4},{REDUCE, 4},{SHIFT, 8},{SHIFT, 9},{REDUCE, 4},{REDUCE, 4},{REDUCE, 4},{REDUCE, 4},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//5
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{SHIFT, 10},{SHIFT, 11},{SHIFT, 12}},
	//6
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 5},{ERROR, 2},{SHIFT, 1},{ERROR, 1},{EMPTY, 0},{SHIFT, 15},{SHIFT, 12}},
	//7
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 5},{ERROR, 2},{SHIFT, 1},{ERROR, 1},{EMPTY, 0},{SHIFT, 16},{SHIFT, 12}},
	//8
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 5},{ERROR, 2},{SHIFT, 1},{ERROR, 1},{EMPTY, 0},{EMPTY, 0},{SHIFT, 17}},
	//9
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 5},{ERROR, 2},{SHIFT, 1},{ERROR, 1},{EMPTY, 0},{EMPTY, 0},{SHIFT, 18}},
	//10
	{{SHIFT, 20},{SHIFT, 21},{EMPTY, 0},{EMPTY, 0},{ERROR, 3},{SHIFT, 19}, {ERROR, 3},{ERROR, 4},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//11
	{{REDUCE, 4},{REDUCE, 4},{SHIFT, 22},{SHIFT, 23},{REDUCE, 4},{ REDUCE, 4 }, {REDUCE, 4},{REDUCE, 4},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//12
	{{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{REDUCE, 7},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//13
	{{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{REDUCE, 9} ,{EMPTY, 0},{EMPTY, 0},{EMPTY, 0} },
	//14
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{SHIFT, 24},{SHIFT, 11},{SHIFT, 12}},
	//15
	{{REDUCE, 2} ,{REDUCE, 2} ,{SHIFT, 8} ,{SHIFT, 9} ,{REDUCE, 2} ,{REDUCE, 2} ,{REDUCE, 2} ,{REDUCE, 2},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//16
	{{REDUCE, 3},{REDUCE, 3},{SHIFT, 8},{SHIFT, 9},{REDUCE, 3},{REDUCE, 3},{REDUCE, 3},{REDUCE, 3},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//17
	{{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//18
	{{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//19
	{{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//20
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{EMPTY, 0},{SHIFT, 25},{SHIFT, 12}},
	//21
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{EMPTY, 0},{SHIFT, 26},{SHIFT, 12}},
	//22
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{EMPTY, 0},{EMPTY, 0},{SHIFT, 27}},
	//23
	{{ERROR, 1},{ERROR, 1},{ERROR, 1},{ERROR, 1},{SHIFT, 14},{ERROR, 2},{SHIFT, 13},{ERROR, 1},{EMPTY, 0},{EMPTY, 0},{SHIFT, 28}},
	//24
	{{SHIFT, 20},{SHIFT, 21},{EMPTY, 0},{EMPTY, 0},{ERROR, 3},{SHIFT, 29},{ERROR, 3},{ERROR, 4},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//25
	{{REDUCE, 2},{REDUCE, 2}, {SHIFT, 22},{SHIFT, 23},{REDUCE, 2},{REDUCE, 2},{REDUCE, 2},{REDUCE, 2},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//26
	{{REDUCE, 3},{REDUCE, 3},{SHIFT, 22},{SHIFT, 23},{REDUCE, 3},{REDUCE, 3},{REDUCE, 3},{REDUCE, 3},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//27
	{{REDUCE, 5},{REDUCE, 5}, {REDUCE, 5},{REDUCE, 5},{REDUCE, 5},{REDUCE, 5}, {REDUCE, 5},{REDUCE, 5},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//28
	{{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{REDUCE, 6},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}},
	//29
	{{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{REDUCE, 8},{EMPTY, 0},{EMPTY, 0},{EMPTY, 0}}
};

//文法生成式
const vector<string> productions = {
	"",
	"S->E",
	"E->E+T",
	"E->E-T",
	"E->T",
	"T->T*F",
	"T->T/F",
	"T->F",
	"F->(E)",
	"F->num",
};

void syntax_analysis(ifstream& token_stream);

