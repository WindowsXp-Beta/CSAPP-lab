/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int ii, i, jj, j, tmp;
    int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
    if(M == 32 && N == 32) {
        for (ii = 0; ii < M; ii+=8) {
            for (jj = 0; jj < N; jj+=8) {
                if (ii == jj) {//处理大块对角线情况
                    for (i = ii; i < ii + 8; i++) {
                        for (j = jj; j < jj + 8; j++) {
                            if (i != j) {
                                tmp = A[i][j];
                                B[j][i] = tmp; 
                            }
                            else switch (i - ii) {
                                case 0:{
                                    a1 = A[i][i];
                                    break;
                                }
                                case 1:{
                                    a2 = A[i][i];
                                    break;
                                }
                                case 2:{
                                    a3 = A[i][i];
                                    break;
                                }
                                case 3:{
                                    a4 = A[i][i];
                                    break;
                                }
                                case 4:{
                                    a5 = A[i][i];
                                    break;
                                }
                                case 5:{
                                    a6 = A[i][i];
                                    break;
                                }
                                case 6:{
                                    a7 = A[i][i];
                                    break;
                                }
                                case 7:{
                                    tmp = A[i][i];
                                    break;
                                }
                            }
                        }
                    }
                    B[ii][ii] = a1;
                    B[ii + 1][ii + 1] = a2;
                    B[ii + 2][ii + 2] = a3;
                    B[ii + 3][ii + 3] = a4;
                    B[ii + 4][ii + 4] = a5;
                    B[ii + 5][ii + 5] = a6;
                    B[ii + 6][ii + 6] = a7;
                    B[ii + 7][ii + 7] = tmp;
                }
                else for (i = ii; i < ii + 8; i++) {//处理大块非对角线情况
                    for (j = jj; j < jj + 8; j++) {
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
        }
    }
    else if (M == 61 && N == 67) {
        for (ii = 0; ii < N; ii += 16) {
            for (jj = 0; jj < M; jj += 16) {
                for (i = ii; i < ii + 16 && i < N; i++) {
                    for (j = jj; j < jj + 16 && j < M; j++)
                    {
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
        } 
    }
    else if (M == 64 && N == 64) {
        for (ii = 0; ii < N; ii += 8){
            for (jj = 0; jj < M; jj += 8){
                for (i = ii; i < ii + 4; i++){
                    a1 = A[i][jj];
                    a2 = A[i][jj + 1];
                    a3 = A[i][jj + 2];
                    a4 = A[i][jj + 3];
                    a5 = A[i][jj + 4];
                    a6 = A[i][jj + 5];
                    a7 = A[i][jj + 6];
                    tmp = A[i][jj + 7];

                    B[jj][i] = a1;
                    B[jj + 1][i] = a2;
                    B[jj + 2][i] = a3;
                    B[jj + 3][i] = a4;
                    B[jj][i + 4] = a5;
                    B[jj + 1][i + 4] = a6;
                    B[jj + 2][i + 4] = a7;
                    B[jj + 3][i + 4] = tmp;
                }

                for (i = jj; i < jj + 4; i++){
                    a1 = A[ii + 4][i];
                    a2 = A[ii + 5][i];
                    a3 = A[ii + 6][i];
                    a4 = A[ii + 7][i];

                    a5 = B[i][ii + 4];
                    a6 = B[i][ii + 5];
                    a7 = B[i][ii + 6];
                    tmp = B[i][ii + 7];

                    B[i][ii + 4] = a1;
                    B[i][ii + 5] = a2;
                    B[i][ii + 6] = a3;
                    B[i][ii + 7] = a4;

                    B[i + 4][ii] = a5;
                    B[i + 4][ii + 1] = a6;
                    B[i + 4][ii + 2] = a7;
                    B[i + 4][ii + 3] = tmp;
                }
                
                for (i = ii + 4 ; i < ii + 8; i++){
                    a1 = A[i][jj + 4];
                    a2 = A[i][jj + 5];
                    a3 = A[i][jj + 6];
                    a4 = A[i][jj + 7];

                    B[jj + 4][i] = a1;
                    B[jj + 5][i] = a2;
                    B[jj + 6][i] = a3;
                    B[jj + 7][i] = a4;
                }
            }
        }
    }
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/* misses = 343 on 32 * 32 */
char trans_simple_desc_1[] = "A simple transpose 1";
void trans_simple_1(int M, int N, int A[N][M], int B[M][N])
{
    int ii, i, jj, j, tmp;
    for (ii = 0; ii < M; ii+=8) {
        for (jj = 0; jj < N; jj+=8) {
            for (i = ii; i < ii + 8; i++) {
                for (j = jj; j < jj + 8; j++) {
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}

/* misses = 308 on 32 * 32 */
char trans_simple_desc_2[] = "A simple transpose 2";
void trans_simple_2(int M, int N, int A[N][M], int B[M][N])
{
    int ii, i, jj, j, tmp;
    int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
    for (ii = 0; ii < M; ii+=8) {
        for (jj = 0; jj < N; jj+=8) {
            if (ii == jj) {//处理大块对角线情况
                for (i = ii; i < ii + 8; i++) {
                    for (j = jj; j < jj + 8; j++) {
                        if (i != j) {
                            tmp = A[i][j];
                            B[j][i] = tmp; 
                        }
                        else switch (i - ii) {
                            case 0:{
                                a1 = A[i][i];
                                break;
                            }
                            case 1:{
                                a2 = A[i][i];
                                break;
                            }
                            case 2:{
                                a3 = A[i][i];
                                break;
                            }
                            case 3:{
                                a4 = A[i][i];
                                break;
                            }
                            case 4:{
                                a5 = A[i][i];
                                break;
                            }
                            case 5:{
                                a6 = A[i][i];
                                break;
                            }
                            case 6:{
                                a7 = A[i][i];
                                break;
                            }
                            case 7:{
                                tmp = A[i][i];
                                break;
                            }
                        }
                    }
                }
                B[ii][ii] = a1;
                B[ii + 1][ii + 1] = a2;
                B[ii + 2][ii + 2] = a3;
                B[ii + 3][ii + 3] = a4;
                B[ii + 4][ii + 4] = a5;
                B[ii + 5][ii + 5] = a6;
                B[ii + 6][ii + 6] = a7;
                B[ii + 7][ii + 7] = tmp;
            }
            else for (i = ii; i < ii + 8; i++) {//处理大块非对角线情况
                for (j = jj; j < jj + 8; j++) {
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}
/* misses 1992 on 67 * 61 */
char trans_simple_desc_3[] = "A simple transpose 3";
void trans_simple_3(int M, int N, int A[N][M], int B[M][N]) {
    int ii, jj, i, j, tmp;
    for (ii = 0; ii < N; ii += 16) {
        for (jj = 0; jj < M; jj += 16) {
            for (i = ii; i < ii + 16 && i < N; i++) {
                for (j = jj; j < jj + 16 && j < M; j++){
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    //registerTransFunction(trans_simple_1, trans_simple_desc_1);
    //registerTransFunction(trans_simple_2, trans_simple_desc_2);
    registerTransFunction(trans_simple_3, trans_simple_desc_3);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}