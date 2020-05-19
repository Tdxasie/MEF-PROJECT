#include"mesh_tools.h"
#include"tabtools.h"
#include"felfunc.h"
#include"fcaltools.h"
#include"forfun.h"
#include"matrix_storage.h"
#include"solver.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main()
{   
    float eps = 1e-6;

    char meshfile[30];
    char *numreffile = "numref";
    int out_filename = -1; // negatif pour print dans la console

    int type, elemCount, elemNodeCount, elemEdgeCount;

    int nRefDom, sizeRefD0, sizeRefD1, sizeRefF1;
    
    int *numRefD0, *numRefD1, *numRefF1;

    float **nodeCoords;

    int **elemGlobalNodeIds;
    int **elemEdgeRefs;

    int nbLign, nbCoef, coefMax;

    int *colIdx, *firstAdLi, *followingAdLi;
    int *colIdxO, *firstAdLiO;

    int *NumDlDir;

    float *Matrix, *SecMembr, *ValDrDir;
    float *MatrixO, *SecMembrO;

    int *Profile;

    float *MatrixP;

    float *U, *UEX;

    nucas = 1;

    omegacas = 1;

    int domain = 1;

    int nbMeshFiles;
    char **MeshFiles;
    char delim[] = "/";

    char *temp;
    char *filename = (char*)malloc(sizeof(char) * 10 );

    printf("Choisissez le numéro d'exemple (1 à 3) : ");
    scanf("%d", &nucas);

    printf("Que faire (entrez le numéro correspondant à l'option): \n");
    printf("1. Lire un seul fichier de maillage\n");
    printf("2. Effectuer une série de calculs sur différentes subdivisions (ne fonctionne pas)\n");
    printf("Votre choix : ");
    int choice = 1;
    scanf("%d", &choice);

    if (choice == 1){
        printf("Entrez le nom du meshfile : ");
        scanf("%s", meshfile);
        
        nbMeshFiles = 1;
        MeshFiles = (char**)malloc(sizeof(char**)*nbMeshFiles);
        MeshFiles[0] = (char*)malloc(sizeof(char) * (strlen(meshfile) + 1 ) );
        strcpy(MeshFiles[0], meshfile);

    } else if (choice == 2){
        printf("Choisissez le domaine : ");
        scanf("%d", &domain);

        char typeStr[8];
        printf("Choisissez la forme de la maille (t ou q) : ");
        scanf("%s", typeStr);

        int ordr = 1;
        printf("Choisissez l'ordre du maillage (1, 2 indisponible pour le moment) : ");
        scanf("%d", &ordr);

        printf("\nTraitement des fichiers suivants : \n\n");

        nbMeshFiles = 6;

        MeshFiles = (char**)malloc(sizeof(char**)*nbMeshFiles);

        for(int i=0; i<nbMeshFiles; i++){
            snprintf(meshfile, 30, "meshes/d%d%s%d_%d", domain, typeStr, ordr, (int)pow(2, i+1));
            printf("%s\n", meshfile);
            MeshFiles[i] = (char*)malloc(sizeof(char) * (strlen(meshfile) + 1 ) );
            strcpy(MeshFiles[i], meshfile);
        }

        printf("\n\n");

        for (int i = 0; i < nbMeshFiles; i++)
        {
            printf("%s\n", MeshFiles[i]);
        }
    }


    

    if(!lecNumRef(numreffile, &nRefDom, &sizeRefD0, &numRefD0, &sizeRefD1, &numRefD1, &sizeRefF1, &numRefF1)) exit(1);
    
    for(int i=0; i<nbMeshFiles; i++){

        /*
        -------------------------------------------------------------------------------------------------------
                Lecture du fichier de maillage
        -------------------------------------------------------------------------------------------------------
        */

        if(!lecfima(MeshFiles[i], &type, &nbLign, &nodeCoords, &elemCount, &elemGlobalNodeIds, &elemNodeCount, &elemEdgeCount, &elemEdgeRefs)) exit(1);


        // ===== On enlève le chemin, on ne garde que le nom de fichier ====

        // filename = (char*)malloc(sizeof(char) * (strlen(MeshFiles[i]) + 1 ) );
        filename = (char*)realloc(filename, sizeof(char) * (strlen(MeshFiles[i]) + 1 ) );

        filename = strtok(MeshFiles[i], delim);

	    while(filename != NULL){
            temp = filename;
            filename = strtok(NULL, delim);
	    }
        filename = temp;


        /*
        -------------------------------------------------------------------------------------------------------
                Assemblage
        -------------------------------------------------------------------------------------------------------
        */

        nbCoef = (nbLign*nbLign - nbLign)/2;

        firstAdLi = calloc(nbLign, sizeof(int));
        SecMembr  = calloc(nbLign, sizeof(float));
        NumDlDir  = calloc(nbLign, sizeof(int));
        ValDrDir  = calloc(nbLign, sizeof(float));

        colIdx         = calloc(nbCoef, sizeof(int));
        followingAdLi  = calloc(nbCoef, sizeof(int));
        Matrix         = calloc(nbCoef+nbLign, sizeof(float));

        assemble(type, elemCount, elemNodeCount, elemEdgeCount,
                 elemGlobalNodeIds, nodeCoords, elemEdgeRefs,
                 nRefDom, sizeRefD0, numRefD0, sizeRefD1, numRefD1, sizeRefF1, numRefF1,
                 nbLign, nbCoef,
                 SecMembr, NumDlDir, ValDrDir, firstAdLi, Matrix, colIdx, followingAdLi);

        free(numRefD0);
        free(numRefD1);
        free(numRefF1);

        freetab(elemGlobalNodeIds);
        freetab(elemEdgeRefs);

        /*
        -------------------------------------------------------------------------------------------------------
                Stockage des matrices & conversions
        -------------------------------------------------------------------------------------------------------
        */


       // ============= STOCKAGE SMD ==================


        char filenameSMD[30] = "SMD_exports/SMD_";
        strcat(filenameSMD, filename);

        exportSMD(filenameSMD, nbLign, SecMembr, NumDlDir, ValDrDir, firstAdLi, Matrix, colIdx, followingAdLi);

        // affsmd_(&nbLign, firstAdLi, colIdx, followingAdLi, Matrix, SecMembr, NumDlDir, ValDrDir);




       // ============= STOCKAGE SMO ==================


        firstAdLiO = calloc(nbLign, sizeof(int));
        SecMembrO = calloc(nbLign, sizeof(float));

        nbCoef = firstAdLi[nbLign-1]-1;

        colIdxO = calloc(nbCoef, sizeof(int));
        MatrixO = calloc(nbLign+nbCoef, sizeof(float));

        char filenameSMO[30] = "SMO_exports/SMO_";
        strcat(filenameSMO, filename);

        SMDtoSMO(filenameSMO, nbLign, firstAdLi, colIdx, followingAdLi, NumDlDir, ValDrDir, Matrix, SecMembr, 
                firstAdLiO, colIdxO, MatrixO, SecMembrO);

        free(NumDlDir);
        free(SecMembr);
        free(ValDrDir);
        free(colIdx);
        free(Matrix);
        free(firstAdLi);
        free(followingAdLi);

        // affsmo_(&nbLign, firstAdLiO, colIdxO, MatrixO, SecMembrO);


        // ============= STOCKAGE Profil ==================


        coefMax = (nbLign*nbLign - nbLign)/2;

        Profile = calloc(nbLign, sizeof(int));
        MatrixP = calloc(nbLign+coefMax, sizeof(float));

        SMOtoPR(nbLign, coefMax, firstAdLiO, colIdxO, MatrixO, Profile, MatrixP);

        free(colIdxO);
        free(MatrixO);
        free(firstAdLiO);

        int terminal=6; 
        // impmpr_(&terminal,&nbLign,Profile, MatrixP, &MatrixP[nbLign]);

        /*
        -------------------------------------------------------------------------------------------------------
                Calcul le la solution
        -------------------------------------------------------------------------------------------------------
        */

        U = calloc(nbLign, sizeof(float));

        CholeskyResol(nbLign, Profile, MatrixP, SecMembrO, eps, U);

        free(Profile);
        free(MatrixP);

        UEX = calloc(nbLign, sizeof(float));

        CalSol(nbLign, nodeCoords, UEX);

        out_filename = -1;

        affsol_(&nbLign, nodeCoords[0], U, UEX, &out_filename);

        free(U);
        free(UEX);

        freetab(nodeCoords);


    }

    for (int i = 0; i < nbMeshFiles; i++)
    {
        free(MeshFiles[i]);
    }
    free(MeshFiles);
    


    return 0;
}