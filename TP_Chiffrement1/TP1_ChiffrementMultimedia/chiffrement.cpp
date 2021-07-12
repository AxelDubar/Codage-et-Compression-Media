#include <stdio.h>
#include "image_ppm.h"

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
      printf("Usage: ImageIn.pgm ImageOut.pgm clef\n"); 
      exit (1) ;
    }
  sscanf (argv[1],"%s",cNomImgLue);
  sscanf (argv[2],"%s",cNomImgEcrite);
  sscanf (argv[3],"%d",&clef);

  //Initialisation des images.
  OCTET *ImgIn,*ImgOut;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  //Chiffrement et écriture de l'image chiffrée.
  chiffrement(ImgIn,ImgOut,nH,nW,clef);
  ecrire_image_pgm(cNomImgEcrite,ImgOut,nH,nW);

  //Désallocation des Images.
  free(ImgIn);
  free(ImgOut);
  return 1;
}