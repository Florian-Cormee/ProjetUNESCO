#ifndef SITE_H
#define SITE_H

/* Prototypes de Structures */

typedef struct site {
    int n; // position dans le tableau initial de tous les sites
    char *nom;
    double lat;
    double lon;
    char *categorie; //cultural,natural,mixed
    char *pays;
    int enDanger; //0,1
} Site;

typedef struct celluleLDC {
    Site *s;
    struct celluleLDC *prec;
    struct celluleLDC *suiv;
} CelluleLDC;//Elem;

typedef struct ldc {
    CelluleLDC *premier;
    CelluleLDC *dernier;
} LDC;

/* Prototypes de fonctions */
// -------------------------------------- //
//                  SITE                  //
// -------------------------------------- //
Site *Site_construire(int n, char *nom, double lat, double lon, char *categorie, char *pays, int enDanger);

void Site_affichage(Site *site);

int Site_equals(Site *site1, Site *site2);

void Site_supprime(Site *site);

Site **Site_tab_init(int *n);

void Site_tab_supprime(Site **pSite, int n);

// -------------------------------------- //
//                  LDC                   //
// -------------------------------------- //
LDC *LDC_nouveau();

int LDC_taille(LDC *ldc);

void LDC_affiche(LDC *ldc);

void LDC_ajoute_deb(LDC *ldc, Site *site);

void LDC_ajoute_fin(LDC *ldc, Site *site);

int LDC_trouve(LDC *ldc, Site *site);

Site *LDC_get(LDC *ldc, int index);

void LDC_rm(LDC *ldc, Site *site);

void LDC_free(LDC **ldc, int freeSite);

#endif
