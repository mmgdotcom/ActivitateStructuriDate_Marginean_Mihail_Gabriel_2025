#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina; //functioneaza ca un alias

void afisareMasina(Masina masina) {
	printf("ID: %d\n", masina.id);
	printf("Numar usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua; //shallow copy
	free(*masini);
	(*masini) = aux;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char separatoare[4] = ",;\n"; //desparte fie dupa , fie dupa ; fie dupa /n
	fgets(buffer, 100, file);
	m.id = atoi(strtok(buffer, separatoare)); //returneaza token-ul pana la primul separator //ascii to integer - functia atoi
	m.nrUsi = atoi(strtok(NULL, separatoare)); //ii dam NULL ca sa se uite la ultimul apel al nostru, sa vada unde a ramas
	//daca ii dadeam inca o data buffer o lua iar de la inceput
	m.pret = atof(strtok(NULL, separatoare));
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {


	return 0;
}