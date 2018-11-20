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

#if EXEMPLE
int main(void) {
/* Test des ex1*/
    Site* test = Site_construire("Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);

/* Creation tableau de site */

    FILE* fid = fopen("unesco.csv", "r");
    int n = 0;
    if(fid!=NULL) {
        while(!feof(fid)) {
            SkipLine(fid);
            n++;
        }
        fclose(fid);
        fid = fopen("unesco.csv", "r");
        if(fid!=NULL) {
            Site* Site_tab = (Site*) malloc(sizeof(Site)*n);
            SkipLine(fid);
            char* nom;
            char* categorie;
            char* pays;
            double LAT, LONG;
            int enDanger;
            for(int i=0; i<n; i++) {
                GetChaine(fid, 64, nom);
                printf("%s ",nom);
                Skip(fid);
                LAT = GetReel(fid);
                Skip(fid);
                LONG = GetReel(fid);
                Skip(fid);
                GetChaine(fid, 64, categorie);
                printf("%s ",nom);
                Skip(fid);
                GetChaine(fid, 64, pays);
                printf("%s \n",nom);
                Skip(fid);
                enDanger = GetEntier(fid);
                test = Site_construire(nom, LAT, LONG, categorie, pays, enDanger);
                Site_tab[i]=*test;
                Site_affichage((Site_tab+i));
            }
        }
        printf("%d\n", n);
        fclose(fid);
    }
    return 0;
}
#endif
