#include<stdio.h>
#include<stdlib.h>

#include "algorithm.h"
#include "site.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    int n = 0;
    double homeLat = atof(argv[1]);
    double homeLong = atof(argv[2]);
    Site** s = Site_tab_init(&n);
    LDC* l_s = Algo_Champ_des_Possibles_init(homeLat, homeLong, s, n);
    long** mat_dist = calculToutesDistances(l_s, homeLat, homeLong, &n);
    
    
    showMap(l_s, homeLat, homeLong);
    LDC_free(&l_s, TRUE);
    Site_tab_supprime(s, n);
    return 0;
}
