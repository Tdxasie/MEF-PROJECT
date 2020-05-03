#include"../mesh_tools.h"
#include"../tabtools.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    // fichiers
    FILE* out = NULL;
    FILE* in = NULL;

    // variables de configuration
    float a, b, c, d;
    int t, n1, n2, r0, r1, r2, r3, r4;

    // variables de l'écriture
    int m, p, q;

    in = fopen("conf", "r");
    out = fopen("mesh", "w");

    if (out != NULL || in != NULL) {
        // lecture du fichier de configuration du maillage
        fscanf(in, "%f%f%f%f\n%d\n%d%d\n%d%d%d%d%d", &a, &b, &c, &d, &n1, &n2, &t, &r0, &r1, &r2, &r3, &r4);

        int nrefcot[4] = {r1, r2, r3, r4};

        if (t == 1){ // quadrangle 
            p = 4;
            q = 4;
            m = (n1-1) * (n2-1);
        }
        if (t == 2){ // triangle
            p = 3;
            q = 3;
            m = (n1-1) * (n2-1) * 2;
        }
        
        // écriture du fichier
        fprintf(out, "%d\n", n1 * n2); // nb de nodes 
        for(int j=0; j<n2; j++){
            for(int i=0; i<n1; i++){
                fprintf(out, "%f %f\n", i*b/(n1-1) + a, j*d/(n2-1) + c);
            }
        }

        fprintf(out, "%d %d %d %d\n", m, t, p, q);

        int **nRefAr = alloctabint(m, q);

        etiqAr(t, n1, n2, r0, nrefcot, m, q, nRefAr);

        int k = 0;

        switch (t) {
        case 1: //quadrangle
            for(int j=2; j<(n2*n1)-n2; j=j+n2){
                for(int i=j; i<n1+(j-1); i++){
                    fprintf(out, "%d %d %d %d %d %d %d %d \n", i, i+n2, i+n2-1, i-1, 
                    nRefAr[k][0], nRefAr[k][1], nRefAr[k][2], nRefAr[k][3]);
                    k++;
                }
            }
            break;
        case 2: //triangle
            for(int j=2; j<(n2*n1)-n2; j=j+n2){
                for(int i=j; i<n1+(j-1); i++){
                    fprintf(out, "%d %d %d %d %d %d \n", i, i+n2-1, i-1, nRefAr[k][0], nRefAr[k][1], nRefAr[k][2]);
                    fprintf(out, "%d %d %d %d %d %d \n", i+n2-1, i, i+n2, nRefAr[k+1][0], nRefAr[k+1][1], nRefAr[k+1][2]);
                    k+=2;   
                }
            }
            break;
        }

        freetab(nRefAr);
        fclose(in);
        fclose(out);
    } else {
        printf("Erreur dans l'ouverture du fichier");
    }
    return 0;
}