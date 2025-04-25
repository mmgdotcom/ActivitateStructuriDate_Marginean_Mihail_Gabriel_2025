#include<stdio.h>

int main() {
	printf("Salutare!\nIntroduceti un numar intreg:");
	int variabila = 0;
	scanf_s("%d", &variabila);
	printf("Ai introdus: %d", variabila);
	return 0;
}