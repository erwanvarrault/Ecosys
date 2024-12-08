#include "ecosys.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    for (int i = 0; i < 20; i++)
    {
        ajouter_animal(random() % SIZE_X, random() % SIZE_X, 240, &liste_predateur);
        ajouter_animal(random() % SIZE_X, random() % SIZE_X, 240, &liste_proie);
    }
    afficher_ecosys(liste_predateur, liste_proie);

    printf("APrès la liberation :\n");
    liste_predateur = liberer_liste_animaux(liste_predateur);
    liste_proie = liberer_liste_animaux(liste_proie);

    printf(" le nombre des prédateurs : %d\n le nombre des prois : %d\n", compte_animal_rec(liste_predateur), compte_animal_rec(liste_proie));

    return 0;
}