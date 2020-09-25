#include <stdio.h>
void print(int var) {
	printf("Integer number: %d \n", var);
}
void print(float var) {
	printf("Float number: %f \n", var);
}
void print(int var1, float var2) {
	printf("Integer number: %d \n", var1);
	printf(" and float number: %f", var2);
}

int main() {
	int a = 7;
	float b = 9;
	print(a);
	print(b);
	print(a, b);
	return 0;
}