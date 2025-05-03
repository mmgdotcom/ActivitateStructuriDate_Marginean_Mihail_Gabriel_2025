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
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info; //de la informatie utila
	struct Nod* next;
};

typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL) {
		afisareMasina(cap->info); //afisez informatia pe care o gasesc in 'cap'
		cap = cap->next; //atribui lui cap valoarea pe care o gasesc in 'next'
	}
}

void adaugaMasinaInLista(Nod* *cap, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	//este corect shallow copy in functie de cum apelam functia:
	//1. daca apelam functia si dupa ce apelam functia dandu-i masinaNoua ca parametru vom sterge acea masina atunci NU e corect sa facem shallow copy
	//2. daca apelam functia, NU stergem si nici NU mai folosim acea masina pe care am inserat-o in alta parte atunci ESTE corect sa facem shallow copy
	nou->next = NULL;
	if (*cap) { //daca exista cap inseamna ca avem cel putin un nod
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou; //practic ii atribui lui cap valoarea lui nou pentru ca nou devine singurul din lista
	}
}

void adaugaLaInceputInLista(Nod* *cap, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) { //returneaza Nod* (pointer) pentru ca va returna adresa primului nod din lista
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	Nod* cap = NULL; //daca returnam o lista => trebuie sa ne declaram o lista
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			//cream lista cu inserare la sfarsit
			adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
			//am deschis fisierul si adaug pana ajung la final cate o masina, pe rand
		}
	}
	fclose(f);
	return cap;
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void dezalocareListaMasini(Nod* * cap) { //daca primim cap prin adresa atunci il dereferentiem cand verificam (in while in cazul nostru)
	//sunt dezalocate toate masinile si lista de elemente
	while (*cap) {
		Nod* p = *cap;
		(*cap) = (*cap)->next; //se poate scrie si (*cap) = p->next;
		//sageata face si ea o dereferentiere;
		//sageata face dereferentiere si accesare;
		if (p->info.model) {
			free(p->info.model);
		}
		if (p->info.numeSofer) {
			free(p->info.numeSofer);
		}
		free(p);
	}
}

float calculeazaPretMediu(Nod* cap) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

void stergeMasiniDinSeria(Nod**cap, const char* serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}

}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(cap));
	printf("Pretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));
	dezalocareListaMasini(&cap);
	return 0;
}