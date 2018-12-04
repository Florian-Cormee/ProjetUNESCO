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
    int enDanger; //vaut TRUE ou FALSE
} Site;

typedef struct celluleLDC {
    Site *s;
    struct celluleLDC *prec;
    struct celluleLDC *suiv;
} CelluleLDC;

typedef struct ldc {
    CelluleLDC *premier;
    CelluleLDC *dernier;
} LDC;

/* Prototypes de fonctions */
// -------------------------------------- //
//                  SITE                  //
// -------------------------------------- //

/**
 * Cree un site en initialisant ses variables.
 * @param n Identifiant du site (pour la matrice des distances notamment.
 * @param nom Nom du site.
 * @param homeLat Latitude du point de depart.
 * @param homeLon Longitude du point de depart.
 * @param categorie Categorie du site.
 * @param pays Pays ou se trouve le site.
 * @param enDanger (booleen) TRUE si en danger sinon FALSE.
 * @return Pointeur vers le site cree.
 */
Site *Site_construire(int n, char *nom, double lat, double lon, char *categorie, char *pays, int enDanger);

/**
 * Affiche un site dans la console
 * @param site Pointeur vers le site a afficher
 */
void Site_affichage(Site *site);

/**
 * Test d'equivalence entre deux sites.
 * @param site1 Pointeur vers un site.
 * @param site2 Pointeur vers un site.
 * @return TRUE si champ à champ les sites sont egaux, sinon FALSE.
 */
int Site_equals(Site *site1, Site *site2);

/**
 * Libere la memoire allouee pour un site
 * @param site Pointeur vers le site a liberer
 */
void Site_supprime(Site *site);

/**
 * Charge les sites depuis "unesco.csv".
 * @param Pointeur vers la taille du tableau.
 * @return Le tableau des sites charges.
 */
Site **Site_tab_init(int *n);

/**
 * Libere la memoire allouee au tableau des sites.
 * @param pSite Tableau des sites.
 * @param n Taille du tableau.
 */
void Site_tab_supprime(Site **pSite, int n);

// -------------------------------------- //
//                  LDC                   //
// -------------------------------------- //
/**
 * Cree une nouvelle liste doublement chainee.
 * @return Un pointeur vers la liste nouvellement cree.
 */
LDC *LDC_nouveau();

/**
 * Donne la taille de la liste.
 * @param ldc Liste de sites.
 * @return La taille de la liste.
 */
int LDC_taille(LDC *ldc);

/**
 * Affiche la liste (dans le detail) dans la console
 * @param ldc Liste a afficher.
 */
void LDC_affiche(LDC *ldc);

/**
 * Ajoute un site au debut d'une liste.
 * @param ldc Liste a laquelle on ajoute un site.
 * @param site Site a ajouter.
 */
void LDC_ajoute_deb(LDC *ldc, Site *site);

/**
 * Ajoute un site a la fin d'une liste.
 * @param ldc Liste a laquelle on ajoute un site.
 * @param site Site a ajouter.
 */
void LDC_ajoute_fin(LDC *ldc, Site *site);

/**
 * Donne l'indice (dans la liste) correspondant au site.
 * @param ldc Liste de sites.
 * @param site Site a chercher.
 * @return Indice du site dans la liste.
 */
int LDC_trouve(LDC *ldc, Site *site);

/**
 * Donne le site a l'indice donnee dans la liste.
 * @param ldc Liste de sites.
 * @param index Indice de l'element a recuperer.
 * @return Le site a l'indice dans la liste.
 */
Site *LDC_get(LDC *ldc, int index);

/**
 * Retire un site de la liste.
 * @param ldc Liste de sites.
 * @param site Site a retirer de la liste.
 */
void LDC_rm(LDC *ldc, Site *site);

/**
 * Libere la memoire alloue a une liste.
 * @param ldc Pointeur du pointeur vers la liste.
 * @param freeSite (booleen) TRUE: les sites sont egalements liberes, sinon seuls les cellules le sont.
 */
void LDC_free(LDC **ldc, int freeSite);

/**
 * Indique si la liste est vide.
 * @param ldc Liste a tester.
 * @return TRUE si la liste est vide, FALSE sinon.
 */
int LDC_empty(LDC *ldc);
#endif
