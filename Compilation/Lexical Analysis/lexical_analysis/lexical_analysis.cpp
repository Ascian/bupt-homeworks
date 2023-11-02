#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

const vector<string> KEYWORD_LIST = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
"else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
"return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
"void", "volatile", "while" };

const int WORD_TYPE_AMOUNT = 40; // word_type数量，不包含注释和具体的关系运算符和赋值运算符
enum word_type
{
    KEYWORD,                    //关键字
    ID,                         //标志符
    STRING,                     //字符串常量
    CHAR,                       //字符常量
    INT,                        //整型常量
    UINT,                       //无符号整型常量
    LONG,                       //长整型常量
    ULONG,                      //无符号长整型
    FLOAT,                      //单精度浮点数
    DOUBLE,                     //双精度浮点数
    RELATION_OPERATOR,          //关系运算符
    ASSIGN_OPERATOR,            //赋值运算符
    PLUS,                       //"+"
    MINUS,                      //"-"
    MULTIPLY,                   //"*"
    DIVIDE,                     //"/"
    MOD,                        //"%"
    INC,                        //"++"
    DEC,                        //"--"
    LOGICAL_AND,                //"&&"
    LOGICAL_OR,                 //"||"
    LOGICAL_NEGATION,           //"!"
    BITWISE_AND,                //"&"
    BITWISE_OR,                 //"|"
    BITWISE_NEGATION,           //"~"
    BITWISE_XOR,                //"^"
    BITWISE_LSHIFT,             //"<<"
    BITWISE_RSHIFT,             //">>"
    QUESTION_MARK,              //"?"
    COLON,                      //":"
    SEMICOLON,                  //";"
    LEFT_SQUARE_BRACKET,        //"["
    RIGHT_SQUARE_BRACKET,       //"]"
    LEFT_PARENTHESE,            //"("
    RIGHT_PARENTHESE,           //")"
    LEFT_BRACE,                 //"{"
    RIGHT_BRACE,                //"}"
    DOT,                        //"."
    COMMA,                      //","
    ARROW,                      //"->"
    ANNOTATION,                  //注释

    //关系运算符属性
    GREATER,                    //">"
    GREATER_EQUAL,              //">="
    LESS,                       //"<"
    LESS_EQUAL,                 //"<="
    EQUAL,                      //"=="
    UNEQUAL,                    //"!="

    //赋值运算符属性
    SIMPLE_EQUAL,               //"="
    PLUS_EQUAL,                 //"+="
    MINUS_EQUAL,                //"-="
    MULTIPLY_EQUAL,             //"*="
    DIVIDE_EQUAL,               //"/="
    MOD_EQUAL,                  //"%="
    AND_EQUAL,                  //"&="
    OR_EQUAL,                   //"|="
    XOR_EQUAL,                  //"^="
    LSHIFT_EQUAL,               //"<<="
    RSHIFT_EQUAL,               //">>="
};

union value_type
{
    int i;
    unsigned int ui;
    long l;
    unsigned long int ul;
    unsigned char c;
    float f;
    double d;
};

struct token
{
    word_type type;
    value_type value;
};

string to_string(word_type type)
{
    switch (type)
    {
    case KEYWORD:
        return "KW";
    case ID:
        return "ID";
    case CHAR:
        return "CHAR";
    case INT:
        return "INT";
    case UINT:
        return "UINT";
    case LONG:
        return "LONG_INT";
    case ULONG:
        return "ULONG";
    case FLOAT:
        return "FLOAT";
    case DOUBLE:
        return "DOUBLE";
    case STRING:
        return "STR";
    case RELATION_OPERATOR:
        return "RELOP";
    case GREATER:
        return ">";
    case GREATER_EQUAL:
        return ">=";
    case LESS:
        return "<";
    case LESS_EQUAL:
        return "<=";
    case EQUAL:
        return "==";
    case UNEQUAL:
        return "!=";
    case ASSIGN_OPERATOR:
        return "ASSIGN";
    case SIMPLE_EQUAL:
        return "=";
    case PLUS_EQUAL:
        return "+=";
    case MINUS_EQUAL:
        return "-=";
    case MULTIPLY_EQUAL:
        return "*=";
    case DIVIDE_EQUAL:
        return "/=";
    case MOD_EQUAL:
        return "%=";
    case AND_EQUAL:
        return "&=";
    case OR_EQUAL:
        return "|=";
    case XOR_EQUAL:
        return "^=";
    case LSHIFT_EQUAL:
        return "<<=";
    case RSHIFT_EQUAL:
        return ">>=";
    case PLUS:
        return "+";
    case MINUS:
        return "_";
    case MULTIPLY:
        return "*";
    case DIVIDE:
        return "/";
    case MOD:
        return "%";
    case INC:
        return "++";
    case DEC:
        return "--";
    case LOGICAL_AND:
        return "&&";
    case LOGICAL_OR:
        return "||";
    case LOGICAL_NEGATION:
        return "!";
    case BITWISE_AND:
        return "&";
    case BITWISE_OR:
        return "|";
    case BITWISE_NEGATION:
        return "~";
    case BITWISE_XOR:
        return "^";
    case BITWISE_LSHIFT:
        return "<<";
    case BITWISE_RSHIFT:
        return ">>";
    case QUESTION_MARK:
        return "?";
    case COLON:
        return ":";
    case SEMICOLON:
        return ";";
    case LEFT_SQUARE_BRACKET:
        return "[";
    case RIGHT_SQUARE_BRACKET:
        return "]";
    case LEFT_PARENTHESE:
        return "(";
    case RIGHT_PARENTHESE:
        return ")";
    case LEFT_BRACE:
        return "{";;
    case RIGHT_BRACE:
        return "}";
    case DOT:
        return ".";
    case COMMA:
        return ",";
    case ARROW:
        return "->";
    case ANNOTATION:
        return "";
    default:
        return "";
    }
}

inline string to_string(struct token token)
{
    ostringstream ostr;
    ostr << "<" + to_string((word_type)token.type) + ", ";
    if (token.type == CHAR)
        ostr << token.value.c;
    else if(token.type == INT || token.type == KEYWORD || token.type == ID || token.type == STRING)
        ostr << token.value.i;
    else if (token.type == UINT)
        ostr << token.value.ui;
    else if (token.type == LONG)
        ostr << token.value.l;
    else if (token.type == ULONG)
        ostr << token.value.ul;
    else if (token.type == FLOAT)
        ostr << token.value.f;
    else if (token.type == DOUBLE)
        ostr << token.value.d;
    else if (token.type == RELATION_OPERATOR || token.type == ASSIGN_OPERATOR)
        ostr << to_string((word_type)token.value.i);
    ostr << ">";
    return ostr.str();
}

/**
 * 对输入程序进行词法分析，输出对应记号流，统计源程序中的语句行数、各类单词的个数、以及字符总数，同时检查源程序中存在的词法错误，并报告错误所在的位置
 * vector<struct token>& token_stream - 需要返回的记号流
 * vector<string>& id_list - 标志符表
 * vector<string>& str_list - 字符串表
 * int& line_num - 行数
 * vector<int>& word_type_num - 每种单词类型的数量
 * int& char_num - 字符总数
 * ifstream& program - 源程序
 */
void lexical_analysis(vector<struct token>& token_stream, vector<string>& id_list, vector<string>& str_list, int& line_num, vector<int>& word_type_num, int& char_num, ifstream& program);

int main()
{

    ifstream program;
    program.open("program.txt", ios::in);
    vector<struct token> token_stream;
    vector<string> id_list;
    vector<string> str_list;
    int line_num = 0;
    int char_num = 0;
    vector<int> word_type_num(WORD_TYPE_AMOUNT);

    cout << "Designed by CHEN YU, built: " << __DATE__ << " " <<  __TIME__ << endl;

    lexical_analysis(token_stream, id_list, str_list, line_num, word_type_num, char_num, program);

    cout << endl << "keyword list:" << endl;
    for (int i = 0; i < KEYWORD_LIST.size(); i++) {
        cout << setiosflags(ios::left) << setw(10) << i << KEYWORD_LIST[i] << endl;
    }

    cout << endl << "ID list:" << endl;
    for (int i = 0; i < id_list.size(); i++) {
        cout << setiosflags(ios::left) << setw(10) << i << id_list[i] << endl;
    }

    cout << endl << "string list:" << endl;
    for (int i = 0; i < str_list.size(); i++) {
        cout << setiosflags(ios::left) << setw(10) << i << str_list[i] << endl;
    }

    cout << endl << "token stream:" << endl;
    for (int i = 0; i < token_stream.size();) {
        for(int j = 0; j < 10 && i < token_stream.size(); j++, i++)
            cout << setiosflags(ios::left) << setw(11) << to_string(token_stream[i]) << " ";
        cout << endl;
    }

    cout << endl << "word type num:" << endl;
    for (int i = 0; i < word_type_num.size(); i++)
    {
        cout << setiosflags(ios::left) << setw(14) << to_string((word_type)i) << word_type_num[i] << endl;
    }

    cout << "char num: " << char_num << endl;

    cout << "line num: " << line_num << endl;

    return 0;
}

inline bool is_letter(char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }

inline bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }

inline char get_char(int& char_num, ifstream& program)
{
    char_num++;
    return program.get();
}

inline void retract(int& char_num, ifstream& program)
{
    char_num--;
    program.unget();
}

inline void error(const string& str, const int& line_num)
{
    cout << "error " << line_num + 1 << ": " << str << endl;
}

//二分搜索str在KEYWORD_LIST的位置，若搜索到返回位置，否者返回-1
int reserve(const string& str)
{
    int high = KEYWORD_LIST.size() - 1;
    int low = 0;
    int middle = (high + low) / 2;
    while (high >= low)
    {
        middle = (high + low) / 2;
        if (KEYWORD_LIST[middle].compare(str) == 0)
            return middle;
        else if (KEYWORD_LIST[middle].compare(str) < 0)
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }
    return -1;
}

//搜索str在table的位置，若搜索到返回位置，否者插入到表格末尾
int table_insert(vector<string>& table, const string& str)
{
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i].compare(str) == 0)
            return i;
    }
    table.push_back(str);
    return table.size() - 1;
}

//将分析出的记号加入记号流
void word_analysis(vector<struct token>& token_stream, vector<string>& id_list, vector<string>& str_list, vector<int>& word_type_num,
    const int& line_num, const word_type& type, const string& buf = "", const int& num_base = 10)
{
    struct token token;
    token.type = type;
    switch (type)
    {
        int is_kw;
        int str_entry;
    case ID:
        is_kw = reserve(buf);
        if (is_kw != -1)
        {
            token = { KEYWORD, is_kw };
        }
        else
        {
            int identry = table_insert(id_list, buf);
            token = { ID, identry };
        }
        break;
    case CHAR:
        if (buf[0] == '\\')
        {
            if (buf.length() <= 1)
            {
                error(buf, line_num);
                return;
            }
            switch (buf[1])
            {
            case 'a':   token.value.c = '\a';   break;
            case 'b':   token.value.c = '\b';   break;
            case 'f':   token.value.c = '\f';   break;
            case 'n':   token.value.c = '\n';   break;
            case 'v':   token.value.c = '\v';   break;
            case '\\':  token.value.c = '\\';   break;
            case '\'':  token.value.c = '\'';   break;
            case '\"':  token.value.c = '\"';   break;
            case '0':   token.value.c = '\0';   break;
            case 'x':
                if (buf.length() >= 2 && (is_digit(buf[2]) || (buf[2] >= 'a' && buf[2] <= 'f') || (buf[2] >= 'A' && buf[2] <= 'F')))
                {
                    int temp = stoi(buf.substr(2), 0, 16);
                    if (temp > 0xff)
                    {
                        error(buf, line_num);
                        return;
                    }
                    token.value.c = stoi(buf.substr(2), 0, 16);
                }
                else
                {
                    error(buf, line_num);
                    return;
                }
                break;
            case '1':   case '2':   case '3':   case '4':
            case '5':   case '6':   case '7':
            {
                token.value.c = stoi(buf.substr(1), 0, 8);
                if (token.value.c > 0xff)
                {
                    error(buf, line_num);
                    return;
                }
            }
            break;
            default:
                token.value.c = buf[1];
            }
        }
        else
            token.value.c = buf[0];
        break;
    case INT:
        token.value.i = stoi(buf, 0, num_base);
        break;
    case UINT:
        token.value.ui = stoul(buf, 0, num_base);
        break;
    case LONG:
        token.value.l = stol(buf, 0, num_base);
        break;
    case ULONG:
        token.value.ul = stoul(buf, 0, num_base);
        break;
    case FLOAT:
        token.value.f = stof(buf);
        break;
    case DOUBLE:
        token.value.d = stod(buf);
        break;
    case STRING:
        str_entry = table_insert(str_list, buf);
        token.value.i = str_entry;
        break;
    case RELATION_OPERATOR: case ASSIGN_OPERATOR:
        token.value.i = stoi(buf);
        break;
    default:
        break;
    }
    word_type_num[token.type]++;
    token_stream.push_back(token);
}

void lexical_analysis(vector<struct token>& token_stream, vector<string>& id_list, vector<string>& str_list, int& line_num, vector<int>& word_type_num, int& char_num, ifstream& program)
{
    int state = 0;
    char c;
    char last_char;
    string buf;
    while (true)
    {
        switch (state)
        {
        case 0:
            buf = "";
            c = get_char(char_num, program);

            if (is_letter(c) || c == '_')
            {
                state = 1;
                break;
            }
            else if (is_digit(c) && c != '0')
            {
                state = 2;
                break;
            }
            switch (c)
            {
            case '0':   state = 3; break;
            case '\'':  state = 12; break;
            case '"':   state = 13; break;
            case '<':   state = 14; break;
            case '>':   state = 15; break;
            case '=':   state = 16; break;
            case '!':   state = 17; break;
            case '+':   state = 18; break;
            case '-':   state = 19; break;
            case '*':   state = 20; break;
            case '/':   state = 21; break;
            case '%':   state = 25; break;
            case '&':   state = 26; break;
            case '|':   state = 27; break;
            case '^':   state = 28; break;
            case '.':   state = 29; break;
            case '~':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_NEGATION);    break;
            case '?':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, QUESTION_MARK);    break;
            case ':':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, COLON);    break;
            case ';':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, SEMICOLON);    break;
            case '[':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LEFT_SQUARE_BRACKET);    break;
            case ']':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RIGHT_SQUARE_BRACKET);    break;
            case '(':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LEFT_PARENTHESE);    break;
            case ')':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RIGHT_PARENTHESE);    break;
            case '{':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LEFT_BRACE);    break;
            case '}':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RIGHT_BRACE);    break;
            case ',':   word_analysis(token_stream, id_list, str_list, word_type_num, line_num, COMMA);    break;
            case ' ':   case '\t':  break;
            case '\n':  line_num++; break;
            case EOF:   
                line_num++; //加上最后一行
                char_num--; //减去文件结束符EOF
                return;
            default:
                error(buf, line_num);
            }
            break;
        case 1: //标志符状态
            buf += c;
            c = get_char(char_num, program);
            if (is_letter(c) || is_digit(c) || c == '_' || c == '$') // 标志符由数字、字母、下划线_、美元符号$组成
                state = 1;
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ID, buf);
                state = 0;
            }
            break;
        case 2: //常数状态
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c))
                state = 2;
            else if (c == '.')
                state = 8;
            else if (c == 'e' || c == 'E')
                state = 9;
            else if (c == 'u' || c == 'U')
            {
                c = get_char(char_num, program);
                if (c == 'l' || c == 'L')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, UINT, buf);
                    state = 0;
                }
            }
            else if (c == 'l' || c == 'L')
            {
                c = get_char(char_num, program);
                if (c == 'u' || c == 'U')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LONG, buf);
                    state = 0;
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, INT, buf);
                state = 0;
            }
            break;
        case 3: //0开头状态
            buf += c;
            c = get_char(char_num, program);
            if (c <= '7' && c >= '0')
                state = 4;
            else if (c == 'x' || c == 'X')
                state = 5;
            else if (c > '7' && c <= '9')
                state = 7;
            else if (c == '.')
                state = 8;
            else if (c == 'e' || c == 'E')
                state = 9;
            else if (c == 'u' || c == 'U')
            {
                c = get_char(char_num, program);
                if (c == 'l' || c == 'L')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, UINT, buf);
                    state = 0;
                }
            }
            else if (c == 'l' || c == 'L')
            {
                c = get_char(char_num, program);
                if (c == 'u' || c == 'U')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LONG, buf);
                    state = 0;
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, INT, buf);
                state = 0;
            }
            break;
        case 4: //八进制数
            buf += c;
            c = get_char(char_num, program);
            if (c <= '7' && c >= '0')
                state = 4;
            else if (c > '7' && c <= '9')
                state = 7;
            else if (c == '.')
                state = 8;
            else if (c == 'e' || c == 'E')
                state = 9;
            else if (c == 'u' || c == 'U')
            {
                c = get_char(char_num, program);
                if (c == 'l' || c == 'L')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf, 8);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, UINT, buf, 8);
                    state = 0;
                }
            }
            else if (c == 'l' || c == 'L')
            {
                c = get_char(char_num, program);
                if (c == 'u' || c == 'U')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf, 8);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LONG, buf, 8);
                    state = 0;
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, INT, buf, 8);
                state = 0;
            }
            break;
        case 5: //十六进制数
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                state = 6;
            else
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            break;
        case 6:
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                state = 6;
            else if (c == 'u' || c == 'U')
            {
                c = get_char(char_num, program);
                if (c == 'l' || c == 'L')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf, 16);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, UINT, buf, 16);
                    state = 0;
                }
            }
            else if (c == 'l' || c == 'L')
            {
                c = get_char(char_num, program);
                if (c == 'u' || c == 'U')
                {
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ULONG, buf, 16);
                    state = 0;
                }
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LONG, buf, 16);
                    state = 0;
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, INT, buf, 16);
                state = 0;
            }
            break;
        case 7: //实型状态
            buf += c;
            c = get_char(char_num, program);
            if (c > '7' && c <= '9')
                state = 7;
            else if (c == '.')
                state = 8;
            else if (c == 'e' || c == 'E')
                state = 9;
            else
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            break;
        case 8: //小数状态
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c))
                state = 8;
            else if (c == 'e' || c == 'E')
                state = 9;
            else if (c == 'f' || c == 'F')
            {
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, FLOAT, buf);
                state = 0;
            }
            else if (c == 'l' || c == 'L')
            {
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, DOUBLE, buf);
                state = 0;
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, FLOAT, buf);
                state = 0;
            }
            break;
        case 9: //指数状态
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c))
                state = 11;
            else if (c == '+' || c == '-')
                state = 10;
            else
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            break;
        case 10:
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c))
                state = 11;
            else
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            break;
        case 11:
            buf += c;
            c = get_char(char_num, program);
            if (is_digit(c))
                state = 11;
            else if (c == 'f' || c == 'F')
            {
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, FLOAT, buf);
                state = 0;
            }
            else if (c == 'l' || c == 'L')
            {
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, DOUBLE, buf);
                state = 0;
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, FLOAT, buf);
                state = 0;
            }
            break;
        case 12: //字符常量
            buf += c;
            last_char = c;
            c = get_char(char_num, program);
            if (c == '\'')
            {
                if (last_char == '\\')
                    state = 12;
                else
                {
                    buf.erase(0, 1);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, CHAR, buf);
                    state = 0;
                }

            }
            else if (c == EOF || c == '\n')
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            else
                state = 12;
            break;
        case 13: // 字符串常量
            buf += c;
            last_char = c;
            c = get_char(char_num, program);
            if (c == '\"')
            {
                if (last_char == '\\')
                    state = 13;
                else
                {
                    buf.erase(0, 1);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, STRING, buf);
                    state = 0;
                }
            }
            else if (c == EOF || c == '\n')
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            else
                state = 13;
            break;
        case 14: //'<'状态
            c = get_char(char_num, program); 
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)LESS_EQUAL));
            else if (c == '<')
            {
                c = get_char(char_num, program);
                if (c == '=')
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)LSHIFT_EQUAL));
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_LSHIFT);
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)LESS));
            }
            state = 0;
            break;
        case 15: //">"状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)GREATER_EQUAL));
            else if (c == '>')
            {
                c = get_char(char_num, program);
                if (c == '=')
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)RSHIFT_EQUAL));
                else
                {
                    retract(char_num, program);
                    word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_RSHIFT);
                }
            }
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)GREATER));
            }
            state = 0;
            break;
        case 16: //'='状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)EQUAL));
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)SIMPLE_EQUAL));
            }
            state = 0;
            break;
        case 17: //'!'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, RELATION_OPERATOR, to_string((int)UNEQUAL));
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LOGICAL_NEGATION);
            }
            state = 0;
            break;
        case 18: //'+'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)PLUS_EQUAL));
            else if (c == '+')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, INC);
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, PLUS);
            }
            state = 0;
            break;
        case 19: //'-'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)MINUS_EQUAL));
            else if (c == '-')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, DEC);
            else if (c == '>')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ARROW);
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, MINUS);
            }
            state = 0;
            break;
        case 20: //'*'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)MULTIPLY_EQUAL));
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, MULTIPLY);
            }
            state = 0;
            break;
        case 21: // '/'状态
            c = get_char(char_num, program);
            if (c == '=')
            {
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)DIVIDE_EQUAL));
                state = 0;
            }
            else if (c == '/') //单行注释
                state = 22;
            else if (c == '*') //多行注释
                state = 23;
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, DIVIDE);
                state = 0;
            }
            break;
        case 22: //单行注释
            c = get_char(char_num, program);
            if (c == EOF || c == '\n')
            {
                retract(char_num, program);
                state = 0;
            }
            else
                state = 22;
            break;
        case 23: //多行注释
            c = get_char(char_num, program);
            if (c == EOF)
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            else if (c == '\n')
                line_num++;
            else if (c == '*')
                state = 24;
            else
                state = 23;
            break;
        case 24:
            c = get_char(char_num, program);
            if (c == EOF)
            {
                retract(char_num, program);
                error(buf, line_num);
                state = 0;
            }
            else if (c == '\n')
                line_num++;
            else if (c == '/')
                state = 0;
            else if (c == '*')
                state = 24;
            else
                state = 23;
            break;
        case 25: //'%'状态
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)MOD_EQUAL));
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, MOD);
            }
            state = 0;
            break;
        case 26: //'&'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)AND_EQUAL));
            else if (c == '&')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LOGICAL_AND);
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_AND);
            }
            state = 0;
            break;
        case 27: //'|'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)OR_EQUAL));
            else if (c == '|')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, LOGICAL_OR);
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_OR);
            }
            state = 0;
            break;
        case 28: //'^'状态
            c = get_char(char_num, program);
            if (c == '=')
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, ASSIGN_OPERATOR, to_string((int)XOR_EQUAL));
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, BITWISE_XOR);
            }
            state = 0;
            break;
        case 29: //'.'状态
            c = get_char(char_num, program);
            if (is_digit(c)) //小数
                state = 8;
            else
            {
                retract(char_num, program);
                word_analysis(token_stream, id_list, str_list, word_type_num, line_num, DOT);
                state = 0;
            }
            break;
        default:
            error(buf, line_num);
            break;
        }     
    }
}