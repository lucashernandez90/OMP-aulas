#include <stdio.h>
#include <omp.h>


int main() {
    int a[5];
    int b[5] = {1, 2, 3, 4, 5};
    int c[5] = {1, 2, 3, 5, 5};

    #pragma omp parallel for
    for (int i = 0; i < 5; i++) {
        a[i] = b[i] + c[i];
    }


    printf("Vetor a (resultado da soma):\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
