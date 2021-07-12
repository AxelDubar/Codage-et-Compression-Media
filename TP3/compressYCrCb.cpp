// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"



int reechantillonage(OCTET *img,int i,int j,int nW,int nH){
  int val=0;
  int nbElement=0;
  if(i!=0){
    if(img[(i-1)*nW+j]>0){
      val+=img[(i-1)*nW+j];
      nbElement++;
    }

    if(j!=0 && img[(i-1)*nW+j-1]>0){
      val+=img[(i-1)*nW+j-1];
      nbElement++;
    }

    if(j!=nW-1 && img[(i-1)*nW+j+1]>0){
      val+=img[(i-1)*nW+j+1];
      nbElement++;
    }
  }

  if(i!=nH-1){
    if(img[(i+1)*nW+j]>0){
      val+=img[(i+1)*nW+j];
      nbElement++;
    }

    if(j!=0 && img[(i+1)*nW+j-1]>0){
      val+=img[(i+1)*nW+j-1];
      nbElement++;
    }

    if(j!=nW-1 && img[(i+1)*nW+j+1]>0){
      val+=img[(i+1)*nW+j+1];
      nbElement++;
    }
  }

  
  if(j!=0 && img[i*nW+j-1]>0){
    val+=img[i*nW+j-1];
    nbElement++;
  }

  if(j!=nW-1 && img[i*nW+j+1]>0){
    val+=img[i*nW+j+1];
    nbElement++;
  }



  if(nbElement>0){
    return val/nbElement;
  }
  else return 0;
}




int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgY[250], cNomImgCr[250],cNomImgCb[250],cNomImgCrRe[250],cNomImgCbRe[250];
  int nH, nW, nTaille, nR, nG, nB;

  if (argc != 8) 
    {
      printf("Usage: ImageIn.ppm ImageOut.ppm ImgY.pgm ImgCr.pgm ImgCb.pgm ImgRECr.pgm ImgRECb.pgm\n"); 
      exit (1) ;
    }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);

  sscanf (argv[3],"%s",cNomImgY);
  sscanf (argv[4],"%s",cNomImgCr);
  sscanf (argv[5],"%s",cNomImgCb);
  sscanf (argv[6],"%s",cNomImgCrRe);
  sscanf (argv[7],"%s",cNomImgCbRe);

  OCTET *ImgIn, *ImgOut, *ImgY, *ImgCr, *ImgCb, *ImgCrRe, *ImgCbRe;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;


  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);
  int TailleReduite=(nH/2) * (nW/2);

  allocation_tableau(ImgCr, OCTET, TailleReduite);
  allocation_tableau(ImgY, OCTET, nTaille);
  allocation_tableau(ImgCb, OCTET, TailleReduite);

  allocation_tableau(ImgCrRe, OCTET, nTaille);
  allocation_tableau(ImgCbRe, OCTET, nTaille);

  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      nR=ImgIn[i*3*nW+j*3];
      nG=ImgIn[i*3*nW+j*3+1];
      nB=ImgIn[i*3*nW+j*3+2];
      
      double Y=0.299*nR+0.587*nG+0.114*nB;
      ImgY[i*nW+j]=(int)Y;
      if(i%2==0 || j%2==0){
        ImgCb[(i/2)*nW/2+(j/2)]=128-0.1687*nR-0.3313*nG+0.5*nB;
        ImgCr[(i/2)*nW/2+(j/2)]=128+0.5*nR-0.4187*nG-0.0813*nB;
      }
    }
  }
  printf("Fin de boucle\n");


  //ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
  ecrire_image_pgm(cNomImgCb,ImgCb,nH/2,nW/2);
  ecrire_image_pgm(cNomImgCr,ImgCr,nH/2,nW/2);
  ecrire_image_pgm(cNomImgY,ImgY,nH,nW);


  //Re echantillonage
  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      
      if(i%2==0 || j%2==0){
        ImgCrRe[i*nW+j]=ImgCr[(i/2)*nW/2+(j/2)];
        ImgCbRe[i*nW+j]=ImgCb[(i/2)*nW/2+(j/2)];
      }
    }
  }
  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      
      if(i%2!=0 && j%2!=0){
        ImgCrRe[i*nW+j]=reechantillonage(ImgCrRe,i,j,nW,nH);
        ImgCbRe[i*nW+j]=reechantillonage(ImgCbRe,i,j,nW,nH);
      }
    }
  }

  ecrire_image_pgm(cNomImgCrRe,ImgCrRe,nH,nW);
  ecrire_image_pgm(cNomImgCbRe,ImgCbRe,nH,nW);

  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      ImgOut[i*3*nW+j*3]=ImgY[i*nW+j]+1.402*(ImgCrRe[i*nW+j]-128);
      ImgOut[i*3*nW+j*3+1]=ImgY[i*nW+j]-0.34414*(ImgCbRe[i*nW+j]-128)-0.714136*(ImgCrRe[i*nW+j]-128);
      ImgOut[i*3*nW+j*3+2]=ImgY[i*nW+j]+1.772*(ImgCbRe[i*nW+j]-128);
      
    }
  }
  
  ecrire_image_ppm(cNomImgEcrite,ImgOut,nH,nW);

  return 1;
}
