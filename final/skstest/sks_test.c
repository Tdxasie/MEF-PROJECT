#include"matrix_storage.h"
//#include"dmoapr.h"
#include<stdio.h>
#include<stdlib.h>

int coefMax = 10; //dim max de la partie inférieure stricte (surdimensionnement au pire, taille exacte au mieux) = (nblign²-nblign)/2
int nbLign = 5;

float MatrixO[11] = {11, 12, 13, 14, 15, 5, 3, 7, 2, 1};
int colIdxO[6] = {1, 2, 1, 1, 4};
int firstAdLiO[5] = {1, 2, 3, 4, 6};

float MatrixP[15];
int Profile[5];

int main(int argc, char const *argv[])
{
    SMOtoPR(nbLign, coefMax, firstAdLiO, colIdxO, MatrixO, Profile, MatrixP);

    // int res; 

    // dmoapr(nbLign, firstAdLiO, colIdxO, MatrixO, nbCoef, Profile, MatrixP, &res);

    printf("MatrixP : [");
    for(int i=0; i<coefMax+nbLign; i++) printf(", %f", MatrixP[i]);
    printf("]\n");

    printf("Profile : [");
    for(int i=0; i<nbLign; i++) printf(", %d", Profile[i]);
    printf("]\n");
    return 0;
}






