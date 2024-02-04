#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// gcc -g -pthread mergesort.c -o merge.o
// ./merge.o

/*
/ 8

! add -g -pthread to your gcc command
* be extremely careful with shared memory in threads
* threads may try to modify the same memory
* that is called a race condition
! gcc -S for assembly compilation

/ Mutex
Protects from race conditions.
Does not allow any other thread to execute the code
inside the locked region.
*Mutex works with more than 2 threads too.
Try not to use it too often, it is a little slow.

! never create and join threads in the same for loop,
! that will only mean thre is no parallel execution.

/ Returning values
cant retrurn local variables ofc, so we return pointer
then free the pointer later

/ Passing arguments
Since you're only passing memory locations,
you have to be careful

/ We use conditions to control thread execution
pthread_cond_wait calls the unlock method and
waits till a signal is send back using the same
cond_t object. Once that signal is recieved
the lock method is again called and execution resumes
It is important to keep pthread_cond_wait inside a
condition loop.
!pthread_cond_signal signals at most 2 waits

/ Monte carlo appraoch, but multithreaded
1.  We use the monte carlo appraoch to approximate the value of pi
    The program generates n random points inside asquare (side 2*r)
    and then counts how many are inside the circle (which has radius r)
2.  pi = 4 * (number of points inside the circle)/(total number of points)
    This program uses OpenMP for achieving parallel computing.
3.  The program only requires C++ for random numbers, otherwise using C is fine.

/ Quicksort, multithreaded
1.  An array is partitioned around a pivot element
    In this case, the last element of any subarray
2.  The array is partitioned into 2 parts, left smaller than pivot
    Right larger than the pivot
3.  Each parition is recursively sorted in parallel on different threads.

/ Merge sort, multithreaded
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
