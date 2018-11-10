#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double** creer_matrice_2D(double** matrice, int x, int y)
{
  //double** matrice[size_val][size_val];
  // Corr lucas : double matrice[x][y];
  //              return matrice;

  int i = 0;
//  int j = 0;
  printf("on essaye de créer la matrice\n");
  matrice = malloc(sizeof(double*)*x);
  for(i = 0; i < x; i++)
  {
    matrice[i] = malloc(sizeof(double)*y);
/*    for(j=0; j < y; j++)
    {
      printf("%lf ", matrice[i][j]);
    }
    printf("\n "); */
  }
return matrice;
}

  // version lucas
  /*
  matrice = malloc(sizeof(double*)*x);
  int i = 0;
  for(i = 0; i < x; i++)
  {
    matrice[i] = malloc(sizeof(double) * y);
  }
  return matrice;

}*/

/* FILE* fopen(const char* nomdufichier, const char* modeouverture);
   int fclose(FILE* pointeursurfichier);
   prototypes des fonctions fopen et fclose */

int lirefichier(char* filename)
{
  FILE* fichier = NULL;
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* flag = 0;
  char* flag2 = 0;
  char* size = 0;
  int size_val = 0;
  double** matrice;
  char* pixel =0;
  double pixel_val=0;

  fichier = fopen(filename, "r");

  if(fichier != NULL)
  {
    // on peut lire le fichier
    do

    /* boucle de lecture des caracteres ligne par ligne, jusqu'a 50 caracteres,
    dans fichier */

    {
      flag = fgets(ligneactuelle, 50, fichier);

      /* fgets stocke ce qu'il lit dans ligneactuelle et renvoie un code (flag)
      pour dire s'il a réussi à lire ou s'il a renconté une erreur (EOF par ex)
      */

      if (!strcmp (ligneactuelle, "[Header Section]\n"))
      {
      /* strcmp compare les caracteres un a un et retourne le nombre de
      caracteres differents. Ici on en veut 0 differents. Donc on veut
      "non strcmp", car 0=faux et tout le reste = vrai */

            do
            {
              if (fgets(ligneactuelle, 50, fichier)!= NULL)

              // on est dans Header. on regarde si ligneactuelle est vide

              {
                strcpy(ligneactuelle_copie, ligneactuelle);
                /* on fait un clone de lignactuelle que l'on modifiera avec
                strtok */

                size = strtok(ligneactuelle_copie, "=");
                if (!strcmp (size, "ImageResolution"))
                {
                  size = strtok(NULL, "=x");
//                  printf("%s\n", size);
                  size_val = atoi(size);
                  printf("\nla valeur de la taille est %d !\n", size_val);
                  double** matrice;
                  matrice = creer_matrice_2D(matrice, size_val, size_val);
                  for (int i = 0; i < size_val; i++)
                  {
                    for (int j = 0; j < size_val; j++)
                    {
                      printf("%f ", matrice[i][j]);
                    }
                    printf("\n");
                  }
                }
              }
              else
              {
                  printf("erreur en lisant la partie Header\n");
                  break;
              }
            } while(strcmp (ligneactuelle, "\n"));

            // on s'arrete de lire quand on arrive a un saut de ligne

      }
      if (!strcmp (ligneactuelle, "[Data Section]\n"))
      {
        printf("Bienvenue dans le monde de la Data Section\n");

        matrice = creer_matrice_2D(matrice, size_val, size_val);

        for (int i = 0; i < size_val;i++)
        {
          fgets(ligneactuelle, 50, fichier);
          strcpy(ligneactuelle_copie, ligneactuelle);
          //printf("%s\n", ligneactuelle);

          pixel = strtok(ligneactuelle_copie, " ");
          pixel_val = atof(pixel);
          //printf("pixel_val vaut %f !\n", pixel_val);
          matrice[i][0] = pixel_val;
          printf("%f ", matrice[i][0]);
          for (int j = 1; j < size_val; j++)
          {
            pixel = strtok(NULL, " ");
            pixel_val = atof(pixel);
            matrice[i][j] = pixel_val;
            printf("%f ", matrice[i][j]);
            //printf("pixel_val vaut %f !\n", pixel_val);
          }
          printf("\n");
        }

      }
    }

      while (flag != NULL); // on continue tant que fgets n'a pas retourné EOF
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
  //int size_val;
  //size_val = 560;
  //creer_matrice_2D(size_val, size_val);

  return 0;
}
