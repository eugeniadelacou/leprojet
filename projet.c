#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        droite de la délimitation */
        image_size = atoi(size);
        printf("image_size = %d\n", image_size);
      }
      else if (!strcmp (size, "Image Resolution"))
      {
        size = strtok(NULL, "x");
        /* en faisant strtok avec NULL, on garde cette fois ce qu'il y a à
        droite de la délimitation, x */
        image_resolution = atoi(size);
        printf("image_resolution = %d\n", image_resolution);
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

    for (int j = 1; j < image_resolution; j++)
    /* boucle de lecture de chaque colonne : attention j commence à 1 */
    {
      pixel = strtok(NULL, "\t");
      pixel_val = atof(pixel);
      matrice[i][j] = pixel_val;
    }
  }
  return matrice;
}

double** lirefichier(char* filename)
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
      }


      if (!strcmp (ligneactuelle, "[Data Section]\r\n"))
      {
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

FILE* remplir_coordonnees_stl(double** matrice, FILE*fichier, double i, double j,
double k, double l, double m, double n, double o, double p, double q)
{

/* un fichier stl décompose le solide à imprimer en petits triangles définis par
trois sommets et un vecteur normal n défini comme le produit scalaire AB^AC
divisé par la norme de n */

  double normale_x = 0;
  double normale_y = 0;
  double normale_z = 0;
  double norme_AB = 0;
  double norme_AC = 0;
  double norme_n = 0;

  norme_AB = (sqrt(pow((l-i), 2) + pow((m-j), 2) + pow((n-k), 2)));
  norme_AC = (sqrt(pow((o-i), 2) + pow((p-j), 2) + pow((q-k), 2)));
  norme_n = norme_AB * norme_AC;

  normale_x = ((m-j)*(q-k)-(n-k)*(p-j))/norme_n;
  normale_y = ((n-k)*(o-i)-(l-i)*(q-k))/norme_n;
  normale_z = ((l-i)*(p-j)-(m-j)*(o-i))/norme_n;

  fprintf(fichier, "facet normal %f %f %f\n", normale_x, normale_y, normale_z);

  fputs("\touter loop\n", fichier);
  fprintf(fichier, "\t \tvertex %f %f %f\n", i, j, k);
  fprintf(fichier, "\t \tvertex %f %f %f\n", l, m, n);
  fprintf(fichier, "\t \tvertex %f %f %f\n", o, p, q);
  fputs("\tendloop\n", fichier);
  fputs("endfacet\n", fichier);
  return fichier;
}

int creer_fichier_stl(char* fichierstl, double** matrice)
{
  FILE* fichier = NULL;
  fichier = fopen(fichierstl, "w");
  double ecart = 0.5;

  if (fichier != NULL)
  {
    fputs("solid lasurface\n", fichier);

    for (int i = 0; i < image_resolution-1; i++)
    {
      /* on risque d'avoir des problèmes avec les dernières lignes et colonnes,
      donc pour éviter ça, on va s'arrêter à une case avant la fin de matrice */

      for (int j = 0; j < image_resolution-1; j++)
      {
        /* triangles du haut */
        remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
        j*espace_pixels, matrice[i][j], (i+1)*espace_pixels, j*espace_pixels,
        matrice[i+1][j], i*espace_pixels, (j+1)*espace_pixels, matrice[i][j+1]);
        remplir_coordonnees_stl(matrice, fichier, (i+1)*espace_pixels,
        (j+1)*espace_pixels, matrice[i+1][j+1], (i+1)*espace_pixels,
        j*espace_pixels, matrice[i+1][j], i*espace_pixels,(j+1)*espace_pixels,
        matrice[i][j+1]);

        /* triangles du bas */
        remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
        j*espace_pixels, matrice[0][0]-ecart, (i+1)*espace_pixels,
        j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels,
        (j+1)*espace_pixels, matrice[0][0]-ecart);
        remplir_coordonnees_stl(matrice, fichier, (i+1)*espace_pixels,
        (j+1)*espace_pixels, matrice[0][0]-ecart, (i+1)*espace_pixels,
        j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels,
        (j+1)*espace_pixels, matrice[0][0]-ecart);

        /* triangles intermédiaires */

        if(i==0)
        {
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels, j*espace_pixels,
          matrice[i][j], i*espace_pixels,
          (j+1)*espace_pixels, matrice[0][0]-ecart);
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          (j+1)*espace_pixels, matrice[i][j+1], i*espace_pixels, j*espace_pixels,
          matrice[i][j], i*espace_pixels,
          (j+1)*espace_pixels, matrice[0][0]-ecart);
        }

        if(i==image_resolution-1)
        {
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels, j*espace_pixels,
          matrice[i][j], i*espace_pixels,
          (j+1)*espace_pixels, matrice[0][0]-ecart);
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          (j+1)*espace_pixels, matrice[i][j+1], i*espace_pixels, j*espace_pixels,
          matrice[i][j], i*espace_pixels,
          (j+1)*espace_pixels, matrice[0][0]-ecart);
        }

        if(j==0)
        {
          remplir_coordonnees_stl(matrice, fichier, (i+1)*espace_pixels,
          j*espace_pixels, matrice[i+1][j], i*espace_pixels, j*espace_pixels,
          matrice[i][j], (i+1)*espace_pixels, j*espace_pixels,
          matrice[0][0]-ecart);
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels, j*espace_pixels,
          matrice[i][j], (i+1)*espace_pixels, j*espace_pixels,
          matrice[0][0]-ecart);
        }

        if(j==image_resolution-1)
        {
          remplir_coordonnees_stl(matrice, fichier, (i+1)*espace_pixels,
          j*espace_pixels, matrice[i+1][j], i*espace_pixels, j*espace_pixels,
          matrice[i][j], (i+1)*espace_pixels, j*espace_pixels,
          matrice[0][0]-ecart);
          remplir_coordonnees_stl(matrice, fichier, i*espace_pixels,
          j*espace_pixels, matrice[0][0]-ecart, i*espace_pixels, j*espace_pixels,
          matrice[i][j], (i+1)*espace_pixels, j*espace_pixels,
          matrice[0][0]-ecart);
        }
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
  char* filename = "./fichier.txt";
  printf("lecture du fichier texte...\n");
  double** matrice = lirefichier(filename);
  char* fichierstl = "./fichierstl.stl";
  printf("ecriture du fichier STL...\n");
  creer_fichier_stl(fichierstl, matrice);
  printf("le fichier STL a ete cree avec succes\n");
  return 0;
}
