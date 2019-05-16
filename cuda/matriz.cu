#include <stdio.h>
#include <math.h>

#define N 1200
#define THREADS 1024

__global__ void matrixMultKernel (double *a, double *b, double *c, int n)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if((row < n) && (col < n)){
        double v = 0;
        for(int k = 0; k < n; k++){
            v += a[row * n + k] * b[k * n + col];
        }
        c[row * n + col] = v;
    }
}

int main(){

    double *a, *b, *c;
    double *d_a, *d_b, *d_c;
    double size = N * N * sizeof(double);

    cudaMalloc((void**) &d_a, size);
    cudaMalloc((void**) &d_b, size);
    cudaMalloc((void**) &d_c, size);

    a = (double*)malloc(size);
    b = (double*)malloc(size);
    c = (double*)malloc(size);

    for(int i = 0; i < N * N; i++){
        a[i] = b[i] = i;
        c[i] = 0;
    }

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    matrixMultKernel<<<(int)ceil(N / THREADS), THREADS>>> (d_a, d_b, d_c, N);
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    
    printf("c[0] = %lf\n", c[0]);
    printf("c[%d] = %lf\n", N-1, c[N-1]);

    free(a);
    free(b);
    free(c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;

}