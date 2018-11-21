#include <stdio.h>
#include "haversine.h"
#include "utils.h"
#include "site.h"

int main(void) {
    double dist = calculDistance(50, 3, 49, 5); //distance entre les points (50, 3) et (49, 5)
    double intVal = 182.27; // valeur attendue
    LDC *l = LDC_nouveau(); // liste de sites
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
    
    char nom[] = {'J', 'u', 's', 's', 'i', 'e', 'u'};//"Jussieu" ;
    Site *s1 = Site_construire(nom, 50, 3, "cultural", "FRANCE", FALSE);
    printf("avant nom[0] = %c\n", nom[0]);
    nom[0] = 'A';
    printf("ap\n");
    Site *s2 = Site_construire("nom", -85.7, -170.6, "natural", "Yolo", TRUE);
    Site *s3 = Site_construire(nom, -85.7, -170.6, "natural", "Yolo", TRUE);
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

    LDC_rm(l, s1);
    LDC_affiche(l);

    freeTab(dists, dists_taille);
    LDC_free(&l, TRUE);
    return 0; // on passe tous les tests
}


