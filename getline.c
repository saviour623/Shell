//#include "shell_header.h"

#include <stdio.h>

static void sp(void)
{
	puts("in");
}
typedef struct op {
	char *a;
	void (*sp)(void);
} op;

int main(){
	op opx[] = {
		{"hi", &sp},
		{"no", &sp},
		{NULL, NULL}
	};

	int oo = 0;
	while ((*opx++).a != NULL)
	{
		puts((opx[oo]).a);
		oo++;
	}
	return 0;
}
