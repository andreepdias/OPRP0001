#include <omp.h>
#include "matrix.h"

void merge(double **matrix, int low, int mid, int high)
{
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // printf("low:%d\thigh:%d\nn1:%d\tn2:%d\n", low, high, n1, n2);

    double *L = malloc(sizeof(double) * n1);
    double *R = malloc(sizeof(double) * n2);

    for (i = 0; i < n1; i++)
        L[i] = matrix[0][low + i];
    for (j = 0; j < n2; j++)
        R[j] = matrix[0][mid + 1 + j];

    i = 0; 
    j = 0; 
    k = low; 
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            matrix[0][k] = L[i];
            i++;
        }
        else
        {
            matrix[0][k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        matrix[0][k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        matrix[0][k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(double **matrix, int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;

        mergeSort(matrix, low, mid);
        mergeSort(matrix, mid + 1, high);

        merge(matrix, low, mid, high);
    }
}

matrix_t *matrix_sort_merge(matrix_t *A)
{
    int n = A->rows * A->cols;

    mergeSort(A->matrix, 0, n - 1);

    return A;
}

void mergeSort_openmp(double **matrix, int low, int high, int current_level, int last_level)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;

        // printf("current: %d\tlast: %d\t\tlow: %d\thigh: %d\n", current_level, last_level, low, high);
        if(current_level < last_level){
            omp_set_nested(1);
            #pragma omp parallel num_threads(2)
            {
                #pragma omp sections nowait
                {
                    #pragma omp section
                    mergeSort_openmp(matrix, low, mid, current_level + 1, last_level);

                    #pragma omp section
                    mergeSort_openmp(matrix, mid + 1, high, current_level + 1, last_level);
                }

            }

        }else{
            mergeSort(matrix, low, mid);
            mergeSort(matrix, mid + 1, high);
        }
        merge(matrix, low, mid, high);
    }
}

matrix_t *matrix_sort_merge_openmp(matrix_t *A, int current_level, int last_level)
{
    int n = A->rows * A->cols;

    mergeSort_openmp(A->matrix, 0, n - 1, current_level, last_level);

    return A;
}