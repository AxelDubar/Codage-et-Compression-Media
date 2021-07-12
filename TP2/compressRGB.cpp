// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"



int reechantillonage(OCTET *img,int i,int j,int nW,int nH){
  int val=0;
  int nbElement=0;
  if(i!=0){
    val+=img[(i-1)*nW+j];
    nbElement++;

    if(j!=0){
      val+=img[(i-1)*nW+j-1];
      nbElement++;
    }

    if(j!=nW-1){
      val+=img[(i-1)*nW+j+1];
      nbElement++;
    }
  }

  if(i!=nH-1){
    val+=img[(i+1)*nW+j];
    nbElement++;

    if(j!=0){
      val+=img[(i+1)*nW+j-1];
      nbElement++;
    }

    if(j!=nW-1){
      val+=img[(i+1)*nW+j+1];
      nbElement++;
    }
  }

  
  if(j!=0){
    val+=img[i*nW+j-1];
    nbElement++;
  }

  if(j!=nW-1){
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
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgCompoRed[250], cNomImgCompoGreen[250],cNomImgCompoBlue[250],cNomImgCompoRedRe[250],cNomImgCompoBlueRe[250];
  int nH, nW, nTaille, nR, nG, nB;

  if (argc != 8) 
    {
      printf("Usage: ImageIn.ppm ImageOut.ppm ImgRed.pgm ImgGreen.pgm ImgBlue.pgm ImgRERed.pgm ImgREBlue.pgm\n"); 
      exit (1) ;
    }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);

  sscanf (argv[3],"%s",cNomImgCompoRed);
  sscanf (argv[4],"%s",cNomImgCompoGreen);
  sscanf (argv[5],"%s",cNomImgCompoBlue);
  sscanf (argv[6],"%s",cNomImgCompoRedRe);
  sscanf (argv[7],"%s",cNomImgCompoBlueRe);

  OCTET *ImgIn, *ImgOut, *ImgRed, *ImgGreen, *ImgBlue, *ImgRedRe, *ImgBlueRe;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;


  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);
  int TailleReduite=(nH/2) * (nW/2);

  allocation_tableau(ImgRed, OCTET, TailleReduite);
  allocation_tableau(ImgGreen, OCTET, nTaille);
  allocation_tableau(ImgBlue, OCTET, TailleReduite);

  allocation_tableau(ImgRedRe, OCTET, nTaille);
  allocation_tableau(ImgBlueRe, OCTET, nTaille);

  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      nR=ImgIn[i*3*nW+j*3];
      nG=ImgIn[i*3*nW+j*3+1];
      nB=ImgIn[i*3*nW+j*3+2];
      
      
      ImgGreen[i*nW+j]=nG;
      if(i%2==0 || j%2==0){
        ImgRed[(i/2)*nW/2+(j/2)]=nR;
        ImgBlue[(i/2)*nW/2+(j/2)]=nB;
      }
    }
  }
  printf("Fin de boucle\n");


  //ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
  ecrire_image_pgm(cNomImgCompoRed,ImgRed,nH/2,nW/2);
  ecrire_image_pgm(cNomImgCompoBlue,ImgBlue,nH/2,nW/2);
  ecrire_image_pgm(cNomImgCompoGreen,ImgGreen,nH,nW);


  //Re echantillonage
  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      
      if(i%2==0 || j%2==0){
        ImgRedRe[i*nW+j]=ImgRed[(i/2)*nW/2+(j/2)];
        ImgBlueRe[i*nW+j]=ImgBlue[(i/2)*nW/2+(j/2)];
      }
    }
  }
  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      
      if(i%2!=0 && j%2!=0){
        ImgRedRe[i*nW+j]=reechantillonage(ImgRedRe,i,j,nW,nH);
        ImgBlueRe[i*nW+j]=reechantillonage(ImgBlueRe,i,j,nW,nH);
      }
    }
  }

  ecrire_image_pgm(cNomImgCompoRedRe,ImgRedRe,nH,nW);
  ecrire_image_pgm(cNomImgCompoBlueRe,ImgBlueRe,nH,nW);

  for(int i=0;i<nH;i++){
    for(int j=0;j<nW;j++){
      ImgOut[i*3*nW+j*3]=ImgRedRe[i*nW+j];
      ImgOut[i*3*nW+j*3+1]=ImgGreen[i*nW+j];
      ImgOut[i*3*nW+j*3+2]=ImgBlueRe[i*nW+j];
      
    }
  }
  
  ecrire_image_ppm(cNomImgEcrite,ImgOut,nH,nW);

  return 1;
}
