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

void W(int nbneel, float *fctbas, float eltdif, float cofvar, float *vecelm){
    for(int i=0; i<nbneel; i++){
        vecelm[i] = vecelm[i]+ eltdif*cofvar*fctbas[i];
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

void ADWDW(int nbneel, float **derfctbas, float  **iJac, float eltdif, float **cofvar, float **matelm){
    float coeff, ci, cj;
    for(int i=0; i<nbneel; i++){
        for(int j=0; j<nbneel; j++){
            for(int a=0; a<2; a++){
                ci = derfctbas[i][0]*iJac[0][a] + derfctbas[i][1]*iJac[1][a];
                for(int b=0; b<2; b++){  
                    cj = derfctbas[j][0]*iJac[0][b] + derfctbas[j][1]*iJac[1][b];
                    matelm[i][j] = matelm[i][j] + eltdif*cofvar[a][b]*ci*cj;
                }
            }
        }
    }
}

void intElem(int t, float *coorEl[]){
    int size = 5-t;
    float *fbase[size];
    // for(int i=0; i<size; i++){
    //     calFbase(t, coorEl[i], fbase);
    // }
    
    // appel de calFbase & calDerFbase
    // matjacob & inverM2x2
    // transFK
    // W WW ADWDW
}

void intAret(){
    
}

void cal1Elem(int nRefDom, int t, int nbneel, int nbaret,
              int nbRefD0, int *numRefD0, int nbRefD1, int *numRefD1, int nbRefF1, int *numRefF1,
              int *nRefArEl,
              float **MatElem, float *SMbrElem, int *NuDElem, float *uDlem){
    
    //ldfkglm
}

int lecNumRef(char *numreffile, int *nRefDom,
               int *nbRefD0, int **numRefD0, int *nbRefD1, int **numRefD1, int *nbRefF1, int **numRefF1){
    FILE *in = NULL;
    in = fopen(numreffile, "r");
    if (in != NULL) {
        fscanf(in, "%d", nRefDom);
        fscanf(in, "%d", nbRefD0);
        numRefD0 = alloctabint(*nbRefD0, 0);
        for(int i=0; i<*nbRefD0; i++){
            fscanf(in, "%d", numRefD0[i]);
        }
        fscanf(in, "%d", nbRefD1);
        numRefD1 = alloctabint(*nbRefD1, 0);
        for(int i=0; i<*nbRefD1; i++){
            fscanf(in, "%d", numRefD1[i]);
        }
        fscanf(in, "%d", nbRefF1);
        numRefF1 = alloctabint(*nbRefF1, 0);
        for(int i=0; i<*nbRefF1; i++){
            fscanf(in, "%d", numRefF1[i]);
        }
        fclose(in);
        printf("La lecture des numref s'est bien passée\n");
        return 1;
    } else {
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }
}

void impCalEl(int K, int typEl, int nbneel, float **MatElem, float *SMbrElem,
              int *NuDElem, float *uDElem) {
  int i, j;
  printf("\n");
  printf(" ELEMENT=%3d    DE TYPE=%5d    NB NOEUDS=%2d\n", K,typEl,nbneel);
  printf(" NuDElem   uDElem    SMbrElem    MatElem\n");
  for (i=0; i < nbneel; i++) {
    printf(" %6d %10.4e %10.4e", NuDElem[i],uDElem[i],SMbrElem[i]);
    for (j=0; j <= i; j++) { printf(" %10.4e", MatElem[i][j]); }
    printf("\n");
  }
}