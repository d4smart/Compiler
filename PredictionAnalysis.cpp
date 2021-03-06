#include "common.h"
#include "PredictionAnalysis.h"

char terminator[] = "+-*/bn()";	// 终结符
char column[] = "+-*/bn()$";	// 预测分析表列元素（终结符和$）
char nonTerminator[] = "EeIiF";	// 非终结符
// 预测分析表
int predictTable[sizeof(nonTerminator) / sizeof(char) - 1][sizeof(column) / sizeof(char) - 1] = {
	{ 0,0,0,0,1,1,1 },
	{ 2,3,0,0,0,0,0,4,4 },
	{ 0,0,0,0,5,5,5 },
	{ 8,8,6,7,0,0,0,8,8 },
	{ 0,0,0,0,9,10,11 }
};

stack<char> analyzeStack;	// 分析栈
Word *input[60];	// 词法分析后生成的单词数组

// 文法规则
Rule rules[] = {
	{ 'E', "Ie" },
	{ 'e', "+Ie" },
	{ 'e', "-Ie" },
	{ 'e', "" },
	{ 'I', "Fi" },
	{ 'i', "*Fi" },
	{ 'i', "/Fi" },
	{ 'i', "" },
	{ 'F', "b" },
	{ 'F', "n" },
	{ 'F', "(E)" }
};

// 根据传入的单词指针，返回其对应的语法符号
// 输入：Word *word 单词指针
// 输出：char 对应的语法符号
char classify(Word *word)
{
	switch (word->code)
	{
		case 10: return 'b';
		case 20: return 'n';
		case 22: return '+';
		case 23: return '-';
		case 24: return '*';
		case 25: return '/';
		case 26: return '(';
		case 27: return ')';
		case -1: return '$';
		default: return 0;
	}
}

// 预测分析表查询函数
// 根据传入的规则左部X和面临的输入符号a，查询推导应该采用的文法规则的位置
// 输入：char X 规则左部，char a 面临的输入符号
// 输出：Rule *rule 文法规则位置
Rule *M(char X, char a)
{
	// 判断规则左部和输入符号是否在预测分析表中
	if (strchr(nonTerminator, X) != NULL && strchr(column, a) != NULL)
	{
		int index = predictTable[strchr(nonTerminator, X) - nonTerminator][strchr(column, a) - column];	// 查表
		// 是否查询到了规则
		if (index)
		{
			return &rules[index - 1];
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

// 出错处理函数
void error()
{
	printf("Grammar error...\n");
}

// 打印分析栈中的内容
// 将栈里面的内容出栈保存，之后再入栈，以保持函数执行前后栈内内容一致
void printStack()
{
	char str[30] = "";	// 用来保存栈内的内容
	int i = 0, j = 0;
	int size = analyzeStack.size();	// 获取栈大小
	// 栈内的内容出栈保存
	while (!analyzeStack.empty())
	{
		str[size - 1 - i] = analyzeStack.top();	// 逆序保存
		analyzeStack.pop();
		i++;
	}
	// 恢复栈内的内容
	while (str[j])
	{
		analyzeStack.push(str[j]);	// 入栈恢复
		j++;
	}
	printf("%-16s", str);
}

// 打印单词数组规约过程中的内容
// 输入：int pos 单词数组位置指针
void printInput(int pos)
{
	char str[30] = {};
	int i = 0;
	while (input[pos])
	{
		str[i] = classify(input[pos]);
		pos++;	i++;
	}
	printf("%-24s", str);
}

// 打印推导使用的文法规则
void printRules(Rule *rule)
{
	if (rule)
	{
		string right = rule->right.empty() ? "ε" : rule->right;
		cout << rule->left << "->" << right;
	}
}

// 语法分析表的总控程序
void master()
{
	char X = 0;	// 分析栈栈顶元素
	char a = 0;	// 输入符号
	int posi = 0;	// 单词数组位置指针

	// 初始化
	analyzeStack.push('$');
	analyzeStack.push('E');
	a = classify(input[posi]);

	// 打印
	printf("Analyse stack\tInput words\t\tUsed rules\n");
	printStack();	printInput(posi);	printf("\n");

	while (1)
	{
		// 分析栈栈顶的符号pop到X中
		X = analyzeStack.top();
		analyzeStack.pop();

		// 判断X是否为终结符
		if (strchr(terminator, X) != NULL)
		{
			if (X == a)
			{
				a = classify(input[++posi]);
				printStack();	printInput(posi);	printf("\n"); // 打印
			}
			else
			{
				error();
				break;
			}
				
		}
		// 判断X是否为结束标志$
		else if (X == '$')
		{
			if (X == a)
			{
				printf("Success in grammer analysis...\n");	// 语法分析成功
				break;
			}
			else {
				error();	// 语法分析失败
				break;
			}
		}
		else {
			Rule *rule = M(X, a); // 查预测分析表
			// 判断是否有可用的规则
			if (rule)
			{
				string right = rule->right;
				// 规则右部逆序放入分析栈
				for (int i = right.length() - 1; i >= 0; i--)
				{
					analyzeStack.push(right[i]);
				}
				printStack();	printInput(posi);	printRules(rule);	printf("\n"); // 打印
			}
			else {
				error();
				break;
			}
		}
	}
}
