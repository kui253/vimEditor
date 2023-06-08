#include<conio.h>
#include<stdio.h>
#include"userctrl.h"
#include"datactrl.h"
#include"display.h"


void starter() {
	char filePath[256];
	char order_input[16];
	char order_input2[32];
	char order[64];
	char ch;
	char* command[2];
	int i = 0;
	int n = 0;
	int k = 0;
	strcpy(filePath, __FILE__);
	command[0] = order_input, command[1] = order_input2;
	char* lastSlash = strrchr(filePath, '\\');
	if (lastSlash != NULL) {
		*lastSlash = '\0';  // 通过设置末尾斜杠的位置为'\0'来截断字符串
	}

	while (1) {
		k = 0, n = 0, i = 0;
		printf("%s $ ", filePath);
		fgets(order, sizeof(order), stdin);
		ch = order[0];
		while (ch != '\n') {
			if (ch == ' ') {


				command[k][n] = '\0';// 用作结尾
				n = 0;
				k++;
				i++;
				ch = order[i];
				continue;
			}
			command[k][n++] = ch;
			i++;
			ch = order[i];
		}
		command[k][n] = '\0';
		if (strcmp(command[0], "exit") == 0) {
			exit(0);
		}
		if (strcmp(command[0], "vim") == 0) {
			editor_drive(command[1]);
			printf("\n");
		}


	}
}



int main() {
	starter();
	//test();
	return 0;

}