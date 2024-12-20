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
    int liml = (k >> 3) << 3;
    int r = k - liml;
    if(r == 0){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
            }
        }
    }
    else if(r == 1){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
            }
        }
    }
    else if(r == 2){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
            }
        }
    }
    else if(r == 3){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
                Z[i][j] += X[i][k - 3] * YP[j][k - 3];
            }
        }
    }
    else if(r == 4){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
                Z[i][j] += X[i][k - 3] * YP[j][k - 3];
                Z[i][j] += X[i][k - 4] * YP[j][k - 4];
            }
        }
    }
    else if(r == 5){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
                Z[i][j] += X[i][k - 3] * YP[j][k - 3];
                Z[i][j] += X[i][k - 4] * YP[j][k - 4];
                Z[i][j] += X[i][k - 5] * YP[j][k - 5];
            }
        }
    }
    else if(r == 6){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
                Z[i][j] += X[i][k - 3] * YP[j][k - 3];
                Z[i][j] += X[i][k - 4] * YP[j][k - 4];
                Z[i][j] += X[i][k - 5] * YP[j][k - 5];
                Z[i][j] += X[i][k - 6] * YP[j][k - 6];
            }
        }
    }
    else if(r == 7){
        for (int i = 0; i != m; ++i) {
            for (int j = 0; j != m; ++j) {
                for (int l = 0; l != liml; l += 8) {
                    Z[i][j] += X[i][l] * YP[j][l];
                    Z[i][j] += X[i][l+1] * YP[j][l+1];
                    Z[i][j] += X[i][l+2] * YP[j][l+2];
                    Z[i][j] += X[i][l+3] * YP[j][l+3];
                    Z[i][j] += X[i][l+4] * YP[j][l+4];
                    Z[i][j] += X[i][l+5] * YP[j][l+5];
                    Z[i][j] += X[i][l+6] * YP[j][l+6];
                    Z[i][j] += X[i][l+7] * YP[j][l+7];
                }
                Z[i][j] += X[i][k - 1] * YP[j][k - 1];
                Z[i][j] += X[i][k - 2] * YP[j][k - 2];
                Z[i][j] += X[i][k - 3] * YP[j][k - 3];
                Z[i][j] += X[i][k - 4] * YP[j][k - 4];
                Z[i][j] += X[i][k - 5] * YP[j][k - 5];
                Z[i][j] += X[i][k - 6] * YP[j][k - 6];
                Z[i][j] += X[i][k - 7] * YP[j][k - 7];
            }
        }
    }
}

void compute_simd() {
#ifdef SIMD
    zero_z();
    int liml = (k >> 3) << 3;
    int temp[4]={};
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l != liml; l += 8) {
                int32x4_t X_4 = vld1q_s32((const int32_t *)X + i * k + l);
                int32x4_t YP_4 = vld1q_s32((const int32_t *)YP + j * k + l);
                int32x4_t Z_4 = vmulq_s32(X_4, YP_4);
                int32x4_t X_8 = vld1q_s32((const int32_t *)X + i * k + l + 4);
                int32x4_t YP_8 = vld1q_s32((const int32_t *)YP + j * k + l + 4);
                int32x4_t Z_8 = vmulq_s32(X_8, YP_8);
                Z_8 = vaddq_s32(Z_4, Z_8);
                vst1q_s32(temp, Z_8);
                Z[i][j] += temp[0];
                Z[i][j] += temp[1];
                Z[i][j] += temp[2];
                Z[i][j] += temp[3];
            }
            for (int ll = liml; ll != k; ++ll) {
                Z[i][j] += X[i][ll] * YP[j][ll];
            }
        }
    }
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

