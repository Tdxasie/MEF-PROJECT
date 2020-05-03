#include"tabtools.h"
#include<stdlib.h>


float **alloctab(int dim1, int dim2) {
    float **ptr;

    ptr = malloc(dim1*sizeof(float *));
    if (ptr != NULL) {
        int i, taille_ligne = dim2*sizeof(float);
        float *tmp = malloc(dim1*taille_ligne);
        if (tmp != NULL) {
            for (i=0; i<dim1; i++) {
                ptr[i] = tmp;
                tmp += dim2;
            }
      }
      else
          ptr = NULL; 
      }
    return(ptr);
}

int **alloctabint(int dim1, int dim2) {
  int **ptr;

  ptr = malloc(dim1*sizeof(int *));
  if (ptr != NULL) {
    int i, taille_ligne = dim2*sizeof(int);
    int *tmp = malloc(dim1*taille_ligne);
    if (tmp != NULL) {
      for (i=0; i<dim1; i++) {
  	    ptr[i] = tmp;
  	    tmp += dim2;
  	  }
    }
    else
      ptr = NULL;
    }
  return(ptr);
}

void freetab(void *ptr) {
    void **ptrT=ptr;
    free(ptrT[0]);
    free(ptr);
}