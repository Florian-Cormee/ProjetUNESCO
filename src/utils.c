#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "site.h"
#include "haversine.h"

double **calculToutesDistances(LDC *ldc, double homeLat, double lon, int *tabDistLength) {
    if (tabDistLength == NULL) { return NULL; }
    if (ldc == NULL) {
        *tabDistLength = 0;
        return NULL;
    }
    int longueur = LDC_taille(ldc); // longueur de la List ldc
    /* matrice carree diagonale des distances entre sites et origine */
    double **dist = (double **) malloc(sizeof(double *) * (longueur + 1));
    int i = 0; // index de l'iteration mere
    int j = 0; // index de la sous iteration
    double distance;

    *tabDistLength = longueur + 1; // on indique la taille du tableau renvoye
    for (int i0 = 0; i0 < *tabDistLength; ++i0) {
        dist[i0] = (double *) malloc(sizeof(double) * (*tabDistLength));//on creer les lignes
        for (int j0 = 0;
             j0 < longueur + 1; ++j0) { dist[i0][j0] = -1; }//on l'initialise avec une valeur par defaut de -1
    }
    for (CelluleLDC *courant1 = ldc->premier; courant1 != NULL; courant1 = courant1->suiv) {//iteration sur la liste
        j = 0;
        for (CelluleLDC *courant2 = ldc->premier; courant2 != NULL; courant2 = courant2->suiv) {
            if (dist[i][j] == -1) {//on a pas calcule la distance
                if (courant1 == courant2) {//on veut connaitre la distance entre deux points identiques
                    distance = 0;
                } else {
                    distance = calculDistance((courant1->s)->lat, (courant1->s)->lon, (courant2->s)->lat,
                                              (courant2->s)->lon);//distance entre lieux
                }
                dist[i][j] = distance;
                dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
            }
            j++;
        }
        distance = calculDistance((courant1->s)->lat, (courant1->s)->lon, homeLat, lon);//distance entre un lieu et le depart
        dist[i][j] = distance;
        dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
        i++;
    }
    dist[(*tabDistLength) - 1][(*tabDistLength) - 1] = 0;
    return dist;
}

void afficheTab(double **tabDist, int tabDistLength) {
    for (int i = 0; i < tabDistLength; i++) {
        printf("|");
        for (int j = 0; j < tabDistLength; j++) {
            printf("%10.2lf|", tabDist[i][j]);
        }
        printf("\n");
    }
}

void freeTab(double **tabDist, int tabDistLength) {
    if (tabDist == NULL) { return; }
    for (int i = 0; i < tabDistLength; i++) {
        free(*(tabDist + i));
    }
    free(tabDist);
}

int score(LDC *ldc, int printDetails) {
    if (ldc == NULL) { return ERROR; }
    int taille = LDC_taille(ldc);//taille & nombre de destinations
    char *pays[taille];//tableau des pays visites
    int nbPays = 0; // nombre de pays visites differents
    int nbEndangered = 0; // nombre de sites en peril visites
    Site *site = NULL; // site courant
    char *paysSite = NULL; // pays associe au site

    for (int j = 0; j < taille; ++j) {
        pays[j] = NULL;
    }

    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
        site = cell->s;
        if (site != NULL) {
            paysSite = site->pays;
            int i;
            for (i = 0; i < taille && pays[i] != NULL; ++i) {
                if (!strcmp(pays[i], paysSite)) {//paysSite est present dans tab
                    break;
                }
            }
            if (pays[i] == NULL) {
                // paysSite est absent (on ne verifi pas i car il y a au plus "taille" pays visite differents)
                pays[i] = paysSite;
            }
            if (site->enDanger) {//le site est en peril
                nbEndangered++;
            }
        }
    }
    for (nbPays = 0; nbPays < taille && pays[nbPays] != NULL; nbPays++); // Compter le nombre de pays visité
    if (printDetails) {
        printf("Evaluation:\n\t%3d destinations x 1 point\n\t%3d pays x 2 points\n\t%3d endangered x 3 points\n",
               taille, nbPays, nbEndangered);
    }
    return taille + 2 * nbPays + 3 * nbEndangered; //retourne le score
}

int difference(LDC *ldc) {
    int difference = 0;
    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) { // Parcours tous les sites du chemin
        if (strcmp(cell->s->categorie, "Cultural") == 0) {
            difference++;
        } else if (strcmp(cell->s->categorie, "Natural") == 0) {
            difference--;
        }
    }
    return difference; // Si la difference > 0 alors il y a plus de Cultural que de Natural // Si la difference < 0 alors il y a plus de Natural que de Cultural 
}

void printPath(LDC *ldc, double homeLat, double homeLon, double **tabDist, int tabSize) {
    if (ldc == NULL || tabDist == NULL) { return; }
    int i = 0; // Indice du lieu
    int nbCult = 0;
    int nbNat = 0;
    int nbMixed = 0;
    double distance = 0;
    double deltaDist = 0;
    double time = 0;
    CelluleLDC *prec = NULL;
    printf("ITINERAIRE DE VOYAGE\n");
    printf("%2d) Starting point - (%lf, %lf)\n", i++, homeLat, homeLon); // Affiche la position d'origine
    #if DEBUG
    printf("\tdist = %ld\n\ttime = %lf\n", distance, time);
    #endif
    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
        printf("%2d) ", i++);
        Site_affichage(cell->s);
/* Compte chaque categories de site */
        if (strcmp(cell->s->categorie, "Cultural") == 0) {
            nbCult++;
        } else if (strcmp(cell->s->categorie, "Natural") == 0) {
            nbNat++;
        } else if (strcmp(cell->s->categorie, "Mixed") == 0) {
            nbMixed++;
        } else {
            printf(" /!\\ No matching type for :");
            Site_affichage(cell->s);
        }
        if (prec == NULL) {
            deltaDist = tabDist[cell->s->n][tabSize - 1];
        } else {
            deltaDist = tabDist[prec->s->n][cell->s->n];
        }
        distance += deltaDist;  // Calcul la distance parcourue
        time += BREAK_TIME + deltaDist / VITESSE;
        prec = cell;
        #if DEBUG
        printf("\tdist = %ld\n\ttime = %lf\n", distance, time);
        #endif
    }
    if (prec != NULL) {
        deltaDist = tabDist[prec->s->n][tabSize - 1];
        distance += deltaDist;
        time += 6 + deltaDist / VITESSE; // Transforme la distance en temps
    }
/* Présentation du resultat */
    printf("%2d) Starting point - (%lf, %lf)\n", i, homeLat, homeLon);
    #if DEBUG
    printf("\tdist = %ld\n\ttime = %lf\n", distance, time);
    #endif
    printf("Nombre de sites culturel :\t %2d\nNombre de sites naturel :\t %2d\nNombre de sites mixed :\t\t %2d\n",
           nbCult, nbNat, nbMixed);
    printf("Distance parcourue :\t\t  %.2lf km\n", distance);
    printf("Duree du voyage :\t\t\t  %lf heures (max = %d heures)\n", time, MAX_TIME);
}

int pathToFile(LDC *ldc, double homeLat, double homeLon) {
    FILE *file = fopen("Tour.txt", "w");
    Site *s;

    if (file != NULL) {
        fprintf(file, "%lf, %lf, \n", homeLat, homeLon);

        if (ldc != NULL) {
            for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
                s = cell->s;
                fprintf(file, "%lf, %lf, %s\n", s->lat, s->lon, s->categorie);
            }
        }

        fprintf(file, "%lf, %lf, \n", homeLat, homeLon);
        fclose(file);
    } else {
        return 0;
    }
    return 1;
}

int showMap(LDC *ldc, double homeLat, double homeLon) {
    if (ldc == NULL) { return ERROR; } // Si la liste n'existe pas
    pathToFile(ldc, homeLat, homeLon); // Envoie des donnees dans le fichier Tours.txt
    return system("java -jar ../UnescoMap.jar"); // Execution du programme .jar
}
