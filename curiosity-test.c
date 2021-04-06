#include"terrain.h"
#include"robot.h"
#include"programme.h"
#include"interprete.h"
#include"environnement.h"
#include"type_pile.h"
#include<stdio.h>
#include<string.h>
#include <stdlib.h>


void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {
  case OK: break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caract√®re incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf("Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for(i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch(e.type_err) {
  case OK_PROGRAMME: break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non ferm√©\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc exc√©dentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}


int main(int argc, char **argv){
	Environnement envt;
	Programme prog;
 	erreur_terrain errt;
 	erreur_programme errp;
 	etat_inter etat;
 	resultat_inter res;
	if (argc != 2){
		printf("Usage : %s <fichierTest>\n",argv[0]);
		return 1;
	}
	FILE *fich_test = fopen(argv[1],"r");
	if(fich_test == NULL){
		return 2;
	}
 
 /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */		
	char terrain_test[DIM_MAX]; 
  fscanf(fich_test,"%s",terrain_test);
  fscanf(fich_test,"\n");
  errt = initialise_environnement(&envt,terrain_test);
	gestion_erreur_terrain(errt);
  
   /* Lecture du programme */
  char routine[DIM_MAX];  
	fscanf(fich_test,"%s",routine);
	fscanf(fich_test,"\n");
	errp = lire_programme(&prog,routine);
	gestion_erreur_programme(errp);	
	
  /* Lecture du nombre de pas maximum*/
  int nbpasmax;
  fscanf(fich_test,"%d",&nbpasmax);
	fscanf(fich_test,"\n");
	
  /* Lecture de l'evenement attendu ‡ la fin*/
  char e;
  fscanf(fich_test,"%c",&e);
	
  /* Si e=N ou e=F on lit la position attendue du robot et son orientation*/
  int x,y;
  char o;
  if(e == 'N' || e=='n' || e=='F' || e == 'f'){
    fscanf(fich_test,"\n");
		fscanf(fich_test,"%d",&x);
		fscanf(fich_test,"%d",&y);
		fscanf(fich_test,"\n");
		fscanf(fich_test,"%c",&o);
	}		
  
  /* Initialisation de l'etat */
  init_etat(&etat);
	int i=0;
  do {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */ 
    afficher_envt(&envt);
	i++;
  } while(res == OK_ROBOT && i<=nbpasmax); 
  
   /* Affichage du resultat */
  switch(e){
	   case'F':case'f': //programme terminÈ et le robot sur position normale
		  if (res == ARRET_ROBOT){
			  printf(" Okay so (Robot sur une case libre, programme termine) just like you said haha;-) \n");
		  }
		  else{
			  printf("Unfortunately the expected event did not happen :/ !! Sorry try     something else!!(peut etre il reste d'autres instructions a executer ou bien soit le robot est sorti ou a rencontre un obstacle)\n");
		  }
		  printf("la position attendue du robot est: \nAbscisse: x= %d\nOrdonnee: y = %d\nOrientation: o = %c\n",x,y,o);
		  break;
     case 'n': case 'N': //le robot est sur une position normale ‡ l'intÈrieur du terrain mais le programme n'est pas termine
		  if (res == OK_ROBOT){
			  printf("Robot sur une case libre, programme non termine (ne devrait pas arriver) but it's just like you said ;-) \n");
		  }
		  else{
			  printf("Unfortunately the expected event did not happen :/ !! Sorry try     something else(peut etre il ne reste plus dinstructions a executer ou bien soit le robot est sorti ou a rencontre un obstacle  \n");
		  }
		  printf("la position attendue du robot est: \nAbscisse: x= %d\nOrdonnee: y = %d\nOrientation: o = %c\n",x,y,o);
		  break;
	   case 's':case'S': //le robot est sorti du terrain
		  if (res == SORTIE_ROBOT){
			  printf("Le robot est sorti :-)\n");
		  }
		  else{
			  printf("Unfortunately the expected event did not happen :/ !! Sorry try     something else(Le robot n'est pas sorti comme vous avez prevenu)\n");
		  }
		  break;
	  case 'o':case'O': //le robot a rencontrÈ un obstacle
		  if(res == CRASH_ROBOT){
			  printf("Le robot s'est ecras√© sur un rocher X-(\n");
		  }
		  else{
			  printf("Unfortunately the expected event did not happen :/ !! Sorry try something else:( (Le robot n'a pas rencontre un obstacle comme vous avez prevenu)\n");
		  }
		  break;
	  case 'p':case'P': //le robot est tombÈ dans l'eau
			if(res == PLOUF_ROBOT){
				 printf("Le robot est tombe dans l'eau :-(\n");
			                      }
			else{
				printf("Unfortunately the expected event did not happen :/ !! Sorry try something else:( (Le robot n'est pas tombe dans l'eau comme vous avez prevenu)\n");
			}
			break;
	  default : //Si l'utilisateur tape autre chose
       printf("Sorry I don't recognize what you said:-(   \n");break;
	  }
  
return 0;
}
