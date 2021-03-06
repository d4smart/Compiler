#include "common.h"
#include "LexicalAnalyzer.h"

char expr[300], token[30];	// 源程序，等待查询关键词表的字符串
int pos = 0, pre_as_attr = 0;	// 读字符指针，前一个字符串是否具有被加减的属性
char ch = 0;	// 当前读进的源程序字符

// 关键词表
char keywords[][maxlen] = {"main", "int", "float", "double", "char", "if", "else", "do", "while"};

// 判断字符是否为字母
// 输入：char ch 待检测的字符
// 输出：bool 字符是否为字母
int letter(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ? true : false;
}

// 判断字符是否为数字
// 输入：char ch 待检测的字符
// 输出：bool 字符是否为数字
int digit(char ch)
{
	return ch >= '0' && ch <= '9' ? true : false;
}

// 输入处理函数
// 对输入的字符串预处理：去除多余空格，Tab字符，回车换行符
void processInput()
{
	int i = 0, preIsSpace = 0, isStart = 1;
	char c;

	while ((c = getchar()) != '$')
	{
		// 输入流开头的空格，Tab字符，回车换行符直接跳过
		if (isStart)
		{
			if (c == ' ' || c == '\t' || c == '\n')
			{
				continue;
			}
			isStart = 0;
		}
		
		// 去除多余空格，Tab字符，回车换行符
		if (c == ' ' || c == '\t' || c == '\n')
		{
			if (preIsSpace)
			{
				continue;
			}
			expr[i] = ' ';
			preIsSpace = 1;
			i++;
		}
		else {
			expr[i] = c;
			preIsSpace = 0;
			i++;
		}
	}
	while (expr[--i] == ' ')
	{
		expr[i] = 0;
	}
}

// 字符串切割函数
// 去除str中开始位置start（包含）到结束位置end（不包含）的字符
// 输入：char *str 字符指针，int start 开始位置，int end 结束位置
void strCutting(char *str, int start, int end)
{
	if (start >= end) return;
	
	while (*(str + start) != 0)
	{
		*(str + start) = *(str + end);
		start++;
		end++;
	}
}

// 去除注释函数
// 去除代码中从"/*"到"*/"之间的内容
void removeComments()
{
	int i = 0, start = -1, end = 0;
	char c = 0;
	
	while ((c = expr[i]) != 0)
	{
		if (c == '/')
		{
			start = i;
			c = expr[++i];
			if (c == '*')
			{
				c = expr[++i];

				state3:
				while (c != '*')
				{
					if (c == 0) return;
					c = expr[++i];
				}
				c = expr[++i];
				
				while (c == '*')
				{
					c = expr[++i];
				}
				if (c == '/')
				{
					end = i + 1;
					strCutting(expr, start, end);
					i = start - 1;
				}
				else {
					goto state3;
				}
			}
		}
		i++;
	}
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

// 清空tail的最后一个字符
void removeTail()
{
	int tail = 0;
	while (token[tail])
	{
		tail++;
	}
	token[tail - 1] = 0;
}

// 字符指针回退一个字符
void retract()
{
	pos--;
}

// 判断前一个字符串是否可以被加减
// 输入：int code 前一个字符串的编码
// 输出：bool 判断结果
int canAddOrSubtract(int code)
{
	// 只有标识符和数字具有可加减性
	if (code == 10 || code == 20)
		return true;
	else
		return false;
}

// 数字处理函数
// 处理数字的各个部分
double processNumber()
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
			return 404;
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
		else {
			removeTail();	// 将多取的token的最后一个字符置0
			retract();	// 多回退一次
		}
	}

	retract();
	pre_as_attr = 20;
	//printf("(20,%f,%s)\n", dtb(), token);
	return dtb();
}

// 十进制转换函数
// 将token中的数字串(12.13e-1)转换为数字，并返回
// 输出：double result 数字串对应的数字
double dtb()
{
	double result, tmp = 0;	// 最终结果，运算中间结果
	int tail = 0,  base_sign = 1;	// 位置指针，底数的符号

	// 获取底数符号值
	if (token[tail] == '+')
	{
		tail++;
	}
	else if (token[tail] == '-')
	{
		base_sign = -1;
		tail++;
	}
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
	tmp = base_sign * tmp;
	// 处理指数部分
	if (token[tail] == 'e')
	{
		int i = 0, index_sign = 1, index = 1;	// 临时变量，指数正负号，指数值（默认为1）
		tail++;

		// 获取指数符号值
		if (token[tail] == '+')
		{
			tail++;
		}
		else if (token[tail] == '-')
		{
			index_sign = -1;
			tail++;
		}
		// 处理指数值
		while (digit(token[tail]))
		{
			index = i * 10 + token[tail] - '0';	// 累加指数值
			i = index;	// 保证在指数值默认为1的情况下，第1次循环可以有10*0
			tail++;
		}
		tmp = tmp * pow(E, index_sign * index);	// 最复杂情况下，将底数与指数进行乘法运算
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
	printf("Lexical error: %s...\n", message);
}

// 词法分析函数
// 读取源程序，获取一个单词，返回（打印）单词符号串
Word *scaner()
{
	int c = 0;
	memset(token, 0, sizeof(token) / sizeof(char));	// 将token置空
	Word *word = (Word*)malloc(sizeof(Word));
	int pre_as_attr_use = pre_as_attr;
	pre_as_attr = 0;

	getch();
	getbc();
	if (ch == 0) return NULL;	// 到达输入末尾

	char opr = ch;	// 记录读入的字符
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
			word->code = c;
			return word;
		}
		else {
			pre_as_attr = 10;
			word->code = 10; strcpy(word->str, token);	// 种别码为10，同时记录标识符的字符串
			return word;
		}
	}
	// 处理没有正负号的数字
	else if (digit(ch))
	{
		double value = processNumber();
		word->code = 20; word->value = value;
		return word;
	}
	else switch (ch)
	{
		case '#':
			word->code = 0;
			return word;
		case '=': getch();
			if (ch == '=')
			{
				word->code = 36;
				return word;
			}
			retract();
			word->code = 21;
			return word;
		case '+':
		case '-':
			// 数字正负号与运算符加减的处理
			getch();
			// 当且仅当符号后一位为数字时，符号可能为正负号
			if (digit(ch))
			{
				// 数字前一位具有可被加减的性质，则是运算符加减；否则是正负号
				if (canAddOrSubtract(pre_as_attr_use))
				{
					// 加减运算符处理
					retract();
					if (opr == '+')
					{
						word->code = 22;
						return word;
					}
					else if (opr == '-')
					{
						word->code = 23;
						return word;
					}
				}
				else {	
					// 正负号处理
					retract();
					retract();

					// 获取正负号并连接
					getch();
					concat();
					// 获取第一个数字，并转数字处理函数
					getch();
 					double value = processNumber();
					word->code = 20; word->value = value;
					return word;
				}
			}
			else {
				// +/- 后面不是数字的一般情况
				retract();
				if (opr == '+')
				{
					word->code = 22;
					return word;
				}
				else if (opr == '-')
				{
					word->code = 23;
					return word;
				}
			}
		case '*': 
			word->code = 24;
			return word;
		case '/': 
			word->code = 25;
			return word;
		case '(': 
			word->code = 26;
			return word;
		case ')': 
			word->code = 27;
			return word;
		case '{': 
			word->code = 28;
			return word;
		case '}': 
			word->code = 29;
			return word;
		case ',': 
			word->code = 30;
			return word;
		case ';': 
			word->code = 31;
			return word;
		case '>': getch();
			if (ch == '=')
			{
				word->code = 33;
				return word;
			}
			retract();
			word->code = 32;
			return word;
		case '<': getch();
			if (ch == '=') 
			{
				word->code = 35;
				return word;
			}
			retract();
			word->code = 34;
			return word;
		case '!': getch();
			if (ch == '=')
			{
				word->code = 37;
				return word;
			}
			retract();
			error("single char ! not defined");
			return NULL;
		case ' ': return NULL;
		default: error("word not regnized");
			return NULL;
	}
}
