#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#ifdef SIMD
#include <arm_neon.h>
#endif

#include "common.h"
#include "compute.h"

void zero_z() {
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            Z[i][j] = 0;
        }
    }
}

void compute_row_major_mnk() {
    zero_z();
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l != k; ++l) {
                Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_mkn() {
    // TODO: task 1
    zero_z();
    for (int i = 0; i != m; ++i) {
        for (int l = 0; l != k; ++l) {
            for (int j = 0; j != n; ++j) {
                Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_kmn() {
    // TODO: task 1
    zero_z();
    for (int l = 0; l != k; ++l) {
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != n; ++j) {
                    Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_nmk() {
    // TODO: task 1
    zero_z();
    for (int j = 0; j != n; ++j) {
        for (int i = 0; i != m; ++i) {
            for (int l = 0; l != k; ++l) {
                Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_nkm() {
    // TODO: task 1
    zero_z();
    for (int j = 0; j != n; ++j) {
        for (int l = 0; l != k; ++l) {
            for (int i = 0; i != m; ++i) {
            Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_knm() {
    // TODO: task 1
    zero_z();
    for (int l = 0; l != k; ++l) {
        for (int j = 0; j != n; ++j) {
            for (int i = 0; i != m; ++i) {
                Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_y_transpose_mnk() {
    // TODO: task 2
    zero_z();
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l != k; ++l) {
                Z[i][j] += X[i][l] * YP[j][l];
            }
        }
    }
}

void compute_row_major_mnkkmn_b32() {
    // TODO: task 2
    zero_z();
    for (int i = 0; i != (m >> 5) + 1; ++i) {
        for (int j = 0; j != (n >> 5) + 1; ++j) {
            for (int l = 0; l != (k >> 5) + 1; ++l) {
                for (int ll = 0; ll != 32 && (l << 5) + ll != k; ++ll) {
                    for (int ii = 0; ii != 32 && (i << 5) + ii != m; ++ii) {
                        for (int jj = 0; jj != 32 && (j << 5) + jj != n; ++jj){
                            Z[(i << 5) + ii][(j << 5) + jj] += X[(i << 5) + ii][(l << 5) + ll] * Y[(l << 5) + ll][(j << 5) + jj];
                        }
                    }
                }
            }
        }
    }
}

void compute_row_major_mnk_lu2() {
    // TODO: task 2
    zero_z();
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l < k-1; l+=2) {
                Z[i][j] += X[i][l] * Y[l][j];
                Z[i][j] += X[i][l+1] * Y[l+1][j];
            }
            if(k&1)
                Z[i][j] += X[i][k-1] * Y[k-1][j];
        }
    }
}

void compute_my_kernel(){
    // TODO: task 2
    zero_z();
    int liml = (k>>2)<<2;
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l != liml; l+=4) {
                Z[i][j] += X[i][l] * YP[j][l];
                Z[i][j] += X[i][l+1] * YP[j][l+1];
                Z[i][j] += X[i][l+2] * YP[j][l+2];
                Z[i][j] += X[i][l+3] * YP[j][l+3];
            }
            if(k&1){
                Z[i][j] += X[i][k-1] * YP[j][k-1];
            }
            if(k&2){
                Z[i][j] += X[i][k-2] * YP[j][k-2];
                Z[i][j] += X[i][k-3] * YP[j][k-3];
            }
        }
    }
}

void compute_simd() {
#ifdef SIMD
    // TODO: task 3
#endif
}

uint64_t elapsed(const struct timespec start, const struct timespec end) {
    struct timespec result;
    result.tv_sec = end.tv_sec - start.tv_sec;
    result.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (result.tv_nsec < 0) {
        --result.tv_sec;
        result.tv_nsec += SEC;
    }
    uint64_t res = result.tv_sec * SEC + result.tv_nsec;
    return res;

}

uint64_t compute() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    switch (COMPUTE_SELECT) {
        case COMPUTE_ROW_MAJOR_MNK:
            //printf("COMPUTE_ROW_MAJOR_MNK\n");
            compute_row_major_mnk();
            break;
        case COMPUTE_ROW_MAJOR_MKN:
            //printf("COMPUTE_ROW_MAJOR_MKN\n");
            compute_row_major_mkn();
            break;
        case COMPUTE_ROW_MAJOR_KMN:
            //printf("COMPUTE_ROW_MAJOR_KMN\n");
            compute_row_major_kmn();
            break;
        case COMPUTE_ROW_MAJOR_NMK:
            //printf("COMPUTE_ROW_MAJOR_NMK\n");
            compute_row_major_nmk();
            break;
        case COMPUTE_ROW_MAJOR_NKM:
            //printf("COMPUTE_ROW_MAJOR_NKM\n");
            compute_row_major_nkm();
            break;
        case COMPUTE_ROW_MAJOR_KNM:
            //printf("COMPUTE_ROW_MAJOR_KNM\n");
            compute_row_major_knm();
            break;
        case COMPUTE_Y_TRANSPOSE_MNK:
            //printf("COMPUTE_Y_TRANSPOSE_MNK\n");
            compute_y_transpose_mnk();
            break;
        case COMPUTE_ROW_MAJOR_MNKKMN_B32:
            //printf("COMPUTE_ROW_MAJOR_MNKKMN_B32\n");
            compute_row_major_mnkkmn_b32();
            break;
        case COMPUTE_ROW_MAJOR_MNK_LU2:
            //printf("COMPUTE_ROW_MAJOR_MNK_LU2\n");
            compute_row_major_mnk_lu2();
            break;
        case COMPUTE_MY_KERNEL:
            //printf("COMPUTE_ROW_MAJOR_MNK_LU2\n");
            compute_my_kernel();
            break;
        case COMPUTE_SIMD:
            //printf("COMPUTE_SIMD\n");
            compute_simd();
            break;
        default:
            printf("Unreachable!");
            return 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return elapsed(start, end);
}

