#include <stdio.h>
#include "utils.h"
#include "site.h"
#include "algorithm.h"

#define TEST_MODE 0

#if !TEST_MODE

int main(int argc, char *argv[]) {
    double homeLat = 0;
    double homeLon = 0;
    Site **siteTab = NULL;
    int siteTabLength = 0;
    double **distTab = NULL;
    int distTabLength = 0;
    LDC *siteLDC = NULL;
    LDC *itineraire = NULL;
    int indice = 0;

    if (argc != 3) {
        printf("Il ne faut que les coordonnées du point de départ.\nUsage :\t./unesco <latitude> <longitude>\n");
        return 1;
    }

    sscanf(argv[1], "%lf", &homeLat);
    sscanf(argv[2], "%lf", &homeLon);

    printf("Point de départ : (%lf, %lf)\n", homeLat, homeLon);

    printf("Chargement des sites..");
    siteTab = Site_tab_init(&siteTabLength);
    printf("%d sites chargees..\n", siteTabLength);

    siteLDC = Algo_Champ_des_Possibles_init(homeLat, homeLon, siteTab, siteTabLength);

    printf("Calcul des distances entre sites..");
    distTab = calculToutesDistances(siteLDC, homeLat, homeLon, &distTabLength);
    printf("Fini..\n");

    printf("Welcome to the Unesco travel challenge!\nChoose the algorithm : ");
    //scanf("%d", &indice);
    //if (indice == 1) {
    itineraire = Algo_itineraire(&siteLDC, distTab, distTabLength);
    //}
    /* LDC *new = Algo_2optSwap(itineraire,35,40);
     printPath(new, homeLat, homeLon, distTab, distTabLength);
     LDC_free(&new,FALSE);*/
    printPath(itineraire, homeLat, homeLon, distTab, distTabLength);
    printf("Score total : %4d pts !\n", score(itineraire, 1));

    printf("Writing path to file\n");
    showMap(itineraire, homeLat, homeLon);

    printf("Liberation de la memoire..");
    LDC_free(&siteLDC, 0);
    LDC_free(&itineraire, 0);
    freeTab(distTab, distTabLength);
    Site_tab_supprime(siteTab, siteTabLength);
    printf("Fini\nFin du programme.\n");

    return 0;
}

#endif
