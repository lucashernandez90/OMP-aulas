#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void iterative_mergesort(int arr[], int n) {
    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int mid = i + width - 1;
            int right = (i + 2 * width - 1 < n) ? i + 2 * width - 1 : n - 1;
            if (mid < right) merge(arr, i, mid, right);
        }
    }
}

void recursive_mergesort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        recursive_mergesort(arr, left, mid);
        recursive_mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallel_mergesort(int arr[], int left, int right, int threads) {
    if (threads <= 1 || left >= right) {
        recursive_mergesort(arr, left, right);
    } else {
        int mid = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_mergesort(arr, left, mid, threads / 2);
            #pragma omp section
            parallel_mergesort(arr, mid + 1, right, threads / 2);
        }
        merge(arr, left, mid, right);
    }
}

void wrapper_recursive_mergesort(int arr[], int left, int right) {
    recursive_mergesort(arr, left, right);
}

void wrapper_parallel_mergesort(int arr[], int left, int right, int threads) {
    parallel_mergesort(arr, left, right, threads);
}

void wrapper_iterative_mergesort(int arr[], int left, int right) {
    iterative_mergesort(arr, right + 1);
}

double measure_time(void (*sort_function)(int[], int, int), int arr[], int n) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    
    clock_t start = clock();
    sort_function(copy, 0, n - 1);
    double time_taken = (double)(clock() - start) / CLOCKS_PER_SEC;
    
    free(copy);
    return time_taken;
}

int main() {
    int n = 100000;
    int num_tests = 10;
    int thread_counts[] = {1, 2, 4, 8, 16};
    
    int *arr = malloc(n * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
    
    printf("\nTamanho do vetor: %d\n", n);
    
    double seq_time = 0;
    for (int i = 0; i < num_tests; i++) seq_time += measure_time(wrapper_iterative_mergesort, arr, n);
    printf("Tempo médio (Iterativo): %.6f seg\n", seq_time / num_tests);
    
    double rec_time = 0;
    for (int i = 0; i < num_tests; i++) rec_time += measure_time(wrapper_recursive_mergesort, arr, n);
    printf("Tempo médio (Recursivo): %.6f seg\n", rec_time / num_tests);
    
    for (int i = 0; i < 5; i++) {
        double par_time = 0;
        for (int j = 0; j < num_tests; j++) {
            int *copy = malloc(n * sizeof(int));
            for (int k = 0; k < n; k++) copy[k] = arr[k];
            clock_t start = clock();
            parallel_mergesort(copy, 0, n - 1, thread_counts[i]);
            par_time += (double)(clock() - start) / CLOCKS_PER_SEC;
            free(copy);
        }
        printf("Tempo médio (%d threads): %.6f seg\n", thread_counts[i], par_time / num_tests);
    }
    
    free(arr);
    return 0;
}
