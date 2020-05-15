int exportSMD(char *filename, int nbLign,
              float *SecMembr, int *NumDLDir, float *ValDrDir, int *firstAdLi, float *matrix, int *colIdx,int *followingAdLi);
int importSMD(char *filename, int *nbLign,
              float **SecMembr, int **NumDLDir, float **ValDrDir, int **firstAdLi, float **matrix, int **colIdx,int **followingAdLi);
int SMDtoSMO(char *filename, int nbLign, int *firstAdLi, int *colIdx, int *followindAdLi, int *NumDLDIR, float *ValDrDir, float *MatrixD, float *SecMembrD,
             int *firstAdLiO, int *colIdxO, float *MatrixO, float *SecMembrO);
int importSMO(char *filename, int *nbLign,
              float **SecMembrO, int **firstAdLiO, float **MatrixO, int **colIdxO);
void SMOtoPR(int nbLign, int nbCoef, int *firstAdLiO, int *colIdxO, float *MatrixO, int *Profile, float *MatrixP);