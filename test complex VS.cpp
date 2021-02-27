#define _CRT_SECURE_NO_WARNINGS
#define _COMPLEX
#include <stdio.h>
#include <complex.h>

#define SIZE 100

typedef struct compl
{
	_Dcomplex a;
}cum;

int main()
{
	cum com[SIZE] = { 0 };
	_Dcomplex  z = _Cbuild(4, 3);
	printf("%lf %lf", creal(z), cimag(z));

	for (int i = 0; i < 10; ++i)
	{
		com[i].a = _Cbuild(i, i+1);
	}

	printf("\n%lf %lf", creal(ccos(z)), cimag(ccos(z)));
	return 0;
}
