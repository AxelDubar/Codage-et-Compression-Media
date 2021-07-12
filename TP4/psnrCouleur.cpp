// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250],cNomImgLue2[250];
  int nH, nW, nTaille, nR, nG, nB, nR2, nG2, nB2;

  if (argc != 3) 
     {
       printf("Usage: ImageSource.ppm ImageCompress.ppm\n"); 
       exit (1) ;
     }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgLue2) ;

    OCTET *ImgSrc,*ImgCmp;
   
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   
    nTaille = nH * nW;


    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgSrc, OCTET, nTaille3);
    allocation_tableau(ImgCmp, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgSrc, nH * nW);
    lire_image_ppm(cNomImgLue2, ImgCmp, nH * nW);
   
    double sommeR=0.0;
    double sommeG=0.0;
    double sommeB=0.0;
    double somme=0.0;

    for (int i=0; i < nTaille3; i+=3){
        nR = ImgSrc[i];
        nG = ImgSrc[i+1];
        nB = ImgSrc[i+2];
      
        nR2 = ImgCmp[i];
        nG2 = ImgCmp[i+1];
        nB2 = ImgCmp[i+2];
        printf("%d %d %d\n",(nR-nR2)*(nR-nR2),(nG-nG2)*(nG-nG2),(nB-nB2)*(nB-nB2));
        sommeR+=(nR-nR2)*(nR-nR2);
        sommeG+=(nG-nG2)*(nG-nG2);
        sommeB+=(nB-nB2)*(nB-nB2);
    }
    somme=sommeR+sommeG+sommeB;
    printf("Somme=%lf\n",somme);
    double EQM=somme/(nH*nW*3.0);
    printf("EQM=%lf/%lf=%lf \n",somme,nH*nW*3.0,EQM);
    double d=255.0*255.0;
    printf("%lf\n",d);
    double PSNR=10* log10(d/EQM);
    printf("PSNR=%f \n",PSNR);
}
