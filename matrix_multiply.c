#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define MAX_VAL 10

int **A;
int **B;
int **C;

// gcc -fopenmp matrix_multiply.c -o matrix_multiply.o -lm
// ./matrix_multiply.o (randomly generate matrices, take size as input)
// ./matrix_multiply.o 1
// Take matrices from mat_in.txt
/*
mat_in.txt format:
n m
a11 a12 ... a1m
.
.
.
an1 ....... anm
o k
b11 b12 ... b1k
.
.
.
bo1 ....... bok
*/

/*
/ Matrix multiplication using multithreading
1. Matrix multiplication is done using the usual formula
2. For each element of the resultant matrix, a task is launched
3. Any available thread can take up the an unpicked task
4. Inputs can be taken from a file, or generated randomly
*/

int **multiply(int **a, int n, int m, int **b, int o, int k)
{
    int **c = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        c[i] = (int *)malloc(k * sizeof(int));
    }

#pragma omp parallel
    {
#pragma omp single
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < k; j++)
            {
#pragma omp task
                {
                    c[i][j] = 0;
                    for (int l = 0; l < o; l++)
                    {
                        c[i][j] += a[i][l] * b[l][j];
                    }
                }
            }
        }
    }
    return c;
}

int main(int argc, char **argv)
{
    int n, m, o, k;
    C = NULL;

    if (argc > 1)
    {
        FILE *f = fopen("mat_in.txt", "r");

        fscanf(f, "%d %d", &n, &m);
        // fscanf(f, "%d", &m);

        A = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
            A[i] = (int *)malloc(m * sizeof(int));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                fscanf(f, "%d ", &A[i][j]);

        fscanf(f, "%d %d", &o, &k);
        // fscanf(f, "%d", &k);
        B = (int **)malloc(o * sizeof(int *));

        for (int i = 0; i < o; i++)
            B[i] = (int *)malloc(k * sizeof(int));

        for (int i = 0; i < o; i++)
            for (int j = 0; j < k; j++)
                fscanf(f, "%d ", &B[i][j]);

        /*

        printf("Matrix A:\n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                printf("%d ", A[i][j]);
            printf("\n");
        }
        printf("Matrix B:\n");
        for (int i = 0; i < o; i++)
        {
            for (int j = 0; j < k; j++)
                printf("%d ", B[i][j]);
            printf("\n");
        }
        */
    }
    else
    {
        scanf("%d", &n);
        scanf("%d", &m);
        scanf("%d", &o);
        scanf("%d", &k);
        A = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
            A[i] = (int *)malloc(m * sizeof(int));

        B = (int **)malloc(o * sizeof(int *));

        for (int i = 0; i < o; i++)
            B[i] = (int *)malloc(k * sizeof(int));

        srand(time(NULL));
        /* Generate random matrices */
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                A[i][j] = rand() % MAX_VAL;
        for (int i = 0; i < o; i++)
            for (int j = 0; j < k; j++)
                B[i][j] = rand() % MAX_VAL;
    }

    if (m != o)
    {
        printf("Invalid matrices!\n");
    }
    else
    {
        struct timeval t1, t2;
        double time_taken;
        gettimeofday(&t1, NULL);
        C = multiply(A, n, m, B, o, k);
        gettimeofday(&t2, NULL);

        FILE *f = fopen("mat_out.txt", "w");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < k; j++)
            {
                fprintf(f, "%d ", C[i][j]);
            }
            fprintf(f, "\n");
        }
        time_taken = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;
        printf("\nTime taken: %f ms\n", time_taken);
    }

    for (int i = 0; i < n; free(A[i++]))
        ;
    free(A);
    for (int i = 0; i < o; free(B[i++]))
        ;
    free(B);
    if (C)
    {
        for (int i = 0; i < n; free(C[i++]))
            ;
        free(C);
    }

    return 0;
}
