// para compilar: mpicc 04-tokenring-mpi-omp.c -o 04-tokenring-mpi-omp -fopenmp
// para rodar: mpirun -np <num_de_processos_desejado> 04-tokenring-mpi-omp

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define NTHREADS 4

void openmpTasks(int rank, int *nekot) {
	#pragma omp parallel num_threads(NTHREADS) shared(nekot)
	{
		#pragma omp critical (incrementa)
		{
			printf("MPI Process %d, received %d and it's sending %d\n", rank, *nekot, (*nekot)+1);
			fflush(0);
			*nekot += 1;
		}
	}
}

int  main(int argc, char *argv[])	{
	int npes, myrank, src, dest;
	int token, nekot;
	int count  = 1;
	int msgtag = 4;

	MPI_Status status;
	
	printf("NTHREADS = %d \n", NTHREADS);
	fflush(0);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	printf("myrank = %d total of processes = %d NTHREADS = %d \n", myrank, npes, NTHREADS);
	fflush(0);
	
	//---------------------------------------------------------
	MPI_Barrier(MPI_COMM_WORLD);
	//---------------------------------------------------------

	dest = (myrank + 1) % npes;
	src   = (myrank - 1 + npes) % npes;

	if (myrank == 0) 
	{
		token = myrank;

        printf("Initial value: %d\n", token);

        // blocking send
		MPI_Send(&token, count, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
		// blocking receive
		MPI_Recv(&nekot, count, MPI_INT, src, msgtag, MPI_COMM_WORLD, &status);
		printf("Computing done. Final value: %d. \n", nekot);
	}
	else
	{
		// blocking receive
		MPI_Recv(&nekot, count, MPI_INT, src, msgtag, MPI_COMM_WORLD, &status);
		
		openmpTasks(myrank, &nekot);

		MPI_Send(&nekot, count, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	
	return 0;
}
