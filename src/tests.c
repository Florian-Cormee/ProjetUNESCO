#include <stdio.h>
#include <time.h>
#include "haversine.h"
#include "utils.h"
#include "site.h"

#define EXO (2)


int main(void) {
    clock_t tDeb = clock();
    clock_t tFin = tDeb;
#if EXO == 1 /* Tests de l'exercice 1 */
    Site *test = Site_construire(0, "Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);
    int n = 0;
    Site **test_tab = Site_tab_init(&n);
    printf("Il y a %d element(s)\n", n);
    Site_supprime(test);
    Site_tab_supprime(test_tab, n);
#endif
#if EXO == 2 /* Tests de l'exercice 2 */
    double dist = calculDistance(50, 3, 49, 5); //distance entre les points (50, 3) et (49, 5)
    double intVal = 182.27; // valeur attendue
    LDC *l = LDC_nouveau(); // liste de sites
    long **dists = NULL; // distances entres tous les points
    int dists_taille = 0;

    if (convKmCm(dist) != convKmCm(intVal)) {
        printf("Erreur : %lf different de %lf (valeure attendue) !\n", dist, intVal);
        return 1;// on ne passe pas le premier test
    }

    dist = calculDistance(-85.7, -170.6, 83.2, 165.3);
    intVal = 18806.7;

    if (convKmCm(dist) != convKmCm(intVal)) {
        printf("Erreur : %.2lf different de %.2lf (valeure attendue) !\n", dist, intVal);
        return 2;// on ne passe pas le second test
    }

    char nom[] = {'J', 'u', 's', 's', 'i', 'e', 'u'};//"Jussieu" ;
    Site *s1 = Site_construire(0, nom, 50, 3, "Cultural", "FRANCE", FALSE);
    printf("avant nom[0] = %c\n", nom[0]);
    nom[0] = 'A';
    printf("ap\n");
    Site *s2 = Site_construire(1, "nom", -85.7, -170.6, "Natural", "Yolo", TRUE);
    Site *s3 = Site_construire(2, nom, -85.7, -170.6, "Natural", "Yolo", TRUE);
    Site_affichage(s1);
    Site_affichage(s2);
    LDC_ajoute_fin(l, s1);
    LDC_ajoute_fin(l, s2);
    LDC_ajoute_deb(l, s3);

    dists = calculToutesDistances(l, 83.2, 165.3, &dists_taille);
    printf("\n");
    afficheTab(dists, dists_taille);
    LDC_affiche(l);

    //Site *s4 = LDC_get(l, 1);
    printf("s2 equals s3 ? %d\n", Site_equals(s2, s3));
    printf("Indice de s2 = %d\n", LDC_trouve(l, s2));

    Site *s = LDC_get(l, LDC_trouve(l, s2));
    printf("%p et on obtient %p\n", s2, s);

    //LDC_rm(l, s1);
    //LDC_affiche(l);
    printPath(l, 83.2, 165.3, dists, dists_taille);
    printf("Score total : %4d pts !\n",
           score(l, 1));

    showMap(l, 83.2, 165.3);

    freeTab(dists, dists_taille);
    LDC_free(&l, TRUE);
#endif
    tFin = clock();
    printf("Duree d'execution : %lf s", (double) (tDeb - tFin) / CLOCKS_PER_SEC);
    return 0; // on passe tous les tests
}