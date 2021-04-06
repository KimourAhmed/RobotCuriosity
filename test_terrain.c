#include "terrain.h"
#include <stdio.h>

int main(int argc, char ** argv) {
  Terrain t;
  int x, y;
  erreur_terrain erreur;
  char nom_fich[DIM_MAX];
  if(argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  lire_terrain(argv[1], &t, &x, &y);
  erreur = lire_terrain(argv[1], &t, &x, &y);
  while (erreur == ERREUR_FICHIER){
    printf("Entrer un nom de fichier valide: \n");
    scanf("%s", nom_fich );
    erreur = lire_terrain(nom_fich, &t, &x, &y);
  }
  /*switch(erreur){
  case E_LECTURE_L:
    printf("erreur de lecture de la largeur\n");
    return 1;
  case E_LECTURE_H:
    printf("erreur de la lecture de l'hauteur\n");
    return 1;
  case E_CARACTERE_INCONNU:
    printf("erreur caractere inconnu\n");
    return 1;
  case  E_POSITION_R:
    printf("erreur position robot\n");
    return 1;
  case E_L_INCORRECTE:
    printf("erreur largeur inorrecte\n");
    return 1;
  case E_H_INCORRECTE:
    printf("erreur hauteur inorrecte\n");
    return 1;
  case E_LIGNE_TL:
    printf("erreur ligne trop longue\n");
    return 1;
  case  E_LIGNE_TC:
    printf("erreur ligne trop courte\n");
    return 1;
  case  E_LIGNE:
    printf("erreur lecture ligne\n");
    return 1;
  case OK:
    break;
  case ERREUR_FICHIER:
    break;
  }*/
  
  
  
  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
}
