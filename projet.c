#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int image_resolution = 0;
double espace_pixels = 0;

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

int extract_data(FILE* fichier)
/* cette fonction récupère dans le fichier texte la taille nécessaire pour la
matrice, càd le nombre de pixels (image_resolution), et la taille de l'image
(image_size) */
{
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* size = 0;
  int image_size = 0;

  do
  /* on s'arrête à la fin de la dernière ligne de Data Section, qui  est codée
  par \r\n */
  {
    if (fgets(ligneactuelle, 50, fichier)!= NULL)
    {
      /* on est dans Header. on vérifie que ligneactuelle n'est pas vide */
      strcpy(ligneactuelle_copie, ligneactuelle);
      /* on fait un clone de lignactuelle que l'on modifiera avec strtok */
      size = strtok(ligneactuelle_copie, "=");
      /* strtok garde ce qu'il y a à gauche de la délimitation, ici = */
      if (!strcmp (size, "Image Size"))
      {
        /* strcmp compare des chaînes de caractères et retourne 0 si elles sont
        identiques. on veut donc "non strcmp" puisque "false" = 0 */
        size = strtok(NULL, " ");
        /* en faisant strtok avec NULL, on garde cette fois ce qu'il y a à
        droite de la délimitation, x */
        image_size = atoi(size);
        printf("image_size = %d\n", image_size);
      }
      else if (!strcmp (size, "Image Resolution"))
      {
        size = strtok(NULL, "x");
        /* en faisant strtok avec NULL, on garde cette fois ce qu'il y a à
        droite de la délimitation, x */
        image_resolution = atoi(size);
      }
    }
  }
  while(strcmp (ligneactuelle, "\r\n"));
  espace_pixels = (double) image_size/image_resolution;
  printf("espace_pixels = %f\n", espace_pixels);
  return 0;
}

double** remplir_matrice(FILE* fichier, double** matrice)
{
  int step = 15*image_resolution;
  /* on va lire combien de caractères ? si chaque pixel fait 11 caractères
  environ, on prend 15*image_resolution (taille de la matrice) pour être sûr */
  char ligneactuelle_data[step];
  char ligneactuelle_copie_data[step];
  char* pixel = 0;
  double pixel_val = 0;

  fgets(ligneactuelle_data, step, fichier);
  //  for (int i = 0; i < image_resolution; i++)
  for (int i = 0; i < image_resolution; i++)
  /* boucle de lecture de chaque ligne de pixels */
  {
    fgets(ligneactuelle_data, step, fichier);
    strcpy(ligneactuelle_copie_data, ligneactuelle_data);
    /* clone de lignactuelle_data que l'on modifiera avec strtok */
    pixel = strtok(ligneactuelle_copie_data, "\t");
    /* strtok garde ce qu'il y a à gauche de la délimitation (tab, \t).
    Le pixel de la 1ère colonne est donc un peu particulier : il ne doit pas
    entrer dans la boucle for j, qui parcourt les colonnes en gardant ce qu'il y
    a à droite de la délimitation à l'aide de NULL */
    pixel_val = atof(pixel);
    matrice[i][0] = pixel_val;
  //  printf("matrice [%d][0] = %f\n", i, pixel_val);
    for (int j = 1; j < image_resolution; j++)
    /* boucle de lecture de chaque colonne : attention j commence à 1 */
//    for (int j = 1; j < image_resolution; j++)
    {
      pixel = strtok(NULL, "\t");
      pixel_val = atof(pixel);
      matrice[i][j] = pixel_val;
    }
//    printf("\n");
    /* à chaque nouvelle itération de i, on passe une ligne pour bien
    afficher la matrice en 2D */
  }
  return matrice;
}

double** lirefichier(char* filename)

/* et si la valeur de retour n'était pas un int mais un pointeur vers le tableau
 ? ou alors on fait une fonction séparée remplir_matrice qui renvoie un pointeur
 sur la matrice remplie ? Lucas dit que c'est une bonne idée de commencer par ça.
 C'est pour qu'on ait les coordonnées de dispo pour créer le fichier stl. */

{
  FILE* fichier = NULL;
  char ligneactuelle[150];
  char ligneactuelle_copie[150];
  char* flag = 0;

  char* size = 0;
  double** matrice;
  char* pixel =0;
  double pixel_val=0;

  fichier = fopen(filename, "r");

  if(fichier != NULL)
  {
    // le fichier n'est pas vide --> on peut le lire
    flag = fgets(ligneactuelle, 50, fichier);
    /* fgets stocke ce qu'il lit dans ligneactuelle et renvoie un code (flag)
    pour dire s'il a réussi à lire ou s'il a renconté une erreur (EOF par ex) */
    do
    {
      // on continue tant que fgets n'a pas retourné EOF (flag = NULL)

      if (!strcmp (ligneactuelle, "[Header Section]\r\n"))
      {
        extract_data(fichier);
        printf("\nla valeur de la taille est %d !\n", image_resolution);
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

        matrice = creer_matrice_2D(matrice, image_resolution, image_resolution);
        matrice = remplir_matrice(fichier, matrice);
      }
      flag = fgets(ligneactuelle, 50, fichier);

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
  return matrice;
}

/**/

int creer_fichier_stl(char* fichierstl, double** matrice)
{
  FILE* fichier = NULL;
  fichier = fopen(fichierstl, "w");

  if (fichier != NULL)
  {
    fputs("solid lasurface\n", fichier);
    for (int i = 0; i < image_resolution-1; i++)
    /* on risque d'avoir des problèmes avec les dernières lignes et colonnes,
    donc pour éviter ça, on va s'arrêter à une case avant la fin de matrice */
    {
      for (int j = 0; j < image_resolution-1; j++)
      {
        fputs("facet normal 0 1 0\n", fichier);

        /* en réalité c'est plus compliqué, il faut les coordonnées de la
        normale, mais on verra ça plus tard */

        fputs("\touter loop\n", fichier);
        fprintf(fichier, "\t \tvertex %f %f %f\n", i*espace_pixels, j*espace_pixels, matrice[i][j]);
        fprintf(fichier, "\t \tvertex %f %f %f\n", (i+1)*espace_pixels, j*espace_pixels, matrice[i+1][j]);
        fprintf(fichier, "\t \tvertex %f %f %f\n", i*espace_pixels, (j+1)*espace_pixels, matrice[i][j+1]);
        //printf("X = %d %d z %f\n", i, j, matrice[i][j]);
        fputs("\tendloop\n", fichier);
        fputs("endfacet\n", fichier);
        fputs("facet normal 0 1 0\n", fichier);
        fputs("\touter loop\n", fichier);
        fprintf(fichier, "\t \tvertex %f %f %f\n", (i+1)*espace_pixels, j*espace_pixels, matrice[i+1][j]);
        fprintf(fichier, "\t \tvertex %f %f %f\n", i*espace_pixels, (j+1)*espace_pixels, matrice[i][j+1]);
        fprintf(fichier, "\t \tvertex %f %f %f\n", (i+1)*espace_pixels, (j+1)*espace_pixels, matrice[i+1][j+1]);
        //printf("X = %d %d z %f\n", i, j, matrice[i][j]);
        fputs("\tendloop\n", fichier);
        fputs("endfacet\n", fichier);

/* on pourrait faire une fonction qui code juste pour ces histoires de fputs ?*/

      }
    }
    fputs("endsolid lasurface\n", fichier);
    fclose(fichier);
  }
  else
  {
    printf("probleme a l'ecriture du fichier stl\n");
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char* filename = "/home/mint/leprojet/fichier.txt";
  double** matrice = lirefichier(filename);
  char* fichierstl = "/home/mint/leprojet/fichierstl.stl";
  creer_fichier_stl(fichierstl, matrice);
  return 0;
}
