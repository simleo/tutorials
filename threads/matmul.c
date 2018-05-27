/*
 * Matrix multiplication of two arrays. Each thread computes one row
 * of the result. In a real world application you probably want to do
 * something different, like setting the number of threads = number of
 * CPU cores and assign a subset of rows to each thread. You also want
 * error checking, omitted here for simplicity.
 *
 * Locks are not needed because each thread writes to a separate row.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


void init_arr(double ***ap, const size_t m, const size_t n, int rng);
void dump_arr(const double **a, const size_t m, const size_t n);
void *dot(void*);


typedef struct {
    int i, m, n;
    const double **m1;
    const double **m2;
    double **res;
} Args;


int main(void) {
    size_t i = 0, L = 3, M = 4, N = 5;
    double **m1 = NULL, **m2 = NULL, **res = NULL;
    pthread_t *threads = NULL;
    Args *args = calloc(1, sizeof(*args));

    srand(time(NULL));
    init_arr(&m1, L, M, 1);
    init_arr(&m2, M, N, 1);
    init_arr(&res, L, N, 0);
    threads = calloc(L, sizeof(pthread_t));

    args->m = M;
    args->n = N;
    args->m1 = (const double**)m1;
    args->m2 = (const double**)m2;
    args->res = res;
    for (i = 0; i < L; ++i) {
	args->i = i;
	pthread_create(&threads[i], NULL, dot, args);
	pthread_join(threads[i], NULL);
    }

    printf("m1\n");
    dump_arr((const double**)m1, L, M);
    printf("m2\n");
    dump_arr((const double**)m2, M, N);
    printf("res\n");
    dump_arr((const double**)res, L, N);

    return 0;
}


void init_arr(double ***ap, const size_t m, const size_t n, int rng) {
    size_t i, j;
    *ap = (double**)calloc(m, sizeof(double*));
    for (i = 0; i < m; ++i) {
	(*ap)[i] = (double*)calloc(n, sizeof(double));
	if (rng) {
	    for (j = 0; j < n; ++j) {
		(*ap)[i][j] = (double)rand() / (double)RAND_MAX;
	    }
	}
    }
}


void dump_arr(const double **a, const size_t m, const size_t n) {
    size_t i, j;
    for (i = 0; i < m; ++i) {
	for(j = 0; j < n; ++j) {
	    printf("%.3f ", a[i][j]);
	}
	printf("\n");
    }
}


void *dot(void *args) {
    int j, k;
    Args *a = args;
    for (j = 0; j < a->n; ++j) {
	for (k = 0; k < a->m; ++k) {
	    a->res[a->i][j] += a->m1[a->i][k] * a->m2[k][j];
	}
    }
    return NULL;
}
