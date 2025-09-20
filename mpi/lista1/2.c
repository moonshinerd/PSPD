// Elaborar um programa MPI com dois processos para imprimir colaborativamente um vetor de tamanho fixo (8 posições).
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int vetor[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    int rank, nprocs; // rank: identificador do processo, nprocs

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo, MPI_COMM_WORLD é o comunicador padrão que inclui todos os processos
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // Obtém o número total de processos no comunicador MPI_COMM_WORLD
    if (nprocs != 2) {
        if (rank == 0) {
            printf("Este programa deve ser executado com exatamente 2 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }
    if (rank == 0) { // se for o processo MASTER (rank 0)
        printf("Processo %d imprimindo as primeiras 4 posições do vetor:\n", rank);
        for (int i = 0; i < 4; i++) {
            printf("vetor[%d] = %d\n", i, vetor[i]);
        }
    } else { // se for o processo SLAVE (rank 1)
        printf("Processo %d imprimindo as últimas 4 posições do vetor:\n", rank);
        for (int i = 4; i < 8; i++) {
            printf("vetor[%d] = %d\n", i, vetor[i]);
        }
    }
    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}