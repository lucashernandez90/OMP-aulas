#include <stdio.h>
#include <omp.h>


int main() {
    int vetor[5] = {8, 1, 7, 2, 5};
    int menor = vetor[0]; 

    #pragma omp parallel for reduction(min: menor)
    for (int i = 1; i < 5; i++) {  
        if (vetor[i] < menor) {
            menor = vetor[i];
        }
    }

    printf("Menor elemento: %d\n", menor);
    return 0;
}
