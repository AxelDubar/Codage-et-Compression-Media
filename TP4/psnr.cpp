// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250],cNomImgLue2[250];
  int nH, nW, nTaille;

  if (argc != 3) 
     {
       printf("Usage: ImageSource.pgm ImageCompress.pgm\n"); 
       exit (1) ;
     }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgLue2) ;

    OCTET *ImgSrc,*ImgCmp;
   
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   
    nTaille = nH * nW;


    allocation_tableau(ImgSrc, OCTET, nTaille);
    allocation_tableau(ImgCmp, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgSrc, nH * nW);
    lire_image_pgm(cNomImgLue2, ImgCmp, nH * nW);
   
    double somme=0.0;

    for (int i=0; i < nTaille; i++){
      
        somme+=(ImgSrc[i]-ImgCmp[i])*(ImgSrc[i]-ImgCmp[i]);
    }
    printf("Somme=%lf\n",somme);
    double EQM=somme/(nH*nW);
    printf("EQM=%lf/%d=%lf \n",somme,nH*nW,EQM);
    double d=255.0*255.0;
    printf("%lf\n",d);
    double PSNR=10* log10(d/EQM);
    printf("PSNR=%f \n",PSNR);
}
