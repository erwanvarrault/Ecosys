#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"

float p_ch_dir = 0.01;
float p_reproduce_proie = 0.4;
float p_reproduce_predateur = 0.5;
int temps_repousse_herbe = -15;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie)
{
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = (rand() % 3) - 1;
  na->dir[1] = (rand() % 3) - 1;
  na->suivant = NULL;
  return na;
}

/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal)
{
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal)
{
  Animal *na = creer_animal(x, y, energie);
  assert(na && na->x < SIZE_X && na->y < SIZE_Y);
  *liste_animal = ajouter_en_tete_animal(*liste_animal, na);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal)
{
  Animal *tmp = *liste;
  Animal *Pprec = NULL;
  if ((*liste) && (animal))
  {
    if (tmp == animal)
    {
      *liste = (*liste)->suivant;
      free(tmp);
      return;
    }
    while ((tmp) && (tmp != animal))
    {
      Pprec = tmp;
      tmp = tmp->suivant;
    }
    if (!tmp)
      return;

    Pprec->suivant = tmp->suivant;
    free(tmp);
  }
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal *liberer_liste_animaux(Animal *liste)
{
  Animal *tmp = NULL;

  while (liste)
  {
    tmp = liste;
    liste = liste->suivant;
    free(tmp);
  }

  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la)
{
  if (!la)
    return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la)
{
  int cpt = 0;
  while (la)
  {
    ++cpt;
    la = la->suivant;
  }
  return cpt;
}

/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur)
{
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa = NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i)
  {
    for (j = 0; j < SIZE_Y; ++j)
    {
      ecosys[i][j] = ' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa)
  {
    ecosys[pa->x][pa->y] = '*';
    pa = pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa)
  {
    if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*'))
    { /* proies aussi present */
      ecosys[pa->x][pa->y] = '@';
    }
    else
    {
      ecosys[pa->x][pa->y] = 'O';
    }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j)
  {
    printf("-");
  }
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i)
  {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j)
    {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j)
  {
    printf("-");
  }
  printf("+\n");
  int nbproie = compte_animal_it(liste_proie);
  int nbpred = compte_animal_it(liste_predateur);

  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}

void clear_screen()
{
  printf("\x1b[2J\x1b[1;1H"); /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la)
{
  Animal *tmp = la;
  while (tmp)
  {
    if ((float)rand() / RAND_MAX < p_ch_dir)
    {
      tmp->dir[0] = (rand() % 3) - 1;
      tmp->dir[1] = (rand() % 3) - 1;
    }

    tmp->x = (tmp->x + tmp->dir[0] + SIZE_X) % SIZE_X;
    tmp->y = (tmp->y + tmp->dir[1] + SIZE_Y) % SIZE_Y;

    // ajustement des coordonnées si  tmp->x (ou tmp->y) sont négatifs ...
    /*if (tmp->x < 0)
    {
      tmp->x += SIZE_X;
    }
    if (tmp->y < 0)
    {
      tmp->y += SIZE_Y;
    }*/

    tmp = tmp->suivant;
  }
}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce)
{
  Animal *tmp = *liste_animal;
  while (tmp)
  {
    if ((float)rand() / RAND_MAX < p_reproduce)
    {
      ajouter_animal(tmp->x, tmp->y, (tmp->energie) / 2, liste_animal);
      tmp->energie /= 2;
    }
    tmp = tmp->suivant;
  }
}

/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y])
{
  bouger_animaux(*liste_proie);
  Animal *tmp = *liste_proie;
  while (tmp)
  {

    (tmp)->energie -= 1;
    if (monde[tmp->x][tmp->y] > 0) // gestion de l'herbe
    {
      tmp->energie += monde[tmp->x][tmp->y];
      monde[tmp->x][tmp->y] = temps_repousse_herbe; 
    }
    if ((tmp)->energie < 0)
    {
      enlever_animal(liste_proie, tmp);
    }
    (tmp) = (tmp)->suivant;
  }
  reproduce(liste_proie, p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y)
{
  Animal *tmp = l;
  while (tmp)
  {
    if ((tmp->x == x) && (tmp->y == y))
    {
      return tmp;
    }
    tmp = tmp->suivant;
  }

  return NULL;
}

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie)
{

  bouger_animaux(*liste_predateur);
  Animal *tmp = *liste_predateur;

  while (tmp)
  {
    Animal *Proie_trouvee = animal_en_XY(*liste_proie, tmp->x, tmp->y);
    if (Proie_trouvee) // si un prédateur a  trouvéé une proie
    {
      tmp->energie += Proie_trouvee->energie;
      Proie_trouvee->energie = 0;
    }
    (tmp)->energie -= 1;
    if ((tmp)->energie < 0)
    {
      enlever_animal(liste_predateur, tmp);
    }
    (tmp) = (tmp)->suivant;
  }
  reproduce(liste_predateur, p_reproduce_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y])
{
  for (int i = 0; i < SIZE_X; i++)
  {
    for (int j = 0; j < SIZE_Y; j++)
    {
      monde[i][j] += 1;
    }
  }
}
