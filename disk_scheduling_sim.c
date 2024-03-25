#include <stdio.h>
#include <stdlib.h>
#define diskSize 200

// gcc disk_scheduling_sim.c -o disk_scheduling_sim.o; ./disk_scheduling_sim.o
/*
/ Disk scheduling simulator
1. Program to demonstrate the workings of 6 different disk scheduling algorithms
2. The program reads a file containing requests for access
3. Each strategy for scheduling is run on the given condition
4. Results include:
    i.   Order of access
    ii.  Number of head position changes
*/

int endOfDisk = diskSize - 1;

void FCFS(int arr[], int n, int head)
{

    int totalSeekOperations = 0;
    int currentPosition = head;

    printf("Order of disk requests visited: ");
    for (int i = 0; i < n; i++)
    {
        totalSeekOperations += abs(currentPosition - arr[i]);
        currentPosition = arr[i];
        printf("%d ", arr[i]);
    }

    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

void SSTF(int arr[], int n, int head)
{
    int totalSeekOperations = 0;
    int currentPosition = head;
    int count = 0, mdist = 0;
    int finished[n];
    for (int i = 0; i < n; i++)
        finished[i] = 0;

    printf("Order of disk requests visited: ");
    while (count < n)
    {
        mdist = -1;
        for (int i = 0; i < n; i++)
            if (finished[i] == 0)
            {
                if ((mdist == -1) || (abs(arr[i] - currentPosition) < abs(arr[mdist] - currentPosition)))
                    mdist = i;
            }
        totalSeekOperations += abs(arr[mdist] - currentPosition);
        currentPosition = arr[mdist];
        finished[mdist] = 1;
        count++;
        printf("%d ", arr[mdist]);
    }
    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

void SCAN(int arr[], int n, int head, int direction)
{

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int totalSeekOperations = 0;
    int currentPosition = head;

    printf("Order of disk requests visited: ");
    if (direction == 1)
    {
        for (int i = 0; i < n; i++)
        {
            if (arr[i] >= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        if (arr[0] < head)
        {
            totalSeekOperations += endOfDisk - currentPosition;
            currentPosition = endOfDisk;
        }
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] < head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] <= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        if (arr[n - 1] > head)
        {
            totalSeekOperations += currentPosition;
            currentPosition = 0;
        }
        for (int i = 0; i < n; i++)
        {
            if (arr[i] > head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }

    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

void CSCAN(int arr[], int n, int head, int direction)
{

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int totalSeekOperations = 0;
    int currentPosition = head;

    printf("Order of disk requests visited: ");
    if (direction == 1)
    {
        for (int i = 0; i < n; i++)
        {
            if (arr[i] >= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        totalSeekOperations += abs(currentPosition - endOfDisk);
        currentPosition = endOfDisk;
        if (arr[0] < head)
        {
            printf("R-0 ");
            totalSeekOperations += endOfDisk - currentPosition;
            totalSeekOperations += endOfDisk;
            currentPosition = 0;
        }
        for (int i = 0; i < n; i++)
        {
            if (arr[i] < head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] <= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        if (arr[n - 1] > head)
        {
            printf("R-%d ", endOfDisk);
            totalSeekOperations += currentPosition;
            totalSeekOperations += endOfDisk;
            currentPosition = endOfDisk;
        }
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] > head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }

    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

void LOOK(int arr[], int n, int head, int direction)
{

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int totalSeekOperations = 0;
    int currentPosition = head;

    printf("Order of disk requests visited: ");
    if (direction == 1)
    {
        for (int i = 0; i < n; i++)
        {
            if (arr[i] >= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] < head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] <= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (arr[i] > head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }

    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

void CLOOK(int arr[], int n, int head, int direction)
{

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int totalSeekOperations = 0;
    int currentPosition = head;

    printf("Order of disk requests visited: ");
    if (direction == 1)
    {
        for (int i = 0; i < n; i++)
        {
            if (arr[i] >= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        if (arr[0] < head)
        {
            printf("R-%d ", arr[0]);
            totalSeekOperations += currentPosition - arr[0];
            currentPosition = arr[0];
        }
        for (int i = 0; i < n; i++)
        {
            if (arr[i] < head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] <= head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
        if (arr[n - 1] > head)
        {
            printf("R-%d ", arr[n - 1]);
            totalSeekOperations += arr[n - 1] - currentPosition;
            currentPosition = arr[n - 1];
        }
        for (int i = n - 1; i >= 0; i--)
        {
            if (arr[i] > head)
            {
                printf("%d ", arr[i]);
                totalSeekOperations += abs(currentPosition - arr[i]);
                currentPosition = arr[i];
            }
        }
    }

    printf("\nTotal head movements: %d\n", totalSeekOperations);
}

int main()
{
    FILE *f = fopen("disk_scheduling_in.txt", "r");
    int n, head, direction, algorithm;
    fscanf(f, "%d", &n); // number of disk requests
    int arr[n];

    for (int i = 0; i < n; i++) // positions of disk requests
    {
        fscanf(f, "%d", &arr[i]);
    }

    fscanf(f, "%d", &head); // initial head position

    // Direction/priority: 1-> higher, 0-> Lower
    fscanf(f, "%d", &direction);

    printf("\nFCFS algorithm:\n\n");
    FCFS(arr, n, head);
    printf("-----\n\n");
    printf("\nSSTF algorithm:\n\n");
    SSTF(arr, n, head);
    printf("-----\n\n");
    printf("\nSCAN algorithm:\n\n");
    SCAN(arr, n, head, direction);
    printf("-----\n\n");
    printf("\nC-SCAN algorithm:\n\n");
    CSCAN(arr, n, head, direction);
    printf("-----\n\n");
    printf("\nLOOK algorithm:\n\n");
    LOOK(arr, n, head, direction);
    printf("-----\n\n");
    printf("\nC-LOOK algorithm:\n\n");
    CLOOK(arr, n, head, direction);
    printf("-----\n\n");

    return 0;
}
