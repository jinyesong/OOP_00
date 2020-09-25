#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

int main() {
	char s[100];

	printf("문자를 입력하세요(100자 이내).\n");
	scanf("%s", s);
	printf("입력된 문자는 %s 입니다.", s);

	system("pause");
	return 0;
}