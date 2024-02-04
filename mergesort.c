#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// gcc -g -pthread mergesort.c -o merge.o
// ./merge.o

/*
Merge sort, multithreaded
1.  An array is divided into two halves recursively
2.  Both halves are sorted separately in parallel
3.  Then they are merged together
*/

typedef struct
{
    int *a;
    int r;
    int l;
} arr;

void *mergesort(void *arg)
{
    arr s = *(arr *)arg;

    if (s.r <= s.l)
        return NULL;
    if (s.r == s.l + 1)
    {
        if (*(s.a + s.l) > *(s.a + s.r))
        {
            *(s.a + s.l) += *(s.a + s.r);
            *(s.a + s.r) = *(s.a + s.l) - *(s.a + s.r);
            *(s.a + s.l) = *(s.a + s.l) - *(s.a + s.r);
        }
        return NULL;
    }
    // Recursion
    pthread_t recs[2];
    arr left = s;
    left.r = (s.l + s.r) / 2;
    arr right = s;
    right.l = left.r + 1;
    pthread_create(&recs[0], NULL, &mergesort, &left);
    pthread_create(&recs[1], NULL, &mergesort, &right);

    pthread_join(recs[0], NULL);
    pthread_join(recs[1], NULL);

    int i, j, k;

    int *L = (int *)malloc((left.r - left.l + 1) * sizeof(int));
    int *R = (int *)malloc((right.r - right.l + 1) * sizeof(int));

    for (i = 0; i < (left.r - left.l + 1); i++)
        L[i] = *(s.a + i + left.l);
    for (i = 0; i < (right.r - right.l + 1); i++)
        R[i] = *(s.a + i + right.l);

    i = 0;
    j = 0;
    k = s.l;

    while (i < (left.r - left.l + 1) && j < (right.r - right.l + 1))
    {
        if (L[i] <= R[j])
            *(s.a + k++) = *(L + i++);
        else
            *(s.a + k++) = *(R + j++);
    }

    while (i < (left.r - left.l + 1))
        *(s.a + k++) = *(L + i++);
    while (j < (right.r - right.l + 1))
        *(s.a + k++) = *(R + j++);

    free(R);
    free(L);

    return NULL;
}

int main(int argc, char *argv[])
{
    int n;
    if (argc < 2)
    {
        printf("Size of array: ");
        scanf("%d", &n);
    }
    else
        n = atoi(argv[1]);
    int a[n];
    srand(time(NULL));
    printf("Generated array:\n");
    int i;
    for (i = 0; i < n; i++)
    {
        a[i] = rand() % 100 - 50;
        printf("%d ", a[i]);
    }
    printf("\n");
    pthread_t *th = (pthread_t *)malloc(sizeof(pthread_t));
    arr s;
    s.a = a;
    s.l = 0;
    s.r = n - 1;
    pthread_create(th, NULL, &mergesort, &s);
    pthread_join(*th, NULL);
    free(th);

    short int wrong = 0;
    printf("\nSorted:  \n");
    for (i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
        if (i && a[i] < a[i - 1])
            wrong = 1;
    }
    printf("\n");
    if (wrong)
    {
        printf("\nWrong answer!\n");
    }
    else
        printf("\nVerified answer!\n");

    return 0;
}
