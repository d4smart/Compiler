#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>

// 单词符号串
typedef struct word
{
	int code;	// 种别编码
	char str[30];	// 标识符的字符串
	double value;	// 数值的值
} Word;
