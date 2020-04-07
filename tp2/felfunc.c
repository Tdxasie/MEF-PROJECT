#include"felfunc.h"
#include"tabtools.h"
#include"fcaltools.h"
#include<stdio.h>
#include<stdlib.h>


int a11(float *x){
    return 1;
}

int a12(float *x){
    return 0;
}

int a22(float *x){
    return 1;
}

int a00(float *x){
    return 1;
}

int bN(float *x){
    return 0;
}

int fOmega(float *x){
    return 1;
}

int fN(float *x){
    return 1;
}

int uD(float *x){
    return 100*x[0] + x[1];
}

void W(int nbneel, float *fctbas, float eltdif, float cofvar, float **matelm){
    for(int i=0; i<nbneel; i++){
        matelm[i][0] = matelm[i][0]+ eltdif*cofvar*fctbas[i];
    }
}

void WW(int nbneel, float *fctbas, float eltdif, float cofvar, float **matelm){
    float coeff;
    for(int i=0; i<nbneel; i++){
        coeff = eltdif*cofvar*fctbas[i];
        for(int j=0; j<nbneel; j++){
            matelm[i][j] = matelm[i][j] + coeff*fctbas[j];
        }
    }
}

void ADWDW(int nbneel, float **derfctbas, float eltdif, float **cofvar, float **matelm){
    float coeff;
    for(int a=0; a<2; a++){
        for(int b=0; b<2; b++){
            for(int i=0; i<nbneel; i++){
                coeff = eltdif*cofvar[a][b]*derfctbas[i][a];
                for(int j=0; j<nbneel; j++){
                    matelm[i][j] = matelm[i][j] + coeff*derfctbas[j][b];
                }
            }
        }
    }
}

void intElem(int t, *float[] coorEl){
    // appel de calFbase & calDerFbase
    // matjacob & inverM2x2
    // transFK
    // W WW ADWDW

}

void intAret(){
    
}
