#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>
#include <time.h>
#include <sys/mman.h>
#include <errno.h>
#include <sched.h>

struct cache_line {
        long int data[8];
};

static int
convert_size(char* str)
{
        int number = 0;
        char err_c = 0;
        char last_sym = 0;
        int ret_code = 0;
        ret_code = sscanf(str, "%d%c%c", &number, &last_sym, &err_c);
        if (err_c != '\0' || ret_code == 0 || number < 0) {
                return -1;
        }
        switch (last_sym) {
        case '\0':
                return number;
        case 'K':
                return 1024 * number;
        case 'M':
                return 1024 * 1024 * number;
        case 'G':
                return 1024 * 1024 * 1024 * number;
        default:
                return -1;
        }
        return -1;
}

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
main(int argc, char* argv[])
{
        struct timespec time_start = {0};
        struct timespec time_end = {0};
        int offset, N, None, Ntwo;
        int i, shift;
        volatile register long int next;
        volatile register long int count;
        volatile register long int test;

        if (argc != 4) {
                fprintf(stderr, "input format: offset Nfrom Nto\n");
                exit(EINVAL);
        }
        if ((offset = convert_size(argv[1])) == -1 ||
            (None   = convert_size(argv[2])) == -1 ||
            (Ntwo   = convert_size(argv[3])) == -1)
                exit(EINVAL);

        for (N = None; N <= Ntwo; N++) {
                // Fill array
                struct cache_line *cache_l = mmap(NULL, N * offset,
                                                  PROT_READ | PROT_WRITE,
                                                  MAP_ANONYMOUS | MAP_SHARED,
                                                  -1, 0);

                shift = 0;
                offset /= sizeof(struct cache_line);
                while (shift <= N) {
                        for (i = 0; i < N * offset; i += offset) {
                                int data = (i + offset + shift +
                                            ((i / offset) == (N - 1))) %
                                            (N * offset);
                                cache_l[i + shift].data[0] = data;
                        }
                        shift++;
                }
                cache_l[(N - 1) * (offset + 1)].data[0] = 0;
                offset *= sizeof(struct cache_line);
#define cycles 100000000L
                next = 0;
                // Test array
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
                for (count = 0; count < cycles; count++) {
                        test = cache_l[next].data[7];
                        next = cache_l[next].data[0];
                }
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_end);
                // Dump results
                fprintf(stdout, "%d %Lf\n", N,
                        diff(time_start, time_end)/cycles);
                munmap(cache_l, N * offset);
        }
        return 0;
}
