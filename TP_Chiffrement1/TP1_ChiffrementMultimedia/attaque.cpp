#include <stdio.h>
#include "image_ppm.h"
/**
 * Calcul et retourne l'entropie de Shannon de Img
*/
double entropieShannon(OCTET *Img,int nTaille){
  double entropie=0;
  int occur[256];
  double prob,totale=0;
  //Initialise le tableau des occurences des niveaux de gris
  for(int i=0;i<256;i++){
    occur[i]=0;
  }
  //Calcul l'ocurence de chaque niveau de gris afin de connaitre sa probabilité
  for(int i=0;i<nTaille;i++){
    occur[Img[i]]++;
  }

  for(int i=0;i<256;i++){
    //Si un ndg n'apparait pas on le saute
    if(occur[i]==0)
      continue;
    prob=(double)(occur[i])/(double)nTaille;
    totale+=prob;
    entropie+=prob*log2(prob);
  }
  return -entropie;
}

//Chiffre ImgIn dans ImgOut de taille nH*nW avec une suite de nombres pseudo aléatoire
//géneré avec une clef.
void chiffrement(OCTET *ImgIn,OCTET *ImgOut,int nH,int nW, int clef){
  //Initialisation de la suite de nombre pseudo-aléatoire à l'aide de la clef
  srand (clef);
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
    {
      //Prochaine valeur pseudo aléatoire
      int randVal=rand()%256;
      //Operation XOR entre deux entiers
      ImgOut[i*nW+j]=randVal^ImgIn[i*nW+j];

    }
}


int main(int argc, char* argv[])
{
  //Initialisation des variables et lecture des parametres.
  char cNomImgLue[250],cNomImgEcrite[250];
  int nH, nW, nTaille, clef;
  if (argc != 3) 
    {
      printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
      exit (1) ;
    }
  sscanf (argv[1],"%s",cNomImgLue);
  sscanf (argv[2],"%s",cNomImgEcrite);


  //Initialisation des images.
  OCTET *ImgIn,*ImgOut;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);

  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  //Recherche de l'entropie minimale qui est associé à l'image en clair
  double entropieMin=9;
  double entropiecourante=0;
  int bestClef=-1;
  //On essaie de déchiffrer l'image avec toutes les clefs possible
  for(int i=0;i<100;i++){
    chiffrement(ImgIn,ImgOut,nH,nW,i);

    entropiecourante=entropieShannon(ImgOut,nTaille);

    if(entropiecourante<entropieMin){
      entropieMin=entropiecourante;
      bestClef=i;
    }

  }
  //bestClef represente la clef qui donne la meilleur entropie,
  // on déchiffre notre image finale avec
  chiffrement(ImgIn,ImgOut,nH,nW,bestClef);

  ecrire_image_pgm(cNomImgEcrite,ImgOut,nH,nW);
  //On affiche l'entropie de l'image d'origine 
  //et celle de sortie ainsi que la clef utilisée
  printf("In:%lf Out:%lf\n",entropieShannon(ImgIn,nTaille),entropieShannon(ImgOut,nTaille));
  printf("clef=%d\n",bestClef);

  //Désallocation des Images.
  free(ImgIn);
  free(ImgOut);
  return 1;
}