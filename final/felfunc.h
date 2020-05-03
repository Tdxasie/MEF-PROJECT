int a11(float *x);
int a12(float *x);
int a22(float *x);
int a00(float *x);
int bN(float *x);
int fOmega(float *x);
int fN(float *x);
int uD(float *x);
void getVarCofMat(float *im, float **matcofvar);
void W(int nbneel, float *fctbas, float eltdif, float cofvar, float *vecelm);
void WW(int nbneel, float *fctbas, float eltdif, float cofvar, float **matelm);
void ADWDW(int nbneel, float **derfctbas, float  **iJac, float eltdif, float **cofvar, float **matelm);
int intElem(int type, int elemNodeCount, float *coorEl[], float *SMbrElem, float **MatElem);
int intAret(int type, float *coorEl[], float *SMbrElem, float **MatElem);
int cal1Elem(int nRefDom, int t, int elemNodeCount, int elemEdgeCount,
              int nbRefD0, int *numRefD0, int nbRefD1, int *numRefD1, int nbRefF1, int *numRefF1,
              int *nRefArEl, float *coorEl[],
              float **MatElem, float *SMbrElem, int *NuDElem, float *uDlem);
int lecNumRef(char *numreffile, int *nRefDom,
               int *nbRefD0, int **numRefD0, int *nbRefD1, int **numRefD1, int *nbRefF1, int **numRefF1);
void impCalEl(int K, int elemNodeCount, int typEl, float **MatElem, float *SMbrElem,
              int *NuDElem, float *uDElem);