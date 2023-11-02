# CFG_transformation

## `class CFG`

上下文无关文法，能够转换到无ε生成式，无单生成式，无无用符号的等价文法，转换到CNF，转换到GNF

### Private Attributes

|Member type|Definition|Description|
|----|----|---|
|set<char\>|N|非终结符|
|set<char\>|T|终结符|
|multimap<char, string>|P|生成式，非终结符到字符串的映射|
|char|InitSymb|初始非终结符|


### Public Functions

|Member Function|Description|
|----|----|
|void addN(const char symbol)|添加非终结符|
|void addT(const char symbol)|添加终结符|
|void addP(const char symbol, const string& str)|添加生成式|
|void setInitSymb(const char symbol)|设置初始非终结符|
|void removeNoUseSymb()|移除无用符号|
|void removeDeltaTrans()|移除ε生成式|
|void removeSingleTrans()|移除单生成式|
|void transformToCNF()|转换到CNF|
|void transformToGNF()|转换到GNF|
|friend ostream& operator<<(ostream& Ostr, const CFG& cfg)|输出上下文无关文法|