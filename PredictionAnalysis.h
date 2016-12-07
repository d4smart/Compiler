#pragma once
#include <string>
#include <stack>
#include <iostream>

using namespace std;

// 表示语法规则的结构体
typedef struct rule
{
	char left;	// 左部字符
	string right;	// 右部字符串
} Rule;

extern Word *input[];

Rule *M(char X, char a);
void error();
void master();
