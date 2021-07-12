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
   
    double somme=0;

    for (int i=0; i < nTaille3; i+=3){
        nR = ImgSrc[i];
        nG = ImgSrc[i+1];
        nB = ImgSrc[i+2];
      
        nR2 = ImgCmp[i];
        nG2 = ImgCmp[i+1];
        nB2 = ImgCmp[i+2];
    
        somme+=pow(nR-nR2,2)+pow(nG-nG2,2)+pow(nB-nB2,2);
    }
    double EQM=somme/(nH*nW);
    printf("EQM=%lf \n",EQM);
    double PSNR=10* std::log10(pow(255,2)/EQM);
    printf("PSNR=%lf \n",PSNR);
}
