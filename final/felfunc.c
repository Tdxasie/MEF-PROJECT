#include"felfunc.h"
#include"tabtools.h"
#include"fcaltools.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int a11(float *x){
    return 1.0;
}

int a12(float *x){
    return 0.0;
}

int a22(float *x){
    return 1.0;
}

int a00(float *x){
    return 1.0;
}

int bN(float *x){
    return 0.0;
}

int fOmega(float *x){
    return 1.0;
}

int fN(float *x){
    return 1.0;
}

int uD(float *x){
    return 100.0*x[0] + x[1];
}

void getVarCofMat(float *im, float **matcofvar){
    matcofvar[0][0] = a11(im) ; matcofvar[0][1] = a12(im);
    matcofvar[1][0] = a12(im) ; matcofvar[1][1] = a22(im);
}

void W(int nbneel, float *fbase, float eltdif, float cofvar, float *vecelm){
    for(int i=0; i<nbneel; i++){
        vecelm[i] = vecelm[i]+ eltdif*cofvar*fbase[i];
    }
}

void WW(int nbneel, float *fbase, float eltdif, float cofvar, float **matelm){
    float coeff;
    for(int i=0; i<nbneel; i++){
        coeff = eltdif*cofvar*fbase[i];
        for(int j=0; j<nbneel; j++){
            matelm[i][j] = matelm[i][j] + coeff*fbase[j];
        }
    }
}

void ADWDW(int nbneel, float **derfbase, float  **iJac, float eltdif, float **cofvar, float **matelm){
    float ci, cj;
    for(int i=0; i<nbneel; i++){
        for(int j=0; j<nbneel; j++){
            for(int a=0; a<2; a++){
                ci = derfbase[i][0]*iJac[0][a] + derfbase[i][1]*iJac[1][a];
                for(int b=0; b<2; b++){  
                    cj = derfbase[j][0]*iJac[0][b] + derfbase[j][1]*iJac[1][b];
                    matelm[i][j] = matelm[i][j] + eltdif*cofvar[a][b]*ci*cj;
                }
            }
        }
    }
}

int intElem(int type, int elemNodeCount, float *coorEl[], float *SMbrElem, float **MatElem){
    int dim = 2;
    if (type>2) dim = 1;

    int q = getQ(type); if (q==0) return 0;

    float det;
    float eltdif;

    float im[2];
    float wk[q];
    float fbase[elemNodeCount];

    float **xk = alloctab(q, dim);
    float **derfbase = alloctab(elemNodeCount, dim);
    float **jacob = alloctab(dim, dim);
    float **iJac = alloctab(dim,dim);
    float **matcofvar = alloctab(dim,dim);

    ppquad(type, wk, xk);

    for(int i=0; i<q; i++){
        calFbase(type, xk[i], fbase); 
        calDerFbase(type, xk[i], derfbase);
        matJacob(elemNodeCount, dim, coorEl, derfbase, jacob);
        invertM2x2(jacob, iJac, &det);
        transFK(elemNodeCount, coorEl, fbase, im);

        eltdif = fabs(det)*wk[i];

        getVarCofMat(im, matcofvar);

        W(elemNodeCount, fbase, eltdif, fOmega(im), SMbrElem);
        WW(elemNodeCount, fbase, eltdif, a00(im), MatElem);
        ADWDW(elemNodeCount, derfbase, iJac, eltdif, matcofvar, MatElem);
    }
    freetab(xk);
    freetab(derfbase);
    freetab(jacob);
    freetab(iJac);
    freetab(matcofvar);
    return 1;
}

// int intAret(int type, float *coorEl[], float *SMbrElem, float **MatElem){
//     int size = 5-type;
//     int dim = 2;
//     if (type>2) dim = 1;

//     int q = getQ(type); if (q==0) return 0;

//     float det;
//     float eltdif;

//     float im[2];
//     float wk[q];
//     float fbase[size];

//     float **xk = alloctab(q, 2);
//     float **derfbase = alloctab(size, 2);
//     float **jacob = alloctab(2, 2);
//     float **iJac = alloctab(2,2);

//     ppquad(type, wk, xk);

//     for(int i=0; i<q; i++){
//         calFbase(type, xk[i], fbase); 
//         calDerFbase(type, xk[i], derfbase);
//         matJacob(size, dim, coorEl, derfbase, jacob);
//         invertM2x2(jacob, iJac, &det);
//         transFK(size, coorEl, fbase, im);

//         eltdif = det*wk[i];

//         W(size, fbase, eltdif, fOmega(im), SMbrElem);
//         WW(size, fbase, eltdif, a00(im), MatElem);
//     }
//     freetab(xk);
//     freetab(derfbase);
//     freetab(jacob);
//     freetab(iJac);
//     return 1;
// }

int cal1Elem(int nRefDom, int type, int elemNodeCount, int elemEdgeCount,
              int nbRefD0, int *numRefD0, int nbRefD1, int *numRefD1, int nbRefF1, int *numRefF1,
              int *nRefArEl, float *coorEl[],
              float **MatElem, float *SMbrElem, int *NuDElem, float *uDlem){

    int nRef, nk, nj;

    int nodeIds[2];
    float *coorAr[2];

    float *SMbrAret;
    float **MatAret;

    SMbrAret = malloc(2*sizeof(float));
    MatAret = alloctab(2,2);
    
    for(int i=0; i<elemNodeCount; i++){
        SMbrElem[i] = 0.0;
        NuDElem[i] = 1;
        uDlem[i] = 0.0;
        for(int j=0; j<elemNodeCount; j++) MatElem[i][j] = 0.0;

    }

    if(!intElem(type, elemNodeCount, coorEl, SMbrElem, MatElem)) return 0;

    for(int i=0; i<elemEdgeCount; i++){
        nRef = nRefArEl[i];
        if(nRef == nRefDom) continue; //pas de conditon aux limites

        numNaret(type, i+1, nodeIds);

        // Dirichlet Homogène
        for(int j=0; j<nbRefD0; j++){
            if(nRef == numRefD0[j]){ // on cherche l'arrete sur laquelle on est
                NuDElem[nodeIds[0]-1] = 0;
                NuDElem[nodeIds[1]-1] = 0;
            }
        }

        selectPts(2, nodeIds, coorEl, coorAr);

        //Neumann
        for(int j=0; j<nbRefD0; j++){
            if(nRef == numRefF1[j]){
                for(int k=0; k<2; k++){
                    SMbrAret[k] = 0;
                    MatAret[k][0] = 0;
                    MatAret[k][1] = 0;
                }
                //intAret
                for(int k=0; k<2; k++){
                    nk = nodeIds[k]-1;
                    SMbrElem[nk] += SMbrAret[k];
                    for(int j=0; j<2; j++){
                        nj = nodeIds[j]-1;
                        MatElem[nk][nj] += MatAret[k][j];
                    }
                }
            }
        }

        // Dirichlet non-homogène
        for(int j=0; j<nbRefD1; j++){
            if(nRef == numRefD1[j]){
                NuDElem[nodeIds[0]-1] = -1;
                NuDElem[nodeIds[1]-1] = -1;
                uDlem[nodeIds[0]-1] = uD(coorAr[0]);
                uDlem[nodeIds[1]-1] = uD(coorAr[1]);
            }
        }
    }

    free(SMbrAret);
    freetab(MatAret);
    return 1;
}

int lecNumRef(char *numreffile, int *nRefDom,
               int *nbRefD0, int **numRefD0, int *nbRefD1, int **numRefD1, int *nbRefF1, int **numRefF1){
    FILE *in = NULL;
    in = fopen(numreffile, "r");
    if (in != NULL) {
        fscanf(in, "%d", nRefDom);

        fscanf(in, "%d", nbRefD0);
        *numRefD0 = malloc(*nbRefD0 * sizeof(int));
        for(int i=0; i<*nbRefD0; i++){
            fscanf(in, "%d", numRefD0[i]);
        }

        fscanf(in, "%d", nbRefD1);
        *numRefD1 = malloc(*nbRefD1 * sizeof(int));
        for(int i=0; i<*nbRefD1; i++){
            fscanf(in, "%d", numRefD1[i]);
        }
        
        fscanf(in, "%d", nbRefF1);
        *numRefF1 = malloc(*nbRefF1 * sizeof(int));
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

void impCalEl(int K, int nbneel, int typEl, float **MatElem, float *SMbrElem,
              int *NuDElem, float *uDElem) {
  int i, j;
  printf("\n");
  printf(" ELEMENT=%3d    DE TYPE=%5d    NB NOEUDS=%2d\n", K, typEl, nbneel);
  printf(" NuDElem   uDElem    SMbrElem    MatElem\n");
  for (i=0; i < nbneel; i++) {
    printf(" %6d %10.4e %10.4e", NuDElem[i],uDElem[i],SMbrElem[i]);
    for (j=0; j <= i; j++) { printf(" %10.4e", MatElem[i][j]); }
    printf("\n");
  }
}