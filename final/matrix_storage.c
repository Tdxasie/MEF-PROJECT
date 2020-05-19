#include"forfun.h"
#include<stdio.h>
#include<stdlib.h>

int exportSMD(char *filename, int nblign, 
              float *SecMembr, int *NumDLDir, float *ValDrDir, int *firstAdLi, float *Matrix, int *colIdx,int *followingAdLi){
    
    FILE *out = NULL;
    out = fopen(filename, "wb");

    if(out != NULL){

        printf("\x1B[32mSauvegarde de la S.M.D dans le fichier %s\n\x1B[0m", filename);

        int format = 1; //1: SMD // 2:SMO

        fwrite(&format, sizeof(int), 1, out); //on spécifie qu'on écrit du SMD

        fwrite(&nblign, sizeof(int), 1, out);
        fwrite(SecMembr, sizeof(float), nblign, out);
        fwrite(NumDLDir, sizeof(int), nblign, out);
        fwrite(ValDrDir, sizeof(float), nblign, out);
        fwrite(firstAdLi, sizeof(int), nblign, out);

        int nbCoef = firstAdLi[nblign-1]-1;

        fwrite(colIdx, sizeof(int), nbCoef, out);
        fwrite(followingAdLi, sizeof(int), nbCoef, out);
        fwrite(Matrix, sizeof(float), nblign+nbCoef, out);

        fclose(out);

    } else {
        printf("\x1B[31mErreur lors de l'ouverture du fichier SMD_export\n\x1B[0m");
        return 0;
    }
    return 1;
}

int importSMD(char *filename, int *nblign,
              float **SecMembr, int **numDlDir, float **ValDrDir, int **firstAdLi, float **Matrix, int **colIdx,int **followingAdLi){
        
    FILE *in = NULL;
    in = fopen(filename, "rb");

    if(in != NULL){

        int format;

        fread(&format, sizeof(int), 1, in);

        if (format != 1){
            printf("\x1B[31mErreur lors de l'import SMD, format incorrect\n\x1B[0m");
            return 0;
        }
    
        fread(nblign, sizeof(int), 1, in);

        *firstAdLi = calloc(*nblign, sizeof(int));
        *SecMembr = calloc(*nblign, sizeof(float));
        *numDlDir = calloc(*nblign, sizeof(int));
        *ValDrDir = calloc(*nblign, sizeof(float));

        fread(*SecMembr, sizeof(float), *nblign, in);
        fread(*numDlDir, sizeof(int), *nblign, in);
        fread(*ValDrDir, sizeof(float), *nblign, in);
        fread(*firstAdLi, sizeof(int), *nblign, in);

        int nbCoef = (*firstAdLi)[*nblign-1]-1;

        *colIdx = calloc(nbCoef, sizeof(int));
        *followingAdLi = calloc(nbCoef, sizeof(int));
        *Matrix = calloc(*nblign+nbCoef, sizeof(float));

        fread(*colIdx, sizeof(int), nbCoef, in);
        fread(*followingAdLi, sizeof(int), nbCoef, in);
        fread(*Matrix, sizeof(float), *nblign+nbCoef, in);

        fclose(in);

    } else {
        printf("\x1B[31mErreur lors de l'ouverture du fichier SMD_export\n\x1B[0m");
        return 0;
    }
    return 1;
}

int SMDtoSMO(char *filename, int nblign, int *firstAdLi, int *colIdx, int *followindAdLi, int *NumDLDIR, float *ValDrDir, float *MatrixD, float *SecMembrD,
             int *firstAdLiO, int *colIdxO, float *MatrixO, float *SecMembrO){

    cdesse_(&nblign, firstAdLi, colIdx, followindAdLi, MatrixD, SecMembrD, NumDLDIR, ValDrDir, firstAdLiO, colIdxO, MatrixO, SecMembrO);

    FILE *out = NULL;
    out = fopen(filename, "wb");

    if(out != NULL){

        printf("\x1B[32mSauvegarde de la S.M.O dans le fichier %s\n\x1B[0m", filename);

        int format = 2; //1: SMD // 2:SMO

        fwrite(&format, sizeof(int), 1, out); //on spécifie qu'on écrit du SMD

        fwrite(&nblign, sizeof(int), 1, out);
        fwrite(SecMembrO, sizeof(float), nblign, out);
        fwrite(firstAdLiO, sizeof(int), nblign, out);

        int nbCoef = firstAdLiO[nblign-1]-1;

        fwrite(colIdxO, sizeof(int), nbCoef, out);
        fwrite(MatrixO, sizeof(float), nblign+nbCoef, out);

        fclose(out);

    } else {
        printf("\x1B[31mErreur lors de l'ouverture du fichier SMD_export\n\x1B[0m");
        return 0;
    }

    return 1;
}

int importSMO(char *filename, int *nblign,
              float **SecMembrO, int **firstAdLiO, float **MatrixO, int **colIdxO){
        
    FILE *in = NULL;
    in = fopen(filename, "rb");

    if(in != NULL){

        int format;

        fread(&format, sizeof(int), 1, in);

        if (format != 2){
            printf("\x1B[31mErreur lors de l'import SMO, format incorrect\n\x1B[0m");
            return 0;
        }
    
        fread(nblign, sizeof(int), 1, in);

        *SecMembrO = calloc(*nblign, sizeof(float));
        *firstAdLiO = calloc(*nblign, sizeof(int));

        fread(*SecMembrO, sizeof(float), *nblign, in);
        fread(*firstAdLiO, sizeof(int), *nblign, in);

        int nbCoef = (*firstAdLiO)[*nblign-1]-1;

        *colIdxO = calloc(nbCoef, sizeof(int));
        *MatrixO = calloc(*nblign+nbCoef, sizeof(float));

        fread(*colIdxO, sizeof(int), nbCoef, in);
        fread(*MatrixO, sizeof(float), *nblign+nbCoef, in);

        fclose(in);

    } else {
        printf("\x1B[31mErreur lors de l'ouverture du fichier SMO_export\n\x1B[0m");
        return 0;
    }
    return 1;
}

void SMOtoPR(int nblign, int coefMax, int *firstAdLiO, int *colIdxO, float *MatrixO, int *Profile, float *MatrixP){

    for(int i=0; i<nblign; i++) MatrixP[i] = MatrixO[i];
    
    for(int i=nblign; i<nblign+coefMax; i++) MatrixP[i] = 0.0;

    int elOnLign;
    
    Profile[0] = 1;

    for(int i = 1; i < nblign; i++){
        if (firstAdLiO[i-1] == firstAdLiO[i]){
            Profile[i] = Profile[i-1];
        } else {
            elOnLign = i+1 - colIdxO[firstAdLiO[i-1]-1]; // lign - col of first el = elOnLign
            Profile[i] = Profile[i-1] + elOnLign;

            for(int k = firstAdLiO[i-1]; k < firstAdLiO[i]; k++){
 				MatrixP[nblign + Profile[i] + colIdxO[k-1]-i-2] = MatrixO[nblign+k-1]; //it just works
  		    }
        }
    }
    MatrixP[nblign + Profile[nblign-1] - 1] = MatrixO[nblign + firstAdLiO[nblign-1]-1]; //last el 


}