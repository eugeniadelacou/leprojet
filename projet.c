#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double** creer_matrice_2D(double** matrice, int x, int y)
{
  int i = 0;
  matrice = malloc(sizeof(double*)*x);
  /* on réserve x cases en mémoire dans lesquelles on stockera des double* */
  for(i = 0; i < x; i++)
  {
    matrice[i] = malloc(sizeof(double)*y);
    /* pour chacune des x cases on réserve y cases mémoire --> tableau 2D */
  }
return matrice;
}

int extract_size(FILE* fichier)
/* cette fonction récupère dans le fichier texte la taille nécessaire pour la
matrice */
{
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* size = 0;
  int size_val = 0;

  do
  /* on s'arrête à la fin de la ligne (Image Resolution) qui  est codée par
  \r\n */
  {
    if (fgets(ligneactuelle, 50, fichier)!= NULL)
    {
      /* on est dans Header. on vérifie que ligneactuelle n'est pas vide */
      strcpy(ligneactuelle_copie, ligneactuelle);
      /* on fait un clone de lignactuelle que l'on modifiera avec strtok */
      size = strtok(ligneactuelle_copie, "=");
      /* strtok garde ce qu'il y a à gauche de la délimitation, ici = */
      if (!strcmp (size, "Image Resolution"))
      /* strcmp compare des chaînes de caractères et retourne 0 si elles sont
      identiques. on veut donc "non strcmp" puisque "false" = 0 */
      {
        size = strtok(NULL, "x");
        /* en faisant strtok avec NULL, on garde cette fois ce qu'il y a à
        droite de la délimitation */
        size_val = atoi(size);
      }
    }
  }
  while(strcmp (ligneactuelle, "\r\n"));
  return size_val;
}

int lirefichier(char* filename)

/* et si la valeur de retour n'était pas un int mais un pointeur vers le tableau
 ? ou alors on fait une fonction séparée remplir_matrice qui renvoie un pointeur
 sur la matrice remplie ? en fait c'est pour qu'on ait les coordonnées de dispo
 pour créer le fichier stl*/

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

      if (!strcmp (ligneactuelle, "[Header Section]\r\n"))
      {
        //printf("%s\n", ligneactuelle);
        size_val = extract_size(fichier);
        printf("\nla valeur de la taille est %d !\n", size_val);
      }
    /*  else
      {
        printf("erreur en lisant la partie Header\n");
        break;
      }*/

      if (!strcmp (ligneactuelle, "[Data Section]\r\n"))
      {
        printf("Bienvenue dans le monde de la Data Section\n");
        printf("on essaye de créer la matrice\n");
        int step = 15*size_val;
        /* on va lire combien de caractères ? si chaque pixel fait 11
        caractères environ, on prend 15*size_val (taille de la matrice) pour
        être sûr */
        char ligneactuelle_data[step];
        char ligneactuelle_copie_data[step];
        matrice = creer_matrice_2D(matrice, size_val, size_val);
        fgets(ligneactuelle_data, step, fichier);

      //  for (int i = 0; i < size_val; i++)
        for (int i = 0; i < 2; i++)
        /* boucle de lecture de chaque ligne */
        {
          fgets(ligneactuelle_data, step, fichier);
          strcpy(ligneactuelle_copie_data, ligneactuelle_data);
          /* clone de lignactuelle_data que l'on modifiera avec strtok */
          pixel = strtok(ligneactuelle_copie_data, "\t");
          /* strtok garde ce qu'il y a à gauche de la délimitation (tab, \t).
          Le pixel de la 1ère colonne est donc un peu particulier : il ne doit
          pas entrer dans la boucle for j, qui parcourt les colonnes en gardant
          ce qu'il y a à droite de la délimitation à l'aide de NULL */
          pixel_val = atof(pixel);
          matrice[i][0] = pixel_val;
        //  printf("matrice [%d][0] = %f\n", i, pixel_val);
          /* cf avant : on donne une instruction particulière qui ne tient pas
          compte de j pour attribuer la 1ère colonne (j=0) */
          //printf("%f ", matrice[i][0]);
      //    for (int j = 1; j < size_val; j++)
          for (int j = 1; j < 2; j++)
          /* boucle de lecture de chaque colonne : attention j commence à 1 */
          {
            pixel = strtok(NULL, "\t");
          //  printf("%s\t", pixel);
          //  break;
            pixel_val = atof(pixel);

            matrice[i][j] = pixel_val;
          //  printf("%f\t", matrice[i][j]);
          }
        //  break;
      //    printf("\n");
          /* à chaque nouvelle itération de i, on passe une ligne pour bien
          afficher la matrice en 2D */
        }
      }
    }
    while (flag != NULL);

    for (int i = 0; i < 2; i++)
    {
      for (int j = 0; j < 2; j++)
      {
        printf("X = %d %d z %f\n", i, j, matrice[i][j]);
      }
    }
    fclose(fichier);
  }
  else
  {
  // message d'erreur
  printf("Impossible d'ouvrir le fichier test.txt/n");
  fclose(fichier);
  }
}

int creer_fichier_stl(char* fichierstl)
{
  FILE* fichier = NULL;
  fichier = fopen(fichierstl, "w");
  if (fichier != NULL)
  {
    fputs("solid lasurface\n", fichier);
    fputs("facet normal 0 1 0\n", fichier);
    fputs("\touter loop\n", fichier);
    fputs("\t \tvertex \n", fichier);
    fclose(fichier);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char* filename = "/home/mint/leprojet/fichier.txt";
  lirefichier(filename);
  char* fichierstl = "/home/mint/leprojet/fichierstl.stl";
  creer_fichier_stl(fichierstl);

  return 0;
}
