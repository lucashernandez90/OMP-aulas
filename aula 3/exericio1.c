#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 1. Construção de uma Árvore Binária Balanceada
typedef struct {
    Node* root;
    int max_value;
} TreeResult;

Node* build_balanced_tree(int arr[], int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    Node* root = create_node(arr[mid]);
    root->left = build_balanced_tree(arr, start, mid - 1);
    root->right = build_balanced_tree(arr, mid + 1, end);
    return root;
}

// 2. Busca do Máximo na Árvore Binária Balanceada
int find_max_in_tree(Node* root) {
    if (root == NULL) return -__INT_MAX__;
    int left_max = find_max_in_tree(root->left);
    int right_max = find_max_in_tree(root->right);
    return fmax(root->value, fmax(left_max, right_max));
}

// 3. Construção da Árvore e Cálculo do Máximo em Paralelo
TreeResult build_and_find_max(int arr[], int start, int end) {
    TreeResult result;
    if (start > end) {
        result.root = NULL;
        result.max_value = -__INT_MAX__;
        return result;
    }
    int mid = (start + end) / 2;
    result.root = create_node(arr[mid]);
    TreeResult left = build_and_find_max(arr, start, mid - 1);
    TreeResult right = build_and_find_max(arr, mid + 1, end);
    result.root->left = left.root;
    result.root->right = right.root;
    result.max_value = fmax(result.root->value, fmax(left.max_value, right.max_value));
    return result;
}

// 4. Algoritmo Utilizando o Teorema de Brent com OpenMP
int brent_max(int arr[], int n) {
    if (n == 0) return -__INT_MAX__;
    int levels = ceil(log2(n));
    int* max_values = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) max_values[i] = arr[i];
    
    for (int l = 0; l < levels; l++) {
        int new_size = (n + 1) / 2;
        int* new_max_values = (int*)malloc(new_size * sizeof(int));
        
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2)
            new_max_values[i / 2] = fmax(max_values[i], max_values[i + 1]);
        
        if (n % 2 == 1)
            new_max_values[new_size - 1] = max_values[n - 1];
        
        free(max_values);
        max_values = new_max_values;
        n = new_size;
    }
    int max_result = max_values[0];
    free(max_values);
    return max_result;
}

// Função de comparação para qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Exemplo de Uso
int main() {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    qsort(arr, n, sizeof(int), compare);

    // 1. Construindo a árvore binária balanceada
    Node* tree_root = build_balanced_tree(arr, 0, n - 1);
    // 2. Encontrando o máximo na árvore
    int max_value_tree = find_max_in_tree(tree_root);
    // 3. Construindo e encontrando o máximo simultaneamente
    TreeResult tree_parallel = build_and_find_max(arr, 0, n - 1);
    // 4. Utilizando o Teorema de Brent com OpenMP
    int max_value_brent = brent_max(arr, n);
    
    printf("Máximo pela busca na árvore: %d\n", max_value_tree);
    printf("Máximo pela construção e busca simultânea: %d\n", tree_parallel.max_value);
    printf("Máximo pelo Teorema de Brent (OMP): %d\n", max_value_brent);
    
    return 0;
}
