#include <omp.h>
#include "matrix.h"

void swap(double* a, double* b) { 
    double t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition_l (double **matrix, int low, int high) { 
    int random = low + rand() % (high - low);
    swap(&matrix[0][random], &matrix[0][high]); 

    double pivot = matrix[0][high];   
    int i = (low - 1);  
  
    for (int j = low; j <= high - 1; j++) { 
        if (matrix[0][j] < pivot) { 
            i++;   
            swap(&matrix[0][i], &matrix[0][j]); 
        } 
    }
    swap(&matrix[0][i + 1], &matrix[0][high]);
    return (i + 1); 
}

int partition(double **matrix, int low, int high)
{
    double pivot = matrix[0][low];
    int i = low - 1, j = high + 1;

    while(1){
        // Find leftmost element greater than
        // or equal to pivot
        do{
            i++;
        }while (matrix[0][i] < pivot);

        // Find rightmost element smaller than
        // or equal to pivot
        do{
            j--;
        }while (matrix[0][j] > pivot);

        // If two pointers met.
        if (i >= j)
            return j;

        swap(&matrix[0][i], &matrix[0][j]);
    }
}

void quickSort(double **matrix, int low, int high) { 
    if (low < high) { 
        int pi = partition(matrix, low, high); 
  
        quickSort(matrix, low, pi - 1); 
        quickSort(matrix, pi + 1, high); 
    } 
} 


matrix_t *matrix_sort_quick(matrix_t *A)
{
    int n = A->rows * A->cols; 

    quickSort(A->matrix, 0, n - 1); 

    return A;
}

void quickSort_openmp(double **matrix, int low, int high, int current_level, int last_level){
    if (low < high) { 
        int pi = partition(matrix, low, high); 
  
        if(current_level < last_level){
            omp_set_nested(1);
            #pragma omp parallel num_threads(2)
            {
                #pragma omp sections nowait
                {
                    #pragma omp section
                    quickSort_openmp(matrix, low, pi, current_level + 1, last_level);

                    #pragma omp section
                    quickSort_openmp(matrix, pi + 1, high, current_level + 1, last_level);
                }

            }

        }else{
            quickSort(matrix, low, pi);
            quickSort(matrix, pi + 1, high);
        }
    }
} 


matrix_t *matrix_sort_quick_openmp(matrix_t *A, int current_level, int last_level)
{
    int n = A->rows * A->cols;

    quickSort_openmp(A->matrix, 0, n - 1, current_level, last_level);

    return A;
}