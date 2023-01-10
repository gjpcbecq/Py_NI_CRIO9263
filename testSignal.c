#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define PI 3.1415926
#define PRINTLINE printf("___________________________________________\n");
#define DISP(a) printf("checking >>> %s\n", a);
#define uInt64 unsigned long long

void mySinus(double *x, uInt64 nObs, double fS, double f0, double a, double phi){
    uInt64 i; 
    for(i=0; i<nObs; i++){
        x[i] = a * sin(2 * PI * (double) i * f0 / fS + phi);
    }
}

int printSignal(double *x, uInt64 nObs){
    uInt64 i; 
    if (nObs < 10){
        printf("[");
        for(i=0; i<nObs-1; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("%+5.3f]", x[nObs-1]);
    }
    else {
        printf("[");
        for(i=0; i<5; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("..."); 
        for(i=nObs-5; i<nObs-1; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("%+5.3f]", x[nObs-1]);
    }
    printf(" (%i samples)\n", nObs);
    return 0; 
}

int main(){
    uInt64 nObs = 100000;
    double *x1 = malloc(nObs * sizeof(double)); 
    double *x2 = malloc(nObs * sizeof(double)); 
    double *x3 = malloc(nObs * sizeof(double)); 
    double *x4 = malloc(nObs * sizeof(double)); 
    double fS = 10000; 
    double f1 = 0.5; 
    double a1 = 0.05; 
    double phi1 = 0; 
    double f2 = 1;  
    double a2 = 0.050; 
    double phi2 = PI / 2;
    double f3 = 1;  
    double a3 = 0.050; 
    double phi3 = PI / 2;
    double f4 = 1;  
    double a4 = 0.050; 
    double phi4 = PI / 2;
    PRINTLINE
    mySinus(x1, nObs, fS, f1, a1, phi1); 
    mySinus(x2, nObs, fS, f2, a2, phi2); 
    mySinus(x3, nObs, fS, f3, a3, phi3); 
    mySinus(x4, nObs, fS, f4, a4, phi4); 
    printSignal(x1, nObs); 
    printSignal(x2, nObs);
    printSignal(x3, nObs);
    printSignal(x4, nObs);
    free(x1);
    free(x2); 
    free(x3); 
    free(x4); 
    PRINTLINE
    return 0; 
}


