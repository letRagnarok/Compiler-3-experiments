#include <bits/stdc++.h>
using namespace std;

string operators[] = {"+", "-", "*", "/"};

bool isOperator(const string &token) // 判断是不是运算符
{
    for (int i = 0; i < 4; i++)
        if (operators[i] == token)
            return true;
    return false;
}

void G_S_translate(string buf)
{
    char n_terminal = buf[0];

    int i = 3;
    string s = "";
    while (i < buf.length())
    {
        if (buf[i] == ':' && buf[i + 1] == '=') // i:=E
        {
            string temp = buf.substr(i + 2);
            cout << "(:=, " << temp << ", -, " << s << ")\n";
            break;
        }
        else if (buf.length() == 4)
        {
            cout << "(:=, " << buf[3] << ", -, " << n_terminal << ")\n";
            break;
        }
        else if (buf[i] == '(')
        {
            int j = i + 1;
            string temp = "";
            temp += buf[j++];
            while (buf[j] != ')' && j < buf.length())
            {
                temp += buf[j];
                j++;
            }
            cout << "(:=, " << temp << ", -, " << n_terminal << ")\n";
            break;
        }
        else if (isOperator(string(1, buf[i]))) // 运算符or负号
        {
            if (s != "") // 运算符
            {
                char op = buf[i];
                string temp = buf.substr(i + 1);
                cout << "(" << op << ", " << s << ", " << temp << ", " << n_terminal << ")\n";
                break;
            }
            else
            {
                s = buf.substr(i + 1);
                cout << "(-, " << s << ", -, " << n_terminal << ")\n";
                break;
            }
        }
        else
            s += buf[i];
        i++;
    }
}

void G_D_translate(string buf)
{
    if (buf.length() <= 3)
        cout << "格式错误！\n";
    char n_terminal = buf[0]; // 记录非终结符
    int i = 3;
    string c = "";
    while (i < buf.length()) // 从->之后开始
    {
        if (buf[i] == ':')
        {
            string temp = buf.substr(i + 1);
            cout << "(:=, " << temp << ", -, " << n_terminal << ")\n";
        }
        else if (buf[i] == ' ' && c == "var")
        {
            string temp = buf.substr(i + 1);
            cout << "(:=, " << temp << ", -, " << n_terminal << ")\n";
        }
        else if (buf[i] == ',')
        {
            string temp = buf.substr(i + 1) + "1";
            cout << "(:=, " << temp << ", -, " << n_terminal << ")\n";
        }
        else
            c += buf[i];
        i++;
    }
}

void open_files(string file_name, int mode) // 0->G[S]，1->G[D]
{
    ifstream ifs;
    ifs.open(file_name, ios::in);
    if (ifs.is_open())
    {
        cout << "\n开始翻译...\n";
        if (mode == 0) // 赋值语句文法的翻译
        {
            string buf;
            while (getline(ifs, buf))
                G_S_translate(buf);
        }
        else // 说明文法的翻译
        {
            string buf;
            while (getline(ifs, buf))
                G_D_translate(buf);
        }
        cout << "翻译完毕...\n";
    }
    ifs.close();
}

int main()
{
    cout << "说明文法G[D]的翻译：\n";
    open_files("G[D].txt", 1);
    cout << "\n赋值语句文法G[S]的翻译：\n";
    open_files("G[S].txt", 0);
    system("pause");
    return 0;
}
