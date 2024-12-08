#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 200000 // 1s
#define MAX_ITERATIONS 200

int main(void)
{

  /* A completer. Part 2:
   * exercice 4, questions 2 et 4
   * exercice 6, question 2
   * exercice 7, question 3
   * exercice 8, question 1
   */
  srand(time(NULL));

  
  /*Animal *na = (Animal *)malloc(sizeof(Animal));
  na->x = 10;
  na->y = 25;
  na->energie = 7;
  na->dir[0] = -9;
  na->dir[1] = 15;
  na->suivant = NULL;
  afficher_ecosys(na, NULL);
  printf("position avant de bouger : x : %d, y : %d ,e : %.1f, dir :[%d,%d]\n", na->x, na->y, na->energie, na->dir[0], na->dir[1]);
  bouger_animaux(na);
  afficher_ecosys(na, NULL);
  printf("position aprés  bouger : x : %d, y : %d ,e : %.1f, dir :[%d,%d]\n", na->x, na->y, na->energie, na->dir[0], na->dir[1]);

  // teste de la fonction de reproduction :
  reproduce(&na, 1); // creation d'un nouveau animal au debut de la liste;
  reproduce(&na, 1); // ajout d'un autre animal au debut

  // affichage du resultat
  afficher_ecosys(na, NULL); // NB :  les nouveaux animaux crées ne seront pas afficher puiqu'ils ont tous la meme position
  // verification d'energie de chaque animal :(je dois avoir 4 animaux de 1.75 d'energie chacun )
  int i = 1;
  Animal *tmp = na;
  while (tmp)
  {
    printf("%d ieme animal energie : %.2f      exit
\n", i++, tmp->energie);
    tmp = tmp->suivant;
  }
  na = liberer_liste_animaux(na);
  
*/
  printf("---------------- ------------------------ SIMULATION ----------------------------------------------\n");
  printf("/n");

  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  // Gestione de l'herbe:
  int monde[SIZE_X][SIZE_Y];
  for (int i = 0; i < SIZE_X; i++)
  {
    for (int j = 0; j < SIZE_Y; j++)
    {
      monde[i][j] = 0;
    }
  }

  /*for (int i = 0; i < NB_PROIES; i++)
  {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 20, &liste_proie); // si l'energie est suffisament grande la boucle ne s'arretra pas jusqu'à l'atteint de MAX_ITERATIONS
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 140, &liste_predateur);
  }*/

  for (int i = 0; i < NB_PROIES; i++)
  {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 20, &liste_proie); // si l'energie est suffisament grande la boucle ne s'arretra pas jusqu'à l'atteint de MAX_ITERATIONS
  }

  for (int i = 0; i < NB_PREDATEURS; i++)
  {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 140, &liste_predateur);
  }

  int iterations = 0;
  FILE *f = fopen("simulation.txt","w");
    if (f == NULL){
      printf("erreur d'ouverture du fichier");
    
    }
  while (liste_proie && liste_predateur && iterations < MAX_ITERATIONS)
  { 
    
    fprintf(f,"%d %d %d\n",iterations,compte_animal_it(liste_proie),compte_animal_it(liste_predateur));
    
    clear_screen();
    afficher_ecosys(liste_proie, liste_predateur);

    rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_predateur, &liste_proie);
    rafraichir_monde(monde);

    usleep(T_WAIT);

    iterations++;
  }
  clear_screen();
  afficher_ecosys(liste_proie, liste_predateur);

  fclose(f);

  FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors de l'ouverture de gnuplot");
        return 1;
    }

    // Envoyer des commandes à gnuplot
    fprintf(gnuplot, "set title 'Simulation ecosystème'\n");
    fprintf(gnuplot, "plot 'simulation.txt' using 1:2 with lines title 'proies'\n");
    fprintf(gnuplot, "replot 'simulation.txt' using 1:3 with lines title 'predateurs'\n");
    
    // Fermer le pipe
    pclose(gnuplot);

  // Graphe d'evolution à fournir plus tard

  return 0;
}
