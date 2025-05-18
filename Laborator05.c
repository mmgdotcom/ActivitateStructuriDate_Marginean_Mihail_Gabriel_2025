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

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};

typedef struct Nod Nod;

//creare structura pentru Lista Dubla 
struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};

typedef struct ListaDubla ListaDubla;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua; //shallow copy
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = lista->first; //shallow copy
	nou->prev = NULL;
	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista; //creez lista
	lista.first = NULL; //initializez first cu NULL;
	lista.last = NULL; //initializez last cu NULL;
	lista.nrNoduri = 0; //initializez nrNoduri cu 0;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void dezalocareLDMasini(ListaDubla* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer) {
			free(aux->masina.numeSofer);
		}
		free(aux);
	};
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	//calculeaza pretul mediu al masinilor din lista.
	if(lista.nrNoduri > 0){
		float suma = 0;
		Nod* p = lista.first;
		while (p) {
			suma += p->masina.pret;
			p = p->next;
		}
		return suma / lista.nrNoduri;
	}

	return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p != NULL && p->masina.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}

	//avem ce sa stergem
	if (p->prev == NULL) { //primul nod din lista
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else { //nu este primul nod
		p->prev->next = p->next;
	}
	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}
	if (p->masina.model) {
		free(p->masina.model);
	}
	if (p->masina.numeSofer) {
		free(p->masina.numeSofer);
	}

	free(p);
	lista->nrNoduri--;
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->masina.numeSofer) + 1);
		strcpy_s(nume, strlen(max->masina.numeSofer) + 1, max->masina.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("-----------------------------------------------\n\n");
	stergeMasinaDupaID(&lista, 10);
	afisareListaMasiniDeLaSfarsit(lista);
	printf("-----------------------------------------------\n\n");
	printf("Pretul mediul al masinilor este %.2f.", calculeazaPretMediu(lista));
	printf("\n\n");
	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("\nSoferul cu cea mai scumpa masina este: %s\n", numeSofer);
	if (numeSofer) {
		free(numeSofer);
	}
	dezalocareLDMasini(&lista);
	return 0;
}