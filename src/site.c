#include<stdio.h>
#include<stdlib.h>
#include "site.h"

#define EXEMPLE 1

Site* Site_construire(char* nom,float
LAT,float LONG,char* categorie,char* pays,int enDanger) {
    Site* s = (Site*) malloc(sizeof(Site));
    s->nom = nom;
    s->LAT = LAT;
    s->LONG = LONG;
    s->categorie = categorie;
    s->pays = pays;
    s->enDanger = enDanger;
    return s;
}

void Site_affichage(Site* s) {
    printf("%s / %s / %s", s->nom, s->categorie, s->pays);
    if(s->enDanger) {printf(" / Endangered");}
    printf("\n");
}

#if EXEMPLE
int main(void) {
/* 
    Site *test = Site_construire("Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);
    return 0;
}
#endif
