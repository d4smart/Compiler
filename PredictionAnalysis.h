#pragma once
#include <string>
#include <stack>
#include <iostream>

using namespace std;

// 表示语法规则的结构体
typedef struct rule
{
	char left;	// 左部
	string right;	// 右部
} Rule;

extern char input[];

Rule *M(char X, char a);
void error();
void master();
