#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define MAX_VAL 10
#define FRAC 10

int **A;
int **B;
int **C;

// gcc -g -pthread matrix_multiply_optimal.c -o mat_mul_optimal.o; ./mat_mul_optimal.o
// ./matrix_multiply_optimal.o 1
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
/ Finding optimal thread number for multithreaded matrix multiplication
1. Matrix multiplication is done using the usual formula
2. We divide elements of the resultant amongst the threads
3. A thread only calculates the elements alotted to it
4. Inputs can be taken from a file, or generated randomly
5. This program finds the optimal number of threads to use for multiplying
*/

struct
{
    int **c;
    int **a;
    int n;
    int m;
    int **b;
    int o;
    int k;
    int begin;
    int end;
} typedef task;

void *thread_task(void *arg)
{
    task *todo = (task *)arg;
    for (int ind = todo->begin; ind <= todo->end; ind++)
    {
        int i = ind / (todo->k);
        int j = ind % (todo->k);
        (todo->c)[i][j] = 0;
        for (int l = 0; l < (todo->m); l++)
        {
            (todo->c)[i][j] += (todo->a)[i][l] * (todo->b)[l][j];
        }
    }
    pthread_exit(NULL);
}

int **multiply(int n, int m, int **a, int o, int k, int **b, int t)
{
    int **c = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        c[i] = (int *)malloc(k * sizeof(int));
    }

    int p = (n * k) / t;
    int q = (n * k) % t;

    pthread_t *threads[t];
    task task_info[t];

    int ind = 0;
    for (int i = 0; i < t; i++)
    {
        task_info[i] = (task){.n = n, .m = m, .o = o, .k = k, .c = c, .a = a, .b = b, .begin = ind};
        ind += p + (i < q ? 1 : 0);
        task_info[i].end = ind;
        if (task_info[i].end > n * k - 1)
            task_info[i].end = n * k - 1;
        threads[i] = (pthread_t *)malloc(sizeof(pthread_t));
        pthread_create(threads[i], NULL, &thread_task, (void *)(task_info + i));
        ind++;
    }

    for (int i = 0; i < t; i++)
        pthread_join(*(threads[i]), NULL);

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
        // printf("%d %d\n", n, m);

        A = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
            A[i] = (int *)malloc(m * sizeof(int));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                fscanf(f, "%d ", &A[i][j]);

        // printf("Matrix A:\n");
        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < m; j++)
        //         printf("%d ", A[i][j]);
        //     printf("\n");
        // }

        fscanf(f, "%d %d", &o, &k);
        // printf("%d %d\n", o, k);
        B = (int **)malloc(o * sizeof(int *));

        for (int i = 0; i < o; i++)
            B[i] = (int *)malloc(k * sizeof(int));

        for (int i = 0; i < o; i++)
            for (int j = 0; j < k; j++)
                fscanf(f, "%d ", &B[i][j]);

        // printf("Matrix B:\n");
        // for (int i = 0; i < o; i++)
        // {
        //     for (int j = 0; j < k; j++)
        //         printf("%d ", B[i][j]);
        //     printf("\n");
        // }
    }
    else
    {
        scanf("%d %d", &n, &m);
        A = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
            A[i] = (int *)malloc(m * sizeof(int));

        scanf("%d %d", &o, &k);
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
        return 0;

    struct timeval t1, t2;
    double time_taken;

    double best_time = -1;
    int best_threads = 0;
    int total = n * k;
    int report_threshold = total / FRAC;
    if (!report_threshold)
        report_threshold = 2;

    for (int ind = 1; ind <= total; ind++)
    {
        gettimeofday(&t1, NULL);
        C = multiply(n, m, A, o, k, B, ind);
        gettimeofday(&t2, NULL);
        time_taken = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;

        if (ind == 1)
        {
            FILE *g = fopen("mat_out.txt", "w");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < k; j++)
                    fprintf(g, "%d ", C[i][j]);
                fprintf(g, "\n");
            }
            best_threads = 1;
            best_time = time_taken;
        }

        if (best_time > time_taken)
        {
            best_time = time_taken;
            best_threads = ind;
        }
        if (ind % report_threshold == 0)
            printf("Current threads: %d |  Current time: %f ms |  Best threads:  %d |  Best time: %f ms\n", ind, time_taken, best_threads, best_time);

        for (int i = 0; i < n; free(C[i++]))
            ;
        free(C);
    }

    printf("\n-----------\nResult:\nOptimal thread number: %d\nTime with optimal threads: %f\n", best_threads, best_time);

    for (int i = 0; i < n; free(A[i++]))
        ;
    free(A);
    for (int i = 0; i < o; free(B[i++]))
        ;
    free(B);

    return 0;
}
