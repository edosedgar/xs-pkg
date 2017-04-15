#include <mpi.h>
#include <stdio.h>

#define err_msg(msg) do {                       \
        if (rank == 0) fprintf(stderr, (msg));  \
} while (0)                                     \


int main(int argc, char* argv[]) {
        /*
         * Variables
         */
        int rank, size;
        long unsigned result = 1;
        long unsigned global_result = 1;
        unsigned input_num = 0;
        char check_symbol = 0;
        int ret = 0;
        /*
         * MPI routines
         */
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );
        /*
         * Error checkng
         */
        if (argc != 2) {
                err_msg("Enter the one number!\n");
                goto fail;
        }
        ret = sscanf(argv[1], "%u%c", &input_num, &check_symbol);
        if (check_symbol != 0 || ret != 1) {
                err_msg("Wrong number!\n");
                goto fail;
        }
        if (input_num > 20 || input_num < 0) {
                err_msg("Number should not be the negative"
                        " or more than 20\n* Current release doesn't"
                        " support bignum arithmetic\n");
                goto fail;
        }
        /*
         * Calculations
         */
        int i = 0;
        for (i = 1; i <= input_num; i++) {
                if (i % size == rank) 
                        result *= i;
        }
        /*
         * Gather results and print out
         */
        MPI_Reduce(&result, &global_result, 1, MPI_UNSIGNED_LONG, MPI_PROD, 0,
                   MPI_COMM_WORLD); 
        if (rank == 0) printf("Result: %lu\n", global_result);
fail:
        MPI_Finalize();
        return 0;
}
