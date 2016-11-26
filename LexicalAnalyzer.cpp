#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>

#define maxlen 10	// 关键词的最大长度（包括'\0'）

char expr[300], token[30];	// 源程序，等待查询关键词表的字符串
int pos = 0;	// 读字符指针
char ch = 0;	// 当前读进的源程序字符

// 关键词表
char keywords[][maxlen] = {"main", "int", "float", "double", "char", "if", "else", "do", "while"};

// 判断字符是否为字母
// 输入：char ch 待检测的字符
// 输出：bool 字符是否为字母
int letter(char ch)
{
	return ch >= 'A' && ch <= 'z' ? true : false;
}

// 判断字符是否为数字
// 输入：char ch 待检测的字符
// 输出：bool 字符是否为数字
int digit(char ch)
{
	return ch >= '0' && ch <= '9' ? true : false;
}

// 读字符函数
// 读进源程序的下一个字符放在ch中，并把读指针指向下一个字符
void getch()
{
	ch = expr[pos];
	pos++;
}

// 检查ch中的字符是否为空白字符，如果是，继续读取源程序的下一个字符，直到ch不为空字符
void getbc()
{
	// 空字符：0x20
	while (ch == 0x20)
	{
		ch = expr[pos];
		pos++;
	}
}

// 连接token字符串和ch中的字符
void concat()
{
	int tail = 0;
	while (token[tail])
	{
		tail++;
	}
	token[tail] = ch;
}

// 字符指针回退一个字符
void retract()
{
	pos--;
}

// 十进制转换函数
// 将token中的数字串转换为数字，并返回
// 输出：int result 数字串对应的数字
int dtb()
{
	int result, tmp, tail = 0;

	tmp = token[tail] - '0';
	tail++;
	while (token[tail])
	{
		tmp = tmp * 10 + token[tail] - '0';
		tail++;
	}
	result = tmp;
	return result;
}

// 关键字表查询函数
// 对token中的字符串查关键词表，如果是关键词，就返回它的编码；否则返回标识符编码10
// 输出：int 关键词或标识符的编码
int reverse()
{
	int i = 0;
	int len = sizeof(keywords) / sizeof(keywords[0]);
	while (i < len)
	{
		if (strcmp(token, keywords[i]) == 0)
		{
			return i + 1;
		}
		i++;
	}
	return 10;
}

// 出错
void error()
{
	printf("Error: word not regnized...\n");
}

// 词法分析函数
// 读取源程序，获取一个单词，返回（打印）单词符号串
void scaner()
{
	int c = 0;
	memset(token, 0, sizeof(token) / sizeof(char));	// 将token置空

	getch();
	getbc();

	if (letter(ch))
	{
		while (letter(ch) || digit(ch))
		{
			concat();
			getch();
		}
		retract();
		c = reverse();
		if (c != 10)
		{
			printf("(%d,%s)\n", c, token);
		}
		else {
			printf("(10,%s)\n", token);
		}
	}
	else if (digit(ch))
	{
		while (digit(ch))
		{
			concat();
			getch();
		}
		retract();
		printf("(20,%d)\n", dtb());
	}
	else switch (ch)
	{
		case'=': getch();
			if (ch == '=')
			{
				printf("(36,-)\n");	break;
			}
			retract();
			printf("(21,-)\n");
			break;
		case'+': printf("(22,-)\n"); break;
		case'-': printf("(23,-)\n"); break;
		case'*': printf("(24,-)\n"); break;
		case'/': printf("(25,-)\n"); break;
		case'(': printf("(26,-)\n"); break;
		case')': printf("(27,-)\n"); break;
		case'{': printf("(28,-)\n"); break;
		case'}': printf("(29,-)\n"); break;
		case',': printf("(30,-)\n"); break;
		case';': printf("(31,-)\n"); break;
		case'>': getch();
			if (ch == '=')
			{
				printf("(33,-)\n");	break;
			}
			retract();
			printf("(32,-)\n");
			break;
		case'<': getch();
			if (ch == '=') 
			{
				printf("(35,-)\n");	break;
			}
			retract();
			printf("(34,-)\n");
			break;
		case'!': getch();
			if (ch == '=')
			{
				printf("(37,-)\n");	break;
			}
			retract();
			error();
			break;
		case'#': printf("(0,-)\n"); break;
		default: error();
			break;
	}
}

int main()
{
	int len = 0;
	
	printf("按5可奉告，按E可赛艇:\n=> ");

	// 可以读取包含空格的整个字符串
	gets_s(expr);
	//scanf("%s", expr);
	
	len = strlen(expr);	// 源程序字符串长度
	while (pos < len)
	{
		// 调用词法分析函数
		scaner();
	}

	printf("Done\n");
}
