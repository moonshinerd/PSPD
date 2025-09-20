// Elaborar um programa MPI que promova a distribuição das partes de um vetor entre todos os processos instanciados (considerar que o número de processos = número de elementos do vetor)

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
    
    // === DISTRIBUIÇÃO DOS ELEMENTOS DO VETOR ===
    // MPI_Scatter: "espalha" pedaços de um vetor do processo raiz para todos os processos
    // 
    // Analogia: É como um professor distribuindo folhas de exercício
    // - O professor (processo 0) tem uma pilha de folhas [10, 20, 30, 40]
    // - Ele dá 1 folha para cada aluno (processo)
    // - Aluno 0 recebe folha 10, Aluno 1 recebe folha 20, etc.
    //
    // Parâmetros explicados:
    // vetor      → dados de origem (só existe no processo 0)
    // 1          → quantos elementos cada processo vai RECEBER
    // MPI_INT    → tipo de dado sendo enviado
    // &elemento  → onde cada processo vai GUARDAR seu pedaço
    // 1          → quantos elementos cada processo espera receber
    // MPI_INT    → tipo de dado que está sendo recebido  
    // 0          → qual processo tem os dados originais (processo raiz)
    // MPI_COMM_WORLD → grupo de processos participando
    MPI_Scatter(vetor, 1, MPI_INT, &elemento, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Após o MPI_Scatter, cada processo tem SEU próprio elemento:
    // Processo 0: elemento = 10
    // Processo 1: elemento = 20  
    // Processo 2: elemento = 30
    // ... e assim por diante
    
    // Cada processo imprime o elemento que recebeu do MPI_Scatter
    printf("Processo %d recebeu o elemento %d\n", rank, elemento);
    
    // Libera a memória alocada dinamicamente para o vetor
    // IMPORTANTE: todos os processos devem liberar, mesmo que só o processo 0 tenha usado
    free(vetor);
    MPI_Finalize();
    return 0;
}