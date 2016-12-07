#include "common.h"
#include "LexicalAnalyzer.h"
#include "PredictionAnalysis.h"

int main()
{
	int len = 0;

	while (1)
	{
		printf("按5可奉告，按E可赛艇：\n=> ");

		processInput();
		removeComments();

		if (strcmp(expr, "quit") == 0)
		{
			break;
		}

		printf("------Exec------\n");

		len = strlen(expr);	// 源程序字符串长度
		while (pos < len)
		{
			// 调用词法分析函数
			Word *word = scaner();
			printf("[%d %s %f]\n", word->code, word->str, word->value);
			free(word);
		}

		memset(expr, 0, sizeof(expr) / sizeof(char));	// 将expr置空
		pos = 0;
		ch = 0;
		pre_as_attr = 0;
		printf("------Done------\n\n");
	}

	printf("\nExiting...\n");

	/*printf("Please input expression\n=> ");
	scanf("%s", input);
	master();*/
}
