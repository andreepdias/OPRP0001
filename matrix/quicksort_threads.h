#ifndef __QUICKSORT_THREADS_H
#define __QUICKSORT_THREADS_H

#include "matrix.h"

typedef struct {
	int id;
	double ***matrix;
	int low;
	int high;
	int current_level;
	int last_level;
} DadosThreadQuickSort;

#endif