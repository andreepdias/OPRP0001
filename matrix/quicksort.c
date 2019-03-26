#include "matrix.h"

void swap(double* a, double* b) 
{ 
    double t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition (double **matrix, int low, int high) 
{ 
    int r = (rand() % (high - low)) + low + 1;
    printf("high:%d\tlow:%d\tr:%d\n", high, low, r);
    double pivot = matrix[0][r];   
    // printf("low:%d\thigh:%d\tr:%d\n", low, high, r);

    int i = (low - 1);  
  
    for (int j = low; j <= high - 1; j++) 
    { 
        if (matrix[0][j] <= pivot) 
        { 
            i++;   
            swap(&matrix[0][i], &matrix[0][j]); 
        } 
    }
    swap(&matrix[0][i + 1], &matrix[0][r]);
    return (i + 1); 
} 

void quickSort(double **matrix, int low, int high) 
{ 
    if (low < high) 
    { 
        int pi = partition(matrix, low, high); 
  
        quickSort(matrix, low, pi - 1); 
        quickSort(matrix, pi + 1, high); 
    } 
} 


matrix_t *matrix_sort_quick(matrix_t *A)
{
    matrix_t *m = matrix_clone(A);

    int n = A->rows * A->cols; 

    quickSort(m->matrix, 0, n - 1); 

    return m;
}