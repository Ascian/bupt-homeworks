# SyntaxTree Class

脚本语法树。包含所有应答逻辑步骤，变量名，入口步骤和出口步骤

依据脚本文件，分析得到脚本语法树。
分析失败抛出异常，可以获取异常内容，分析脚本错误原因

example：

    //可以输入脚本文件路径，分析得到脚本语法树
    //分析失败抛出异常，可以获取异常内容，分析脚本错误原因
    try
    {
        syntaxTree.parse(scriptFileName);
    }
    catch(ScriptFileException& e)
    {
 		cout << e.what();
    }
    catch(ScriptParseException& e)
    {
 		cout << e.what();
    } 

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [void parse(const string\& scriptFilePath)](#void-parseconst-string-scriptfilepath)
  - [const STEP\_INDEX\& getEntry() const](#const-step_index-getentry-const)
  - [const bool isExit(const STEP\_INDEX\& id) const](#const-bool-isexitconst-step_index-id-const)
  - [const Step\& getStep(const STEP\_INDEX\& index) const](#const-step-getstepconst-step_index-index-const)
  - [const vector\& getVarNames() const](#const-vector-getvarnames-const)


---
## Properties

|Type|Name|Description|
|---|---|---|
|vector\<Step>|steps|应答逻辑步骤|
|vector\<string>|variables|变量名|
|STEP_INDEX|entry|入口步骤|
|set\<STEP_INDEX>|exits|出口步骤|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||SyntaxTree()||
|void|parse(const string& scriptFilePath)|解析脚本文件|
|const STEP_INDEX&|getEntry() const|获取入口步骤|
|const bool|isExit(const STEP_INDEX& id) const|判断指定步骤是否为出口步骤|
|const Step&|getStep(const STEP_INDEX& index) const|依据步骤索引获取步骤|
|const vector<string>&|getVarNames() const|获取变量名表|

---
### void parse(const string& scriptFilePath)

输入脚本文件路径，解析脚本文件，存储脚本分析树

#### parameter

* const string& scriptFilePath - 脚本文件路径

#### exception

* 若文件打开错误，抛出ScriptFileException
* 若脚本语法错误，抛出ScriptParseException
  
---
### const STEP_INDEX& getEntry() const

获取入口步骤

---
### const bool isExit(const STEP_INDEX& id) const

判断指定步骤是否为出口步骤

#### parameter

* const STEP_INDEX& id - 指定步骤索引

---
### const Step& getStep(const STEP_INDEX& index) const

依据步骤索引获取步骤

---
### const vector<string>& getVarNames() const

获取变量名表