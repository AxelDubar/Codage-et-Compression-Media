// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"


double distance(int nR1,int nG1,int nB1,int nR2,int nG2,int nB2){
  //printf("%d %d %d %d %d %d\n",nR1,nG1,nB1,nR2,nB2,nG2);

  int deltaR=nR2-nR1;
  int deltaG=nG2-nG1;
  int deltaB=nB2-nB1;
  return std::sqrt(std::pow(deltaR,2)+std::pow(deltaG,2)+std::pow(deltaB,2));

}



int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite1[250], cNomImgEcrite2[250], cNomImgEcrite3[250], cNomImgEcritePGM[250];
  int nH, nW, nTaille, nR, nG, nB, K;
  K=256;
  if (argc != 6) 
     {
       printf("Usage: ImageIn.ppm ImageOut1.ppm ImageOut2.ppm ImageOut3.ppm ImagePGM.pgm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite1);
   sscanf (argv[3],"%s",cNomImgEcrite2);
   sscanf (argv[4],"%s",cNomImgEcrite3);
   sscanf (argv[5],"%s",cNomImgEcritePGM);

   OCTET *ImgIn, *ImgOut1, *ImgOut2, *ImgOut3, *ImgOutPGM;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;


   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut1, OCTET, nTaille3);
   allocation_tableau(ImgOut2, OCTET, nTaille3);
   allocation_tableau(ImgOut3, OCTET, nTaille3);
   allocation_tableau(ImgOutPGM, OCTET, nTaill);


  int centroid[K][3];int nextcentroid[K][3];
  int centroidN[K];
   for(int k=0;k<K;k++){
     int i=std::rand()%nH;
     int j=std::rand()%nW;
     nextcentroid[k][0]=ImgIn[i*nW+j];
     nextcentroid[k][1]=ImgIn[i*nW+j+1];
     nextcentroid[k][2]=ImgIn[i*nW+j+2];
     centroidN[k]=0;
   }
  printf("Fin initialisation\n");

  for(int i=0;i<K;i++){
    centroid[i][0]=nextcentroid[i][0];
    centroid[i][1]=nextcentroid[i][1];
    centroid[i][2]=nextcentroid[i][2];
    nextcentroid[i][0]=0;nextcentroid[i][1]=0;nextcentroid[i][2]=0;
  }
  
  double minDist=100000000;
  double currentDist=0;
  int numCentroid=-1;
  for (int i=0; i < nTaille3; i+=3){
      nR = ImgIn[i];
      nG = ImgIn[i+1];
      nB = ImgIn[i+2];
      minDist=100000000;
      currentDist=0;
      numCentroid=-1;
      for(int k=0;k<K;k++){
        currentDist=distance(nR,nG,nB,centroid[k][0],centroid[k][1],centroid[k][2]);
        if(currentDist<minDist){
          numCentroid=k;
          minDist=currentDist;
        }
      }
      nextcentroid[numCentroid][0]+=nR;
      nextcentroid[numCentroid][1]+=nG;
      nextcentroid[numCentroid][2]+=nB;
      ImgOut1[i]=centroid[numCentroid][0];
      ImgOut1[i+1]=centroid[numCentroid][1];
      ImgOut1[i+2]=centroid[numCentroid][2];
      centroidN[numCentroid]++;
      
    
  }
  for(int i=0;i<K;i++){
    if(centroidN[i]==0)
    centroidN[i]=1;
    nextcentroid[i][0]=nextcentroid[i][0]/centroidN[i];
    nextcentroid[i][1]=nextcentroid[i][1]/centroidN[i];
    nextcentroid[i][2]=nextcentroid[i][2]/centroidN[i];
    centroidN[i]=0;
  }
  printf("Image1 calc\n");
  ecrire_image_ppm(cNomImgEcrite1, ImgOut1,  nH, nW);
  for(int i=0;i<K;i++){
    centroid[i][0]=nextcentroid[i][0];
    centroid[i][1]=nextcentroid[i][1];
    centroid[i][2]=nextcentroid[i][2];
    nextcentroid[i][0]=0;nextcentroid[i][1]=0;nextcentroid[i][2]=0;
  }

  for (int i=0; i < nTaille3; i+=3){
    nR = ImgIn[i];
    nG = ImgIn[i+1];
    nB = ImgIn[i+2];
    minDist=100000000;
    currentDist=0;
    numCentroid=-1;
    for(int k=0;k<K;k++){
      currentDist=distance(nR,nG,nB,centroid[k][0],centroid[k][1],centroid[k][2]);
      if(currentDist<minDist){
        numCentroid=k;
        minDist=currentDist;
      }
    }
    nextcentroid[numCentroid][0]+=nR;
    nextcentroid[numCentroid][1]+=nG;
    nextcentroid[numCentroid][2]+=nB;
    ImgOut2[i]=centroid[numCentroid][0];
    ImgOut2[i+1]=centroid[numCentroid][1];
    ImgOut2[i+2]=centroid[numCentroid][2];
    centroidN[numCentroid]++;
  }
  for(int i=0;i<K;i++){
    if(centroidN[i]==0)
      centroidN[i]=1;
    nextcentroid[i][0]=nextcentroid[i][0]/centroidN[i];
    nextcentroid[i][1]=nextcentroid[i][1]/centroidN[i];
    nextcentroid[i][2]=nextcentroid[i][2]/centroidN[i];
    centroidN[i]=0;
  }

  printf("Fin image2\n");
  ecrire_image_ppm(cNomImgEcrite2, ImgOut2,  nH, nW);

  bool convergence;
	do{
    convergence=true;
    for(int i=0;i<K;i++){
      centroid[i][0]=nextcentroid[i][0];
      centroid[i][1]=nextcentroid[i][1];
      centroid[i][2]=nextcentroid[i][2];
      nextcentroid[i][0]=0;nextcentroid[i][1]=0;nextcentroid[i][2]=0;
    }
   for (int i=0; i < nTaille3; i+=3){
      nR = ImgIn[i];
      nG = ImgIn[i+1];
      nB = ImgIn[i+2];
      minDist=100000000;
      currentDist=0;
      numCentroid=-1;
      for(int k=0;k<K;k++){
        currentDist=distance(nR,nG,nB,centroid[k][0],centroid[k][1],centroid[k][2]);
        if(currentDist<minDist){
          numCentroid=k;
          minDist=currentDist;
        }
      }
      nextcentroid[numCentroid][0]+=nR;
      nextcentroid[numCentroid][1]+=nG;
      nextcentroid[numCentroid][2]+=nB;
      centroidN[numCentroid]++;
        
      }
   for(int i=0;i<K;i++){
     if(centroidN[i]==0)
      continue;
     nextcentroid[i][0]=nextcentroid[i][0]/centroidN[i];
     nextcentroid[i][1]=nextcentroid[i][1]/centroidN[i];
     nextcentroid[i][2]=nextcentroid[i][2]/centroidN[i];
     centroidN[i]=0;
     if(nextcentroid[i][0]!=centroid[i][0] && nextcentroid[i][1]!=centroid[i][1] && nextcentroid[i][2]!=centroid[i][2])
      convergence=false;
   }
   printf("Tour de boucle\n");
  }while(!convergence);
  printf("Convergence\n");

  int centroidIndex[nTaille];

  for (int i=0; i < nTaille3; i+=3){
    nR = ImgIn[i];
    nG = ImgIn[i+1];
    nB = ImgIn[i+2];
    double minDist=100000000;
    double currentDist=0;
    int numCentroid=-1;
    for(int k=0;k<K;k++){
      currentDist=distance(nR,nG,nB,centroid[k][0],centroid[k][1],centroid[k][2]);
      if(currentDist<minDist){
        numCentroid=k;
        minDist=currentDist;
      }
    }
    
    //printf("%d \n",numCentroid);
    ImgOut3[i]=centroid[numCentroid][0];
    ImgOut3[i+1]=centroid[numCentroid][1];
    ImgOut3[i+2]=centroid[numCentroid][2];
    centroidIndex[i/3]=k;
    ImgOutPGM[i/3]=k;
    printf("%d\n", centroidIndex[i/3]);
  }


   ecrire_image_pgm(cNomImgEcritePGM,ImagePGM,nH,nW);
   ecrire_image_ppm(cNomImgEcrite3, ImgOut3,  nH, nW);
   free(ImgIn);
   return 1;
}
