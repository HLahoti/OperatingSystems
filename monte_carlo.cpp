#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <random>
#include <omp.h>
#include <numbers>
using namespace std;

#define PI 3.14159265358979323846

// g++ -fopenmp monte_carlo_fast.cpp -o monte_carlo.o -lm
// ./monte_carlo.o

/*
Monte carlo appraoch, but multithreaded
1.  We use the monte carlo appraoch to approximate the value of pi
    The program generates n random points inside asquare (side 2*r)
    and then counts how many are inside the circle (which has radius r)
2.  pi = 4 * (number of points inside the circle)/(total number of points)
    This program uses OpenMP for achieving parallel computing.
3.  The program only requires C++ for random numbers, otherwise using C is fine.
*/

int main(int argc, char *argv[])
{
    long int n, s = 0;
    double x, y, d, t1, t2;
    printf("Number of points to generate: ");
    scanf("%ld", &n);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-1.0, 1.0);

    t1 = omp_get_wtime();
#pragma omp parallel for private(x, y, d) default(shared) reduction(+ : s) schedule(dynamic, 1000)
    for (long int i = 0; i < n; i++)
    {
        srand((unsigned int)time(NULL));

#pragma omp tasks
        {
            x = dis(gen);
            y = dis(gen);
            d = sqrt(x * x + y * y);

            if (d <= 1.0)
            {
                // #pragma omp critical
                //! I removed critical because I added reduction to the directive
                s += 1;
            }
        }
    }
    t2 = omp_get_wtime();

    long double pi = (s * 4) / (n * 1.0);
    long double err = abs((PI - pi) * 100 / PI);

    printf("Points inside circle:         %ld\n", s);
    printf("Approximation of pi :         %Lf\n", pi);
    printf("Error :                       %Lf%%\n", err);
    printf("Time taken :                  %g s\n", t2 - t1);

    return 0;
}
