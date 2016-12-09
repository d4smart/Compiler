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

char classify(Word *word);
Rule *M(char X, char a);
void error();
void printStack();
void printInput(int pos);
void printRules(Rule *rule);
void master();
