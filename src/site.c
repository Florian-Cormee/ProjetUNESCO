#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lectureFichiers.h"
#include "site.h"

#define EXEMPLE 1

Site* Site_construire(char* nom,float
LAT,float LONG,char* categorie,char* pays,int enDanger) {
    Site* s = (Site*) malloc(sizeof(Site));
    s->nom = strdup(nom);
    s->LAT = LAT;
    s->LONG = LONG;
    s->categorie = strdup(categorie);
    s->pays = strdup(pays);
    s->enDanger = enDanger;
    return s;
}

void Site_affichage(Site* s) {
    printf("%s / %s / %s", s->nom, s->categorie, s->pays);
    if(s->enDanger) {printf(" / Endangered");}
    printf("\n");
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
    Site** s = (Site **)malloc(sizeof(Site)*(*n));
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
            *(s+i) = Site_construire(nom, LAT, LONG, categorie, pays, EnDanger);
            Site_affichage(*(s+i));
        }
        fclose(fid);
    }
    return s;
}

void Site_supprime(Site* s)
{
    free(s);
}

void Site_tab_supprime(Site** s,int n)
{
    for(int i=0; i<n; i++)
    {
            free(*(s+i));
    }
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
