#include <mpi.h>
#include <stdio.h>
#include "unistd.h"
#define N 10
int main(int argc, char* argv[]) {
        int rank, size;
        int buf = -1;
        MPI_Status status;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );
        while (1) {
                if (rank != 0 || buf != -1) {
                        int adr = (rank - 1 == -1) ? (size - 1) : rank - 1;
                        MPI_Recv(&buf, 1, MPI_INT, adr, adr, MPI_COMM_WORLD,
                                 &status);
                        fprintf(stderr, "Process %d received <msg> "
                                        "from process %d\n", rank, adr);
                        usleep(10000);
                }
                if (++buf == N * size && rank == 0)
                        break;
                fprintf(stderr, "Process %d send <msg> to process %d\n", rank,
                                (rank + 1)%(size));
                usleep(10000);
                MPI_Send(&buf, 1, MPI_INT, (rank + 1)%(size), rank,
                         MPI_COMM_WORLD);
                if (rank != 0 && buf == (N - 1) * size + rank)
                        break;
        }
        MPI_Finalize();
        return 0;
}

