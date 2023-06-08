#include"display.h"
#include<stdio.h>



void move_l(int n)//用来寻找上一行
{
	printf("\033[%dD",n);
}

void move_r(int n)
{
	printf("\033[%dC",n);
}

void move_u(int n)
{
	if (n == 1)
		printf("\033[%dA", 1);//向上移动一行
	else if (n == 0)
		printf("\033[F");// 对应空行的情况
	else
		printf("\033[F\033[0;0H");
	
}
void move_d(int n)
{	
	if (n == 1)
		printf("\033[%dB", 1);
	else if (n == 0)
		printf("\033[E");
	else
		printf("\033[E\033[0;0H");
}

void del_line() {
	printf("\033[2K");
}

void del_ch()// 删除光标所指定的
{
	printf("\033[P");
}
void ist_ch(char ch) {
	printf("\033[@%c", ch);
}

