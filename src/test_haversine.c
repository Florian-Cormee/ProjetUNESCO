#include "haversine.h"
#include <stdlib.h>

int main(void) {
    double dist = calculDistance(50, 3, 49, 5); //distance entre les points (50, 3) et (49, 5)
    double intVal = 182.27; // valeur attendue
    
    if (dist != intVal) {
        printf("Erreur : %lf different de %lf (valeure attendue) !\n", dist, intVal);
        return 1;// on ne passe pas le premier test
    }
    
    dist = calculDistance(-85.7, -170.6, 83.2, 165.3);
    intVal = 18806.7;
    
    if(dist != intVal) {
        printf("Erreur : %lf different de %lf (valeure attendue) !\n", dist, intVal);
        return 2;// on ne passe pas le second test
    }
    return 0; // on passe tous les tests
}
