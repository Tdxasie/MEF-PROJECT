#include"mesh_tools.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    char *meshfile = "mesh";

    int t, m, n, p, q;

    float **coord;

    int **ngnel, **nRefAr;

    if(lecfima(meshfile, &t, &n, &coord, &m, &ngnel, &p, &q, &nRefAr)){
        printf("La lecture du meshfile s'est bien passée\n");
        printf("Les données sont en mémoire\n");
        freetab(coord);
        freetab(ngnel);
        freetab(nRefAr);
    } else {
        printf("Erreur lors de la lecture du meshfile, verifier le fomatage\n");
    }

    return 0;
}

int lecfima(char *meshfile, int *t, int *n, float ***pcoord, int *m, int ***pngnel, int *p, int *q, int ***pnRefAr){
    
    FILE *in = NULL;

    in = fopen(meshfile, "r");

    if (in != NULL) {
        fscanf(in, "%d", n);
        *pcoord = alloctab(*n, 2);
        for(int i=0; i<*n; i++){
            fscanf(in, "%f %f", &(*pcoord)[i][0], &(*pcoord)[i][1]);
        }

        fscanf(in, "%d %d %d %d", m, t, p, q);

        *pngnel = alloctabint(*m, *p);
        *pnRefAr = alloctabint(*m, *q);

        switch (*t){
        case 1: //quadrangle
            for(int i=0; i<*m; i++){
                fscanf(in, "%d %d %d %d %d %d %d %d", &(*pngnel)[i][0], &(*pngnel)[i][1], &(*pngnel)[i][2], &(*pngnel)[i][3], &(*pnRefAr)[i][0], &(*pnRefAr)[i][1], &(*pnRefAr)[i][2], &(*pnRefAr)[i][3]);
            }
            break;
        case 2: //triangle
            for(int i=0; i<*m; i++){
                fscanf(in, "%d %d %d %d %d %d", &(*pngnel)[i][0], &(*pngnel)[i][1], &(*pngnel)[i][2], &(*pnRefAr)[i][0], &(*pnRefAr)[i][1], &(*pnRefAr)[i][2]);
            }
            break;
        }
        fclose(in);
        return 1;
    } else {
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }
}