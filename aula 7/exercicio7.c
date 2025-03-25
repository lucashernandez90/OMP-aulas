#include <stdio.h>
#include <stdlib.h>

// Função para alocar matriz
int** alocaMatriz(int n) {
    int** matriz = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matriz[i] = (int*)malloc(n * sizeof(int));
    }
    return matriz;
}

// Multiplicação de Matrizes (Divisão e Conquista)
void multiplicaMatrizDC(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }
    
    int newSize = n / 2;
    int **A11 = alocaMatriz(newSize), **A12 = alocaMatriz(newSize), **A21 = alocaMatriz(newSize), **A22 = alocaMatriz(newSize);
    int **B11 = alocaMatriz(newSize), **B12 = alocaMatriz(newSize), **B21 = alocaMatriz(newSize), **B22 = alocaMatriz(newSize);
    int **C11 = alocaMatriz(newSize), **C12 = alocaMatriz(newSize), **C21 = alocaMatriz(newSize), **C22 = alocaMatriz(newSize);
    
    // Dividir matriz em partes menores (pode ser otimizado)
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }
    
    // Recursão para multiplicação das submatrizes
    multiplicaMatrizDC(A11, B11, C11, newSize);
    multiplicaMatrizDC(A12, B21, C12, newSize);
    multiplicaMatrizDC(A11, B12, C21, newSize);
    multiplicaMatrizDC(A12, B22, C22, newSize);
    
    // Combinar resultados
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j] + C12[i][j];
            C[i][j + newSize] = C21[i][j] + C22[i][j];
            C[i + newSize][j] = C21[i][j] + C22[i][j];
            C[i + newSize][j + newSize] = C21[i][j] + C22[i][j];
        }
    }
}

// Algoritmo Mergesort
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Algoritmo de busca binária
int buscaBinaria(int arr[], int left, int right, int x) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] < x)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    mergeSort(arr, 0, n - 1);
    printf("Array ordenado: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    int x = 6;
    int resultado = buscaBinaria(arr, 0, n - 1, x);
    if (resultado != -1)
        printf("Elemento encontrado na posicao %d\n", resultado);
    else
        printf("Elemento nao encontrado\n");
    return 0;
}