#include <stdio.h>
#include <math.h>

__global__ void matrixMultKernel(double *d_m, double *d_n, double *d_p, int n){

    __shared__ double Mds[TILE_WIDTH][TILE_WIDTH];
    __shared__ double Nds[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;

    double pvalue = 0;

    for(int ph = 0; ph < n / TILE_WIDTH; ph++){

        Mds[ty][tx] = d_m[row * n + ph * TILE_WIDTH + tx];
        Nds[ty][tx] = d_n[(ph * TILE_WIDTH + ty) * n + col];
        __syncthreads();
        
        for(int k = 0; k < TILE_WIDTH; k++){
            pvalue += Mds[ty][k] * Nds[k][tx];
        }
        __syncthreads();
    }
    d_p[row * n + col = pvalue];
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