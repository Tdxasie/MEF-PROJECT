#include"fcaltools.h"
#include"tabtools.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int getQ(int t){
    switch (t)
    {
    case 1:
        return 9;
        break;
    case 2:
    case 3:
        return 3;
        break;
    default:
        printf("Erreur getQ: t inconnu");
        return 0;
        break;
    }
}

void ppquad(int t, float *omegak, float **xk) { //type int * for returning NULL in default case
    switch (t)
    {
    case 1: //quadrangle
        for(int i=0; i<4; i++){
            omegak[i] = 1.0/36.0;
        }
        for(int i=4; i<8; i++){
            omegak[i] = 1.0/9.0;
        }
        omegak[8] = 4.0/9.0;
        xk[0][1] = xk[2][0] = xk[3][0] = xk[3][1] = xk[6][0] = xk[7][1] = 0.0;
        xk[0][0] = xk[1][0] = xk[1][1] = xk[2][1] = xk[4][0] = xk[5][1] = 1.0;
        xk[4][1] = xk[5][0] = xk[6][1] = xk[7][0] = xk[8][0] = xk[8][1] = 1.0/2.0;
        break;
    case 2: //triangle
        for(int i=0; i<3; i++){
            omegak[i] = 1.0/6.0;
        }
        xk[0][0] = xk[0][1] = xk[1][1] = xk[2][0] = 1.0/2.0;
        xk[1][0] = xk[2][1] = 0.0;
        break;
    case 3: //line
        omegak[0] = omegak[1] = 1.0/6.0;
        omegak[2] = 2.0/3.0;
        xk[0][0] = 1.0;
        xk[1][0] = 0.0;
        xk[2][0] = 1.0/2.0;
        break;
    default:
        printf("Erreur ppquad: t inconnu");
        break;
    }
}

void calFbase(int t, float *coords, float *fbase){ //type int * for returning NULL in default case
    switch (t)
    {
    case 1: //quadrangle
        fbase[0] = coords[0]*(1-coords[1]);       //x(1-y)
        fbase[1] = coords[0]*coords[1];           //xy
        fbase[2] = (1-coords[0])*coords[1];       //(1-x)y
        fbase[3] = (1-coords[0])*(1-coords[1]);   //(1-x)(1-y)
        break;
    case 2: //triangle
        fbase[0] = coords[0];                     //x
        fbase[1] = coords[1];                     //y
        fbase[2] = 1-coords[0]-coords[1];         //1-x-y
        break;
    case 3: //line
        fbase[0] = coords[0];                     //x
        fbase[1] = 1-coords[0];                   //1-x
        break;
    default:
        printf("Erreur calFBase: t inconnu");
        break;
    }
}

void calDerFbase(int t, float *coords, float **derfbase){ //type int * for returning NULL in default case
    switch (t)
    {
    case 1:
        derfbase[0][0] = 1-coords[1]   ; derfbase[0][1] = -coords[0];
        derfbase[1][0] =  coords[1]    ; derfbase[1][1] = coords[0];
        derfbase[2][0] = -coords[1]    ; derfbase[2][1] = 1-coords[0];
        derfbase[3][0] =  coords[1]-1  ; derfbase[3][1] = coords[0]-1;
        break;
    case 2:
        derfbase[0][0] = 1  ; derfbase[0][1] = 0;
        derfbase[1][0] = 0  ; derfbase[1][1] = 1;
        derfbase[2][0] = -1 ; derfbase[2][1] = -1;
        break;
    case 3:
        derfbase[0][0] = 1 ;
        derfbase[1][0] = -1;
        break;
    default:
        printf("Erreur calDerFBase: t inconnu");
        break;
    }
}

void transFK(int nb, float *coords[], float *fbase, float *im){
    for(int k=0; k<2; k++){
        im[k] = 0;
        for(int i=0; i<nb; i++){
            im[k] += coords[i][k] * fbase[i];
        }
    }
    
}

void matJacob(int nb, int d, float *coords[], float **derfbase, float **jacob){
    for(int j=0; j<2; j++){
        for(int k=0; k<d; k++){
            jacob[j][k] = 0;
            for(int i=0; i<nb; i++){
                jacob[j][k] += coords[i][j] * derfbase[i][k];
            }
        }
    }
}

void invertM2x2(float **mat, float **inv, float *det){
    *det = mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
    inv[0][0] =  mat[1][1]/(*det);
    inv[0][1] = -mat[0][1]/(*det);
    inv[1][0] = -mat[1][0]/(*det);
    inv[1][1] =  mat[0][0]/(*det);
}

void numNaret(int t, int side, int *res){ //type int * for returning NULL in default case
    switch (t)
    {
    case 1: //quadrangle
        switch (side)
        {
        case 1: res[0] = 1 ; res[1] = 2; break;
        case 2: res[0] = 2 ; res[1] = 3; break;
        case 3: res[0] = 3 ; res[1] = 4; break;
        case 4: res[0] = 4 ; res[1] = 1; break;         
        default: printf("Erreur numNaret: side inconnu"); break;
        }
        break;
    case 2: // triangle
        switch (side)
        {
        case 1: res[0] = 1 ; res[1] = 2; break; 
        case 2: res[0] = 2 ; res[1] = 3; break;      
        case 3: res[0] = 3 ; res[1] = 1; break;       
        default: printf("Erreur numNaret: side inconnu"); break;
        }
        break;
    case 3: //line
        res[0] = 1 ; res[1] = 2; 
        break;
    default:
        printf("Erreur numNaret: t inconnu");
        break;
    }
}

void selectPts(int nb, int num[], float *coordEns[], float *coordSel[]){
    for(int i=0; i<nb; i++){
        coordSel[i] = coordEns[num[i]-1];
    }
}

float norm2(float **jacob){
    // return sqrt(pow(jacob[0][0], 2) + pow(jacob[1][0], 2));
    return sqrt(jacob[0][0]*jacob[0][0] + jacob[1][0]*jacob[1][0]);
}