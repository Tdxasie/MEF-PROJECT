#include"mesh_tools.h"
#include"tabtools.h"
#include<stdio.h>
#include<stdlib.h>

void etiqAr(int t, int n1, int n2, int nrefdom, int *nrefcot, int m, int q, int **nRefAr){
    // m : elemCount
    // q : edgeCount
    // n1 : nodeCount on side 1
    // n1 : nodeCount on side 2

    // init mid
    for(int i=0; i<m; i++) { 
        for(int j=0; j<q; j++){
            nRefAr[i][j] = nrefdom;
        }
    }
    // init each edge in its own loop
    switch (t){
    case 1:
        // edge 2
        for(int i=(n1-1); i<=m; i+=(n1-1)){
            nRefAr[i-1][0] = nrefcot[1];
        }
        // edge 3
        for(int i=m-(n1-1)+1; i<=m; i++){
            nRefAr[i-1][1] = nrefcot[2];
        }
        // edge 4
        for(int i=1; i<=m; i+=(n1-1)){
            nRefAr[i-1][2] = nrefcot[3];
        }
        // edge 1
        for(int i=1; i<=(n1-1); i++){
            nRefAr[i-1][3] = nrefcot[0];
        }
        break;
    case 2:
        // edge 2
        for(int i=(n1-1)*2; i<=m; i+=(n1-1)*2){
            nRefAr[i-1][1] = nrefcot[1];
        }
        // edge 3
        for(int i=(n1*n2)+1; i<=m; i+=2){
            nRefAr[i-1][2] = nrefcot[2];
        }
        // edge 4
        for(int i=1; i<m; i+=(n1-1)*2){
            nRefAr[i-1][1] = nrefcot[3];
        }
        // edge 1
        for(int i=1; i<(n1-1)*2; i+=2){
            nRefAr[i-1][2] = nrefcot[0];
        }
        break;
    }
}

int lecfima(char *meshfile, int *t, int *n, float ***pcoord, int *m, int ***pngnel, int *p, int *q, int ***pnRefAr){
    // t : type of el
    //    1 : quadrangle
    //    2 : triangle
    //    3 : line
    // n : nodeCount
    // m : elemCount
    // p : elemNodeCount
    // q : elemEdgeCount
    
    FILE *in = NULL;

    in = fopen(meshfile, "r");

    if (in != NULL) {

        printf("\n\nLecture du fichier de maillage : %s\n", meshfile);

        fscanf(in, "%d", n);
        *pcoord = alloctab(*n, 2);
        for(int i=0; i<*n; i++){
            fscanf(in, "%f %f", &((*pcoord)[i][0]), &((*pcoord)[i][1]));
        }
        fscanf(in, "%d %d %d %d", m, t, p, q);

        *pngnel = alloctabint(*m, *p);
        *pnRefAr = alloctabint(*m, *q);

        switch (*t){
        case 1: //quadrangle
            for(int i=0; i<*m; i++){
                fscanf(in, "%d %d %d %d %d %d %d %d", 
                &(*pngnel)[i][0], &(*pngnel)[i][1], &(*pngnel)[i][2], &(*pngnel)[i][3], 
                &(*pnRefAr)[i][0], &(*pnRefAr)[i][1], &(*pnRefAr)[i][2], &(*pnRefAr)[i][3]);
            }
            break;
        case 2: //triangle
            for(int i=0; i<*m; i++){
                fscanf(in, "%d %d %d %d %d %d", 
                &(*pngnel)[i][0], &(*pngnel)[i][1], &(*pngnel)[i][2], 
                &(*pnRefAr)[i][0], &(*pnRefAr)[i][1], &(*pnRefAr)[i][2]);
            }
            break;
        }
        fclose(in);
        printf("\x1B[32mLa lecture du meshfile s'est bien passée \x1B[0m \n");
        return 1;
    } else {
        printf("\x1B[31mErreur lors de l'ouverture du fichier\x1B[0m\n");
        return 0;
    }
}