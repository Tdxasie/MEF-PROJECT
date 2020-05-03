#include"mesh_tools.h"
#include"tabtools.h"
#include"felfunc.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    char * meshfile = "mesh";
    char * numreffile = "numref";

    int t, m, n, p, q;

	int nRefDom, nbRefD0, nbRefD1, nbRefF1;
	int *numRefD0, *numRefD1, *numRefF1;

    float **coord;

    int **ngnel, **nRefAr;

    if(lecfima(meshfile, &t, &n, &coord, &m, &ngnel, &p, &q, &nRefAr)
	   &&
	   lecNumRef(numreffile, &nRefDom, &nbRefD0, &numRefD0, &nbRefD1, &numRefD1, &nbRefF1, &numRefF1)){
        printf("Les données sont en mémoire\n");
        for(int i=0; i<m; i++){
            cal1Elem(nRefDom, t, p, q,
                     nbRefD0, numRefD0, nbRefD1, numRefD1, nbRefF1, numRefF1,
                     nRefAr[i],
                     MatElem, SMbrElem, NuDElem, uDlem);
            impCalEl(i, t, nbneel, MatElem, SMbrElem, NuDElem, uDElem);
        }
    } else {
        printf("Erreur lors de la lecture du meshfile ou du numreffile, verifier le formatage\n");
    }

    freetab(coord);
    freetab(ngnel);
    freetab(nRefAr);

    freetab(numRefD0);
    freetab(numRefD1);
    freetab(numRefF1);

    return 0;
}