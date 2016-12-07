#include "common.h"
#include "LexicalAnalyzer.h"
#include "PredictionAnalysis.h"

int main()
{
	int i = 0, len = 0;

	while (1)
	{
		printf("Reloading...\nPlease enter your code\n=> ");

		processInput();	// 处理输入
		removeComments(); // 去除注释

		if (strcmp(expr, "quit") == 0)
		{
			break;
		}

		printf("------Exec------\n");

		len = strlen(expr);	// 源程序字符串长度
		while (pos < len)
		{
			// 调用词法分析函数，将分析结果放入单词数组
			input[i] = scaner();
			i++;
		}
		Word *tail = (Word*)malloc(sizeof(Word));
		tail->code = -1; strcpy(tail->str, "$");
		input[i] = tail;

		// 对输入进行语法分析
		master();
		
		for (; i > 0; i--)	free(input[i - 1]);	// 释放单词的结构体内存
		memset(expr, 0, sizeof(expr) / sizeof(char));	// 将expr置空
		pos = 0;
		ch = 0;
		pre_as_attr = 0;

		printf("------Done------\n\n");
	}

	printf("\nExiting...\n");
}
