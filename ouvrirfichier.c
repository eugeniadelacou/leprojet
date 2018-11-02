#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

float* creermatrice()
{
  return 0;
}

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

  fichier = fopen(filename, "r");

  if(fichier != NULL)
  {
    // on peut lire le fichier
    do

    /* boucle de lecture des caracteres ligne par ligne, jusqu'a 50 caracteres,
    dans fichier */

    {
      flag = fgets(ligneactuelle, 50, fichier);

      // fgets stocke ce qu'il lit dans ligneactuelle et renvoi un code pour
      // dire s'il a réussi à lire ou s'il a renconté une erreur (EOF par ex)

      if (!strcmp (ligneactuelle, "[Header Section]\n"))
      {
      printf("AYAYAYAYA\n");
      /*strcmp compare les caracteres un a un et retourne le nombre de
      caracteres differents. Ici on en veut 0 differents. Donc on veut
      "non strcmp", car 0=faux et tout le reste = vrai */

            do {
              if(fgets(ligneactuelle, 50, fichier)!= NULL)
              {
                strcpy(ligneactuelle_copie, ligneactuelle);
                size = strtok(ligneactuelle_copie, "=");
                
                size = strtok(NULL, "=");
                printf("%s", size);
              }
              else
              {
                  printf("erreur en lisant la partie Header\n");
                  break;
              }
            } while(strcmp (ligneactuelle, "\n"));
      }


       // on affiche size qui correspond a la taille

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

  return 0;
}
