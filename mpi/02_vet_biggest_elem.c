// Dado um vetor vet[TAM], determinar quantos nrs maiores que vet[K] existem em
// vet e em quais posições eles estão, conforme especificação.

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define K 128  // number of vector elements

// mpicc -o vet_biggest_elem 02_vet_biggest_elem.c
// mpirun -np 4 --oversubscribe ./vet_biggest_elem
int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int valueToCompare, localSize;
    int *vet;

    if (rank == 0) {
        vet = (int *)malloc(K * sizeof(int));

        for (int i = 0; i < K; i++) {
            vet[i] = rand() % 100;
        }

        valueToCompare = vet[K-1];

        printf("Process %d: Sending vector to all processes\n", rank);
        for (int i = 1; i < size; i++) {
            // int MPI_Send(
            //    void ∗bufferE,
            //    int count,
            //    MPI_Datatype datatype,
            //    int dest,
            //    int tag,
            //    MPI_Comm comm
            // );

            // send value to be compared
            MPI_Send(&valueToCompare, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            int numWorkToProcess = K / size;
            int start = i * numWorkToProcess;
            int end = (i + 1) * numWorkToProcess;
            if (i == size - 1) {
                end = K;  // Last process takes the remaining elements
            }
            localSize = end - start;

            printf("Process %d: Sending elements from %d to %d\n", rank, start, end - 1);

            // send the amount of elements to be processed
            MPI_Send(&localSize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

            // send the elements to be processed
            MPI_Send(&vet[start], localSize, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
    } else {
        // int MPI_Recv(
        //     void ∗bufferR,
        //     int count,
        //     MPI_Datatype datatype,
        //     int source,
        //     int tag,
        //     MPI_Comm comm,
        //     MPI_Status ∗status
        // );
        MPI_Recv(&valueToCompare, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&localSize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vet = (int *)malloc(localSize * sizeof(int));
        MPI_Recv(vet, localSize, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d: Received vector with %d elements\n", rank, localSize);
    }

    // Each process counts how many elements are greater than the value to compare
    int count = 0;
    for (int i = 0; i < localSize; i++) {
        if (vet[i] > valueToCompare) {
            printf("Process %d: Element %d at position %d is greater than %d\n", rank, vet[i], i, valueToCompare);
            count++;
        }
    }

    printf("Process %d: Found %d elements greater than %d\n", rank, count, valueToCompare);
    int totalCount = 0;

    // easier to reduce than passing via message the number of elements
    MPI_Reduce(&count, &totalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Total elements greater than %d: %d\n", valueToCompare, totalCount);
        free(vet);
    } else {
        free(vet);
    }

    MPI_Finalize();
}
