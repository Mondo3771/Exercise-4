#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
// a={1,3,4,6,5,2,9,8,7}
// if a[i] = 5
// then temp = [0,0,0,0,5] , count = 4
// meain that 5 is bigger than 4 numbers in the array
// if a[i] = 9
// then temp = [9,0,0,0,5] , count = 0
// this sort is produces somethings in decending order
void count_sort_seq(int a[], int n)
{
    int i, j, count;
    int *temp = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
    {
        count = 0;
        for (j = 0; j < n; j++)
        {
            if (a[i] < a[j])
            {
                count++;
            }
            else if (a[j] == a[i] && j < i)
            {
                count++;
            }
        }
        temp[count] = a[i];
    }
    memcpy(a, temp, n * sizeof(int));
    free(temp);
}
void count_sort_Par(int a[], int n)
{
    int i, j, count;
    int *temp = malloc(n * sizeof(int));
#pragma omp parallel private(j,count) shared(i,n,temp)
    {
#pragma omp for
        for (i = 0; i < n; i++)
        {
            count = 0;
            for (j = 0; j < n; j++)
            {
                if (a[i] < a[j])
                {
                    count++;
                }
                else if (a[j] == a[i] && j < i)
                {
                    count++;
                }
            }
#pragma omp critical
            temp[count] = a[i];
        }
    }
    memcpy(a, temp, n * sizeof(int));
    free(temp);
}
bool Sorted(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > arr[i - 1])
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
    int *arr_par = (int *)malloc(p * sizeof(int));

    srand(time(0));
    for (int i = 0; i < p; i++)
    {
        arr_serial[i] = rand() % p;
        arr_par[i] = arr_serial[i];
    }
    double start, end;

    start = omp_get_wtime();
    count_sort_seq(arr_serial, p);
    end = omp_get_wtime();
    double s_time = end - start;
    printf("Time taken By Sequential: %f \n", end - start);
    Sorted(arr_serial, p) ? printf("Sorted \n") : printf("UnSorted \n");

    start = omp_get_wtime();
    count_sort_Par(arr_par, p);
    end = omp_get_wtime();
    double p_time = end - start;
    printf("Time taken By Parallel: %f \n", end - start);
    Sorted(arr_par, p) ? printf("Sorted \n") : printf("UnSorted \n");

    printf("Speedup: %f \n", s_time / p_time);
    free(arr_par);
    free(arr_serial);

    return 0;
}
