#include"solver.h"
#include"forfun.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

extern int nucas;

float solex(float *coor){

    const float PI=3.141592;
    float val;

    switch (nucas) {
        case 1 :
            val=16.*coor[0]*coor[1]*(1-coor[0])*(1-coor[1]);
            break;
        case 2 :
            val=sin(PI*coor[0])*sin(PI*coor[1]);
            break;
        case 3 :
            val=cos(PI*coor[0])*cos(PI*coor[1]);
            break;
        default :
            printf("*** SOLEX : exemple non prevu. Abandon.\n");
            exit(1);
        break;
    }
    return(val);
}

void CalSol(int nbLign, float **coord, float *UEX){
    for(int i=0; i<nbLign; i++){
        UEX[i] = solex(coord[i]);
    }
}

void CholeskyResol(int nbLign, int *Profile, float *MatrixP, float *SecMembr, float eps, float *U){

    float *ld, *ll, *Y;

    ld = calloc(nbLign, sizeof(float));
    ll = calloc(Profile[nbLign-1], sizeof(float));
    Y  = calloc(nbLign, sizeof(float));

    ltlpr_(&nbLign, Profile, &MatrixP[0], &MatrixP[nbLign], &eps, ld, ll);
    rsprl_(&nbLign, Profile, ld, ll, SecMembr, Y);
    rspru_(&nbLign, Profile, ld, ll, Y, U);
    
    free(ld);
    free(ll);
    free(Y);
}