/* 
Utilizando a biblioteca MPI, elabore um programa para somar os elementos de duas matrizes A e B, quadradas (int), para gerar uma matriz C, de acordo com as seguintes regras:

O programa deve conter um processo master e dois workers, que deverão trabalhar em conjunto para garantir a realização de soma dos elementos das matrizes A e B.

Supor que as matrizes sejam de 16 posições (int) e as matrizes A e B devem ser inicializadas com valores randômicos

As operações de soma devem ser distribuídas entre os workers, de modo que a primeira e a terceira linha das matrizes sejam processadas pelo primeiro worker e a segunda e a quarta linha sejam processadas pelo segundo worker

A comunicação entre os processos master e workers deve ser feita utilizando especificamente as funções MPI_Send e MPI_Recv

Ao final, a matriz C resultante deve ser impressa (em colunas, formato de matriz) pelo processo master
*/