#include "common.h"
#include "PredictionAnalysis.h"

char terminator[] = { 'a', '^', '(', ')', ',' }; // 终结符
char nonTerminator[] = { 'S', 'T', 't' };	// 非终结符
int predictTable[sizeof(nonTerminator) / sizeof(char)][sizeof(terminator) / sizeof(char)] = {
	{ 1, 2, 3 },
	{ 4, 4, 4 },
	{ 0, 0, 0, 6, 5 }
};

char input[100];
char a = 0, X = 0;
int posi = 0;

// 文法规则
Rule rules[] = {
	{ 'S', "a" },
	{ 'S', "^" },
	{ 'S', "(T)" },
	{ 'T', "St" },
	{ 't', ",St" },
	{ 't', "" }
};

Rule *M(char X, char a)
{
	if (strchr(nonTerminator, X) != NULL && strchr(terminator, a) != NULL)
	{
		int index = predictTable[strchr(nonTerminator, X) - nonTerminator][strchr(terminator, a) - terminator];
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

void error()
{
	printf("Error...\n");
	exit(2333);
}

void master()
{
	analyzeStack.push('$');
	analyzeStack.push('S');
	a = input[posi];

	while (1)
	{
		X = analyzeStack.top();
		analyzeStack.pop();

		if (strchr(terminator, X) != NULL)
		{
			if (X == a)
			{
				a = input[++posi];
			}
			else
				error();
		}
		else if (X == '$')
		{
			if (X == a)
			{
				printf("Success...\n");
				exit(666);
			}
			else {
				error();
			}
		}
		else {
			// 查预测分析表的操作
			Rule *rule = M(X, a);
			if (rule)
			{
				string right = rule->right;
				for (int i = right.length() - 1; i >= 0; i--)
				{
					analyzeStack.push(right[i]);
				}
			}
			else {
				error();
			}
		}
	}
}
