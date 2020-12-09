# include <stdio.h>

float datos [] = { 100.84, 103.17, 104.83, 105.97,\
106.99,107.40,107.48,107.50,\
106.70,106.14,105.23,104.45,103.29,102.66,102.53,\
102.71,103.74,104.75,105.95,106.19,107.24,108.05,\
108.61,107.72,107.14,106.43,105.89,105.18,104.64,\
103.93,103.79,103.76,104.15,104.83,105.77,106.15,\
106.63,107.33,107.47,106.89,106.38,105.48,105.10,\
104.02,104.00,105.20,106.64,107.33,107.47,107.49,\

107.10,106.42,105.28,105.06,105.01,105.00,105.00};  // 57 valores

int numPico = 0;
float senal[]= {0.0,0.0,0.0};
float varAnt=0.0;
float var=0.0;
int flag=0;


int main(){

    for (int i =0; i<57; i++){
        var = datos[i];
        printf("Var Ant: %f\n", varAnt);
        printf("Var: %f\n", var);
        if (var != varAnt){
            senal[0] = senal [1]; senal[1]=senal[2];
            senal[2] = var;
        }
        printf("\n\nA: %f\n", senal[0]);
        printf("B: %f\n", senal[1]);
        printf("C: %f\n", senal[2]);
        if (senal[1]>senal[0]&&senal[1]>senal[2]){
            numPico = numPico+1;
        }
        printf("Picos: %d\n", numPico);
        varAnt=var;
    }

    printf("Num picos: %d", numPico);

    return (numPico);
}