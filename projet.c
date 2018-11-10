#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double** creer_matrice_2D(double** matrice, int x, int y)
{
  int i = 0;
  matrice = malloc(sizeof(double*)*x);
  for(i = 0; i < x; i++)
  {
    matrice[i] = malloc(sizeof(double)*y);
  }
return matrice;
}

int extract_size(char* filename)
{
  FILE* fichier = NULL;
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* size = 0;
  int size_val = 0;

  fichier = fopen(filename, "r");

  do
  {
    if (fgets(ligneactuelle, 50, fichier)!= NULL)
    // on est dans Header. on vérifie que ligneactuelle n'est pas vide
    {
      strcpy(ligneactuelle_copie, ligneactuelle);
      /* on fait un clone de lignactuelle que l'on modifiera avec strtok */
      size = strtok(ligneactuelle_copie, "=");
      if (!strcmp (size, "ImageResolution"))
      {
        size = strtok(NULL, "=x");
        size_val = atoi(size);

      }
    }
  }
  while(strcmp (ligneactuelle, "\n"));
  return size_val;
}

int lirefichier(char* filename)
{
  FILE* fichier = NULL;
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* flag = 0;
  char* size = 0;
  int size_val = 0;
  double** matrice;
  char* pixel =0;
  double pixel_val=0;

  fichier = fopen(filename, "r");

  if(fichier != NULL)
  {
    // le fichier n'est pas vide --> on peut le lire
    do
    // on continue tant que fgets n'a pas retourné EOF (flag = NULL)
    {
      flag = fgets(ligneactuelle, 50, fichier);
      /* fgets stocke ce qu'il lit dans ligneactuelle et renvoie un code (flag)
      pour dire s'il a réussi à lire ou s'il a renconté une erreur (EOF par ex) */

      if (!strcmp (ligneactuelle, "[Header Section]\n"))
      {
        size_val = extract_size(filename);
        printf("\nla valeur de la taille est %d !\n", size_val);
      }
    /*  else
      {
        printf("erreur en lisant la partie Header\n");
        break;
      }*/

      if (!strcmp (ligneactuelle, "[Data Section]\n"))
      {
        printf("Bienvenue dans le monde de la Data Section\n");
        printf("on essaye de créer la matrice\n");
        matrice = creer_matrice_2D(matrice, size_val, size_val);
        for (int i = 0; i < size_val; i++)
        /* boucle de lecture de chaque ligne */
        {
          fgets(ligneactuelle, 50, fichier);
          strcpy(ligneactuelle_copie, ligneactuelle);
          /* on fait un clone de lignactuelle que l'on modifiera avec strtok */

          pixel = strtok(ligneactuelle_copie, " ");
          /* strtok garde ce qu'il y a à gauche de la délimitation. Le pixel
          de la 1ère colonne est donc un peu particulier : il ne doit pas entrer
          dans la boucle for j, qui parcourt les colonnes en gardant ce qu'il y
          a à droite de la délimitation à l'aide de NULL */
          pixel_val = atof(pixel);
          matrice[i][0] = pixel_val;
          /* cf avant : on donne une instruction particulière qui ne tient pas
          compte de j pour attribuer la 1ère colonne (j=0) */
          printf("%f ", matrice[i][0]);
          for (int j = 1; j < size_val; j++)
          /* boucle de lecture de chaque colonne : attention j commence à 1 */
          {
            pixel = strtok(NULL, " ");
            pixel_val = atof(pixel);
            matrice[i][j] = pixel_val;
            printf("%f ", matrice[i][j]);
          }
          printf("\n");
          /* à chaque nouvelle itération de i, on passe une ligne pour bien
          afficher la matrice en 2D */
        }
      }
    }
    while (flag != NULL);
    fclose(fichier);
  }
  else
  {
  // message d'erreur
  printf("Impossible d'ouvrir le fichier test.txt/n");
  fclose(fichier);
  }
}


int main(int argc, char *argv[])
{
  char* filename = "/home/mint/leprojet/test.txt";
  lirefichier(filename);

  return 0;
}
