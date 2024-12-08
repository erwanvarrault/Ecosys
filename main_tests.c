#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ecosys.h"

int main(void)
{
  int i;
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  int energie = 10;

  srand(time(NULL));

  Animal *a1 = creer_animal(0, 10, energie);
  Animal *a2 = creer_animal(15, 35, energie);
  Animal *a3 = creer_animal(2, 42, energie);
  Animal *a4 = creer_animal(18, 13, energie);

  a1->suivant = a2;
  a2->suivant = a3;
  a3->suivant = a4;

  liste_proie = a1;

  Animal *a5 = creer_animal(2, 5, energie);
  Animal *a6 = creer_animal(15, 35, energie);
  Animal *a7 = creer_animal(9, 22, energie);
  Animal *a8 = creer_animal(17, 3, energie);

  a5->suivant = a6;
  a6->suivant = a7;
  a7->suivant = a8;

  liste_predateur = a5;

  ajouter_animal(2, 2, 552, &liste_predateur);

  ajouter_animal(3, 4, 552, &liste_predateur);

  afficher_ecosys(liste_proie, liste_predateur);

  enlever_animal(&liste_proie, a1);     // enlever le premièr element de la liste des proies
  enlever_animal(&liste_proie, a4);     // enlever le dernier element de la liste des proies
  enlever_animal(&liste_predateur, a7); // enlever un element de liste des prédateurs
  printf("Après les modifcation :\n");
  afficher_ecosys(liste_proie, liste_predateur);

  liste_predateur = liberer_liste_animaux(liste_predateur);
  liste_proie = liberer_liste_animaux(liste_proie);
  printf("Après la liberation:\n");
  printf(" le nombre des prédateurs : %d\n le nombre des prois : %d\n", compte_animal_rec(liste_predateur), compte_animal_rec(liste_proie));

  return 0;
}
