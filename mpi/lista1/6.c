// Elaborar um programa MPI para imprimir os N (informado pelo usuário) primeiros números naturais (considerar que o número de processos instanciados sempre será menor ou igual a N).

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, nprocs, N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // Processo master (rank 0) lê o valor de N
    if (rank == 0) {
        printf("Digite o valor de N (números naturais a imprimir): ");
        scanf("%d", &N);
        if (N < 1) {
            printf("N deve ser um número natural positivo.\n");
            N = 0; // Define N como 0 para evitar impressão
        }
    }

    // Broadcast do valor de N para todos os processos
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo imprime seus números naturais atribuídos
    for (int i = rank; i < N; i += nprocs) {
        printf("Processo %d: %d\n", rank, i);
    }

    MPI_Finalize();
    return 0;
}