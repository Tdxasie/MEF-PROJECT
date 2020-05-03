#include"mesh_tools.h"
#include"tabtools.h"
#include"felfunc.h"
#include"fcaltools.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    char * meshfile = "car1x1t_1";
    char * numreffile = "numref";

    int type, elemCount, nodeCount, elemNodeCount, elemEdgeCount;

	int nRefDom, nbRefD0, nbRefD1, nbRefF1;
	int *numRefD0, *numRefD1, *numRefF1;

    float **coord;
    float **coorEl;

    int **ngnel, **nRefAr;

    int *NuDElem;
    float *SMbrElem, *uDElem;
    float **MatElem;

    if(lecfima(meshfile, &type, &nodeCount, &coord, &elemCount, &ngnel, &elemNodeCount, &elemEdgeCount, &nRefAr)
	   &&
	   lecNumRef(numreffile, &nRefDom, &nbRefD0, &numRefD0, &nbRefD1, &numRefD1, &nbRefF1, &numRefF1)
       ){
        printf("Les données sont en mémoire\n");

        SMbrElem = malloc(elemNodeCount*sizeof(float));
        NuDElem = malloc(elemNodeCount*sizeof(int));
        uDElem = malloc(elemNodeCount*sizeof(float));
        MatElem = alloctab(elemNodeCount, elemNodeCount);
        coorEl = malloc(elemNodeCount*sizeof(float*));

        for(int i=0; i<nodeCount; i++) printf("%f %f\n", coord[i][0], coord[i][1]); //coord[0] ne semble pas exister, je n'arrive pas à savoir pourquoi 

        // for(int i=0; i<elemCount; i++){
        //     selectPts(elemNodeCount, ngnel[i], coord, coorEl);
        //     cal1Elem(nRefDom, type, elemNodeCount, elemEdgeCount,
        //              nbRefD0, numRefD0, nbRefD1, numRefD1, nbRefF1, numRefF1,
        //              nRefAr[i], coorEl,
        //              MatElem, SMbrElem, NuDElem, uDElem);
        //     impCalEl(i, elemNodeCount, type, MatElem, SMbrElem, NuDElem, uDElem);
        // }

        free(SMbrElem);
        free(NuDElem);
        free(uDElem);
        free(coorEl);
        freetab(MatElem);

    } else {
        printf("Erreur lors de la lecture du meshfile ou du numreffile, verifier le formatage\n");
    }

    freetab(coord);
    freetab(ngnel);
    freetab(nRefAr);

    free(numRefD0);
    free(numRefD1);
    free(numRefF1);

    return 0;
}