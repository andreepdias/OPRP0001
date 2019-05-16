#include <stdio.h>
#include <math.h>

#define N 1200
#define THREADS 1024

__global__ void vecAdd (int *a, int *b, int *c)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if(index < N){
        c[index] = a[index] + b[index];
    }
}

int main(){

    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    int size = N * sizeof(int);

    cudaMalloc((void**) &d_a, size);
    cudaMalloc((void**) &d_b, size);
    cudaMalloc((void**) &d_c, size);

    a = (int*)malloc(size);
    b = (int*)malloc(size);
    c = (int*)malloc(size);

    for(int i = 0; i < N; i++){
        a[i] = b[i] = i;
        c[i] = 0;
    }

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    // vecAdd<<<1, N>>> (d_a, d_b, d_c);
     vecAdd<<<(int)ceil(N/THREADS), THREADS>>> (d_a, d_b, d_c);
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    
    printf("c[0] = %d\n", c[0]);
    printf("c[%d] = %d\n", N-1, c[N-1]);

    free(a);
    free(b);
    free(c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;

}