#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "site.h"
#include "utils.h"

#define EXEMPLE 0

Site* Site_construire(char* nom,float LAT,float LONG,char* categorie,char* pays,int enDanger) {
    Site* s = (Site*) malloc(sizeof(Site));
    s->nom = nom;
    s->LAT = LAT;
    s->LONG = LONG;
    s->categorie = categorie;
    s->pays = pays;
    s->enDanger = enDanger;
    return s;
}

void Site_affichage(Site* s) {
    printf("%s / %s / %s", s->nom, s->categorie, s->pays);
    if(s->enDanger) {printf(" / Endangered");}
    printf("\n");
}

int Site_equals(Site *s1, Site *s2){
    if(s1 == NULL || s2 == NULL) { return ERROR; }
    if(s1 == s2) { return TRUE; }//Meme adresse memoire
    
    if(s1->LAT != s2->LAT) { return FALSE; }
    if(s1->LONG != s2->LONG) { return FALSE; }
    if(s1->enDanger != s2->enDanger) { return FALSE; }
    if(!strcmp(s1->nom, s2->nom)) { return FALSE; }
    if(!strcmp(s1->categorie, s2->categorie)) { return FALSE; }
    if(!strcmp(s1->pays, s2->pays)) { return FALSE; }
    return TRUE;
}

/*  ---------------- List ---------------- */
List *List_nouveau(){
    List *l = (List *) malloc(sizeof(List));
    l->premier = NULL;
    l->dernier = NULL;
}

void List_ajoute(List *l, Site *s){
    if(l == NULL || s == NULL) { return; }
    
    Elem *e = (Elem *) malloc(sizeof(Elem));
    e->s = s;
    e->precedant = NULL;
    e->suivant = NULL;
    
    if(l->dernier == NULL){// si la liste est vide
        l->premier = e;
        l->dernier = e;
    } else {
        Elem *courant = l->dernier;
        courant->suivant = e;
        e->precedant = courant;
        l->dernier = e;
    }
}

int List_trouve(List *l, Site *s){
    if(l == NULL || s == NULL) { return ERROR; }
    int i = 0;
    
    for(Elem *e = l->premier; e != NULL; e = e->suivant) {
        if(Site_equals(e->s, s)){
            return i;
        }
        i++;
    }
    return ERROR;
}

Site *List_get(List *l, int i){
    if(l == NULL || i < 0) { return NULL; }
    Elem *courant = l->premier;
    
    for(int index = 0; index < i && courant != NULL; index++){
        courant = courant->suivant;
    }

    if (courant == NULL) { return NULL; }
    return courant->s;
}

void List_rm(List *l, Site *s){
    if(l == NULL || s == NULL) { return; }
    Elem *courant = l->premier;
    
    while(courant != NULL && Site_equals(courant->s, s)) {
        courant = courant->suivant;
    }
    if(courant->precedant == NULL && courant->suivant == NULL) {//c'est l'unique element de la liste
        l->premier = NULL;
        l->dernier = NULL;
    } if(courant->precedant == NULL){//c'est le premier element de la liste
        (courant->suivant)->precedant = NULL;
        l->premier = courant->suivant;
    } else if(courant->suivant == NULL){//c'est le dernier element de la liste
        (courant->precedant)->suivant = NULL;
        l->dernier = courant->precedant;
    } else {
        (courant->precedant)->suivant = courant->suivant;
        (courant->suivant)->precedant = courant->precedant;
    }
    free(courant);
}

void List_free(List **l, int freeSite){
    if(l == NULL || *l == NULL) { return; }
    for(Elem *e = (*l)->premier; e != NULL; e = (*l)->premier){
        (*l)->premier = e->suivant;
        if(freeSite) {
            free(e->s);
        }
        free(e);
    }
    *l = NULL;
}

int List_taille(List *list){
	if(list == NULL) { return ERROR; }
	int size = 0; // compteur d'elements de la liste

	for(Elem *e = list->premier; e != NULL; e = e->suivant){
		size++;
	}

	return size;
}

#if EXEMPLE
int main(void) {
    Site *test = Site_construire("Tour Zamansky", 46.131930, 6.430720, "mixed", "France", 1);
    Site_affichage(test);
    return 0;
}
#endif
