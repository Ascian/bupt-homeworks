#pragma once

#include<iostream>
#include<string>
#include<set>
#include<map>

using namespace std;

class CFG
{
private:
	set<char> N;//全为大字母
	set<char> T;//除大写字母以外字符
	multimap<char, string> P;
	char InitSymb;
public:
	inline void addN(const char symbol) {
		if (symbol < 'A' || symbol > 'Z'){
			throw("Invalid non-terminal symbol");
		}
		if (N.contains(symbol))
			throw("The symbol is already used");
		N.insert(symbol);
	}

	inline void addT(const char symbol) {
		if (symbol >= 'A' && symbol <= 'Z') {
			throw("Invalid terminal symbol");
		}
		if (T.contains(symbol))
			throw("The symbol is already used");
		T.insert(symbol);
	}

	inline void addP(const char symbol, const string& str) {
		if (!N.contains(symbol))
			throw("No such symbol");
		for (char ch : str) {
			if (!(T.contains(ch) || N.contains(ch)))
				throw("The string contains symbol not exists");
		}
		P.insert({ symbol, str });
	}

	inline void setInitSymb(const char symbol) {
		if (!N.contains(symbol))
			throw("No such symbol");
		InitSymb = symbol;
	}

	//删除无用符号
	void removeNoUseSymb();

	//删除ε生成式
	void removeDeltaTrans();

	//删除单生成式
	void removeSingleTrans();

	//转变为CNF
	void transformToCNF();

	//转变为GNF
	void transformToGNF();

	friend ostream& operator<<(ostream& Ostr, const CFG& cfg);
};
