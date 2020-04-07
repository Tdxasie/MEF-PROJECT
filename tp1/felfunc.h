int a11(float *x);
int a12(float *x);
int a22(float *x);
int a00(float *x);
int bN(float *x);
int fOmega(float *x);
int fN(float *x);
int uD(float *x);
void W(int nbneel, float *fctbas, float eltdif, float cofvar, float **matelm);
void WW(int nbneel, float *fctbas, float eltdif, float cofvar, float **matelm);
void ADWDW(int nbneel, float **derfctbas, float eltdif, float **cofvar, float **matelm);
void intElem(int t, float *coorEl[]);
void cal1Elem(int nRefDom, int t, int nbneel, int nbaret,
              int nbRefD0, int *numRefD0, int nbRefD1, int *numRefD1, int nbRefF1, int *numRefF1,
              int *nRefArEl,
              float **MatElem, float *SMbrElem, int *NuDElem, float *uDlem);
void lecNumRef(char *numreffile, int *nRefDom,
               int *nbRefD0, int **numRefD0, int *nbRefD1, int **numRefD1, int *nbRefF1, int **numRefF1);
void impCalEl(int K, int typEl, int nbneel, float **MatElem, float *SMbrElem,
              int *NuDElem, float *uDElem);