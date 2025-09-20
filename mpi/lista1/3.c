// Elaborar um programa MPI para formar um anel de processos com execução serial, controlada por um token que é gerado pelo MASTER e passado para cada um até retornar de volta ao MASTER.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    // Variáveis para identificação e controle dos processos MPI
    int rank;     // Identificador único do processo atual (0, 1, 2, ...)
    int nprocs;   // Número total de processos no comunicador
    
    // Variável que representa o token que circula pelo anel
    int token;
    
    // Inicializa o ambiente MPI - deve ser a primeira função MPI chamada
    MPI_Init(&argc, &argv);
    
    // Obtém o identificador único (rank) deste processo no comunicador MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Obtém o número total de processos participando da comunicação
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    // Validação: verifica se há pelo menos 2 processos (mínimo para formar um anel)
    
    if (nprocs < 2) {
        // Apenas o processo master exibe a mensagem de erro para evitar duplicação
        if (rank == 0) {
            printf("Este programa deve ser executado com pelo menos 2 processos.\n");
        }
        MPI_Finalize(); // Finaliza o ambiente MPI antes de terminar
        return 1;
    }
    
    // === LÓGICA DO PROCESSO MASTER (rank 0) ===
    if (rank == 0) {
        // Inicializa o token com um valor específico (42)
        token = 42;
        
        // Calcula o próximo processo no anel: (0 + 1) % nprocs = 1
        // Se nprocs = 4, o anel será: 0 → 1 → 2 → 3 → 0
        printf("Processo %d enviando token %d para o processo %d\n", rank, token, (rank + 1) % nprocs);
        
        // Envia o token para o próximo processo no anel
        // MPI_Send(dados, quantidade, tipo, destino, tag, comunicador)
        MPI_Send(&token, 1, MPI_INT, (rank + 1) % nprocs, 0, MPI_COMM_WORLD);
        
        // Aguarda o token retornar do último processo do anel
        // Calcula o processo anterior: (0 - 1 + nprocs) % nprocs = nprocs - 1 (último processo)
        // MPI_Recv(buffer, quantidade, tipo, origem, tag, comunicador, status)
        MPI_Recv(&token, 1, MPI_INT, (rank - 1 + nprocs) % nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu de volta o token %d do processo %d\n", rank, token, (rank - 1 + nprocs) % nprocs);
    
    // === LÓGICA DOS PROCESSOS INTERMEDIÁRIOS (rank 1, 2, ..., nprocs-1) ===    
    } else {
        // Primeiro: recebe o token do processo anterior no anel
        // Para processo rank N: processo anterior = (N - 1 + nprocs) % nprocs
        // Exemplo: se rank=2 e nprocs=4, processo anterior = (2-1+4)%4 = 1
        MPI_Recv(&token, 1, MPI_INT, (rank - 1 + nprocs) % nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu o token %d do processo %d\n", rank, token, (rank - 1 + nprocs) % nprocs);
        
        // Segundo: envia o token para o próximo processo no anel
        // Para processo rank N: próximo processo = (N + 1) % nprocs
        // Exemplo: se rank=2 e nprocs=4, próximo processo = (2+1)%4 = 3
        printf("Processo %d enviando token %d para o processo %d\n", rank, token, (rank + 1) % nprocs);
        MPI_Send(&token, 1, MPI_INT, (rank + 1) % nprocs, 0, MPI_COMM_WORLD);
    }
    
    // Finaliza o ambiente MPI - deve ser a última função MPI chamada
    // Todos os processos devem chamar MPI_Finalize()
    MPI_Finalize();
    return 0;
}