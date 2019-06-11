#include <bits/stdc++.h>
#include <omp.h>
#include <stdio.h>
using namespace std;


int main(int argc, char **argv)
{
    int a = 0, b = 5000;
    #pragma omp parallel num_threads(8)
    {
        while(a < b)
        {
            a++;
            int tid = omp_get_thread_num();
            if (tid == omp_get_num_threads() - 1)
            {
                printf("%d\t%d\n", tid, a);
            }
        }
    }
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA\n");

}
