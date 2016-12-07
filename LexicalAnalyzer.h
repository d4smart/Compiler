#pragma once
#include <math.h>
#include <stdlib.h>

#define maxlen 10	// 关键词的最大长度（包括'\0'）
#define E 10	// e的定义

typedef struct word
{
	int code;
	char str[30];
	double value;
} Word;

extern char expr[300];
extern int pos;
extern char ch;
extern int pre_as_attr;

int letter(char ch);
int digit(char ch);
void processInput();
void strCutting(char *str, int start, int end);
void removeComments();
void getch();
void getbc();
void concat();
void removeTail();
void retract();
int canAddOrSubtract(int code);
double processNumber();
double dtb();
int reverse();
void error(char *message);
Word *scaner();