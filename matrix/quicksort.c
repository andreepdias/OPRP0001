#include "matrix.h"

void swap(double* a, double* b) 
{ 
    double t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition (double **matrix, int low, int high) 
{ 
    double pivot = matrix[0][high];   
    int i = (low - 1);  
  
    for (int j = low; j <= high- 1; j++) 
    { 
        if (matrix[0][j] <= pivot) 
        { 
            i++;   
            swap(&matrix[0][i], &matrix[0][j]); 
        } 
    } 
    swap(&matrix[0][i + 1], &matrix[0][high]); 
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