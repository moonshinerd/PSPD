/*
 * Programa MPI com dois processos para imprimir colaborativamente um vetor de tamanho fixo (8 posições).
 * 
 * Funcionamento:
 * - Processo 0 (master): imprime elementos nas posições pares (0, 2, 4, 6)
 * - Processo 1 (slave): imprime elementos nas posições ímpares (1, 3, 5, 7)
 * 
 * Execução: mpirun -np 2 ./programa
 */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    // Vetor com 8 elementos para ser impresso colaborativamente
    int vetor[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    
    // Variáveis MPI: rank = identificador do processo, nprocs = número total de processos
    int rank, nprocs;
    
    // Contador para percorrer o vetor
    int i = 0;
    
    // Inicializa o ambiente MPI - deve ser a primeira chamada MPI
    MPI_Init(&argc, &argv);
    
    // Obtém o identificador único deste processo (0 ou 1)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Obtém o número total de processos no comunicador padrão
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    // Verifica se o programa está sendo executado com exatamente 2 processos
    if (nprocs != 2) {
        // Apenas o processo master (rank 0) exibe mensagens de erro para evitar duplicação
        if (rank == 0) {
            printf("Este programa deve ser executado com exatamente 2 processos.\n");
        }
        MPI_Finalize(); // Finaliza o ambiente MPI antes de sair
        return 1;
    }
    
    // Loop para percorrer todas as 8 posições do vetor
    for (i; i < 8; i++){
        // Processo master (rank 0) imprime elementos em posições pares (0, 2, 4, 6)
        if (rank == 0) {
            if (i % 2 == 0){
                printf("vetor[%d] = %d\n", i, vetor[i]);
                continue; // Continua para a próxima iteração
            }
        // Processo slave (rank 1) imprime elementos em posições ímpares (1, 3, 5, 7)
        } else {
            if (i % 2 != 0){
                printf("vetor[%d] = %d\n", i, vetor[i]);
                continue; // Continua para a próxima iteração
         }
        }
    }
    // Finaliza o ambiente MPI - deve ser a última chamada MPI
    MPI_Finalize();
    return 0;
}