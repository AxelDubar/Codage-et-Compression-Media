// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"

void composante(OCTET *ImgIn, OCTET *ImgOutY, OCTET *ImgOutCr, OCTET *ImgOutCb,int nTaille3){
  int nH, nW, nR, nG, nB; 
  double Y,Cr,Cb;
  for(int i=0;i<nTaille3;i+=3){
    nR=ImgIn[i];
    nG=ImgIn[i+1];
    nB=ImgIn[i+2];

    Y=0.299*nR+0.587*nG+0.114*nB;
    Cb=128-(0.168736*nR)-(0.331265*nG)+(0.5*nB);
    Cr=128+(0.5*nR)-(0.418688*nG)-(0.081312*nB);

    //nextnR=Y+1.402*(Cr-128);
    //nextnG=Y-0.344136*(Cb-128)-0.714136*(Cr-128);
    //nextnB=Y+1.772*(Cb-128);

    ImgOutY[i/3]=Y;
    ImgOutCr[i/3]=Cr;
    ImgOutCb[i/3]=Cb;
  }

}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcriteY[250], cNomImgEcriteCr[250], cNomImgEcriteCb[250];
  int nH, nW, nTaille;

  if (argc != 5) 
    {
      printf("Usage: ImageIn.ppm ImageOutY.pgm ImageOutCr.pgm ImageOutCb.pgm\n"); 
      exit (1) ;
    }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcriteY);
  sscanf (argv[3],"%s",cNomImgEcriteCr);
  sscanf (argv[4],"%s",cNomImgEcriteCb);

  OCTET *ImgIn, *ImgOutY, *ImgOutCr, *ImgOutCb, *ImgOut, *ImgReconstruit, *ImgTampon;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;


  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOutY, OCTET, nTaille);
  allocation_tableau(ImgOutCr, OCTET, nTaille);
  allocation_tableau(ImgOutCb, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgReconstruit, OCTET, nTaille);

  allocation_tableau(ImgTampon, OCTET, nTaille);


  composante(ImgIn,ImgOutY,ImgOutCr,ImgOutCb,nTaille3);

  int A,B,C,D;

  for(int i=0;i<nTaille;i++){
      ImgTampon[i]=ImgOutCb[i];
  }
  int N=4;
  for(int n=1;n<=N;n++){
    printf("%d\n",n);
    for(int i=0;i<nH/pow(2,n-1);i+=2){
      for(int j=0;j<nW/pow(2,n-1);j+=2){

        double BF,MFVert,MFHori,HF;
        A=ImgTampon[i*nW+j];	B=ImgTampon[i*nW+j+1];
        C=ImgTampon[(i+1)*nW+j];	D=ImgTampon[(i+1)*nW+j+1];

        BF=(A+B+C+D)/4.0;
        MFVert=128+((A+B)-(C+D))/2.0;
        if(MFVert<0)MFVert=0;
        if(MFVert>255)MFVert=255;
        MFHori=128+((A-B)+(C-D))/2.0;
        if(MFHori<0)MFHori=0;
        if(MFHori>255)MFHori=255;
        HF=128+((A-B)-(C-D));//HF=0;

        ImgOut[((i/2) + (int)(nH/pow(2,n)) ) * nW   +   j/2  +  (int)(nW/pow(2,n))]=HF/255;//HF
        ImgOut[(i/2) * nW + j/2 ]=BF;//BF
        ImgOut[(i/2) * nW+ j/2 + (int)(nW/pow(2,n))]=MFVert/(2*n);//MFVert
        ImgOut[((i/2) + (int)(nH/pow(2,n)) ) * nW + j/2 ]=MFHori/(2*n);//MFHori
      }
    }

    for(int i=0;i<nTaille;i++){
      ImgTampon[i]=ImgOut[i];
  }
  }

  for(int n=N;n>=1;n--){

    for(int i=0;i<nH/pow(2,n-1);i+=2){
      for(int j=0;j<nW/pow(2,n-1);j+=2){
        float BF,MFVert,MFHori,HF;
        
        BF=ImgTampon[(i/2)*nW+j/2];

        MFVert=ImgTampon[(i/2)*nW+j/2+(int)(nW/pow(2,n))]*(2*n)-128;
        if(MFVert<0)MFVert=0;
        if(MFVert>255)MFVert=255;
        
        MFHori=ImgTampon[ ((i/2) + (int)(nH/pow(2,n))) * nW+ j/2 ]*(2*n)-128;
        if(MFHori<0)MFHori=0;
        if(MFHori>255)MFHori=255;
        
        HF=ImgTampon[ ((i/2) + (int)(nH/pow(2,n))) * nW + j/2 + (int)(nW/pow(2,n))] -128;
        if(HF<0)HF=0;
        if(HF>255)HF=255;


        A=(4*BF+2*MFVert+2*MFHori+HF)/4.0;	
        printf("%f\n",(4*BF+2*MFVert+2*MFHori+HF)/4.0);
        if(A<0)A=0;
        B=2*BF+MFVert-A;
        if(B<0)B=0;
        C=2*BF+MFHori-A;
        if(C<0)C=0;	
        D=2*BF+HF-A;
        if(D<0)D=0;

        ImgReconstruit[i*nW+j]=A;
        ImgReconstruit[i*nW+j+1]=B;
        ImgReconstruit[(i+1)*nW+j]=C;
        ImgReconstruit[(i+1)*nW+j+1]=D;
      }
    }
    
    for(int i=0;i<nH/n;i++){
      for(int j=0;j<nW/n;j++){
        ImgTampon[i*nW+j]=ImgReconstruit[i*nW+j];
      }
    }
  }


  ecrire_image_pgm(cNomImgEcriteY, ImgOutY,  nH, nW);
  ecrire_image_pgm(cNomImgEcriteCr, ImgOutCr,  nH, nW);
  ecrire_image_pgm(cNomImgEcriteCb, ImgOutCb,  nH, nW);
  ecrire_image_pgm("out.pgm", ImgOut,  nH, nW);
  ecrire_image_pgm("tamp.pgm", ImgTampon,  nH, nW);
  ecrire_image_pgm("recons.pgm", ImgReconstruit,  nH, nW);
  free(ImgIn);free(ImgOutY);
  return 1;
}
