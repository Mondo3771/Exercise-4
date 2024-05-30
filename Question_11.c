#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

int fib_seq(int n)
{
    if (n <= 1)
    {
        return n;
    }
    else
    {
        return fib_seq(n - 1) + fib_seq(n - 2);
    }
}

int fib_seq_task(int n, int cut_off)
{
    int i, j;
    if (n <= 1)
    {
        return n;
    }
    else if (n < cut_off)
    {
        return fib_seq(n - 1) + fib_seq(n - 2);
    }
    else
    {
        int sum = 0;
#pragma omp task shared(i)
        {
            i = fib_seq_task(n - 1, cut_off);
        }
#pragma omp task shared(j)
        {
            j = fib_seq_task(n - 2, cut_off);
        }
#pragma omp taskwait
        sum = i + j;
    }
}

int fib_seq_sec(int n, int cut)
{
    int i, j;

    if (n <= 1)
    {
        return n;
    }
    else if (n < cut+5)
    {
        return fib_seq(n - 1) + fib_seq(n - 2);
    }
    else
    {
        omp_set_num_threads(2);
#pragma omp parallel sections shared(i, j)
        {
#pragma omp section
            {
                i = fib_seq_sec(n - 1, cut);
            }

#pragma omp section
            {
                j = fib_seq_sec(n - 2, cut);
            }
        }
        return i + j;
    }
}

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    double start, end, p_time, s_time, t_time;
    int fib = 0;

    start = omp_get_wtime();
    fib = fib_seq(n);
    end = omp_get_wtime();
    p_time = end - start;
    printf("Fib number: %i \n", fib);
    printf("Time Taken Seq: %f \n \n", p_time);
    int cut = n / 2;
    start = omp_get_wtime();

    omp_set_nested(1);
#pragma omp parallel shared(n)
    {
#pragma omp single
        fib = fib_seq_task(n, cut);
    }
    printf("Fib number: %i \n", fib);
    end = omp_get_wtime();
    t_time = end - start;
    printf("Time Taken Task: %f \n", t_time);
    printf("SpeedUp Task: %f \n \n", p_time / t_time);

    start = omp_get_wtime();
    fib = fib_seq_sec(n, cut);
    end = omp_get_wtime();
    s_time = end - start;
    printf("Fib number: %i \n", fib);
    printf("Time Taken Sections: %f \n", s_time);
    printf("SpeedUp Task: %f \n", p_time / s_time);

    return 0;
}
