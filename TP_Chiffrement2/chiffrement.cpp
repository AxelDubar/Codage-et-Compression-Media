#include <stdio.h>
#include "image_ppm.h"
#include <vector>

using namespace std;

//Chiffre ImgIn dans ImgOut de taille nH*nW avec une suite de nombres pseudo aléatoire
//géneré avec une clef.
int chiffrement(int x, int n, int e){
  //Initialisation de la suite de nombre pseudo-aléatoire à l'aide de la clef
  //return (int)fmod(pow(x,e), n);
  
  double res=1;

  for(int i=0;i<e;i++){
    res=fmod((res*x),n);
  }

  return (int)fmod(res,n);
}

//Chiffre ImgIn dans ImgOut de taille nH*nW avec une suite de nombres pseudo aléatoire
//géneré avec une clef.
int dechiffrement(int x, int n, int d){
  //Initialisation de la suite de nombre pseudo-aléatoire à l'aide de la clef
  double res=1;

  for(int i=0;i<d;i++){
    res=fmod((res*x),n);
  }

  return (int)fmod(res,n);
}

double entropieShannon(OCTET *Img,int nTaille){
  double entropie=0;
  int occur[256];
  double prob,totale=0;
  //Initialise le tableau des occurences des niveaux de gris
  for(int i=0;i<256;i++){
    occur[i]=0;
  }
  //Calcul l'ocurence de chaque niveau de gris afin de connaitre sa probabilité
  for(int i=0;i<nTaille;i++){
    occur[Img[i]]++;
  }

  for(int i=0;i<256;i++){
    //Si un ndg n'apparait pas on le saute
    if(occur[i]==0)
      continue;
    prob=(double)(occur[i])/(double)nTaille;
    totale+=prob;
    entropie+=prob*log2(prob);
  }
  return -entropie;
}

//Calcul les deux entiers u et v tel que 
// a*u+b*v=PGCD(a,b)
void euclide_etendu(int a,int b,int &d,int &u,int &v){
  int r,r2,rs,us,vs,u2,v2;
  r=a;r2=b;u=1;v=0;u2=0;v2=1;
  int q;
  while(r2!=0){
    q=r/r2;
    rs=r;us=u;vs=v;
    r=r2;u=u2;v=v2;
    r2=rs-q*r2; u2=us-q*u2; v2=vs-q*v2;
  }
  d=r;
}

//Calcul l'inverse modulaire de psi
int inverse_modulaire(int e,int psi){
  int a=e, b=psi,d=0,u=0,v=0;
  euclide_etendu(a,b,d,u,v);
  return u;
}

//Renvoie vrai si a est premier
bool est_premier(int n){
  for(int i=2;i<sqrt(n);i++){
    if(n%i==0)
      return false;
  }
  return true;
}

//Renvoie vrai si a et b nombres sont premiers entre eux
bool sont_premiers_entre_eux(int a,int b){
  int r=a%b;
  if(r==0){
    return false;
  }
  else if(r==1){
    return true;
  }
  else{
    return sont_premiers_entre_eux(b,r);
  }
}

//Calcul la liste des exposants de chiffrements de psi,
//les exposants sont les nombres premiers avec psi.
void compute_exposant_chiff(std::vector<int> &vect,int psi){
  for(int i=2;i<psi;i++){
    if(sont_premiers_entre_eux(psi,i))
      vect.push_back(i);
  }
}


int main(int argc, char* argv[])
{
  //Initialisation des variables et lecture des parametres.
  char cNomImgLue[250],cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille, clef;
  if (argc != 4) 
    {
      printf("Usage: ImageIn.pgm ImageOut.pgm ImgDechif.pgm\n"); 
      exit (1) ;
    }
  sscanf (argv[1],"%s",cNomImgLue);
  sscanf (argv[2],"%s",cNomImgEcrite);
  sscanf (argv[3],"%s",cNomImgEcrite2);

  //Initialisation des images.
  OCTET *ImgIn,*ImgOut,*ImgDechif;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgDechif, OCTET, nTaille);
  
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  //Chiffrement et écriture de l'image chiffrée.
  //chiffrement(ImgIn,ImgOut,nH,nW,clef);

  

  int p=11, q=23;
  if(est_premier(p))//Test si p est premier
    printf("%d est premier\n",p);

  if(sont_premiers_entre_eux(p,q))//Test si p et q sont premiers entre eux 
    printf("%d et %d sont premiers\n",p,q);

  int n=p*q;//module de chiffrement
  int psi=(p-1)*(q-1);//indicatrice

  std::vector<int> vect;
//Calcul les exposants de chiffrements et les stocks dans la liste vect;
  compute_exposant_chiff(vect,psi);

  int e=17;//On prend comme exposant de chiffrement 17
  
  int d= inverse_modulaire(e,psi);//Calcul l'inverse modulaire de e par rapport a psi, ici, 13.
  
  //Chiffre tous les pixels de l'image
  for(int i=0;i<nTaille;i++){
    int val=ImgIn[i];
    ImgOut[i]=chiffrement(val,n,e);

  }

  //Enregistre l'image chiffrée
  ecrire_image_pgm(cNomImgEcrite,ImgOut,nH,nW);

  //Dechiffre l'image chiffrée
  for(int i=0;i<nTaille;i++){
    int val=ImgOut[i];
    ImgDechif[i]=dechiffrement(val,n,d);

  }
  //Enregistre l'image déchiffrée
  ecrire_image_pgm(cNomImgEcrite2,ImgDechif,nH,nW);

  printf("en clair:%lf, chiffree:%lf\n",entropieShannon(ImgIn,nTaille),entropieShannon(ImgOut,nTaille));


  //Désallocation des Images.
  free(ImgIn);
  free(ImgOut);
  free(ImgDechif);
  return 1;
}