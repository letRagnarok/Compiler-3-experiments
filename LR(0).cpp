#include <bits/stdc++.h>
using namespace std;

char terminals[100] = {};     // 终结符
int no_t;                     // the number of terminals
char non_terminals[100] = {}; // 非终结符
int no_nt;                    // the number of non terminals
char goto_table[100][100];
string action[100][100] = {};
char GOTO[100][100] = {};

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
    action[0][no_t + 1] = "#"; // 增加结束符号

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
        for (int m = 1; m <= no_nt; m++)
            if (GOTO[0][m] == c)
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
    cout << "************ACTION************" << endl;
    for (int i = 0; i <= state_count; i++)
    {
        for (int j = 0; j <= no_t + 1; j++)
        {
            cout << setw(5) << action[i][j] << "   ";
        }
        cout << endl;
    }
}

int main()
{
    struct state init;
    struct state temp;   // backup
    struct state temp1;  // backup
    int state_count = 1; // 状态的个数
    get_prods(&init);
    temp = init;
    temp1 = temp;
    add_dots(&init);

    // 初始化状态表
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            goto_table[i][j] = '~';

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
                    goto_table[i][k] = characters[j]; // I[i]通过characters[j]这个字符会转移到I[k]
                    action_or_GOTO(i, k, characters[j]);
                    break;
                }
            }
            if (flag == 0)
            {
                state_count++;
                cout << "I" << i << " on reading the symbol " << characters[j] << " goes to I" << state_count - 1 << ":\n";
                goto_table[i][state_count - 1] = characters[j];
                action_or_GOTO(i, state_count - 1, characters[j]);
                print_prods(&I[state_count - 1]);
            }
        }
    }
    print_table(state_count);
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
*/