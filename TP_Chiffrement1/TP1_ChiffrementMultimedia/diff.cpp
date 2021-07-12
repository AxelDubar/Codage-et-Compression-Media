// histo_gris.cpp : Affiche l'histogramme d'une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"



int main(int argc, char* argv[])
{
  char cNomImgLue[250],cNomImgLue2[250],cNomImgEcrite[250];
  int nH, nW, nTaille;
  if (argc != 4) 
    {
      printf("Usage: ImageIn.pgm ImgIn2.pgm ImageOut.pgm\n"); 
      exit (1) ;
    }
  sscanf (argv[1],"%s",cNomImgLue);
  sscanf (argv[2],"%s",cNomImgLue2);
  sscanf (argv[3],"%s",cNomImgEcrite);


  OCTET *ImgIn,*ImgIn2,*ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgIn2, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);

  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);

  for(int i=0;i<nTaille;i++){
    ImgOut[i]=ImgIn[i]-ImgIn2[i];
    printf("%d\n",ImgOut[i]);
  }
  ecrire_image_pgm(cNomImgEcrite,ImgOut,nH,nW);
  free(ImgIn);
  free(ImgOut);
  return 1;
}