#ifndef SITE_H
#define SITE_H

/* Prototypes de Structures */

typedef struct site {
    char* nom ;
    float LAT ;
    float LONG ;
    char* categorie ; //cultural,natural,mixed
    char* pays ;
    int enDanger ; //0,1
} Site;

typedef struct elem {
    Site s;
    Site* suivant;
}Elem;

typedef struct list {
    Site* premier;
    Site* dernier;
}List;

/* Prototypes de fonctions */

Site* Site_construire(char* nom,float
LAT,float LONG,char* categorie,char* pays,int enDanger);

void Site_affichage(Site* s);

Site* Site_tab_init();

#endif
