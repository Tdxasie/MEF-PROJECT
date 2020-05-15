#include"forfun.h"
#include<stdio.h>
#include<stdlib.h>

int exportSMD(char *filename, int nblign, 
              float *SecMembr, int *NumDLDir, float *ValDrDir, int *firstAdLi, float *Matrix, int *colIdx,int *followingAdLi){
    
    FILE *out = NULL;
    out = fopen(filename, "wb");

    if(out != NULL){

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
        printf("Erreur lors de l'ouverture du fichier SMD_export\n");
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
            printf("Erreur lors de l'import SMD, format incorrect\n");
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
        printf("Erreur lors de l'ouverture du fichier SMD_export\n");
        return 0;
    }
    return 1;
}

int SMDtoSMO(char *filename, int nblign, int *firstAdLi, int *colIdx, int *followindAdLi, int *NumDLDIR, float *ValDrDir, float *MatrixD, float *SecMembrD,
             int *firstAdLiO, int *colIdxO, float *MatrixO, float *SecMembrO){

    // cdesse_(&nblign, firstAdLi, colIdx, followindAdLi, MatrixD, SecMembrD, NumDLDIR, ValDrDir, firstAdLiO, colIdxO, MatrixO, SecMembrO);

    FILE *out = NULL;
    out = fopen(filename, "wb");

    if(out != NULL){

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
        printf("Erreur lors de l'ouverture du fichier SMD_export\n");
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
            printf("Erreur lors de l'import SMO, format incorrect\n");
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
        printf("Erreur lors de l'ouverture du fichier SMD_export\n");
        return 0;
    }
    return 1;
}

void SMOtoPR(int nblign, int coefMax, int *firstAdLiO, int *colIdxO, float *MatrixO, int *Profile, float *MatrixP){

    for(int i=0; i<nblign; i++) MatrixP[i] = MatrixO[i];
    
    for(int i=nblign; i<nblign+coefMax; i++) MatrixP[i] = 0.0;

    int lign, maxAdditions, addedThisLine, col, totalAdded = 0, offset = 0, ind = 0;

    for(lign=0; lign<nblign; lign++){
        //add el to profile
        Profile[lign] = firstAdLiO[lign]+offset;
        maxAdditions = lign+1; //maximum number of additions possible on that lign i.e lign length
        

        // for example a line like this |0A0B00| 
        // colIdx at this stage is :    [ 2 4  ]
        // elsToAdd is 2
        // matrixP need to have :       [ A0B00]       

        addedThisLine = 0;
        for(col=1; col<=maxAdditions; col++){
            if (col == colIdxO[totalAdded]){                    //we're here |0A0B00| -> add the elements
                                                                //           |-#-#--|
                ind = totalAdded+offset;                        // offset accounting for the eventual zeros
                MatrixP[nblign+ind] = MatrixO[nblign+totalAdded];
                
                totalAdded++;
                addedThisLine++;
            } else if (addedThisLine > 0){                      //we're here |0A0B00| -> fill the gaps with zeros
                                                                //           |--#-##|
                //add zero
                offset++;
            } else {                                            //we're here |0A0B00| -> don't add anything
                                                                //           |#-----|
                //do nothing
            }
        }

    }



}