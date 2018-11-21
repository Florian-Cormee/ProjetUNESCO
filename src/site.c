
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lectureFichiers.h"
#include "site.h"
#include "utils.h"

#define EXEMPLE 0

Site* Site_construire(char* nom,float LAT,float LONG,char* categorie,char* pays,int enDanger) {
    Site* site = (Site*) malloc(sizeof(Site));
    site->nom = strdup(nom);
    site->LAT = LAT;
    site->LONG = LONG;
    site->categorie = strdup(categorie);
    site->pays = strdup(pays);
    site->enDanger = enDanger;
    return site;
}

void Site_affichage(Site* site) {
    printf("%s / %s / %s", site->nom, site->categorie, site->pays);
    if(site->enDanger) {printf(" / Endangered");}
    printf("\n");
}

int Site_equals(Site *s1, Site *s2) {
    if(s1 == NULL || s2 == NULL) { return ERROR; }
    if(s1 == s2) { return TRUE; }//Meme adresse memoire
    
    if(s1->LAT != s2->LAT) { return FALSE; }
    if(s1->LONG != s2->LONG) { return FALSE; }
    if(s1->enDanger != s2->enDanger) { return FALSE; }
    if(!strcmp(s1->nom, s2->nom)) { return FALSE; }
    if(!strcmp(s1->categorie, s2->categorie)) { return FALSE; }
    if(!strcmp(s1->pays, s2->pays)) { return FALSE; }
    return TRUE;
}

void Site_supprime(Site* site) {
    free(site);
}

Site** Site_tab_init(int *n) {
    FILE* fid = fopen("unesco.csv", "r");
    *n = 0;
    if(fid!=NULL) {
        while(!feof(fid)) {
            (*n)++;
            SkipLine(fid);
        }
        fclose(fid);
    }
    fid = fopen("unesco.csv", "r");
        printf("Il y a %d element(s)\n", *n);
    (*n)-=2;
    Site** site = (Site **)malloc(sizeof(Site)*(*n));
    char nom[512], categorie[32], pays[256], continents[512];
    double LAT, LONG;
    int EnDanger;
    if(fid!=NULL) {
        SkipLine(fid);
        for(int i=0; i<*n; i++)
        {
            GetChaine(fid, 512, nom);
            LAT = GetReel(fid);
            LONG = GetReel(fid);
            GetChaine(fid, 32, categorie);
            GetChaine(fid, 256, pays);
            GetChaine(fid, 512, continents);
            EnDanger = GetEntier(fid);
            *(site+i) = Site_construire(nom, LAT, LONG, categorie, pays, EnDanger);
            Site_affichage(*(site+i));
        }
        fclose(fid);
    }
    return site;
}


void Site_tab_supprime(Site** site,int n) {
    for(int i=0; i<n; i++) {
            free(*(site+i));
    }
}

/*  ---------------- LDC ---------------- */
LDC *LDC_nouveau() {
    LDC *ldc = (LDC *) malloc(sizeof(LDC));
    ldc->premier = NULL;
    ldc->dernier = NULL;
}

int LDC_taille(LDC *list) {
	if(list == NULL) { return ERROR; }
	int size = 0; // compteur d'elements de la liste

	for(CelluleLDC *cell = list->premier; cell != NULL; cell = cell->suiv){
		size++;
	}

	return size;
}

void LDC_affiche(LDC *ldc) {
	printf("LDC {\n");

	for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
		printf("\t%p > prec = %p, suiv= %p, s=", cell, cell->prec, cell->suiv);
		Site_affichage(cell->s);
	}
	printf("}\n");
}

void LDC_ajoute_deb(LDC *ldc, Site *site) {
	if(ldc == NULL || site == NULL) { return; }
    
    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));
    cell->s = site;
    cell->prec = NULL;
    cell->suiv = NULL;
    
    if(ldc->dernier == NULL){// si la liste est vide
        ldc->premier = cell;
        ldc->dernier = cell;
    } else {
        CelluleLDC *cellp = ldc->premier;
        cell->suiv = cellp;
        cellp->prec = cell;
        ldc->premier = cell;
    }
}

void LDC_ajoute_fin(LDC *ldc, Site *site){
    if(ldc == NULL || site == NULL) { return; }
    
    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));
    cell->s = site;
    cell->prec = NULL;
    cell->suiv = NULL;
    
    if(ldc->dernier == NULL){// si la liste est vide
        ldc->premier = cell;
        ldc->dernier = cell;
    } else {
        CelluleLDC *celld = ldc->dernier;
        celld->suiv = cell;
        cell->prec = celld;
        ldc->dernier = cell;
    }
}

int LDC_trouve(LDC *ldc, Site *site){
    if(ldc == NULL || site == NULL) { return ERROR; }

    int i = 0;
    
    for(CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
        if(Site_equals(cell->s, site)){
            return i;
        }
        i++;
    }
    return ERROR;
}

Site *LDC_get(LDC *ldc, int i){
    if(ldc == NULL || i < 0) { return NULL; }

    CelluleLDC *courant = ldc->premier;
    
    for(int index = 0; index < i && courant != NULL; index++){
        courant = courant->suiv;
    }

    if (courant == NULL) { return NULL; }
    return courant->s;
}

void LDC_rm(LDC *ldc, Site *site){
    if(ldc == NULL || site == NULL) { return; }

    CelluleLDC *courant = ldc->premier;
    
    while(courant != NULL && !Site_equals(courant->s, site)) {
        courant = courant->suiv;
    }
    if(courant->prec == NULL && courant->suiv == NULL) {//c'est l'unique element de la liste
    	printf("rm unique element\n" );
        ldc->premier = NULL;
        ldc->dernier = NULL;
    } if(courant->prec == NULL){//c'est le premier element de la liste
    	printf("rm deb\n");
        (courant->suiv)->prec = NULL;
        ldc->premier = courant->suiv;
    } else if(courant->suiv == NULL){//c'est le dernier element de la liste
    	printf("rm fin\n");
        (courant->prec)->suiv = NULL;
        ldc->dernier = courant->prec;
    } else {
    	printf("rm autre\n");
        (courant->prec)->suiv = courant->suiv;
        (courant->suiv)->prec = courant->prec;
    }
    free(courant);
}

void LDC_free(LDC **ldc, int freeSite){
    if(ldc == NULL || *ldc == NULL) { return; }

    for(CelluleLDC *cell = (*ldc)->premier; cell != NULL; cell = (*ldc)->premier){
        (*ldc)->premier = cell->suiv;
        if(freeSite) {
            free(cell->s);
        }
        free(cell);
    }
    *ldc = NULL;
}

#if EXEMPLE
int main(void) {
/* Test des ex1*/
    Site* test = Site_construire("Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);
    int n = 0;
    Site** test_tab = Site_tab_init(&n);
    printf("Il y a %d element(s)\n", n);
    Site_supprime(test);
    Site_tab_supprime(test_tab, n);
    return 0;
}
#endif
