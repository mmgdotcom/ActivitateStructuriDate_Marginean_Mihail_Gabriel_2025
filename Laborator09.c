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
	Masina infoUtil;
	struct Nod* fiuStanga;
	struct Nod* fiuDreapta;
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

int calculGradEchilibru(Nod* radacina) {

	return calculeazaInaltimeArbore(radacina->fiuStanga) - calculeazaInaltimeArbore(radacina->fiuDreapta);

}

void rotireStanga(Nod** radacina) {

	Nod* aux = (*radacina)->fiuDreapta;
	(*radacina)->fiuDreapta = aux->fiuStanga;
	aux->fiuStanga = (*radacina);
	(*radacina) = aux;

}

void rotireDreapta(Nod** radacina) {

	Nod* aux = (*radacina)->fiuStanga;
	(*radacina)->fiuStanga = aux->fiuDreapta;
	aux->fiuDreapta = (*radacina);
	(*radacina) = aux;

}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {

	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->infoUtil.id) {
			adaugaMasinaInArbore(&(*radacina)->fiuStanga, masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&(*radacina)->fiuDreapta, masinaNoua);
		}

		//verificam gradul de echilibru
		int GE = calculGradEchilibru(*radacina);
		if (GE == -2) {

			// dezechilibru in partea dreapta
			if (calculGradEchilibru((*radacina)->fiuDreapta) == 1) {

				// dubla rotire
				rotireDreapta(&(*radacina)->fiuDreapta);

			}
			rotireStanga(radacina);

		}

		if (GE == 2) {

			// dezechilibru in partea stanga
			if (calculGradEchilibru((*radacina)->fiuStanga) == -1) {

				rotireStanga(&(*radacina)->fiuStanga);

			}
			rotireStanga(radacina);

		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->fiuStanga = NULL;
		nou->fiuDreapta = NULL;
		nou->infoUtil = masinaNoua;
		nou->infoUtil.model = (char*)malloc(sizeof(char) * (strlen(masinaNoua.model) + 1));
		strcpy(nou->infoUtil.model, masinaNoua.model);
		nou->infoUtil.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy(nou->infoUtil.numeSofer, masinaNoua.numeSofer);

		(*radacina) = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");
	Nod* nou = NULL;
	if (file != NULL) {
		while (!feof(file)) {
			Masina masinaNoua = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&nou, masinaNoua);
			free(masinaNoua.model);
			free(masinaNoua.numeSofer);
		}
	}
	fclose(file);
	return nou;
}

void afisareMasiniDinArbore(Nod* radacina) {//RSD

	if (radacina != NULL) {
		afisareMasina(radacina->infoUtil);
		afisareMasiniDinArbore(radacina->fiuStanga);
		afisareMasiniDinArbore(radacina->fiuDreapta);
	}
}

void afisareArboreInOrdine(Nod* radacina) {//SDR

	if (radacina != NULL) {
		afisareArboreInOrdine(radacina->fiuStanga);
		afisareMasina(radacina->infoUtil);
		afisareArboreInOrdine(radacina->fiuDreapta);
	}
}

void afisareArborePreOrdine(Nod* radacina) {//SRD

	if (radacina != NULL) {
		afisareArborePreOrdine(radacina->fiuStanga);
		afisareArborePreOrdine(radacina->fiuDreapta);
		afisareMasina(radacina->infoUtil);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {  //postordine

	if ((*radacina) != NULL) {
		dezalocareArboreDeMasini((&(*radacina)->fiuStanga));
		dezalocareArboreDeMasini((&(*radacina)->fiuDreapta));
		free((*radacina)->infoUtil.model);
		free((*radacina)->infoUtil.numeSofer);
		free(*radacina);

		(*radacina) = NULL;
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	Masina m;
	m.id = -1;

	if (rad != NULL) {
		if (id < rad->infoUtil.id) {
			return getMasinaByID(rad->fiuStanga, id);
		}
		else if (id > rad->infoUtil.id) {
			return getMasinaByID(rad->fiuDreapta, id);
		}
		else {
			return rad->infoUtil;
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return 1 + determinaNumarNoduri(radacina->fiuStanga) + determinaNumarNoduri(radacina->fiuDreapta);
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {

		int Hs = calculeazaInaltimeArbore(radacina->fiuStanga);
		int Hd = calculeazaInaltimeArbore(radacina->fiuDreapta);

		return 1 + (Hs > Hd ? Hs : Hd);

	}
	else {

		return 0;

	}

}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return radacina->infoUtil.pret +
		calculeazaPretTotal(radacina->fiuStanga) +
		calculeazaPretTotal(radacina->fiuDreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL) {
		return 0;
	}

	float suma = 0;
	if (strcmp(radacina->infoUtil.numeSofer, numeSofer) == 0) {
		suma += radacina->infoUtil.pret;
	}

	suma += calculeazaPretulMasinilorUnuiSofer(radacina->fiuStanga, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->fiuDreapta, numeSofer);

	return suma;
}

int main() {

	Nod* radacina = citireArboreDeMasiniDinFisier("masini.txt");
	printf("\n--- Afisare preordine ---\n");
	afisareArborePreOrdine(radacina);

	printf("\n--- Masina cu ID 7 ---\n");
	afisareMasina(getMasinaByID(radacina, 7));

	printf("\n--- Numar noduri ---\n");
	printf("%d\n", determinaNumarNoduri(radacina));

	printf("\n--- Pret total masini ---\n");
	printf("%.2f\n", calculeazaPretTotal(radacina));

	printf("\n--- Pret masini pentru soferul 'Ionescu' ---\n");
	printf("%.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));

	dezalocareArboreDeMasini(&radacina);

	return 0;
}