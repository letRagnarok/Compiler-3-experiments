#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cctype>
using namespace std;

set<pair<int, string>> se;
vector<string> keywords = {"auto", "double", "int", "struct", "break", "else", "long", "switch",
                           "case", "enum", "register", "typedef", "char", "extern", "return", "union",
                           "const", "float", "short", "unsigned", "continue", "for", "signed", "void",
                           "default", "goto", "sizeof", "volatile", "do", "if", "while", "static"}; // 关键词
vector<char> delimiters = {'(', ')', '{', '}', '[', ']', ';', ','};                                 // 分界符
vector<string> operators = {"+", "-", "*", "/", "=", "%", "++", "--", "==", "!=", "<=", ">=", "<", ">", "&&",
                            "||", "!", "&", "|", "^", "~", ">>", "<<", ".", "::"}; // 运算符
string map[5] = {"关键字", "标识符", "常数", "运算符", "分界符"};                  // 映射
bool isKeyword(const string &token)                                                // 判断是不是关键字
{
    for (const auto &keyword : keywords)
    {
        if (keyword == token)
            return true;
    }
    return false;
}

bool isDelimiter(char ch) // 判断是不是分界符
{
    for (const auto &delimiter : delimiters)
    {
        if (delimiter == ch)
            return true;
    }
    return false;
}

bool isOperator(const string &token) // 判断是不是运算符
{
    for (const auto &op : operators)
    {
        if (op == token)
            return true;
    }
    return false;
}

bool isIdentifier(const string &token) // 判断是不是标识符
{
    if (!isalpha(token[0]) && token[0] != '_')
        return false;
    for (char ch : token)
    {
        if (!isalnum(ch) && ch != '_')
            return false;
    }
    return true;
}

bool isConstantNumber(const string &token)
{
    bool decimalPoint = false;
    for (char ch : token)
    {
        if (!isdigit(ch) && ch != 'e' && ch != '-')
        {
            if (ch == '.' && !decimalPoint)
            {
                decimalPoint = true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

bool isConstantString(const string &token)
{
    for (const auto &keyword : keywords)
    {
        if (keyword == token)
            return false;
    }
    return true;
}

int main()
{
    ifstream ifs;
    ifs.open("code.txt", ios::in);
    if (ifs.is_open())
    {
        cout << "success to open the txt!" << endl;
        for (int i = 0; i < 5; i++)
            cout << i + 1 << " : " << map[i] << endl;
        string buf, sentence;
        while (getline(ifs, buf))
            sentence += buf;
        string token; // 保存当前解析的单词
        int i = 0;    // 源代码遍历索引
        bool correct = false;
        while (i < sentence.length())
        {
            char ch = sentence[i];
            correct = false;
            // 跳过空格和换行符
            if (ch == ' ' || ch == '\t' || ch == '\n')
            {
                ++i;
                continue;
            }
            // 处理分界符
            if (isDelimiter(ch))
            {
                cout << "(5, " << ch << ")" << endl;
                correct = true;
                ++i;
                continue;
            }
            // 处理运算符
            string op, temp;
            bool two = false;
            op += ch;
            temp += sentence[i + 1];
            if (isOperator(temp))
            {
                op += sentence[++i];
                two = true;
            }
            if (isOperator(op))
            {
                cout << "(4, " << op << ")" << endl;
                correct = true;
                ++i;
                continue;
            }
            else
            {
                if (two)
                {
                    op = {ch};
                    --i;
                    if (isOperator(op))
                    {
                        cout << "(4, " << op << ")" << endl;
                        correct = true;
                        ++i;
                        continue;
                    }
                }
            }
            // 处理标识符或关键字
            if (isalpha(ch) || ch == '_')
            {
                token += ch;
                while (++i < sentence.length() && isalnum(sentence[i]))
                {
                    token += sentence[i];
                }
                if (isKeyword(token))
                {
                    cout << "(1, " << token << ")" << endl;
                    correct = true;
                }
                else if (isIdentifier(token))
                {
                    cout << "(2, " << token << ")" << endl;
                    correct = true;
                }
                token.clear();
                continue;
            }
            // 处理数字常量
            if (isdigit(ch) || ch == '-')
            {
                token += ch;
                while (++i < sentence.length() && (isdigit(sentence[i]) || sentence[i] == '.' || sentence[i] == 'e'))
                {
                    token += sentence[i];
                }
                if (isConstantNumber(token))
                {
                    cout << "(3, " << token << ")" << endl;
                    correct = true;
                }
                token.clear();
                continue;
            }
            // 处理字符or字符串常量
            if (ch == '\"')
            {
                token += ch;
                while (++i < sentence.length() && sentence[i] != '\"')
                {
                    token += sentence[i];
                }
                token += "\"";
                if (isConstantString(token))
                {
                    cout << "(3, " << token << ")" << endl;
                    correct = true;
                }
                token.clear();
                ++i;
                continue;
            }
            if (!correct)
            {
                token.clear();
                ++i;
            }
        }
    }
    else
        cout << "failed to open the txt!" << endl;
    ifs.close();

    system("pause");
    return 0;
}
