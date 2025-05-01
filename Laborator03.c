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
	printf("Serie: %c\n\n", masina.serie);
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
	
	char* aux;
	
	aux = strtok(NULL, separatoare);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);

	aux = strtok(NULL, separatoare);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);

	m.serie = strtok(NULL, separatoare)[0]; //indexez ca sa imi returneze doar prima pozitie

	return m;

	//ar fi trebuit sa mai facem si validar ca in cazul in care lipsesc valori sa returnam eroare

}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r"); //"r" - modul read
	Masina* masini = NULL; //declar un vector
	(*nrMasiniCitite) = 0;//este transmis prin pointer deci trebuie dereferentiat (ca mai sus)
	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++) {
		if ((*vector)[i].model != NULL) {
			free((*vector)[i].model);
		}
		if ((*vector)[i].numeSofer != NULL) {
			free((*vector)[i].numeSofer);
		}
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}