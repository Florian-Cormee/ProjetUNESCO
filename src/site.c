#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lectureFichiers.h"
#include "site.h"

Site *Site_construire(int n, char *nom, double lat, double lon, char *categorie, char *pays, int enDanger) {
    Site *site = (Site *) malloc(sizeof(Site));// site a retourner
    site->n = n;
    site->nom = nom == NULL ? NULL : strdup(nom);
    site->lat = lat;
    site->lon = lon;
    site->categorie = categorie == NULL ? NULL : strdup(categorie);
    site->pays = pays == NULL ? NULL : strdup(pays);
    site->enDanger = enDanger;
    return site;
}

void Site_affichage(Site *site) {
    printf("%s / %s / %s", site->nom, site->categorie, site->pays);
    if (site->enDanger) { printf(" / Endangered"); }
    printf("\n");
}

int Site_equals(Site *site1, Site *site2) {
    if (site1 == site2) { return TRUE; }//Meme adresse memoire

    if (site1 == NULL || site2 == NULL) { return ERROR; }
    if (site1->n != site2->n) { return FALSE; }
    if (site1->lat != site2->lat) { return FALSE; }
    if (site1->lon != site2->lon) { return FALSE; }
    if (site1->enDanger != site2->enDanger) { return FALSE; }
    if (strcmp(site1->nom, site2->nom) != 0) { return FALSE; }
    if (strcmp(site1->categorie, site2->categorie) != 0) { return FALSE; }
    if (strcmp(site1->pays, site2->pays) != 0) { return FALSE; }
    return TRUE;
}

void Site_supprime(Site *site) {
    free(site);
}

Site **Site_tab_init(int *n) {
    // Mesure du nombre de sites dans le fichier : unesco.csv
    FILE *fid = fopen("unesco.csv", "r");// Fichier a lire.
    *n = 0; // initialisation du compteur pour la taille de la liste.
    if (fid != NULL) {// si le fichier existe
        while (!feof(fid)) {// tant qu'on n'a pas atteint la fin du fichier
            (*n)++; // il y a un nouvel element a lire
            SkipLine(fid);
        }
        fclose(fid);// fermeture du fichier
    }
    (*n) -= 2;// On ne compte pas la 1ere ligne ni la derniere qui est vide
    // Chargement des sites
    fid = fopen("unesco.csv", "r");// reouverture du fichier (pour reprendre au debut)
    Site **site = (Site **) malloc(sizeof(Site) * (*n));// tableau de tous les sites
    char nom[512], categorie[32], pays[256], continents[512];// tempons pour la lecture des differents champs
    double LAT, LONG;// coordonnees du site
    int EnDanger;// statut du site

    if (fid != NULL) {// si le fichier existe
        SkipLine(fid);// on ignore la premiere ligne (ne decrit pas un site)
        for (int i = 0; i < *n; i++) {
            GetChaine(fid, 512, nom);// lecture du nom
            LAT = GetReel(fid);// lecture de la latitude
            LONG = GetReel(fid);// lecture de la longitude
            GetChaine(fid, 32, categorie); // lecture de la categorie
            GetChaine(fid, 256, pays);// lecture du pays
            GetChaine(fid, 512, continents);// lecture du continent
            EnDanger = GetEntier(fid);// lecture du statut
            site[i] = Site_construire(i, nom, LAT, LONG, categorie, pays, EnDanger);// creation du site
        }
        fclose(fid);// fermeture du fichier
    }
    return site;
}

void Site_tab_supprime(Site **pSite, int n) {
    if (pSite == NULL) { return; }
    for (int i = 0; i < n; i++) {// iteration sur tous les elements
        Site_supprime(pSite[i]); // liberation du site
    }
    free(pSite);//liberation du tableau
}

/*  ---------------- LDC ---------------- */
LDC *LDC_nouveau() {
    LDC *ldc = (LDC *) malloc(sizeof(LDC));//nouvelle liste
    ldc->premier = NULL;//valeur par defaut
    ldc->dernier = NULL;//valeur par defaut
}

int LDC_taille(LDC *list) {
    if (list == NULL) { return ERROR; }
    int size = 0; // compteur d'elements de la liste

    for (CelluleLDC *cell = list->premier; cell != NULL; cell = cell->suiv) {//iteration sur les cellules de la liste
        size++;
    }

    return size;
}

void LDC_affiche(LDC *ldc) {
    printf("LDC {\n");

    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {//iteration sur les cellules de la liste
        printf("\t%p > prec = %p, suiv= %p, s=", cell, cell->prec, cell->suiv);
        Site_affichage(cell->s);
    }
    printf("}\n");
}

void LDC_ajoute_deb(LDC *ldc, Site *site) {
    if (ldc == NULL || site == NULL) { return; }

    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));// nouvelle cellule
    cell->s = site;// valeur de la cellule
    cell->prec = NULL;// valeur par defaut
    cell->suiv = NULL;// valeur par defaut

    if (ldc->dernier == NULL) {// si la liste est vide
        ldc->premier = cell;
        ldc->dernier = cell;
    } else {
        CelluleLDC *cellp = ldc->premier;
        cell->suiv = cellp;
        cellp->prec = cell;
        ldc->premier = cell;
    }
}

void LDC_ajoute_fin(LDC *ldc, Site *site) {
    if (ldc == NULL || site == NULL) { return; }

    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));// nouvelle cellule
    cell->s = site;// valeur de la cellule
    cell->prec = NULL;// valeur par defaut
    cell->suiv = NULL;// valeur par defaut

    if (ldc->dernier == NULL) {// si la liste est vide
        ldc->premier = cell;
        ldc->dernier = cell;
    } else {
        CelluleLDC *celld = ldc->dernier;
        celld->suiv = cell;
        cell->prec = celld;
        ldc->dernier = cell;
    }
}

int LDC_trouve(LDC *ldc, Site *site) {
    if (ldc == NULL || site == NULL) { return ERROR; }

    int i = 0;// indice de la cellule consultee

    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {// iteration sur la liste
        if (Site_equals(cell->s, site)) {// s'il y a equivalence entre la valeur de la cellule et site
            return i;
        }
        i++;
    }
    return ERROR;//l'element est absent de la liste
}

Site *LDC_get(LDC *ldc, int i) {
    if (ldc == NULL || i < 0) { return NULL; }

    CelluleLDC *courant = ldc->premier;// cellule courante

    for (int index = 0; index < i && courant != NULL; index++) {//iteration jusqu'a index = i ou fin de liste
        printf("ldc.get(%3d) = %p\n", index, courant->s);
        courant = courant->suiv;
    }

    if (courant == NULL) { return NULL; }//fin de la liste atteinte
    return courant->s;
}

void LDC_rm(LDC *ldc, Site *site) {
    if (ldc == NULL || site == NULL) { return; }

    CelluleLDC *courant = ldc->premier;// cellule a supprimer

    while (courant != NULL && !Site_equals(courant->s, site)) {// recherche de la cellule a supprimer
        courant = courant->suiv;
    }

    if (courant == NULL) { return; }

    if (courant->prec == NULL && courant->suiv == NULL) {//c'est l'unique element de la liste
        ldc->premier = NULL;
        ldc->dernier = NULL;
    } else if (courant->prec == NULL) {//c'est le premier element de la liste
        (courant->suiv)->prec = NULL;
        ldc->premier = courant->suiv;
    } else if (courant->suiv == NULL) {//c'est le dernier element de la liste
        (courant->prec)->suiv = NULL;
        ldc->dernier = courant->prec;
    } else {
        (courant->prec)->suiv = courant->suiv;
        (courant->suiv)->prec = courant->prec;
    }
    free(courant);// liberation de la cellule
}

void LDC_free(LDC **ldc, int freeSite) {
    if (ldc == NULL || *ldc == NULL) { return; }

    for (CelluleLDC *cell = (*ldc)->premier; cell != NULL; cell = (*ldc)->premier) {//iteration sur la liste
        (*ldc)->premier = cell->suiv;
        if (freeSite) {// si on doit liberer les sites
            Site_supprime(cell->s);
        }
        free(cell);
    }
    free(*ldc);
    *ldc = NULL;
}

int LDC_empty(LDC *ldc) {
    if (ldc == NULL) {
        return 1;
    }
    return ldc->premier == NULL ? 1 : 0;// la liste est consideree vide si le pointeur vers le premier element est NULL
}
