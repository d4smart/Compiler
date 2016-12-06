//#define _CRT_SECURE_NO_DEPRECATE
//#include <stdio.h>
//
//char sym = 0;
//
//void S();
//void T();
//void T1();
//void Scaner();
//void error();
//
//void Scaner()
//{
//	sym = getchar();
//}
//
//void error()
//{
//	printf("Error: not right...\n");
//}
//
//void S()
//{
//	if (sym == 'a' || sym == '^')	Scaner();
//	else if (sym == '(')
//	{
//		Scaner();
//		T();
//		if (sym == ')')	Scaner();
//		else error();
//	}
//	else error();
//}
//
//void T()
//{
//	S();
//	T1();
//}
//
//void T1()
//{
//	if (sym == ',')
//	{
//		Scaner();
//		S();
//		T1();
//	}
//	else if (sym != ')')
//		error();
//}
//
//int main()
//{
//	Scaner();
//	S();
//	if (sym == '$')	printf("success\n");
//	else printf("error\n");
//}
