#include"felfunc.h"
#include"tabtools.h"
#include"fcaltools.h"
#include"forfun.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

extern int omegacas;
extern int nucas;

const float PI=3.141592;

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
    return 0.0;
}

int bN(float *x){
    return 0.0;
}

float fOmega(float *x){
    float val;

    switch (omegacas) {
        case 1 :

            val=32.0*(x[0]*(1-x[0])+x[1]*(1-x[1]));
            break;
        case 2 :
            val=2*PI*PI*sin(PI*x[0])*sin(PI*x[1]);
            break;
        case 3 :
            val=(2*PI*PI+1)*cos(PI*x[0])*cos(PI*x[1]);
            break;
        default :
            printf("*** fomega : exemple non prevu. Abandon.\n");
            exit(1);
        break;
    }
    return(val);
}

float fN(float *x, int edgeRef){
    float val;

    if (nucas == 3){
        switch (edgeRef){
            case 1:
                val=PI*cos(PI*x[0])*sin(PI*x[1]);
                break;
            case 2:
                val = -PI*cos(PI*x[1])*sin(PI*x[0]);
                break;
            case 3:
                val = -PI*cos(PI*x[0])*sin(PI*x[1]);
                break;
            case 4:
                val = PI*cos(PI*x[1])*sin(PI*x[0]);
                break;
            default:
                printf("*** fN : edgeRef inconnu. Abandon.\n");
                exit(1);
                break;
        }
    }
    return val;
}

float uD(float *x){ 
    float val;

    switch (nucas) {
        case 1 :
            val=16.0*x[0]*x[1]*(1-x[0])*(1-x[1]);
            break;
        case 2 :
            val=sin(PI*x[0])*sin(PI*x[1]);
            break;
        case 3 :
            val=cos(PI*x[0])*cos(PI*x[1]);
            break;
        default :
            printf("*** uD : exemple non prevu. Abandon.\n");
            exit(1);
        break;
    }
  return(val);
}

void getVarCofMat(float *im, float **matcofvar){
    matcofvar[0][0] = a11(im) ; matcofvar[0][1] = a12(im);
    matcofvar[1][0] = a12(im) ; matcofvar[1][1] = a22(im);
}

void W(int nbneel, float *fbase, float eltdif, float cofvar, float *vecelm){
    for(int i=0; i<nbneel; i++){
        vecelm[i] = vecelm[i] + eltdif*cofvar*fbase[i];
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
                    matelm[i][j] += eltdif*cofvar[a][b]*ci*cj;
                }
            }
        }
    }
}

int intElem(int type, int elemNodeCount, float *coorEl[], float *SMbrElem, float **MatElem){
    int intgrtDim = 2;
    int problemDim = 2;

    int q = getQ(type); if (q==0) return 0;

    float det;
    float eltdif;

    float im[problemDim];
    float wk[q];
    float fbase[elemNodeCount];

    float **xk = alloctab(q, intgrtDim);
    float **derfbase = alloctab(elemNodeCount, problemDim);
    float **jacob = alloctab(problemDim, intgrtDim);
    float **iJac = alloctab(problemDim, intgrtDim);
    float **matcofvar = alloctab(intgrtDim, intgrtDim);

    ppquad(type, wk, xk);

    for(int i=0; i<q; i++){
        calFbase(type, xk[i], fbase); 
        calDerFbase(type, xk[i], derfbase);
        matJacob(elemNodeCount, intgrtDim, coorEl, derfbase, jacob);
        invertM2x2(jacob, iJac, &det);
        transFK(elemNodeCount, coorEl, fbase, im);

        eltdif = fabsf(det)*wk[i];

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

int intAret(int type, int elemNodeCount, float *coorEl[], float *SMbrElem, float **MatElem, int edgeRef){

    int intgrtDim = 1;
    int problemDim = 2;

    int q = getQ(type); if (q==0) return 0;

    float eltdif;

    float im[problemDim];
    float wk[q];
    float fbase[elemNodeCount];

    float **xk = alloctab(q, intgrtDim);
    float **derfbase = alloctab(elemNodeCount, intgrtDim);
    float **jacob = alloctab(problemDim, intgrtDim);

    ppquad(type, wk, xk);

    for(int i=0; i<q; i++){
        calFbase(type, xk[i], fbase);
        calDerFbase(type, xk[i], derfbase);
        matJacob(elemNodeCount, intgrtDim, coorEl, derfbase, jacob);
        transFK(elemNodeCount, coorEl, fbase, im);

        eltdif = norm2(jacob)*wk[i];

        W(elemNodeCount, fbase, eltdif, fN(im, edgeRef), SMbrElem);
        WW(elemNodeCount, fbase, eltdif, bN(im), MatElem);
    }

    freetab(xk);
    freetab(derfbase);
    freetab(jacob);
    return 1;
}

int cal1Elem(int nRefDom, int type, int elemNodeCount, int elemEdgeCount,
              int sizeRefD0, int *numRefD0, int sizeRefD1, int *numRefD1, int sizeRefF1, int *numRefF1,
              int *elemEdgeRef, float *coorEl[],
              float **MatElem, float *SMbrElem, int *NuDElem, float *uDlem){

    int edgeRef, nk, nl;

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
        edgeRef = elemEdgeRef[i];
        if(edgeRef == nRefDom) continue; //pas de conditon aux limites

        numNaret(type, i+1, nodeIds);

        // Dirichlet Homogène
        for(int j=0; j<sizeRefD0; j++){
            if(edgeRef == numRefD0[j]){ // on cherche l'arrete sur laquelle on est
                NuDElem[nodeIds[0]-1] = 0;
                NuDElem[nodeIds[1]-1] = 0;
            }
        }

        selectPts(2, nodeIds, coorEl, coorAr);

        //Neumann
        for(int j=0; j<sizeRefF1; j++){
            if(edgeRef == numRefF1[j]){
                for(int k=0; k<2; k++){
                    SMbrAret[k] = 0.0;
                    MatAret[k][0] = 0.0;
                    MatAret[k][1] = 0.0;
                }
                intAret(3, 2, coorAr, SMbrAret, MatAret, edgeRef);
                for(int k=0; k<2; k++){
                    nk = nodeIds[k]-1;
                    SMbrElem[nk] += SMbrAret[k];
                    for(int l=0; l<2; l++){
                        nl = nodeIds[l]-1;
                        MatElem[nk][nl] += MatAret[k][l];
                    }
                }
            }
        }

        // Dirichlet non-homogène
        for(int j=0; j<sizeRefD1; j++){
            if(edgeRef == numRefD1[j]){
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
               int *sizeRefD0, int **numRefD0, int *sizeRefD1, int **numRefD1, int *sizeRefF1, int **numRefF1){
    FILE *in = NULL;
    in = fopen(numreffile, "r");
    if (in != NULL) {
        printf("Lecture du fichier de référence : ./%s\n\n", numreffile);

        
        fscanf(in, "%d", nRefDom);
        printf("Numéro de référence du domaine : %d\n", *nRefDom);


        fscanf(in, "%d", sizeRefD0);
        *numRefD0 = malloc(*sizeRefD0 * sizeof(int));

        printf("\nNuméros de référence Dirichlet homogène : ");
        for(int i=0; i<*sizeRefD0; i++){
            fscanf(in, "%d", &((*numRefD0)[i]));
            printf("%d ", (*numRefD0)[i]);
        }
        
        fscanf(in, "%d", sizeRefD1);
        *numRefD1 = malloc(*sizeRefD1 * sizeof(int));

        printf("\nNuméros de référence Dirichlet non homogène : ");
        for(int i=0; i<*sizeRefD1; i++){
            fscanf(in, "%d", &((*numRefD1)[i]));
            printf("%d ", (*numRefD1)[i]);
        }
        
        fscanf(in, "%d", sizeRefF1);
        *numRefF1 = malloc(*sizeRefF1 * sizeof(int));

        printf("\nNuméros de référence Neumann : ");
        for(int i=0; i<*sizeRefF1; i++){
            fscanf(in, "%d", &((*numRefF1)[i]));
            printf("%d ", (*numRefF1)[i]);
        }
        fclose(in);
        printf("\nLa lecture des numref s'est bien passée\n");
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

int assemble(int type, int elemCount, int elemNodeCount, int elemEdgeCount,
             int **elemGlobalNodeIds, float **nodeCoords, int **elemEdgeRefs,
             int nRefDom, int sizeRefD0, int *numRefD0, int sizeRefD1, int *numRefD1, int sizeRefF1, int *numRefF1,
             int nbLign, int nbCoef, 
             float *SecMembr, int *NumDLDir, float *ValDrDir, int *firstAdLi, float *Matrix, int *colIdx,int *followingAdLi){

    int a, b, I, J, nextAd = 1;
    
    int *NuDElem;
    float *SMbrElem, *uDElem;
    float **MatElem;

    float **elemNodeCoords;

    float *Diag, *lowMat;

    SMbrElem = calloc(elemNodeCount, sizeof(float));
    NuDElem = calloc(elemNodeCount,  sizeof(int));
    uDElem = calloc(elemNodeCount, sizeof(float));
    MatElem = alloctab(elemNodeCount, elemNodeCount);

    elemNodeCoords = calloc(elemNodeCount, sizeof(float*));

    Diag = Matrix;
    lowMat = &Matrix[nbLign];

    for(int i=0; i<nbLign; i++){
        Matrix[i] = 0.0;
        SecMembr[i] = 0.0;
        NumDLDir[i] = i+1;
        ValDrDir[i] = 0;
        firstAdLi[i] = 0;
    }

    for(int i=0; i<nbCoef; i++){
        Matrix[nbLign+i] = 0;
        followingAdLi[i] = 0;
        colIdx[i] = 0;
    }

    for(int k=0; k<elemCount; k++){
        selectPts(elemNodeCount, elemGlobalNodeIds[k], nodeCoords, elemNodeCoords);
        cal1Elem(nRefDom, type, elemNodeCount, elemEdgeCount,
                    sizeRefD0, numRefD0, sizeRefD1, numRefD1, sizeRefF1, numRefF1,
                    elemEdgeRefs[k], elemNodeCoords,
                    MatElem, SMbrElem, NuDElem, uDElem);
        // impCalEl(k, elemNodeCount, type, MatElem, SMbrElem, NuDElem, uDElem);

        for(int i=0; i<elemNodeCount; i++){
            I = elemGlobalNodeIds[k][i]-1; //optimisation des opérations sans avoir à créer une nouvelle variable pour I-1

            Diag[I] += MatElem[i][i];

            SecMembr[I] += SMbrElem[i];

            if(NumDLDir[I]>0){ // le noeud I appartient à un bord soumis à une cond Dir non homogène
                ValDrDir[I] = uDElem[i];
                NumDLDir[I] *= NuDElem[i];
            }

            for(int j=0; j<i; j++){
                J = elemGlobalNodeIds[k][j];
                a = I+1; b=J;
                if(a<b) {b=a; a=J;}
                assmat_(&a, &b, &MatElem[i][j], firstAdLi, colIdx, followingAdLi, lowMat, &nextAd);
            }
        }
    }

    firstAdLi[nbLign-1]=nextAd; // on ajoute le nbLign à la fin de firstAdLi 

    free(SMbrElem);
    free(NuDElem);
    free(uDElem);
    freetab(MatElem);

    free(elemNodeCoords);

    return 1;
}