#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

int max_level = 0;
int crit_size = 0;

void simple_sort(int *begin, int *end)
{
    size_t size = (end - begin);
    size_t i, j;
    int buf;

    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (begin[j] > begin[j + 1]) {
                buf = begin[j];
                begin[j] = begin[j + 1];
                begin[j + 1] = buf;
            }
        }
    }
}

void
sort_array(int *begin, int *end, int level)
{
    size_t size = (end - begin);
    /*
     * Final stage
     */
    if (size <= 1)
        return;
    /*
     * Small arrays do not require big openmp overhead
     */
    if (size <= crit_size) {
        simple_sort(begin, end);
        return;
    }

    size_t left_size = size / 2;
    size_t right_size = (size - left_size);

    if (level < max_level) {
        #pragma omp parallel
        #pragma omp single nowait
        {
            #pragma omp task
            {
                sort_array(begin, begin + left_size, level + 1);
            }
            #pragma omp task
            {
                sort_array(begin + left_size, end, level + 1);
            }
            #pragma omp taskwait
        }
    } else {
        sort_array(begin, begin + left_size, level + 1);
        sort_array(begin + left_size, end, level + 1);
    }

    size_t li = 0, ri = left_size, i = 0;
    int *tmp_array = (int *) malloc(size * sizeof(int));

    while (li < left_size || ri < size)
    {
        if (((li != left_size) && (begin[li] < begin[ri])) || (ri == size))
        {
            tmp_array[i] = begin[li];
            i++;
            li++;
        }
        else
        {
            tmp_array[i] = begin[ri];
            i++;
            ri++;
        }
    }

    memcpy(begin, tmp_array, size * sizeof(int));
    free(tmp_array);
}

int main(int argc, char** argv)
{
    char* endptr;
    unsigned long N = 0;
    int i, buf;

    if (argc < 4) {
        fprintf(stderr, "Usage: ElemNum MaxOpenMPDepth CritSize\n");
        exit(1);
    }
    sscanf(argv[1], "%ld", &N);
    sscanf(argv[2], "%d", &max_level);
    sscanf(argv[3], "%d", &crit_size);
    int *array = malloc(N * sizeof(int));
    for (i = 0; i < N; i++)
        array[i] = random() >> 8;

    omp_set_nested(1);
    double begin = omp_get_wtime();
    sort_array(&array[0], array + N, 0);
    fprintf(stderr, "%lf sec\n", omp_get_wtime() - begin);

    for (i = 1; i < N; i++) {
        if ((array[i] - array[i - 1]) < 0)
            return 1;
    }

    fprintf(stderr, "Fine. Sorted\n");
    return 0;
}
