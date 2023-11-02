#include "syntax_tree.h"

#include <fstream>
#include <map>

//记号类型
enum class TokenType
{
	//关键字
	KEY,
	//标志符
	ID,
	//字符串
	STRING,
	//变量名
	VAR,
	//数字
	NUM,
	//+
	ADD,
	//，
	COMMA,
};

//关键字类型
enum class KeyType
{
	STEP,
	SPEAK,
	LISTEN,
	BRANCH,
	SILENCE,
	DEFAULT,
	EXIT,
};

//词法分析状态转移表状态
enum class State
{
	//初始状态
	INIT,
	//标志符状态
	ID,
	//变量名状态1
	VAR1,
	//变量名状态2
	VAR2,
	//数字状态
	NUM,
	//字符串状态
	STRING,
};

//记号
struct Token
{
	//类型
	TokenType type;
	//值
	int value;
};

//Step存储信息
struct StepStoreInform
{
	//Step是否已定义
	bool isDefined;
	//Step索引
	STEP_INDEX index;
};

//判断字符串是否已在字符串表，若存在返回其索引，否则加入该字符串并返回索引
const int insertTable(vector<string>* table, const string& str)
{
	for (int i = 0; i < table->size(); i++)
	{
		if (table->at(i) == str)
			return i;
	}
	table->push_back(str);
	return table->size() - 1;
}

//分析标志符是否为关键字，若不是加入标志符表
const Token idAnalyse(vector<string>* idTable, const string& buf)
{
	if (buf == "Step")
		return { TokenType::KEY, static_cast<int>(KeyType::STEP) };
	else if (buf == "Speak")
		return { TokenType::KEY, static_cast<int>(KeyType::SPEAK) };
	else if (buf == "Listen")
		return { TokenType::KEY, static_cast<int>(KeyType::LISTEN) };
	else if (buf == "Branch")
		return { TokenType::KEY, static_cast<int>(KeyType::BRANCH) };
	else if (buf == "Silence")
		return { TokenType::KEY, static_cast<int>(KeyType::SILENCE) };
	else if (buf == "Default")
		return { TokenType::KEY, static_cast<int>(KeyType::DEFAULT) };
	else if (buf == "Exit")
		return { TokenType::KEY, static_cast<int>(KeyType::EXIT) };
	else
		return { TokenType::ID, insertTable(idTable, buf) };
}

//对句子进行词法分析，输出记号流，并将标志符、字符串和变量名的值加入字符表
void parseLine(vector<Token>* tokens, vector<string>* strTable, const string& line, 
	const int& lineNum)
{
	string tempLine = line;
	tempLine.push_back('#');	//添加注释符作为结束符，便于统一处理结束
	int pos = 0;
	string buf;
	State state = State::INIT;
	bool isEnd = false;
	while (!isEnd)
	{	
		char curChar = tempLine[pos];
		switch (state)
		{
		case State::INIT:
			buf = "";
			//首字符为字母或下划线，判定为标志符
			if (isalpha(curChar) || curChar == '_')  
			{
				state = State::ID;
				break;
			}
			//首字符为数字，判定为数字
			if (isdigit(curChar))
			{
				state = State::NUM;
				break;
			}
			switch (curChar)
			{
			case ' ': case '\t': pos++; break;
			case '#': isEnd = true; break;
			case '+': tokens->push_back({ TokenType::ADD, 0 }); pos++; break;
			case ',': tokens->push_back({ TokenType::COMMA, 0 }); pos++; break;
			case '$': state = State::VAR1; pos++; break;
			case '\"': state = State::STRING; pos++; break;
			default:
				throw ScriptParseException(lineNum, pos, "unknown token \"" + 
					string(1, curChar) + "\"");
			}
			break;

		case State::ID:
			buf.push_back(curChar);
			curChar = tempLine[++pos];
			//读入非字母，下划线和数字的字符，标志符识别结束
			if (!isalpha(curChar) && !isdigit(curChar) && curChar != '_')
			{
				tokens->push_back(idAnalyse(strTable, buf));
				state = State::INIT;
			}
			break;

		case State::NUM:
			buf.push_back(curChar);
			curChar = tempLine[++pos];
			//读入非数字的字符，数字识别结束
			if (!isdigit(curChar))
			{
				tokens->push_back({ TokenType::NUM, stoi(buf) });
				state = State::INIT;
			}
			break;

		case State::VAR1:
			//变量命名规则和标志符相同
			if (isalpha(curChar) || curChar == '_')
			{
				state = State::VAR2;
				break;
			}
			else
				throw ScriptParseException(lineNum, pos, 
					"the first character of the variable name \'" + 
					string(1, curChar) + "\' is invalid");

		case State::VAR2:
			buf.push_back(curChar);
			curChar = tempLine[++pos];
			if (!isalpha(curChar) && !isdigit(curChar) && curChar != '_')
			{
				tokens->push_back({TokenType::VAR, insertTable(strTable, buf) });
				state = State::INIT;
			}
			break;

		case State::STRING:
			if (curChar == '\"')
			{
				tokens->push_back({ TokenType::STRING, insertTable(strTable, buf) });
				pos++;
				state = State::INIT;
			}
			else if (curChar == '#')
				throw ScriptParseException(lineNum, pos, "missing closing quote on string \"" + 
					buf + "\"");
			else
			{
				buf.push_back(curChar);
				pos++;
			}
			break;
		}
	}	
}

//对记号流进行语法分析，返回记号流的关键字类型
const KeyType parseTokens(const vector<Token>& tokens, const int& lineNum)
{
	//判断记号流语法格式是否正确
	if (tokens[0].type != TokenType::KEY)
		throw ScriptParseException(lineNum, "the first token is not a keyword");

	switch (static_cast<KeyType>(tokens[0].value))
	{
	case KeyType::STEP:
		if (tokens.size() != 2 || tokens[1].type != TokenType::ID)
			throw ScriptParseException(lineNum, "the step identifier is missing or invalid");
		return KeyType::STEP;

	case KeyType::SPEAK:
		if (tokens.size() < 2)
			throw ScriptParseException(lineNum, "the speaking expression is missing");
		return KeyType::SPEAK;

	case KeyType::LISTEN:
		if (tokens.size() != 4 || tokens[1].type != TokenType::NUM ||
			tokens[2].type != TokenType::COMMA ||
			tokens[3].type != TokenType::NUM)
			throw ScriptParseException(lineNum, "the listening expression is missing or invalid");
		return KeyType::LISTEN;

	case KeyType::BRANCH:
		if (tokens.size() != 4 || tokens[1].type != TokenType::STRING ||
			tokens[2].type != TokenType::COMMA ||
			tokens[3].type != TokenType::ID)
			throw ScriptParseException(lineNum, "the branch expression is missing or invalid");
		return KeyType::BRANCH;

	case KeyType::SILENCE:
		if (tokens.size() != 2 || tokens[1].type != TokenType::ID)
			throw ScriptParseException(lineNum, "the next step identifier of silence is missing or invalid");
		return KeyType::SILENCE;

	case KeyType::DEFAULT:
		if (tokens.size() != 2 || tokens[1].type != TokenType::ID)
			throw ScriptParseException(lineNum, "the next step identifier of default is missing or invalid");
		return KeyType::DEFAULT;

	case KeyType::EXIT:
		if (tokens.size() != 1)
			throw ScriptParseException(lineNum, "unexpected tokens");
		return KeyType::EXIT;
	}
}

//返回Step标志符对应的索引
const STEP_INDEX getStepIndex(map<string, StepStoreInform>* stepInform, 
	const string& stepId, const bool isDefined = false)
{
	auto iter = stepInform->find(stepId);
	STEP_INDEX index;
	//若没有找到step标志符，将最小未被使用的索引，即step标志符数量作为新的标志符的索引
	if (iter == stepInform->end())
	{
		index = stepInform->size();
		stepInform->insert({ stepId, {isDefined, index} });
	}
	else
	{
		index = iter->second.index;
		iter->second.isDefined |= isDefined;	//只有isDefined为true时修改step状态
	}
	return index;
}

//处理Step类型句子，加入新的step
void processStep(vector<Step>* steps, STEP_INDEX* currentStepIndex, 
	map<string, StepStoreInform>* stepInform, const Token& stepIdentifier, 
	const vector<string>& strTable)
{
	//获取新step标志符的索引
	*currentStepIndex = getStepIndex(stepInform, strTable[stepIdentifier.value], true);

	//在新step标志符的索引位置上创建新的step
	if (*currentStepIndex > static_cast<int>(steps->size()) - 1)
	{
		Step emptyStep;
		steps->insert(steps->end(), *currentStepIndex - steps->size() + 1, emptyStep);
	}
}

//处理Speak类型句子，获取Speak表达式，并加入新的变量名
void processSpeak(Step* step, vector<string>* varNames, const vector<Token>& items,
	const vector<string>& strTable, const int& lineNum)
{
	for (int i = 0; i < items.size(); i++)
	{
		//判断应答表达式的语法格式是否正确
		if (items[i].type == TokenType::STRING)
			step->addExpressionItem(false, strTable[items[i].value]);
		else if (items[i].type == TokenType::VAR)
		{
			varNames->push_back(strTable[items[i].value]);
			step->addExpressionItem(true, strTable[items[i].value]);
		}
		else
			throw ScriptParseException(lineNum, "the expression item is not a string or a variable name");
		//两个语法元素中间应该由加号连接
		if (i + 1 < items.size() && items[++i].type != TokenType::ADD)
			throw ScriptParseException(lineNum, "missing plus mark");
	}
}

//处理Listen语句，获取时间限制
inline void processListen(Step* step, const Token& beginTimer, const Token& endTimer)
{
	step->setListen(beginTimer.value, endTimer.value);
}

//处理Branch语句，加入新的Branch
inline void processBranch(Step* step, map<string, StepStoreInform>* stepInform,
	const Token& answer, const Token& stepId, const vector<string>& strTable)
{
	STEP_INDEX index = getStepIndex(stepInform, strTable[stepId.value]);
	step->addBranch(strTable[answer.value], index);
}

//处理Silence语句，设置无输入时的跳转步骤
inline void processSilence(Step* step, map<string, StepStoreInform>* stepInform,
	const Token& stepId, const vector<string>& strTable)
{
	STEP_INDEX index = getStepIndex(stepInform, strTable[stepId.value]);
	step->setSilence(index);
}

//处理Default语句，设置默认时的跳转步骤
inline void processDefault(Step* step, map<string, StepStoreInform>* stepInform,
	const Token& stepId, const vector<string>& strTable)
{
	STEP_INDEX index = getStepIndex(stepInform, strTable[stepId.value]);
	step->setDefault(index);
}

void SyntaxTree::parse(const string& scriptFilePath)
{
	ifstream scriptFile(scriptFilePath, ios::in);
	if (!scriptFile.is_open())
		throw ScriptFileException("No such file", scriptFilePath);
	//存储step标志符相关信息，允许在step定义前存储被其他step调用的标志符
	map<string, StepStoreInform> stepInform;
	vector<string> strTable;
	string line;
	vector<Token> tokens;
	int lineNum = 0;
	STEP_INDEX currentStepIndex = 0;

	//获取第一个step定义语句，避免未定义step时对当前step进行操作
	while (!scriptFile.eof())
	{
		getline(scriptFile, line);
		lineNum++;
		parseLine(&tokens, &strTable, line, lineNum);  //对句子进行词法分析，获取记号流
		if (tokens.size() != 0)
		{
			KeyType type = parseTokens(tokens, lineNum);  //对记号流进行语法分析，分析句子类型
			//第一条有意义的语句非STEP语句
			if (type != KeyType::STEP)
				throw ScriptParseException(lineNum, "the first step is not defined");
			else
				processStep(&steps, &currentStepIndex, &stepInform, tokens[1], strTable);
			break;
		}
	}
	if(stepInform.size() == 0)
		throw ScriptParseException("no defined steps");
	entry = 0;	//入口step

	while (!scriptFile.eof())
	{
		getline(scriptFile, line);
		lineNum++;
		tokens.clear();
		parseLine(&tokens, &strTable, line, lineNum);  //对句子进行词法分析，获取记号流
		if (tokens.size() != 0)
		{
			KeyType type = parseTokens(tokens, lineNum);  //对记号流进行语法分析，分析句子类型
			switch (type)
			{
			case KeyType::STEP:
				processStep(&steps, &currentStepIndex, &stepInform, tokens[1], strTable);
				break;
			case KeyType::SPEAK:
			{
				vector<Token> items(tokens.begin() + 1, tokens.end());
				processSpeak(&steps[currentStepIndex], &variables, items, strTable, lineNum);
			}
			break;
			case KeyType::LISTEN:
				processListen(&steps[currentStepIndex], tokens[1], tokens[3]);
				break;
			case KeyType::BRANCH:
				processBranch(&steps[currentStepIndex], &stepInform, tokens[1], tokens[3],
					strTable);
				break;
			case KeyType::SILENCE:
				processSilence(&steps[currentStepIndex], &stepInform, tokens[1], strTable);
				break;
			case KeyType::DEFAULT:
				processDefault(&steps[currentStepIndex], &stepInform, tokens[1], strTable);
				break;
			case KeyType::EXIT:
				exits.insert(currentStepIndex);
				break;
			}
		}
	}

	//判断使用到的step标志符是否全部定义
	for (auto iter : stepInform)
	{
		if (iter.second.isDefined == false)
			throw ScriptParseException("the step identifier \'" + iter.first + "\' is undefined");
	}

	scriptFile.close();
}