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
    Site *s;
    struct elem *suivant;
    struct elem *precedant;
} Elem;

typedef struct list {
    Elem* premier;
    Elem* dernier;
} List;

/* Prototypes de fonctions */

Site* Site_construire(char* nom,float LAT,float LONG,char* categorie,char* pays,int enDanger);

void Site_affichage(Site *s);

int Site_equals(Site *s1, Site *s2);

Site** Site_tab_init();

void Site_supprime(Site* s);

void Site_tab_supprime(Site** s, int n);

int List_taille(List *l);

List *List_nouveau();

void List_ajoute(List *l, Site *s);

int List_trouve(List *l, Site *s);

Site *List_get(List *l, int i);

void List_rm(List *l, Site *s);

void List_free(List **l, int freeSite);

#endif
