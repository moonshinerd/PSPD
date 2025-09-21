/* 
Utilizando a biblioteca MPI, elabore um programa para somar os elementos de duas matrizes A e B, quadradas (int), para gerar uma matriz C, de acordo com as seguintes regras:

O programa deve conter um processo master e dois workers, que deverão trabalhar em conjunto para garantir a realização de soma dos elementos das matrizes A e B.

Supor que as matrizes sejam de 16 posições (int) e as matrizes A e B devem ser inicializadas com valores randômicos

As operações de soma devem ser distribuídas entre os workers, de modo que a primeira e a terceira linha das matrizes sejam processadas pelo primeiro worker e a segunda e a quarta linha sejam processadas pelo segundo worker

A comunicação entre os processos master e workers deve ser feita utilizando especificamente as funções MPI_Send e MPI_Recv

Ao final, a matriz C resultante deve ser impressa (em colunas, formato de matriz) pelo processo master
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 4 // Define o tamanho da matriz quadrada (4x4)

void printMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, nprocs;
    int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    int rowsPerWorker = SIZE / 2; // Cada worker processa 2 linhas

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    if (nprocs != 3) {
        if (rank == 0) {
            printf("Este programa deve ser executado com exatamente 3 processos (1 master e 2 workers).\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Processo master inicializa as matrizes A e B com valores randômicos
        srand(42); // Semente para reprodutibilidade
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                A[i][j] = rand() % 100; // Valores entre 0 e 99
                B[i][j] = rand() % 100; // Valores entre 0 e 99
            }
        }

        printf("Matriz A:\n");
        printMatrix(A);
        printf("\nMatriz B:\n");
        printMatrix(B);

        // Envia as linhas apropriadas para cada worker
        MPI_Send(&A[0][0], rowsPerWorker * SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD); // Envia linhas 0 e 1 para worker 1
        MPI_Send(&B[0][0], rowsPerWorker * SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD); // Envia linhas 0 e 1 para worker 1
        MPI_Send(&A[rowsPerWorker][0], rowsPerWorker * SIZE, MPI_INT, 2, 0, MPI_COMM_WORLD); // Envia linhas 2 e 3 para worker 2
        MPI_Send(&B[rowsPerWorker][0], rowsPerWorker * SIZE, MPI_INT, 2, 1, MPI_COMM_WORLD); // Envia linhas 2 e 3 para worker 2

        // Recebe os resultados dos workers
        MPI_Recv(&C[0][0], rowsPerWorker * SIZE, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recebe linhas 0 e 1 de worker 1
        MPI_Recv(&C[rowsPerWorker][0], rowsPerWorker * SIZE, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recebe linhas 2 e 3 de worker 2

        // Imprime a matriz resultante C
        printf("\nMatriz C (Resultado da soma A + B):\n");
        printMatrix(C);

    } else {
        int localA[rowsPerWorker][SIZE];
        int localB[rowsPerWorker][SIZE];
        int localC[rowsPerWorker][SIZE];

        // Recebe as linhas apropriadas da matriz A e B
        MPI_Recv(&localA[0][0], rowsPerWorker * SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&localB[0][0], rowsPerWorker * SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Realiza a soma das matrizes locais
        for (int i = 0; i < rowsPerWorker; i++) {
            for (int j = 0; j < SIZE; j++) {
                localC[i][j] = localA[i][j] + localB[i][j];
            }
        }

        // Envia o resultado de volta para o processo master
        MPI_Send(&localC[0][0], rowsPerWorker * SIZE, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}