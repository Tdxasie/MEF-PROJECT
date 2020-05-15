int getQ(int t);
void ppquad(int t, float * omegak, float ** xk);
void calFbase(int t, float *coords, float *fbase);
void calDerFbase(int t, float *coords, float **derfbase);
void transFK(int nb, float *coords[], float *fbase, float *im);
void matJacob(int nb, int d, float *coords[], float **derfbase, float **jacob);
void invertM2x2(float **mat, float **inv, float *det);
void numNaret(int t, int side, int *num);
void selectPts(int nb, int num[], float *coordEns[], float *coordSel[]);
float norm2(float **jacob);