/*
Elabore um programa MPI que imprima um vetor de 100 posições (de tipo int), considerando o seguinte:

O vetor deve ser impresso da posição 0 até a posição 99, nesta ordem

O master deve inicializar o vetor de 100 posições da seguinte forma: v[i]=i

O master deve distribuir a impressão entre os workers de modo que todos possam imprimir pelo menos uma porção do vetor

Cada worker, uma vez acionado, deve imprimir o vetor a partir do ponto de impressão recebido do master
Considerar que este programa pode ser executado por, no máximo, 6 processos (1 master e 5 workers)

O número de posições a serem impressas pelo worker deve obedecer a um offset dinâmico, ou seja, um valor randômico – menor que 15 – que é calculado por cada processo, no momento em que é acionado para imprimir o vetor

O programa deve controlar a impressão de modo que o vetor inteiro seja impresso, mas nenhuma posição seja impressa mais de uma vez. Por exemplo, se o worker anterior imprimiu até a posição 18 e o offset dinâmico calculado foi 10, a thread atual deve imprimir da posição 19 considerando 10 posições adiante

A ação dos workers e do master acaba quando o vetor de 100 posições tiver sido todo impresso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECTOR_SIZE 100
#define MAX_OFFSET rand() % 15 + 1 // Offset dinâmico entre 1 e 15

int main(int argc, char** argv) {
    int rank, nprocs;
    int vetor[VECTOR_SIZE];
    int start, end, offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (nprocs < 2 || nprocs > 6) {
        if (rank == 0) {
            printf("Este programa deve ser executado com entre 2 e 6 processos (1 master e 1-5 workers).\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Processo master inicializa o vetor
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vetor[i] = i;
        }
    }
    int i = 0; // posição atual global
    MPI_Bcast(vetor, VECTOR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    
    while(i < VECTOR_SIZE) {
        
        if (rank == 0) {
            // MASTER: Envia trabalho para um worker
            
            // === LÓGICA DE SELEÇÃO DE WORKER (Round-Robin) ===
            // Esta linha calcula qual worker deve processar a próxima parte do vetor
            //
            // PASSO A PASSO:
            // 1. (i / MAX_OFFSET) = quantos "blocos" já foram processados
            //    - Exemplo: se i=30 e MAX_OFFSET=15, então 30/15 = 2 blocos processados
            //
            // 2. % (nprocs - 1) = aplica módulo para rotacionar entre workers disponíveis
            //    - (nprocs - 1) porque temos 1 master e (nprocs-1) workers
            //    - Exemplo: com 6 processos, temos 5 workers (1,2,3,4,5)
            //
            // 3. + 1 = ajusta para os IDs corretos dos workers (1,2,3,4,5 ao invés de 0,1,2,3,4)
            //    - Workers têm rank de 1 a (nprocs-1), master tem rank 0
            //
            // EXEMPLO PRÁTICO com 6 processos (1 master + 5 workers):
            // i=0:  (0/15) % 5 + 1 = 0 % 5 + 1 = 0 + 1 = Worker 1
            // i=15: (15/15) % 5 + 1 = 1 % 5 + 1 = 1 + 1 = Worker 2  
            // i=30: (30/15) % 5 + 1 = 2 % 5 + 1 = 2 + 1 = Worker 3
            // i=45: (45/15) % 5 + 1 = 3 % 5 + 1 = 3 + 1 = Worker 4
            // i=60: (60/15) % 5 + 1 = 4 % 5 + 1 = 4 + 1 = Worker 5
            // i=75: (75/15) % 5 + 1 = 5 % 5 + 1 = 0 + 1 = Worker 1 (volta ao início)
            //
            // RESULTADO: Distribui trabalho de forma circular: 1→2→3→4→5→1→2→3...
            int worker = (i / MAX_OFFSET) % (nprocs - 1) + 1;
            
            // Envia posição inicial para o worker
            MPI_Send(&i, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
            
            // Recebe nova posição do worker
            MPI_Recv(&i, 1, MPI_INT, worker, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (i >= VECTOR_SIZE) break; // Se já terminou, sai do loop
        } else {
            // WORKERS: Aguardam comando do master
            int start_pos;
            
            // Recebe posição inicial do master
            MPI_Recv(&start_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // Se recebeu -1, significa fim do trabalho
            if (start_pos == -1) {
                break; // Sai do while
            }
            
            // Calcula quantas posições vai imprimir (offset dinâmico)
            int offset = MAX_OFFSET;
            int end_pos = start_pos + offset;
            if (end_pos > VECTOR_SIZE) end_pos = VECTOR_SIZE;
            
            // Imprime sua parte
            for (int j = start_pos; j < end_pos; j++){
                printf("Processo %d: vetor[%d] = %d\n", rank, j, vetor[j]);
            }

            if (end_pos >= VECTOR_SIZE) end_pos = VECTOR_SIZE; // Garante que não ultrapasse o tamanho do vetor
            
            // Envia nova posição de volta para o master
            MPI_Send(&end_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    // Depois que terminou, master envia sinal de fim para todos os workers
    if (rank == 0) {
        int fim = -1; // Sinal de fim
        for (int w = 1; w < nprocs; w++) {
            MPI_Send(&fim, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}