#include "shell_header.h"

int main(){
	char *s = "hello";
	int c;

	while (!(c = delimCharcmp("/", s++)))
		putchar(*s);
	if (c == 1)
		puts("found");
	return 0;
}
