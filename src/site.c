#include<stdio.h>
#include<stdlib.h>
#include "lectureFichiers.h"
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

Site* Site_tab_init() {
    FILE* fid = fopen("unesco.csv", "r");
    int n = 0;
    if(fid!=NULL) {
        while(!feof(fid)) {
            SkipLine(fid);
            n++;
        }
        fclose(fid);
    }
    FILE* fid = fopen("unesco.csv", "r");
    char* s;
    if(fid!=NULL) {
        while(!feof(fid)) {
            GetChaine(fid, 64, s);
            printf("%s\n", s);
        }
        fclose(fid);
    }
    return NULL;
}

#if EXEMPLE
int main(void) {
/* Test des ex1*/
    Site* test = Site_construire("Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);
    return 0;
}
#endif
