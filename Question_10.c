#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

int min_Seq(int a[], int n)
{
    int min = 100000000;

    for (int i = 0; i < n; i++)
    {
        if (min > a[i])
        {
            min = a[i];
        }
    }
    return min;
}

int min_Par(int a[], int n)
{
    int min = 100000000;
    int local_min = min;
    int i;
#pragma omp parallel shared(a, i, n, min) firstprivate(local_min)
    {

#pragma omp for
        for (i = 0; i < n; i++)
        {
            if (local_min > a[i])
            {
                local_min = a[i];
            }
        }

#pragma omp critical
        {
            if (local_min < min)
            {
                min = local_min;
            }
        }
    }
    return min;
}

int min_Par_Reduction(int a[], int n)
{
    int min = 100000000;
#pragma omp parallel shared(a, n, min)
    {
#pragma omp for reduction(min : min)
        for (int i = 0; i < n; i++)
        {
            if (min > a[i])
            {
                min = a[i];
            }
        }
    }
    return min;
}

bool Min(int a[], int n, int x)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] < x)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int p = atoi(argv[1]);
    int *arr_serial = (int *)malloc(p * sizeof(int));

    srand(time(0));
    for (int i = 0; i < p; i++)
    {
        arr_serial[i] = rand() % p;
    }

    double start, end;
    int m = 0;
    start = omp_get_wtime();
    m = min_Seq(arr_serial, p);
    end = omp_get_wtime();
    double s_time = end - start;
    Min(arr_serial, p, m) ? printf("Found it:\n") : printf("Didnt find it\n");
    printf("time sequtially: %f \n", s_time);

    start = omp_get_wtime();
    m = min_Par(arr_serial, p);
    end = omp_get_wtime();
    double p_time = end - start;
    Min(arr_serial, p, m) ? printf("Found it:\n") : printf("Didnt find it\n");
    printf("time sequtially: %f \n", p_time);
    printf("Speedup: %f \n", s_time / p_time);

    start = omp_get_wtime();
    m = min_Par_Reduction(arr_serial, p);
    end = omp_get_wtime();
    double r_time = end - start;
    Min(arr_serial, p, m) ? printf("Found it:\n") : printf("Didnt find it\n");
    printf("time sequtially: %f \n", r_time);
    printf("Speedup: %f \n", s_time / r_time);

    free(arr_serial);

    return 0;
}
