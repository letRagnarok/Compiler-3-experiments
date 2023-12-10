#include <bits/stdc++.h>
using namespace std;

map<char, int> getnum; // 映射,用来存储每个非终结符和终结符的编号
vector<char> getzf;    // 用来存储产生式中出现的所有的非终结符和终结符
vector<string> proce(10);
vector<string> first(20);
vector<string> follow(20);
int table[100][100]; // 预测分析表
int num;
int numv; // 终结符的数量-1
string j;

// 读取终结符、非终结符与产生式
void read()
{
    char c;
    int i = 0;
    int n = 0;
    cout << "******************************LL(1)语法分析****************************" << endl;
    cout << "注意：E'用H代替，T'用J代替，空串用@代替 " << endl;
    cout << "请输入产生式的集合（空串用'@'表示）,输入一条产生式后换行，最后以'end'结束:" << endl;
    string ss;
    int j = 0; // 记录（展开后）有几条产生式
    int y = 0;
    while (cin >> ss && ss != "end") // 读入产生式
    {
        proce[j] += ss[0];
        for (i = 3; i < ss.length(); i++)
        {
            if (ss[i] != '|')
            {
                proce[j] += ss[i];
            }
            else
            {
                proce[++j] += ss[0];
                proce[j] += ss[++i];
            }
        }
        j++;
    }
    getnum['#'] = 0; // #代表结束标志
    //	getzf[0]='#';没有定义数组大小的时候这样输入是错误的
    getzf.push_back('#');
    // 终结符压栈
    for (int i = 0; i < proce.size(); i++) // 遍历每一条产生式
    {
        for (int k = 0; k < proce[i].length(); k++) // 遍历每条产生式里的每个字符
        {
            if (proce[i][k] != '|')
            {
                if (proce[i][k] < 64 || proce[i][k] > 90) // 处理非终结符（比较的是ascii码）
                {
                    for (y = 0; y < getzf.size(); y++) // 检查proce[i][k]是否已经在getzf中
                    {
                        if (proce[i][k] == getzf[y])
                            break;
                    }
                    if (y == getzf.size()) // 如果proce[i][k]未在getzf中
                    {
                        getnum[proce[i][k]] = ++n;
                        getzf.push_back(proce[i][k]);
                    }
                }
            }
        }
    }
    getnum['@'] = ++n; // 映射‘空’
    numv = n;          // 终结符的数量等于当前n的值
    getzf.push_back('@');
    // 非终结符压栈
    for (int i = 0; i < proce.size(); i++)
    {
        for (int k = 0; k < proce[i].length(); k++)
        {
            if (proce[i][k] != '|')
            {
                if (proce[i][k] > 64 && proce[i][k] < 91)
                {
                    for (y = 0; y < getzf.size(); y++) // 检查proce[i][k]是否已经在getzf中
                    {
                        if (proce[i][k] == getzf[y])
                            break;
                    }
                    if (y == getzf.size())
                    {
                        getnum[proce[i][k]] = ++n;
                        // num = n;
                        getzf.push_back(proce[i][k]);
                    }
                }
            }
        }
    }
    num = n; // 非终结符+终结符的总个数
}

// 给终结符的first数组赋值
void get_firstT()
{
    int i;
    // 先给终结符的first数组赋值
    for (i = 1; i <= numv; i++)
    {
        // itoa(i, j, 10);
        j = to_string(i);
        first[i] = j; // 之前写的是first[i].push_back(j[0])是错的，字符串数组的输入不需要加下标,且如果是j[0]一个字符不能装到一个字符串当中去
    }
}

// 给非终结符的first数组赋值
string get_firstF(int *a)
{ // 犯了一个错误，下面的a没有加*
    for (int i = 0; i < proce.size(); i++)
    {
        if (getnum[proce[i][0]] == *a)
        {
            if (getnum[proce[i][1]] <= numv)
            {
                // itoa(getnum[proce[i][1]], j, 10);
                j = to_string(getnum[proce[i][1]]);
                first[*a] += j;
            }
            else
            {
                // first[getnum[proce[i][0]]].clear();
                first[getnum[proce[i][0]]] = get_firstF(&getnum[proce[i][1]]);
            }
        }
    }
    return first[*a];
}

// 求follow集
void get_follow(int *a)
{
    // 犯了一个错误，以stirng开头但是没有返回值
    int i, j1;
    int flag = 0;
    for (i = 0; i < proce.size(); i++)
    {
        for (j1 = 1; j1 < proce[i].length(); j1++)
        {
            if (getnum[proce[i][j1]] == *a) // 这地方应该是j1我写成了k
            {
                if (j1 == proce[i].length() - 1)
                {
                    if (getnum[proce[i][j1]] != getnum[proce[i][0]])
                        follow[*a] += follow[getnum[proce[i][0]]];
                }
                else
                {
                    if (getnum[proce[i][j1 + 1]] <= numv)
                    {
                        // itoa(getnum[proce[i][j1 + 1]], j, 10);
                        j = to_string(getnum[proce[i][j1 + 1]]);
                        follow[*a] += j;
                    }
                    else
                    {
                        for (int jj = 0; jj < first[getnum[proce[i][j1 + 1]]].size(); jj++)
                        {
                            if (atoi(&first[getnum[proce[i][j1 + 1]]][jj]) == numv) // 等于空时
                                follow[*a] += follow[getnum[proce[i][0]]];
                            else
                                follow[*a] += first[getnum[proce[i][j1 + 1]]][jj];
                        }
                        flag = 1; // 标志位，如果已经找到*a后面的非终结符就可以停止了
                    }
                }
            }
        }
        if (flag == 1)
            break; // 停止寻找
    }
}

// 求LL(1)分析表
void get_table()
{
    memset(table, -1, sizeof(table));      // 刚开始tableM没有初始化，导致本该是空格的地方出现E->TA
    for (int i = 0; i < proce.size(); i++) // 扫所有产生式
    {
        if (proce[i][1] == '@') // 直接推出空字的，特判下（follow集=产生式左边的vn中元素填）
        {
            string flw = follow[getnum[proce[i][0]]];
            for (int k = 0; k < flw.size(); k++)
            {
                table[getnum[proce[i][0]]][flw[k] - '0'] = i;
            }
        }
        string temps = first[getnum[proce[i][1]]];
        for (int j = 0; j < temps.size(); j++) // 考察first集
        {
            if (atoi(&temps[j]) != numv)
            {
                // table[getnum[proce[i][0]]][atoi(&temps[j])]=i;//atoi不能这么用，他表示的是从当前位一直到末位
                table[getnum[proce[i][0]]][temps[j] - '0'] = i;
            }
            else // 有空字的，考察follw集
            {
                string flw = follow[getnum[proce[i][1]]];
                for (int k = 0; k < flw.size(); k++)
                {
                    table[getnum[proce[i][0]]][flw[k] - '0'] = i;
                }
            }
        }
    }
}

// 由对应下标获得对应产生式
string get_proce(int i)
{
    if (i < 0)
        return " "; // 无该产生式
    string ss;
    ss += proce[i][0];
    ss += "->"; // 把->要加上
    for (int j = 1; j < proce[i].size(); j++)
        ss += proce[i][j];
    return ss;
}

// 输出预测分析表
void print_table()
{
    cout << "该文法对应的预测分析表如下：" << endl;
    cout << "________________________________________________________" << endl;
    for (int i = 0; i < numv; i++)
        cout << '\t' << getzf[i];
    // cout << endl;
    for (int i = numv + 1; i <= num; i++)
    {
        cout << endl
             << "________________________________________________________" << endl;
        cout << getzf[i];
        for (int j = 0; j < numv; j++)
        {
            cout << '\t' << get_proce(table[i][j]) << "";
        }
    }
    cout << endl
         << "________________________________________________________" << endl;
    cout << endl;
}

// 打印栈中元素
void print_stack(stack<char> sta)
{
    int length = sta.size(); // 栈中元素数目
    vector<char> temp;
    for (int i = 0; i < length; i++)
    {
        // cout<<sta.top();
        temp.push_back(sta.top());
        sta.pop();
    }
    for (int j = length - 1; j >= 0; j--)
    {
        cout << temp[j];
    }
    cout << "\t\t";
}

// 打印判定串的当前元素
void print_string(string str, int index)
{
    int length = str.size();
    for (int i = index; i < length; i++)
    {
        cout << str[i];
    }
    cout << '#';
    cout << "\t\t";
}

// 分析word符号串的合法性(关键)
string word;
bool analyze()
{
    stack<char> sta;       // 分析栈
    sta.push('#');         // #最先进栈
    sta.push(proce[0][0]); // 将开始符压入分析栈中进行初始化
    int i = 0;
    int count = 1;                                    // 步骤计数
    printf("步骤\t\t分析栈\t\t判定串\t\t使用规则\n"); // 表头
    while (!sta.empty())
    {
        cout << count << "\t\t";
        print_stack(sta);      // 打印当前分析栈
        print_string(word, i); // 打印当前判定串
        int cur = sta.top();   // 取出分析栈的栈顶元素
        sta.pop();
        if (cur == word[i]) // 如果分析栈的栈顶元素与判定串的第一个元素相同（即是终结符的话），则匹配成功，分析栈弹出栈顶元素
        {
            i++;
            printf("匹配出栈\n");
        }
        else if (cur == '#') // 如果分析栈的栈顶元素为#则表面分析结束
        {
            return 1;
        }
        else if (table[getnum[cur]][getnum[word[i]]] != -1) // 查找对应的预测分析表
        {

            int k = table[getnum[cur]][getnum[word[i]]];
            cout << proce[k][0] << "->";
            for (int j = 1; j < proce[k].size(); j++)
                cout << proce[k][j];
            cout << endl;
            // 将使用的产生式逆序加入分析栈中，以取代上一个出栈的元素
            for (int j = proce[k].size() - 1; j > 0; j--)
            {
                if (proce[k][j] != '@')
                    sta.push(proce[k][j]);
            }
        }
        else
        {
            return 0;
        }
        count++;
    }
    return 1;
}

// 输入待判断的符号串
void scanf()
{
    cout << "请输入待判定的符号串：";
    cin >> word;
    cout << "判断分析表如下：" << endl;
    if (analyze())
        cout << endl
             << "综上：该符号串有效，是该文法的句型！" << endl;
    else
        cout << endl
             << "出错，该符号串不是该文法的句型！" << endl;
}

int main()
{
    int k;
    int j;
    read();
    // 终结符的first集
    get_firstT();
    // 非终结符的first集
    for (k = numv + 1; k <= num; k++) // 犯了一个错误，numv的位置写成7
    {
        get_firstF(&k);
    }

    follow[numv + 1] += '0';          // 这地方加的是0而不是#
    for (k = numv + 1; k <= num; k++) // 犯了一个错误，numv的位置写成7
    {
        get_follow(&k);
    }
    cout << endl;
    get_table();
    print_table();
    scanf();
    system("pause");
    return 0;
}
/*测试数据：
注意：E'用H代替，T‘用J代替，空串用@代替
E->TH
H->+TH
H->@
T->FJ
J->*FJ
J->@
F->(E)
F->i
end
*/
