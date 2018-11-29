#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lectureFichiers.h"
#include "site.h"

Site *Site_construire(int n, char *nom, double lat, double lon, char *categorie, char *pays, int enDanger) {
    Site *site = (Site *) malloc(sizeof(Site));
    site->n = n;
    site->nom = strdup(nom);
    site->lat = lat;
    site->lon = lon;
    site->categorie = strdup(categorie);
    site->pays = strdup(pays);
    site->enDanger = enDanger;
    return site;
}

void Site_affichage(Site *site) {
    printf("%s / %s / %s", site->nom, site->categorie, site->pays);
    if (site->enDanger) { printf(" / Endangered"); }
    printf("\n");
}

int Site_equals(Site *site1, Site *site2) {
    if (site1 == NULL || site2 == NULL) { return ERROR; }
    if (site1 == site2) { return TRUE; }//Meme adresse memoire

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
    FILE *fid = fopen("unesco.csv", "r");
    *n = 0;
    if (fid != NULL) {// si le fichier existe
        while (!feof(fid)) {// tant qu'on n'a pas atteint la fin du fichier
            (*n)++;
            SkipLine(fid);
        }
        fclose(fid);
    }
    (*n) -= 2;// On ne compte pas la 1ere ligne ni la derniere qui est vide
    // Chargement des sites
    fid = fopen("unesco.csv", "r");
    Site **site = (Site **) malloc(sizeof(Site) * (*n));// tableau de tous les sites
    char nom[512], categorie[32], pays[256], continents[512];
    double LAT, LONG;
    int EnDanger;

    if (fid != NULL) {// si le fichier existe
        SkipLine(fid);
        for (int i = 0; i < *n; i++) {
            GetChaine(fid, 512, nom);
            LAT = GetReel(fid);
            LONG = GetReel(fid);
            GetChaine(fid, 32, categorie);
            GetChaine(fid, 256, pays);
            GetChaine(fid, 512, continents);
            EnDanger = GetEntier(fid);
            site[i] = Site_construire(i, nom, LAT, LONG, categorie, pays, EnDanger);
        }
        fclose(fid);
    }
    return site;
}

void Site_tab_supprime(Site **pSite, int n) {
    if (pSite == NULL) { return; }
    for (int i = 0; i < n; i++) {
        free(*(pSite + i));
    }
}

/*  ---------------- LDC ---------------- */
LDC *LDC_nouveau() {
    LDC *ldc = (LDC *) malloc(sizeof(LDC));
    ldc->premier = NULL;
    ldc->dernier = NULL;
}

int LDC_taille(LDC *list) {
    if (list == NULL) { return ERROR; }
    int size = 0; // compteur d'elements de la liste

    for (CelluleLDC *cell = list->premier; cell != NULL; cell = cell->suiv) {
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
    if (ldc == NULL || site == NULL) { return; }

    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));
    cell->s = site;
    cell->prec = NULL;
    cell->suiv = NULL;

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

    CelluleLDC *cell = (CelluleLDC *) malloc(sizeof(CelluleLDC));
    cell->s = site;
    cell->prec = NULL;
    cell->suiv = NULL;

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

    int i = 0;

    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
        if (Site_equals(cell->s, site)) {
            return i;
        }
        i++;
    }
    return ERROR;
}

Site *LDC_get(LDC *ldc, int i) {
    if (ldc == NULL || i < 0) { return NULL; }

    CelluleLDC *courant = ldc->premier;

    for (int index = 0; index < i && courant != NULL; index++) {
        courant = courant->suiv;
    }

    if (courant == NULL) { return NULL; }
    return courant->s;
}

void LDC_rm(LDC *ldc, Site *site) {
    if (ldc == NULL || site == NULL) { return; }

    CelluleLDC *courant = ldc->premier;

    while (courant != NULL && !Site_equals(courant->s, site)) {
        courant = courant->suiv;
    }

    if (courant == NULL) { return; }

    if (courant->prec == NULL && courant->suiv == NULL) {//c'est l'unique element de la liste
        // printf("rm unique element\n");
        ldc->premier = NULL;
        ldc->dernier = NULL;
    } else if (courant->prec == NULL) {//c'est le premier element de la liste
        // printf("rm deb\n");
        (courant->suiv)->prec = NULL;
        ldc->premier = courant->suiv;
    } else if (courant->suiv == NULL) {//c'est le dernier element de la liste
        // printf("rm fin\n");
        (courant->prec)->suiv = NULL;
        ldc->dernier = courant->prec;
    } else {
        // printf("rm autre\n");
        (courant->prec)->suiv = courant->suiv;
        (courant->suiv)->prec = courant->prec;
    }
    free(courant);
}

void LDC_free(LDC **ldc, int freeSite) {
    if (ldc == NULL || *ldc == NULL) { return; }

    for (CelluleLDC *cell = (*ldc)->premier; cell != NULL; cell = (*ldc)->premier) {
        (*ldc)->premier = cell->suiv;
        if (freeSite) {
            free(cell->s);
        }
        free(cell);
    }
    *ldc = NULL;
}
