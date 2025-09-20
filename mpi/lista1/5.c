// Elaborar um programa MPI para imprimir um vetor, compartilhado entre os processos.

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int *vetor = (int*) malloc(nprocs * sizeof(int)); // Aloca dinamicamente o vetor com tamanho igual ao número de processos
    int elemento;
    if (rank == 0) {
        // Processo master inicializa o vetor
        for (int i = 0; i < nprocs; i++) {
            vetor[i] = (i + 1) * 10; // Exemplo: vetor = {10, 20, 30, ..., nprocs*10}
        }
    }
    
    // === MPI_BCAST vs MPI_SCATTER: DIFERENÇA FUNDAMENTAL ===
    //
    // MPI_BCAST (usado aqui):
    // - Copia o VETOR COMPLETO para TODOS os processos
    // - Resultado: Todos têm [10, 20, 30, 40, 50]
    // - Analogia: Xerox - todos recebem cópia completa do documento
    // - Uso: Quando todos precisam dos mesmos dados (configurações, matrizes compartilhadas)
    //
    // MPI_SCATTER (usado no exercício 4):
    // - Distribui PEDAÇOS diferentes do vetor para cada processo
    // - Resultado: P0 tem 10, P1 tem 20, P2 tem 30, P3 tem 40, P4 tem 50
    // - Analogia: Pizza - cada um recebe uma fatia diferente
    // - Uso: Quando cada processo precisa trabalhar com dados diferentes (divisão de trabalho)
    //
    // IMPORTANTE: Ambos são operações coletivas - TODOS os processos devem chamar!
    MPI_Bcast(vetor, nprocs, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos antes de imprimir

    // Após MPI_Bcast, TODOS os processos têm o vetor COMPLETO!
    // Qualquer processo pode acessar vetor[0], vetor[1], vetor[2], etc.
    // Aqui cada processo imprime apenas sua posição correspondente (vetor[rank])
    // mas poderiam imprimir qualquer posição ou o vetor inteiro
    printf("Processo %d tem: %d\n", rank, vetor[rank]);
    free(vetor);
    MPI_Finalize();
    return 0;
}