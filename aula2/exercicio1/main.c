#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]){

    int maior = 0; 
    int i= 0;

    int vetor[] = {1 , 2 , 7 , 4 , 5};

    #pragma omp parallel shared(maior, vetor)
    {
        #pragma omp for 
        for(i =0; i < 5; i++)
        {
            if(maior < vetor[i])
            {
                maior = vetor[i];
            }
        }    
    }

    printf("Maior valor: %d\n", maior);

    return 0;
}