// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"

void composante(OCTET *ImgInY, OCTET *ImgInCr, OCTET *ImgInCb,OCTET *ImgOut,int nTaille3){
  int nH, nW, nR, nG, nB; 
  double Y,Cr,Cb;
  for(int i=0;i<nTaille3;i+=3){
    Y=ImgInY[i/3]+5;
    Cr=ImgInCr[i/3]+5;
    Cb=ImgInCb[i/3]+5;
    

    ImgOut[i]=Y+1.402*(Cr-128);
    ImgOut[i+1]=Y-0.344136*(Cb-128)-0.714136*(Cr-128);
    ImgOut[i+2]=Y+1.772*(Cb-128);

    
  }

}

int main(int argc, char* argv[])
{
  char cNomImgEcrite[250], cNomImgLueY[250], cNomImgLueCr[250], cNomImgLueCb[250];
  int nH, nW, nTaille;

  if (argc != 5) 
    {
      printf("Usage: ImageInY.pgm ImageInCr.pgm ImageInCb.pgm ImgOut.ppm\n"); 
      exit(1) ;
    }
   
  sscanf (argv[1],"%s",cNomImgLueY);
  sscanf (argv[2],"%s",cNomImgLueCr);
  sscanf (argv[3],"%s",cNomImgLueCb);
  sscanf (argv[4],"%s",cNomImgEcrite);

  OCTET *ImgInY, *ImgInCr, *ImgInCb, *ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLueY, &nH, &nW);
  nTaille = nH * nW;


  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgInY, OCTET, nTaille);
  allocation_tableau(ImgInCr, OCTET, nTaille);
  allocation_tableau(ImgInCb, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille3);
  lire_image_pgm(cNomImgLueY, ImgInY, nH * nW);
  lire_image_pgm(cNomImgLueCr, ImgInCr, nH * nW);
  lire_image_pgm(cNomImgLueCb, ImgInCb, nH * nW);

  composante(ImgInY,ImgInCr,ImgInCb, ImgOut,nTaille3);

  ecrire_image_ppm(cNomImgEcrite,ImgOut,nH,nW);

  return 1;
}
