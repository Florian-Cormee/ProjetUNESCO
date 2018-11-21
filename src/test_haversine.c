#include <stdio.h>
#include "haversine.h"
#include "utils.h"
#include "site.h"

int main(void) {
    double dist = calculDistance(50, 3, 49, 5); //distance entre les points (50, 3) et (49, 5)
    double intVal = 182.27; // valeur attendue
    List *l = List_nouveau(); // liste de sites
    int **dists = NULL; // distances entres tous les points
    int dists_taille = 0;
    
    if (convKmCm(dist) != convKmCm(intVal)) {
        printf("Erreur : %lf different de %lf (valeure attendue) !\n", dist, intVal);
        return 1;// on ne passe pas le premier test
    }
    
    dist = calculDistance(-85.7, -170.6, 83.2, 165.3);
    intVal = 18806.7;
    
    if(convKmCm(dist) != convKmCm(intVal)) {
        printf("Erreur : %.2lf different de %.2lf (valeure attendue) !\n", dist, intVal);
        return 2;// on ne passe pas le second test
    }
    
    Site *s1 = Site_construire("Jussieu", 50, 3, "cultural", "FRANCE", FALSE);
    Site *s2 = Site_construire("Yololand", -85.7, -170.6, "natural", "Yolo", TRUE);
    Site *s3 = Site_construire("Test", 0, 0, "natural","Test", FALSE);
    
    List_ajoute(l, s1);
    List_ajoute(l, s2);
    List_ajoute(l, s3);
    
    dists = calculToutesDistances(l, 83.2, 165.3, &dists_taille);
    printf("\n");
    afficheTab(dists, dists_taille);
    freeTab(dists, dists_taille);
    List_free(&l, TRUE);
    return 0; // on passe tous les tests
}


