#include <stdio.h>
#include <omp.h>


int dobro(int x) { 
    return 2 * x; 
}

// Função map que aplica 'func' a todos os elementos do vetor
void map(int vetor[], int tamanho, int (*func)(int)) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = func(vetor[i]); 
    }
}

int main() {
    int vetor[5] = {1, 2, 3, 4, 5};

    printf("Vetor original:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    // Aplica a função dobro a todos os elementos do vetor
    map(vetor, 5, dobro);

    printf("Vetor após map(dobro):\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    return 0;
}
