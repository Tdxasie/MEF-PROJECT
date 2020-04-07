#include"mesh_tools.h"
#include"tabtools.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    char * meshfile = "mesh";

    int t, m, n, p, q;

    float **coord;

    int **ngnel, **nRefAr;

    if(lecfima(meshfile, &t, &n, &coord, &m, &ngnel, &p, &q, &nRefAr)){
        printf("La lecture du meshfile s'est bien passée\n");
        printf("Les données sont en mémoire\n");
    } else {
        printf("Erreur lors de la lecture du meshfile, verifier le fomatage\n");
    }

    freetab(coord);
    freetab(ngnel);
    freetab(nRefAr);

    return 0;
}