#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <math.h>

#define maxlen 10	// 关键词的最大长度（包括'\0'）
#define E 2.7182818	// e的定义

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
// 将token中的数字串(12.13e-1)转换为数字，并返回
// 输出：float result 数字串对应的数字
float dtb()
{
	double result, tmp = 0;	// 最终结果，运算中间结果
	int tail = 0;	// 位置指针

	// 处理整数部分
	while (digit(token[tail]))
	{
		tmp = tmp * 10 + token[tail] - '0';	// 累加整数
		tail++;
	}
	// 处理小数部分
	if (token[tail] == '.')
	{
		int i = 1;
		tail++;

		// 累加小数
		while (digit(token[tail]))
		{
			tmp = tmp + (token[tail] - '0') * pow(10, -i);
			i++;
			tail++;
		}
	}
	// 处理指数部分
	if (token[tail] == 'e')
	{
		int i = 0, sign = 1, index = 1;	// 临时变量，指数正负号，指数值（默认为1）
		tail++;

		// 获取指数符号值
		if (token[tail] == '+')
		{
			tail++;
		}
		else if (token[tail] == '-')
		{
			sign = -1;
			tail++;
		}
		// 处理指数值
		while (digit(token[tail]))
		{
			index = i * 10 + token[tail] - '0';	// 累加指数值
			i = index;	// 保证在指数值默认为1的情况下，第1次循环可以有10*0
			tail++;
		}
		tmp = tmp * pow(E, sign * index);	// 将底数与指数进行运算
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

// 出错处理函数
// 输入：char *message 错误信息
void error(char *message)
{
	printf("Error: %s...\n", message);
}

// 词法分析函数
// 读取源程序，获取一个单词，返回（打印）单词符号串
void scaner()
{
	int c = 0;
	memset(token, 0, sizeof(token) / sizeof(char));	// 将token置空

	getch();
	getbc();

	// 处理关键词和标识符
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
	// 处理没有正负号的数字
	else if (digit(ch))
	{
		// 处理整数部分
		while (digit(ch))
		{
			concat();
			getch();
		}
		// 处理小数部分
		if (ch == '.')
		{
			concat();
			getch();
			
			// 获取小数
			if (digit(ch))
			{
				while (digit(ch))
				{
					concat();
					getch();
				}
			}
			else {
				// 处理底数没有小数部分错误：12.e3
				retract();
				error("no fractional part");
				return;
			}
		}
		// 处理指数部分
		if (ch == 'e')
		{
			concat();
			getch();

			// 连接指数符号
			if (ch == '+' || ch == '-')
			{
				concat();
				getch();
			}
			// 连接指数
			if (digit(ch))
			{
				while (digit(ch))
				{
					concat();
					getch();
				}
			}
		}
		retract();
		printf("(20,%f)\n", dtb());
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
			error("single char ! not defined");
			break;
		case'#': printf("(0,-)\n"); break;
		default: error("word not regnized");
			break;
	}
}

int main()
{
	int len = 0;

	while (1)
	{
		printf("按5可奉告，按E可赛艇：\n=> ");

		// gets_s可以读取包含空格的整个字符串
		gets_s(expr);
		if (strcmp(expr, "quit") == 0)
		{
			break;
		}
		//scanf("%s", expr);
		printf("------Exec------\n");
	
		len = strlen(expr);	// 源程序字符串长度
		while (pos < len)
		{
			// 调用词法分析函数
			scaner();
		}

		memset(expr, 0, sizeof(expr) / sizeof(char));	// 将expr置空
		pos = 0;
		ch = 0;
		printf("------Done------\n\n");
	}
	
	printf("\nExiting...\n");
}
