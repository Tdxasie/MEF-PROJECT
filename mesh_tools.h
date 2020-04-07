void etiqAr(int t, int n1, int n2, int nrefdom, int *nrefcot, int m, int q, int **nRefAr);

int lecfima(char *meshfile, int *t, int *n, float ***pcoord, int *m, int ***pngnel, int *p, int *q, int ***pnRefAr);

float **alloctab(int dim1, int dim2);

int **alloctabint(int dim1, int dim2);

void freetab(void *ptr);