#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;


void predir(OCTET* img, OCTET* imgP, int w, int h)
{
	int A = 0;
	int B = 0;
	int C = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (i == 0 && j == 0) imgP[i * w + j] = img[i * w + j];
			else if (j == 0) imgP[i * w + j] = img[(i - 1) * w + j];
			else if (i == 0) imgP[i * w + j] = img[i * w + (j - 1)];
			else
			{
				A = img[(i - 1) * w + j];
				B = img[(i - 1) * w + (j - 1)];
				C = img[i * w + (j - 1)];

				//if (abs(B - C) <= abs(B - A)) imgP[i * w + j] = A;
				//else imgP[i * w + j] = C;

				imgP[i * w + j] = (3 * (A + C) - 2 * B) / 4;

			}
		}
	}
}



int main(int argc, char* argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250];
	int nH, nW, nTaille;

	if (argc != 3)
	{
		printf("Usage: ImageIn.pgm ImageOut.pgm\n");
		exit(1);
	}

	sscanf(argv[1], "%s", cNomImgLue);
	sscanf(argv[2], "%s", cNomImgEcrite);


	OCTET *ImgIn, *ImgTemp, *ImgOut;

	lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
	nTaille = nH * nW;

	int nTaille3 = nTaille;
	allocation_tableau(ImgIn, OCTET, nTaille);
	lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
	allocation_tableau(ImgTemp, OCTET, nW * nH);
	allocation_tableau(ImgOut, OCTET, nW * nH);

	predir(ImgIn, ImgTemp, nW, nH);

	for (int i = 0; i < nH; i++)
	{
		for (int j = 0; j < nW; j++)
		{
			ImgOut[i * nW + j] = (ImgIn[i * nW + j] - ImgTemp[i * nW + j]) + 128;
			//printf("%i \n", ImgOut[i * nW + j]);
		}
	}

	ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

	free(ImgIn);
	return 1;
}
