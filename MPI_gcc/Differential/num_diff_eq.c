#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/*
    Define left part of your equation here:
    u'_t + a * u'_x = f(x, y, y') = t + x
    u(0, x) = x, 0 <= x <= 1
    u(t, 0) = t, 0 <= t <= 1
*/
double f(double x, double t){ return t + x; }
/*
    u_l_k = u(t0 + l*tau, x0 + k*h)
    u_l_l_1 = u(t0 + l*tau, x0 + (k - 1)*h )
    u_next = u(t0 + (l + 1)*tau, x0 + k*h)
*/
double u_next(double x, double t, double u_l_k, \
              double u_l_k_1, double tau, double h, double f, double a)
{
    return u_l_k + tau * f - a * tau / h * (u_l_k - u_l_k_1);
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);
    int K = strtol(argv[1], NULL, 10);
    int L = strtol(argv[2], NULL, 10);
    int rank, size;
    double start_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t0 = 0;
    double tL = 1.0;
    double x0 = 0;
    double xK = 1.0;
    double a = 10;
    double tau = (tL - t0) / L;
    double h = (xK - x0) / K;
    double** u;
    double shift = 0;
    int width = K / size;
    int width_root = width + K % size + 1;
    int width_common = width;

    if ( rank == 0 )
    {
        width = width_root;
        printf("rank:%d K:%d h:%lf L:%d tau:%lf width:%d\n", rank, K, h, L, tau, width);
    } else
    {
        shift = h * width_root + (rank - 1) * h * width;
        printf("rank:%d K:%d h:%lf L:%d tau:%lf width:%d\n", rank, K, h, L, tau, width);
    }

    if ( rank == 0 )
    {
        start_time = MPI_Wtime();
        u = (double**) calloc (sizeof(double*), (K + 1));

        for(int i = 0; i <= K; ++i)
            u[i] = (double*) calloc (sizeof(double), (L + 1));

        for(int i = 0; i <= L; ++i)
            u[0][i] = i * tau;

    } else
    {
        u = (double**) calloc (sizeof(double*), width);

        for(int i = 0; i < width; ++i)
            u[i] = (double*) calloc (sizeof(double), (L + 1));
    }

    for(int i = 0; i < width; ++i)
        u[i][0] = i * h + shift;

    for(int i = 0; i < L; ++i)
    {
        for(int j = 1; j < width; j++)
            u[j][i+1] = u_next(h * j + shift, tau * i, u[j][i], u[j-1][i], \
                               tau, h, f(h * j + shift, tau * i), a);
        if ( rank == 0 )
        {
            if ( size != 1 )
            {
                double tmp = u[width-1][i]; 
                MPI_Send(&tmp, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            }
        }

        if ( size != 1 && rank != 0 && rank != size - 1 )
        {
            double tmp_recv = 0;
            MPI_Recv(&tmp_recv, 1, MPI_DOUBLE, rank - 1, 0, \
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            double tmp_send = u[width-1][i];
            MPI_Send(&tmp_send, 1, MPI_DOUBLE, \
                     rank + 1, 0, MPI_COMM_WORLD);

            u[0][i+1] = u_next(shift, tau * i, u[0][i], tmp_recv, \
                               tau, h, f(shift, tau * i), a);
        }

        if ( size != 1 && rank == size - 1 )
        {
            double tmp_recv = 0;
            MPI_Recv(&tmp_recv, 1, MPI_DOUBLE, rank - 1, 0, \
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            u[0][i+1] = u_next(shift, tau * i, u[0][i], tmp_recv, \
                               tau, h, f(shift, tau * i), a);
        }
    }

    // End of computational part. Collecting information
    if ( rank == 0 )
    {
        printf("Time: %lf\n", MPI_Wtime() - start_time);

        for(int i = 1; i < size; ++i)
        {
            for(int j = width + width_common * (i - 1); \
                j < width + width_common * i; ++j)
            {
                MPI_Recv(u[j], L + 1, MPI_DOUBLE, i, 0, \
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    
        /*FILE* out = fopen("output.csv", "w");
        for(int i = 0; i <= K; ++i)
        {
            fprintf(out, "%lf", u[i][0]);
            for(int j = 1; j <= L; ++j)
                fprintf(out, ",%lf", u[i][j]);
            fprintf(out, "\n");
        }*/
    }

    if ( rank != 0 )
    {
        for(int i = 0; i < width; ++i)
            MPI_Send(u[i], L + 1, MPI_DOUBLE, 0, 0,
                     MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
