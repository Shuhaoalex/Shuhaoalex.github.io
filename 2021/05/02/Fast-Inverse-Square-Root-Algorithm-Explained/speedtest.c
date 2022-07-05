#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>

void logspace(float start, float end, int n, float* u) {
    float pow_start = log2f(start);
    float pow_end = log2f(end);
    float step = (pow_end - pow_start) / (n - 1);
    for (int i = 0; i < n; ++i) {
        u[i] = powf(2, pow_start + i * step);
    }
}

static inline float Q_rsqrt(float y) {
    long i;
    float x2;
    
    x2 = y * 0.5F;
    i = 0x5f3759df - ((*( long * ) &y) >> 1);               // what the fuck?
    y = * (float * ) &i;
    y *= ( 1.5F - (x2 * y * y) );   // 1st iteration
//  y *= ( 1.5F - (x2 * y * y) );   // 2nd iteration, can be removed
    return y;
}

int main() {
    int profiling = 1;
    int trail = 100;
    int sz = 1 << 20;
    float * xs = (float*)malloc(sizeof(float) * sz);
    logspace(FLT_MIN, FLT_MAX, sz, xs);
    float * result = (float*)malloc(sizeof(float) * sz);
    float * ground = (float*)malloc(sizeof(float) * sz);
    for (int i = 0; i < sz; ++i) {
        result[i] = Q_rsqrt(xs[i]);
    }

    for (int i = 0; i < sz; ++i) {
        ground[i] = 1 / sqrt(xs[i]);
    }

    if (profiling) {
        double quick_alg_time = 0;
        double orig_alg_time = 0;
        clock_t start, end;
        for (int t = 0; t < trail; ++t) {
            
            for (int i = 0; i < sz; ++i) {
                result[i] = 0;
            }

            for (int i = 0; i < sz; ++i) {
                ground[i] = 0;
            }

            start = clock();
            for (int i = 0; i < sz; ++i) {
                result[i] = Q_rsqrt(xs[i]);
            }
            end = clock();
            quick_alg_time += ((double) (end - start)) / CLOCKS_PER_SEC;

            start = clock();
            for (int i = 0; i < sz; ++i) {
                ground[i] = 1 / sqrt(xs[i]);
            }
            end = clock();
            orig_alg_time += ((double) (end - start)) / CLOCKS_PER_SEC;
        }
        quick_alg_time /= trail;
        printf("Average time used for quick algorithm: %f\n", quick_alg_time);
        orig_alg_time /= trail;
        printf("Average time used for naive approach: %f\n", orig_alg_time);
    }

    FILE* output_f = fopen("output.bin", "wb");
    if (fwrite(xs, sizeof(float), sz, output_f) != sz) {
        perror("Cannot write to file!");
        return 1;
    }
    if (fwrite(result, sizeof(float), sz, output_f) != sz) {
        perror("Cannot write to file!");
        return 1;
    }
    if (fwrite(ground, sizeof(float), sz, output_f) != sz) {
        perror("Cannot write to file!");
        return 1;
    }
}
