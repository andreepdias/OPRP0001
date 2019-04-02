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