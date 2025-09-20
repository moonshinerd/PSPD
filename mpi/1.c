// Elaborar um programa MPI no qual cada Slave informa ao MASTER que está ativo.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int rank, nprocs; // rank: identificador do processo, nprocs: número total de processos
    int msg = 1;
    MPI_Status status;

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo, MPI_COMM_WORLD é o comunicador padrão que inclui todos os processos
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // Obtém o número total de processos no comunicador MPI_COMM_WORLD

    printf("Processo %d de %d iniciado\n", rank, nprocs);
    fflush(stdout);

    if (rank == 0) { // se for o processo MASTER (rank 0)
        printf("Sou MASTER (rank %d), estou aguardando mensagens dos SLAVES...\n", rank);
        for (int i =1; i < nprocs; i++) {
            MPI_Recv(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status); // Recebe mensagem de qualquer SLAVE (rank 1 a nprocs-1), MPI_RECV(mensagem(endereço onde é armazenado o dado recebido), tamanho(qtd de elementos a serem recebidos), tipo de dado, fonte(identificação do processo que enviou os dados), tag(identificação da mensagem), comunicador, status)
            printf("MASTER recebeu mensagem do SLAVE %d\n", i);
        }
        printf("MASTER finalizou recebimento de todas as mensagens\n");
    } else { // se for um processo SLAVE (rank 1 a nprocs-1)
        printf("Sou SLAVE %d, enviando mensagem ao MASTER...\n", rank);
        MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Envia mensagem ao MASTER (rank 0), MPI_SEND(mensagem(endereço onde está o dado a ser enviado), tamanho(qtd de elementos a serem enviados), tipo de dado, destino(identificação do processo que receberá os dados), tag(identificação da mensagem), comunicador)
        printf("SLAVE %d finalizou envio da mensagem\n", rank);
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}