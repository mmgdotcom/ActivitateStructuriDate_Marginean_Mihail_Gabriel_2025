#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
} Nod;

typedef struct ListaSimpla {
	Masina info;
	struct ListaSimpla* next;
} ListaSimpla;

typedef struct ListaDubla {
	Masina info;
	struct ListaDubla* next;
	struct ListaDubla* prev;
} ListaDubla;

Masina citireMasinaDinFisier(FILE* f) {
	Masina m;
	char buffer[100], * token;
	fgets(buffer, 100, f);
	token = strtok(buffer, ",\n");
	m.id = atoi(token);
	token = strtok(NULL, ",\n");
	m.nrUsi = atoi(token);
	token = strtok(NULL, ",\n");
	m.pret = (float)atof(token);
	token = strtok(NULL, ",\n");
	m.model = (char*)malloc(strlen(token) + 1);
	strcpy(m.model, token);
	token = strtok(NULL, ",\n");
	m.numeSofer = (char*)malloc(strlen(token) + 1);
	strcpy(m.numeSofer, token);
	token = strtok(NULL, ",\n");
	m.serie = token[0];
	return m;
}

void afisareMasina(Masina m) {
	printf("ID: %d | Pret: %.2f | Model: %s | Sofer: %s\n", m.id, m.pret, m.model, m.numeSofer);
}

int inaltime(Nod* rad) {
	if (rad == NULL) return 0;
	int hst = inaltime(rad->st);
	int hdr = inaltime(rad->dr);
	return 1 + (hst > hdr ? hst : hdr);
}

int balans(Nod* rad) {
	if (rad == NULL) return 0;
	return inaltime(rad->st) - inaltime(rad->dr);
}

void rotireSt(Nod** rad) {
	if (rad == NULL || *rad == NULL || (*rad)->dr == NULL) return;
	Nod* y = (*rad)->dr;
	(*rad)->dr = y->st;
	y->st = *rad;
	*rad = y;
}

void rotireDr(Nod** rad) {
	if (rad == NULL || *rad == NULL || (*rad)->st == NULL) return;
	Nod* y = (*rad)->st;
	(*rad)->st = y->dr;
	y->dr = *rad;
	*rad = y;
}

void inserareAVL(Nod** rad, Masina m) {
	if (rad == NULL) return;
	if (*rad == NULL) {
		*rad = (Nod*)malloc(sizeof(Nod));
		if (*rad == NULL) return;
		(*rad)->info = m;
		(*rad)->st = (*rad)->dr = NULL;
		return;
	}
	if (m.id < (*rad)->info.id) inserareAVL(&(*rad)->st, m);
	else inserareAVL(&(*rad)->dr, m);

	int b = balans(*rad);
	if (b > 1) {
		if ((*rad)->st != NULL && m.id > (*rad)->st->info.id) rotireSt(&(*rad)->st);
		rotireDr(rad);
	}
	else if (b < -1) {
		if ((*rad)->dr != NULL && m.id < (*rad)->dr->info.id) rotireDr(&(*rad)->dr);
		rotireSt(rad);
	}
}

Masina cauta(Nod* rad, int id) {
	if (rad == NULL || rad->info.id == id) return rad ? rad->info : (Masina) { -1 };
	if (id < rad->info.id) return cauta(rad->st, id);
	return cauta(rad->dr, id);
}

void inordineVec(Nod* rad, Masina* v, int* i) {
	if (rad) {
		inordineVec(rad->st, v, i);
		v[(*i)++] = rad->info;
		inordineVec(rad->dr, v, i);
	}
}

void preordineVec(Nod* rad, Masina* v, int* i) {
	if (rad) {
		v[(*i)++] = rad->info;
		preordineVec(rad->st, v, i);
		preordineVec(rad->dr, v, i);
	}
}

void postordineVec(Nod* rad, Masina* v, int* i) {
	if (rad) {
		postordineVec(rad->st, v, i);
		postordineVec(rad->dr, v, i);
		v[(*i)++] = rad->info;
	}
}

void inordineListaSimpla(Nod* rad, ListaSimpla** cap) {
	if (rad) {
		inordineListaSimpla(rad->dr, cap);
		ListaSimpla* nou = (ListaSimpla*)malloc(sizeof(ListaSimpla));
		if (!nou) return;
		nou->info = rad->info;
		nou->next = *cap;
		*cap = nou;
		inordineListaSimpla(rad->st, cap);
	}
}

void inordineListaDubla(Nod* rad, ListaDubla** cap, ListaDubla** coada) {
	if (rad) {
		inordineListaDubla(rad->st, cap, coada);
		ListaDubla* nou = (ListaDubla*)malloc(sizeof(ListaDubla));
		if (!nou) return;
		nou->info = rad->info;
		nou->next = NULL;
		nou->prev = *coada;
		if (*coada) (*coada)->next = nou;
		else *cap = nou;
		*coada = nou;
		inordineListaDubla(rad->dr, cap, coada);
	}
}

int main() {
	FILE* f = fopen("masini.txt", "r");
	if (!f) {
		printf("Eroare deschidere fisier.\n");
		return -1;
	}
	Nod* radacina = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserareAVL(&radacina, m);
	}
	fclose(f);

	printf("Parcurgere inordine:\n");
	Masina v[100];
	int i = 0;
	inordineVec(radacina, v, &i);
	for (int j = 0; j < i; j++) afisareMasina(v[j]);

	Masina cautat = cauta(radacina, 7);
	if (cautat.id != -1) {
		printf("\nMasina cu ID 7:\n");
		afisareMasina(cautat);
	}
	else {
		printf("\nMasina cu ID 7 nu a fost gasita.\n");
	}

	return 0;
}
