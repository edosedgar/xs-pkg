#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "block_matrix.h"

#define min(a,b) \
        ({ __typeof__ (a) _a = (a); \
           __typeof__ (b) _b = (b); \
           _a < _b ? _a : _b; })

matrix_t *
matrix_create(int bs, int height, int width)
{
        matrix_t *m;
        int r_height = height % bs;
        int r_width = width % bs;

        m = calloc(1, sizeof(matrix_t));
        m->_height = height;
        m->_width = width;
        m->_a_height += (height / bs) * bs + (height % bs != 0) * bs;
        m->_a_width += (width / bs) * bs + (width % bs != 0) * bs;

        fprintf(stderr, "Matrix size was aligned to %dx%d\n",
                m->_a_height, m->_a_width);
        m->arr = calloc(m->_a_height * m->_a_width, sizeof(data_t));
        return m;
}

void
matrix_print(matrix_t *M)
{
        int i, j;

        for (i = 0; i < M->_height; i++) {
                for (j = 0; j < M->_width; j++)
                        printf("%.2f ", M->arr[i * M->_a_width + j]);
                printf("\n");
        }
}

void
matrix_mul(int bs, matrix_t *A, matrix_t *B, matrix_t *C)
{
        int Aw = A->_a_width;
        int Ah = A->_a_height;
        int Bw = B->_a_width;
        int Bh = B->_a_height;
        int Cw = C->_a_width;
        int Ch = C->_a_height;
        data_t *arrA = A->arr;
        data_t *arrB = B->arr;
        data_t *arrC = C->arr;
        int JA, JB, JC, IA, IB, IC, ia, ib, ic, ja, jb, jc;


        for (JC = 0; JC < Ch; JC += bs) {
                JA = JC;
        #pragma omp parallel for
                for (IC = 0; IC < Cw; IC += bs) {
                        JB = IC;
                        IB = 0;
                        for (IA = 0; IA < Aw; IA += bs) {
                                jc = JC;
                                for (ja = JA; ja < JA + bs; ja++) {
                                        ic = IC;
                                        for (jb = JB; jb < JB + bs; ic++) {
                                                ib = IB;
        #pragma omp simd
                                                for (ia = IA; ia < IA + bs; ia++) {
                                                        arrC[ jc*Cw+ic ] += arrA[ja*Aw+ia] * arrB[jb*Bw+ib];
                                                        ib++;
                                                }
                                                jb++;
                                        }
                                        jc++;
                                }
                                IB += bs;
                        }
                 }
        }
/*

        //#pragma omp parallel for
        for (int jk = 0; jk < Ah / bs; jk++)
        //#pragma omp parallel for
            for (int register ik = 0; ik < Aw / bs; ik++)
        #pragma omp parallel for
                for (int register j = jk * bs; j < jk * bs + bs; j++)
        //#pragma omp parallel for
                    for (int register k = ik * bs; k < ik * bs + bs; k++)
        #pragma omp simd
                        for (int register i = 0; i < Bw; i++)
                            arrC[j * Cw + i] += arrA[j * Aw + k] *
                                                arrB[k * Bw + i];
*/
}

void
matrix_free(matrix_t *M)
{
        free(M->arr);
        free(M);
}

void
matrix_fill(matrix_t *M)
{
        int i, j;

        for (i = 0; i < M->_height; i++)
                for (j = 0; j < M->_width; j++)
                        M->arr[i * M->_a_width + j] = 1.0 * (rand() % 100);
}

/*
 * ######### main.c ##########
 */

static long double
diff(struct timespec start, struct timespec end)
{
    struct timespec temp;

    if (end.tv_nsec < start.tv_nsec)
    {
            temp.tv_sec = end.tv_sec - start.tv_sec - 1;
            temp.tv_nsec = 1000000000L + end.tv_nsec - start.tv_nsec;
    }
    else
    {
            temp.tv_sec = end.tv_sec - start.tv_sec;
            temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return (long double)temp.tv_nsec + temp.tv_sec * 1000000000L;
}

int
main(int argc, char *argv[])
{
        matrix_t *M, *N, *R;
        struct timespec time_start = {0};
        struct timespec time_end = {0};

        int bs, height, width;
        uint64_t par_start, par_end;

        if (argc != 5) {
                fprintf(stderr, "%s BlockSize H1 W1 W2\n", argv[0]);
                return 1;
        }
        bs = atoi(argv[2]);
        M = matrix_create(bs, atoi(argv[2]), atoi(argv[3]));
        N = matrix_create(bs, atoi(argv[3]), atoi(argv[4]));
        R = matrix_create(bs, M->_height, N->_width);
        matrix_fill(M);
        matrix_fill(N);

        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_start);
        matrix_mul(bs, M, N, R);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_end);
        printf("time=%Lf\n", diff(time_start, time_end) / 1000000000.);

        /*matrix_print(M);
        matrix_print(N);
        matrix_print(R);
        int i,j,k;
        double c;
        for (i = 0; i < M->_a_height; i++)
                for (j = 0; j < M->_a_height; j++) {
                        for (k = 0; k < M->_a_height; k++)
                                c += M->arr[i * M->_a_height + k] * N->arr[j * M->_a_height + k];
                        if (c != R->arr[i * M->_a_height + j]) {
                                printf("Pizdets\n");
                                fprintf(stderr, "ref: %.2f, our: %.2f\n", c, R->arr[i * M->_a_height + j]);
                        }
                        c=0;
                }*/
        matrix_free(M);
        matrix_free(N);
        matrix_free(R);

        return 0;
}
