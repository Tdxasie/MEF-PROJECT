#include"fcaltools.h"
#include"tabtools.h"
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char const *argv[])
{   
    int t;
    int q;
    float **xk;
    float *omegak;
    // testing ppquad
    printf("-->Testing ppquad:\n\n");
    printf("Quadrangle:\n");
    t=1;
    q = getQ(t);
    omegak = malloc(q*sizeof(float));
    xk = alloctab(q, 2);
    ppquad(t, omegak, xk);
    printf("Valeurs de omegak:\n");
    for(int i=0; i<q; i++){
        printf("[%f] ", omegak[i]);
    }
    printf("\nValeurs de xk:\n");
    for(int i=0; i<q; i++){
        printf("[%f, %f]\n", xk[i][0], xk[i][1]);
    }
    printf("\nTriangle:\n");
    t=2;
    q = getQ(t);
    omegak = malloc(q*sizeof(float));
    freetab(xk);
    xk = alloctab(q, 2);
    ppquad(t, omegak, xk);
    printf("Valeurs de omegak:\n");
    for(int i=0; i<q; i++){
        printf("[%f] ", omegak[i]);
    }
    printf("\nValeurs de xk:\n");
    for(int i=0; i<q; i++){
        printf("[%f, %f]\n", xk[i][0], xk[i][1]);
    }
    printf("\nSegment:\n");
    t=3;
    q = getQ(t);
    omegak = malloc(q*sizeof(float));
    freetab(xk);
    xk = alloctab(q, 1);
    ppquad(t, omegak, xk);
    printf("Valeurs de omegak:\n");
    for(int i=0; i<q; i++){
        printf("[%f] ", omegak[i]);
    }
    printf("\nValeurs de xk:\n");
    for(int i=0; i<q; i++){
        printf("[%f, %f]\n", xk[i][0], xk[i][1]);
    }
    free(omegak);
    freetab(xk);

    //testing fbase
    printf("\n-->Testing calFbase:\n");
    t=1;
    float coords[2] = {1,0};
    float fbase[4];
    // float fbase[2];
    calFbase(t, coords, fbase);
    for(int i=0; i<4; i++){
        printf("[%f]\n", fbase[i]);
    }
    printf("\n");

    //testing calDerFbase
    printf("-->Testing calDerFbase:\n");
    float **derfbase = alloctab(4,2);
    calDerFbase(t, coords, derfbase);
    for(int i=0; i<4; i++){
        printf("[%f , %f] \n", derfbase[i][0], derfbase[i][1]);
    }
    // printf("[%f] \n [%f] \n", derfbase[0][0], derfbase[1][0]);
    // printf("[%f , %f] \n", derfbase[0][0], derfbase[1][0]);

    //testing transFK
    printf("\n-->Testing transFK:\n");

    float a1[2] = {1,0};
    float a2[2] = {2,1};
    float a3[2] = {0,2};
    float a4[2] = {0,0};

    float *a[4] = {a1, a2, a3, a4};
    // float *a[4] = {a1, a2};

    // int nume[2] = {2,3};

    // float *v[2];

    // selectPts(2, nume, a, v);

    float res[2] = {0,0};

    transFK(4, a, fbase, res);
    printf("[%f , %f] \n", res[0], res[1]);

    //testing matJacob
    printf("\n-->Testing matJacob:\n");
    float **jacob = alloctab(2,2);
    matJacob(4, 2, a, derfbase, jacob);
    printf("[%f , %f] \n[%f , %f] \n", jacob[0][0], jacob[0][1], jacob[1][0], jacob[1][1]);
    // printf("[%f , %f] \n", jacob[0][0], jacob[0][1]);

    freetab(derfbase);
    freetab(jacob);
    // testing invertM2x2
    printf("\n-->Testing invertM2x2\n");
    float **mat = alloctab(2,2);
    mat[0][0] = mat[1][1] = 1;
    mat[0][1] = mat[1][0] = 2;
    float **inv = alloctab(2,2);
    float det;
    invertM2x2(mat, inv, &det);
    printf(">Déterminant: %f (valeur attendue: -3)\n", det);
    printf(">Matrice inverse: \n");
    printf("[%f , %f]\n[%f , %f]\n", inv[0][0], inv[0][1], inv[1][0], inv[1][1]);
    freetab(mat);
    freetab(inv);

    // testing numNaret
    printf("\n-->Testing numNaret\n");
    int num[2];
    printf(">Quadrangle: \n");
    t = 1;
    for(int side=1; side<5; side++){
        numNaret(t, side, num);
        printf("Arrete %d contient les noeuds %d et %d\n", side, num[0], num[1]);
    }
    printf(">Triangle: \n");
    t = 2;
    for(int side=1; side<4; side++){
        numNaret(t, side, num);
        printf("Arrete %d contient les noeuds %d et %d\n", side, num[0], num[1]);

    }
    printf(">Segment: \n");
    t = 3;
    numNaret(t, 1, num);
    printf("Arrete %d contient les noeuds %d et %d\n", 1, num[0], num[1]);
    return 0;
}
