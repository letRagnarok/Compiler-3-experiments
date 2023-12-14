#include <bits/stdc++.h>
using namespace std;

char terminals[100] = {};     // 终结符
int no_t;                     // the number of terminals
char non_terminals[100] = {}; // 非终结符
int no_nt;                    // the number of non terminals
string action[100][100] = {};
int GOTO[100][100] = {};

struct state
{
    int prod_count = 0;         // 产生式的数量
    char prod[100][100] = {{}}; // 产生式们
};

void add_dots(struct state *I) // 在产生式左侧加一个.
{
    for (int i = 0; i < I->prod_count; i++)
    {
        for (int j = 99; j > 3; j--)
            I->prod[i][j] = I->prod[i][j - 1];
        I->prod[i][3] = '.';
    }
}

void augument(struct state *S, struct state *I) // I[0]，init
{
    if (I->prod[0][0] == 'S')
        strcpy(S->prod[0], "Z->.S");
    else
    {
        strcpy(S->prod[0], "S->.");
        S->prod[0][4] = I->prod[0][0];
    }
    S->prod_count++;
}

void get_prods(struct state *I)
{
    cout << "Enter the number of productions:\n";
    cin >> I->prod_count;
    cout << "Enter the number of non terminals:" << endl;
    cin >> no_nt;
    cout << "Enter the non terminals one by one:" << endl;
    for (int i = 0; i < no_nt; i++)
    {
        cin >> non_terminals[i];
        GOTO[0][i + 1] = non_terminals[i];
    }
    cout << "Enter the number of terminals:" << endl;
    cin >> no_t;
    cout << "Enter the terminals (single lettered) one by one:" << endl;
    for (int i = 0; i < no_t; i++)
    {
        cin >> terminals[i];
        action[0][i + 1] = terminals[i];
    }
    terminals[no_t++] = '#'; // 增加结束符号
    action[0][no_t] = "#";

    cout << "Enter the productions one by one in form (S->ABc):\n";
    for (int i = 0; i < I->prod_count; i++)
    {
        cin >> I->prod[i];
    }
}

bool is_non_terminal(char a)
{
    if (a >= 'A' && a <= 'Z')
        return true;
    else
        return false;
}

bool in_state(struct state *I, char *a) // 检查给定的产生式是否已经在当前的状态中
{
    for (int i = 0; i < I->prod_count; i++)
    {
        if (!strcmp(I->prod[i], a))
            return true;
    }
    return false;
}

char char_after_dot(char a[100]) // 返回.之后的第一个字符
{
    char b;
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == '.')
        {
            b = a[i + 1];
            return b;
        }
}

string move_dot(const string &b) //.向后移一位，返回修改后的字符串a
{
    string a = b;
    for (int i = 0; i < b.length(); i++)
    {
        if (a[i] == '.')
        {
            swap(a[i], a[i + 1]);
            break;
        }
    }
    return a;
}

bool same_state(struct state *I0, struct state *I) // 判断两个state是否相同
{

    if (I0->prod_count != I->prod_count)
        return false;

    for (int i = 0; i < I0->prod_count; i++)
    {
        int flag = 0;
        for (int j = 0; j < I->prod_count; j++)
            if (strcmp(I0->prod[i], I->prod[j]) == 0)
                flag = 1;
        if (flag == 0)
            return false;
    }
    return true;
}

void closure(struct state *I, struct state *I0) // init,I[0]
{
    char a = {};
    for (int i = 0; i < I0->prod_count; i++)
    {
        a = char_after_dot(I0->prod[i]);
        if (is_non_terminal(a))
        {
            for (int j = 0; j < I->prod_count; j++)
            {
                if (I->prod[j][0] == a)
                {
                    if (!in_state(I0, I->prod[j]))
                    {
                        strcpy(I0->prod[I0->prod_count], I->prod[j]);
                        I0->prod_count++;
                    }
                }
            }
        }
    }
}

void goto_state(struct state *I, struct state *S, char a) // 状态转移
{
    for (int i = 0; i < I->prod_count; i++)
    {
        if (char_after_dot(I->prod[i]) == a)
        {
            strcpy(S->prod[S->prod_count], move_dot(I->prod[i]).c_str());
            S->prod_count++;
        }
    }
}

void print_prods(struct state *I)
{
    for (int i = 0; i < I->prod_count; i++)
        printf("%s\n", I->prod[i]);
    cout << endl;
}

bool in_array(char a[20], char b) // 检查给定的字符是否在给定的数组中
{
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == b)
            return true;
    return false;
}

void cleanup_prods(struct state *I) // 清除给定状态中的所有产生式
{
    char a[100] = {};
    for (int i = 0; i < I->prod_count; i++)
        strcpy(I->prod[i], a);
    I->prod_count = 0;
}

void action_or_GOTO(int i, int k, char c)
{
    if (c >= 'A' && c <= 'Z') // 说明是非终结符，完善GOTO
    {
        for (int m = 0; m < no_nt; m++)
            if (non_terminals[m] == c)
                GOTO[i + 1][m] = k; // 说明第i+1个状态（状态i）会通过字符m转移到状态k
    }
    else // 说明是终结符，完善action
    {
        for (int m = 1; m <= no_t; m++)
        {
            if (action[0][m][0] == c)
            {
                action[i + 1][m] = "S" + to_string(k);
            }
        }
    }
}

void print_table(int state_count)
{

    for (int i = 1; i <= state_count; i++)
    {
        action[i][0] = to_string(i - 1);
    }
    cout << endl
         << "------------------ACTION------------------" << endl;
    for (int i = 0; i <= state_count; i++)
    {
        for (int j = 0; j <= no_t; j++)
        {
            cout << setw(5) << action[i][j];
        }
        cout << endl;
    }
    cout << endl
         << "------------------GOTO------------------" << endl;
    cout << setw(5) << " ";
    for (int i = 0; i < no_nt; i++)
        cout << setw(5) << non_terminals[i];
    cout << endl;
    for (int i = 1; i <= state_count; i++)
    {
        cout << setw(5) << i - 1;
        for (int j = 0; j < no_nt; j++)
        {
            if (GOTO[i][j] != 0)
                cout << setw(5) << GOTO[i][j];
            else
                cout << setw(5) << " ";
        }
        cout << endl;
    }
}

void analysis(struct state *temp)
{
    string str;
    string sta = "#0";
    string act;
    cout << "Enter a string to analysis:\n";
    cin >> str;
    str += "#"; // 加上结束符号
    cout << endl
         << setw(5) << "步骤" << setw(12) << "符号栈" << setw(10) << "输入串" << setw(30) << "动作" << endl;
    for (int i = 1;; i++)
    {
        if (str == "")
            break;
        cout << setw(5) << i << setw(12) << sta << setw(10) << str;
        for (int j = 0; j < no_t; j++)
        {
            if (str[0] == terminals[j])
            {
                act = action[(sta.back() - '0') + 1][j + 1];
                if (act[0] == 'S') // 移进
                {
                    act += "移进";
                    sta = sta + str[0] + act[1];
                    str = str.substr(1);
                    break;
                }
                else if (act[0] == 'r') // 归约
                {
                    int p = act[1] - '0';
                    string s = temp->prod[p], Goto;
                    // action归约
                    act = act + "用" + s + "归约;GOTO(";
                    for (int m = 0; m < s.length() - 3; m++)
                    {
                        sta = sta.substr(0, sta.size() - 2);
                    }
                    act = act + sta.back() + "," + s[0] + ")=";
                    // GOTO
                    for (int j = 0; j < no_nt; j++)
                    {
                        if (s[0] == non_terminals[j])
                        {
                            Goto = to_string(GOTO[sta.back() - '0' + 1][j]);
                            act = act + Goto;
                            sta = sta + s[0] + Goto;
                            break;
                        }
                    }
                    break;
                }
                str = "";
                break;
            }
        }
        cout << setw(30) << act << endl;
    }
}

int main()
{
    struct state init;
    struct state temp;   // backup
    int state_count = 1; // 状态的个数
    get_prods(&init);
    // temp = init;

    strcpy(temp.prod[0], "Z->S");
    temp.prod_count = 1;
    for (int i = 0; i < init.prod_count; i++)
        strcpy(temp.prod[temp.prod_count++], init.prod[i]);

    add_dots(&init);

    struct state I[50];
    augument(&I[0], &init);
    closure(&init, &I[0]);
    cout << "\nI0:\n";
    print_prods(&I[0]);

    char characters[20] = {};
    for (int i = 0; i < state_count; i++)
    {
        char characters[20] = {};
        for (int z = 0; z < I[i].prod_count; z++) // 将状态I[i]里的每个.之后的字符加入character中（不重复）
            if (!in_array(characters, char_after_dot(I[i].prod[z])))
                characters[strlen(characters)] = char_after_dot(I[i].prod[z]);

        for (int j = 0; j < strlen(characters); j++)
        {
            goto_state(&I[i], &I[state_count], characters[j]); // 状态转移
            closure(&init, &I[state_count]);                   // 构造新状态的闭包
            int flag = 0;
            for (int k = 0; k < state_count - 1; k++) // 检查新状态是否与其他状态重复
            {
                if (same_state(&I[k], &I[state_count]))
                {
                    cleanup_prods(&I[state_count]);
                    flag = 1;
                    cout << "I" << i << " on reading the symbol " << characters[j] << " goes to I" << k << ".\n";
                    action_or_GOTO(i, k, characters[j]);
                    break;
                }
            }
            if (flag == 0)
            {
                // 处理转移状态

                cout << "I" << i << " on reading the symbol " << characters[j] << " goes to I" << state_count << ":\n";
                action_or_GOTO(i, state_count, characters[j]);
                print_prods(&I[state_count]);

                // 处理新状态
                if (I[state_count].prod_count == 1 && !char_after_dot(I[state_count].prod[0])) // 说明这个产生式已经处理完毕，可以归约
                {
                    string produce = I[state_count].prod[0];
                    if (produce == "Z->S.")
                        action[state_count + 1][no_t] = "acc";
                    else
                    {
                        produce = produce.substr(0, produce.length() - 1);
                        for (int m = 0; m < temp.prod_count; m++)
                            if (produce == temp.prod[m])
                            {
                                for (int n = 1; n <= no_t + 1; n++)
                                    action[state_count + 1][n] = "r" + to_string(m);
                                break;
                            }
                    }
                }
                state_count++;
            }
        }
    }

    // 打印增广后的产生式
    for (int i = 0; i < temp.prod_count; i++)
    {
        cout << "(" << i << ") " << temp.prod[i] << endl;
    }
    print_table(state_count); // 打印LR(0)的action和goto
    analysis(&temp);          // 对一个串进行LR分析
    system("pause");
    return 0;
}

/*
4
3
S I T
5
v : , i r
S->vI:T
I->I,i
I->i
T->r

vi,i:r
*/
