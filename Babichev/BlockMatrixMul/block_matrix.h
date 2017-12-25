#ifndef __BLOCK_MATR_H__
#define __BLOCK_MATR_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef float data_t;

typedef struct matrix {
        uint32_t _height;
        uint32_t _a_height;
        uint32_t _width;
        uint32_t _a_width;
        data_t *arr;
} matrix_t, *pmatrix_t;

matrix_t *matrix_create(int bs, int height, int width);

void matrix_print(matrix_t *M);

void matrix_free(matrix_t *M);

void matrix_mul(int bs, matrix_t *A, matrix_t *B, matrix_t *C);

void matrix_fill(matrix_t *M);

#endif
