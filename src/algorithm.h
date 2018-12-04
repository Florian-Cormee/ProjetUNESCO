#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "site.h"

/**
 * Calcule la longueur d'un itineraire en tenant compte de la portee perdu par la visite.
 * @param itineraire itineraire dont on souhaite connaitre la longueur
 * @param tabDist matrice des distances
 * @param tabDistLength taille de la matrice
 * @return la lougueur en km du parcours.
 */
double Algo_longueur(LDC *itineraire, double **tabDist, int tabDistLength);

/**
 * Selectionne les sites initialement a porte (cad qui permettent de revenir ensuite) du point de depart
 * @param homeLat Latitude du point de depart.
 * @param homeLong Longitude du point de depart.
 * @param s_tab tableau de l'ensemble des sites.
 * @param n taille du tableau.
 * @return La liste des sites initialement a porte.
 */
LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n);

/**
 * Selectionne les sites a porte (cad qui permettent de revenir au point de depart) du site actuel.
 * @param l Liste des sites precedemment a porte
 * @param tab_dist matrice des distances entre points
 * @param ind_d indice du point de depart dans la matrice des distances
 * @param ind_a indice du point actuel dans la matrice des distances
 * @param distance_restant distance restante pour le voyage
 * @param n la taille de la matrice des distances
 * @return La listes des sites toujours a porte.
 */
LDC *Algo_Champ_des_Possibles(LDC *l, double **tab_dist, int ind_d, int ind_a, double distance_restant, int n);

/**
 * Determine un itineraire.
 * @param ldc Liste des sites accessibles
 * @param tabDist matrice des distances
 * @param tabDistLength taille de cette matrice
 * @return un itineraire
 */
LDC *Algo_itineraire(LDC **ldc, double **tabDist, int tabDistLength);

/**
 * Calcule le score d'un site potentiel (pour obtenir le plus proche voisin)
 * @param tabDist matrice des distances
 * @param tabDistLength taille de la matrice des distances
 * @param difference equart a l'equilibre des types de sites
 * @param potentialSite le site a tester
 * @param portee portee restante
 * @param currentSite dernier site visite
 * @return le score associe au site potentiel
 */
double Algo_scorePlusProche(double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite);

/**
 * Calcule le score d'un site potentiel
 * @param tabDist matrice des distances
 * @param tabDistLength taille de la matrice des distances
 * @param difference equart a l'equilibre des types de sites
 * @param potentialSite le site a tester
 * @param portee portee restante
 * @param currentSite dernier site visite
 * @return le score associe au site potentiel
 */
double Algo_score(LDC *itineraire, double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite);

/**
 * Optimise un itineraire
 * @param itineraire itineraire a optimiser
 * @param tabDist matrice des distances
 * @param tabDistLength taille de la matrice des distances
 * @return la longueur de l'itineraire en fin d'execution
 */
double Algo_2opt(LDC *itineraire, double **tabDist, int tabDistLength); //Par echange

LDC *Algo_Rand(LDC *ldc, double **tabDist, int tabDistLength);

LDC *Algo_Best_Rand(LDC **ldc, double **tabDist, int tabDistLength);

#endif