#pragma once
#include <string>
#include <stack>
#include <iostream>

using namespace std;

static stack<char> analyzeStack;	// 分析栈
// 表示语法规则的结构体
typedef struct rule
{
	char left;	// 左部
	string right;	// 右部
} Rule;

extern char input[100];

Rule *M(char X, char a);
void error();
void master();