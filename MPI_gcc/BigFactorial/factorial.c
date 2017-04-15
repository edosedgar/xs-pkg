#include <mpi.h>
#include <stdio.h>
#include "bigdigits.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

#define err_msg(msg) do {                       \
        if (rank == 0) fprintf(stderr, (msg));  \
} while (0)                                     \

#define LENGTH_BIG_INT 5000

int main(int argc, char* argv[]) {
        /*
         * Variables
         */
        int rank, size;
        clock_t start, end;
        unsigned input_num = 0;
        long unsigned delta = 0;
        start = clock();
        DIGIT_T* number = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));
        DIGIT_T* buffer = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));
        DIGIT_T* result = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));
        char check_symbol = 0;
        int ret = 0;
        /*
         * MPI routines
         */
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        /*
         * Error checkng
         */
        if ((size <= 0) || ((size & (size - 1)) != 0)) {
                err_msg("Number of proccess should be power of 2!\n");
                goto fail;
        }
        if (argc != 2) {
                err_msg("Enter the one number!\n");
                goto fail;
        }
        ret = sscanf(argv[1], "%u%c", &input_num, &check_symbol);
        if (check_symbol != 0 || ret != 1) {
                err_msg("Wrong number!\n");
                goto fail;
        }
        if (input_num < 0) {
                err_msg("Number should not be the negative.\n");
                goto fail;
        }
        if (input_num % size != 0) {
                err_msg("Input number should be divided by processor number "
                        "without remainder.\n");
                goto fail;
        }
        mpSetDigit(number, 1, LENGTH_BIG_INT);
        end = clock();
        delta = end - start;
        if (rank == 0) {
                fprintf(stderr, "Preliminary routines: %lu units.\n", delta);
        }
        /*
         * Calculations
         */
        int i = 0;
        start = clock();
        for (i = rank + 1; i <= input_num; i += size) {
                mpSetDigit(buffer, i, LENGTH_BIG_INT);
                mpMultiply(result, number, buffer, LENGTH_BIG_INT/2);
                memcpy(number, result, LENGTH_BIG_INT*sizeof(DIGIT_T));
        }
        end = clock();
        delta = end - start;
        if (rank == 0) {
                fprintf(stderr, "Main calculations: %lu units.\n", delta);
        }
        /*
         * Gather results, log_2(size)
         */
        int depth = 31 - __builtin_clz(size);
        int step = size;
        start = clock();
        for (i = 0; i < depth; i++) {
                step /= 2;
                if (rank < step) {
                        //dump_nsec(spec);
                        MPI_Recv(buffer, LENGTH_BIG_INT, MPI_INT, rank + step,
                                 0, MPI_COMM_WORLD, &status);
                        //dump_nsec(spec);
                        mpMultiply(number, result, buffer, LENGTH_BIG_INT/2);
                        memcpy(result, number, LENGTH_BIG_INT*sizeof(DIGIT_T));
                }
                if (rank >= step && rank < step * 2) {
                        MPI_Send(result, LENGTH_BIG_INT, MPI_INT, rank - step,
                                 0, MPI_COMM_WORLD);
                }
        }
        end = clock();
        delta = end - start;
        if (rank == 0) {
                fprintf(stderr, "Data reduce: %lu units.\n", delta);
        }
        /*
         * Print out
         */
        if (rank == 0) {
               mpPrintHex("Result: ", result, LENGTH_BIG_INT, "\n");
        }
fail:
        free(buffer);
        free(number);
        free(result);
        MPI_Finalize();
        return 0;
}
