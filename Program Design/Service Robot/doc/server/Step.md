# Step Class

脚本应答逻辑的步骤。包含应答文本、用户输入时限、跳转到下一步骤的条件、无用户输入时的下一步骤、默认下一步骤

note：下一步骤的索引`STEP_INDEX`定义来自于[SyntaxTree](SyntaxTree.md)，用于在`SyntaxTree`中获取下一`Step`

- [Macros](#macros)
- [Public Types](#public-types)
  - [struct Item](#struct-item)
  - [struct Expression](#struct-expression)
  - [struct Listen](#struct-listen)
  - [struct Branch](#struct-branch)
- [Properties](#properties)
- [Public Functions](#public-functions)
  - [const Expression\& getSpeakExpression() const](#const-expression-getspeakexpression-const)
  - [const Listen\& getListen() const](#const-listen-getlisten-const)
  - [void addExpressionItem(const bool isVarName, const string\& str)](#void-addexpressionitemconst-bool-isvarname-const-string-str)
  - [void setListen(const int\& beginTimer, const int\& endTimer)](#void-setlistenconst-int-begintimer-const-int-endtimer)
  - [void addBranch(const string\& answer, const STEP\_INDEX\& index)](#void-addbranchconst-string-answer-const-step_index-index)
  - [void setSilence(const STEP\_INDEX\& index)](#void-setsilenceconst-step_index-index)
  - [void setDefault(const STEP\_INDEX\& index)](#void-setdefaultconst-step_index-index)
  - [const STEP\_INDEX getNextStep(const string\& answer) const](#const-step_index-getnextstepconst-string-answer-const)



---
## Macros

|Name|Description|
|---|---|
|STEP_INDEX|来自`SyntaxTree`中的`Step`索引|

---
## Public Types

|Type|Name|
|---|---|---|
|struct|Item|
|struct|Expression|
|struct|Listen|
|struct|Branch|

---
### struct Item

应答文本的元素

|Type|Name|Description|
|---|---|---|
|bool|isVarName|标识字符串还是变量名|
|string|str|字符串或变量名|

---
### struct Expression

应答文本表达式

|Type|Name|Description|
|---|---|---|
|vector\<Item>|items|应答文本元素序列|

---
### struct Listen

用户输入时限

|Type|Name|Description|
|---|---|---|
|int|beginTimer|开始时间| 
|int|endTimer|结束时间| 

---
### struct Branch

跳转下一步骤的分支

|Type|Name|Description|
|---|---|---|
|string|answer|输入条件字符串| 
|STEP_INDEX|nextStep|下一步骤STEP_INDEX| 

---
## Properties

|Type|Name|Description|
|---|---|---|
|Expression|speak|应答文本|
|Listen|listenTimer|用户输入时限|
|vector\<Branch>|branches|跳转下一步骤的所有分支, 序号在前的匹配优先级更高|
|STEP_INDEX|silence|无用户输入时下一步骤|
|STEP_INDEX|deflt|默认下一步骤|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||Step()||
|const Expression&|getSpeakExpression() const|获取Speak表达式|
|const Listen&|getListen() const|获取输入时限|
|void|addExpressionItem(const bool isVarName, const string& str)|添加Speak表达式元素|
|void|setListen(const int& beginTimer, const int& endTimer)|设置输入时限|
|void|addBranch(const string& answer, const STEP_INDEX& index)|添加跳转步骤分支|
|void|setSilence(const STEP_INDEX& index)|设置无输入时下一步骤|
|void|setDefault(const STEP_INDEX& index)|设置默认下一步骤|
|const STEP_INDEX|getNextStep(const string& answer)|获取下一步骤|

---
### const Expression& getSpeakExpression() const

获取Speak表达式

---
### const Listen& getListen() const

获取输入时限

---
### void addExpressionItem(const bool isVarName, const string& str)

添加Speak表达式元素

#### parameter

* const bool isVarName - 是否为变量名
* const string& str - 变量名或字符串

---
### void setListen(const int& beginTimer, const int& endTimer)

设置输入时限

---
### void addBranch(const string& answer, const STEP_INDEX& index)

添加跳转步骤分支

#### parameter

* const string& answer - 用户意愿
* const STEP_INDEX& index - 下一分支索引

---
### void setSilence(const STEP_INDEX& index)

设置无输入时下一步骤

---
### void setDefault(const STEP_INDEX& index)

设置默认下一步骤

---
### const STEP_INDEX getNextStep(const string& answer) const

依据用户输入获取下一步骤的STEP_INDEX，简单匹配用户输入字符串和跳转条件，序号在前的branch优先匹配

#### parameter

* const string& answer - 输入字符串

#### return

返回下一步骤的STEP_INDEX

若字符串为空，返回无用户输入时的下一步骤;
若字符串不为空但没有匹配的下一步骤，返回默认下一步骤