#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

// gcc banker_algorithm.c -o banker_algorithm.o; ./banker_algorithm.o
/*
/ Banker's Algorithm
1. Deadlock avoidance algorithm for multiple isntance resources and processes
2. Consists of three components:
    i.   Safety algorithm - Checks if system is in safe state or not
    ii.  Request checking algorithm - Checks if a process may be granted
                                    a request vector without entering unsafe
                                    state.
    iii. Deadlock handling algorithm - Breaks a deadlock condition by iteratively
                                    preempting processes and freeing their resources.
3. Takes input from text file.
*/

int safety(int m, int n, int resource_instances[m], int allocated[n][m], int max_needed[n][m], int needed[n][m])
{
    printf("------\nSafe state checking:\n\n");

    int available[m];
    for (int i = 0; i < m; i++)
        available[i] = resource_instances[i];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            available[j] -= allocated[i][j];
    int work[m];
    for (int i = 0; i < m; i++)
        work[i] = available[i];
    int finished[n];
    for (int i = 0; i < n; i++)
        finished[i] = 0;
    int flag, finished_sum = 0;
    int safe_sequence[n], safe_ind = 0;

    while (finished_sum < n)
    {
        flag = 0;
        int temp = -1;
        for (int i = 0; i < n; i++)
        {
            if (finished[i] == 0)
            {
                int temp2 = 1;
                for (int j = 0; j < m; j++)
                    if (work[j] < needed[i][j])
                    {
                        temp2 = 0;
                        break;
                    }
                if (temp2)
                {
                    temp = i;
                    break;
                }
                flag++;
            }
        }

        if (flag == n || temp == -1)
        {
            printf("Unsafe\n------\n");
            return 0;
        }

        finished[temp] = 1;
        finished_sum++;
        safe_sequence[safe_ind++] = temp;
        for (int i = 0; i < m; i++)
        {
            work[i] += allocated[temp][i];
            allocated[temp][i] = 0;
        }
    }

    printf("Safe sequence: ");
    for (int k = 0; k < n; k++)
        printf("%d ", safe_sequence[k]);
    printf("\n------\n");
    return 1;
}

int request_check(int m, int n, int resource_instances[m], int allocated[n][m], int max_needed[n][m], int needed[n][m], int pind, int request[m])
{
    printf("Request checking:\n\n");
    int available[m];
    for (int i = 0; i < m; i++)
        available[i] = resource_instances[i];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            available[j] -= allocated[i][j];
    // int flag = 1;
    for (int i = 0; i < n; i++)
    {
        if (available[i] < request[i] || request[i] > needed[pind][i])
        {
            return 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        allocated[pind][i] += request[i];
        available[i] -= request[i];
    }

    return safety(m, n, resource_instances, allocated, max_needed, needed);
}

int deadlock_handle(int m, int n, int resource_instances[m], int allocated[n][m], int max_needed[n][m], int needed[n][m])
{
    printf("Deadlock resolution:\n\n");
    if (safety(m, n, resource_instances, allocated, max_needed, needed))
        return 1;

    int preempted = 0;

    for (; preempted < n; preempted++)
    {
        for (int i = 0; i < m; i++)
        {
            allocated[preempted][i] = 0;
            needed[preempted][i] = max_needed[preempted][i];
        }
        printf("Preempted process %d.\n", preempted);
        if (safety(m, n, resource_instances, allocated, max_needed, needed))
        {
            printf("Deadlock resolved.\n");
            return 1;
        }
    }
    printf("Deadlock could not be resolved.\n");
    return 0;
}

int main()
{
    FILE *f = fopen("banker_algo_in.txt", "r");
    int m;
    fscanf(f, "%d", &m);
    int resource_instances[m];
    for (int i = 0; i < m; i++)
        fscanf(f, "%d ", &resource_instances[i]);
    int n;
    fscanf(f, "%d ", &n);
    int allocated[n][m];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(f, "%d ", &allocated[i][j]);
    int max_needed[n][m];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(f, "%d ", &max_needed[i][j]);
    int needed[n][m];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            needed[i][j] = max_needed[i][j] - allocated[i][j];

    int ans = safety(m, n, resource_instances, allocated, max_needed, needed);

    int pind;
    fscanf(f, "%d", &pind);
    int request[m];
    for (int i = 0; i < m; i++)
    {
        fscanf(f, "%d ", &request[i]);
    }
    if (request_check(m, n, resource_instances, allocated, max_needed, needed, pind, request))
    {
        printf("Request can be granted.\n---\n");
        for (int i = 0; i < m; i++)
        {
            allocated[pind][i] += request[i];
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                needed[i][j] = max_needed[i][j] - allocated[i][j];
    }
    else
        printf("Request cannot be granted.\n---\n");

    if (!ans)
        deadlock_handle(m, n, resource_instances, allocated, max_needed, needed);

    fclose(f);
    return 0;
}
