#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 3

int A[SIZE][SIZE];
int AT[SIZE][SIZE];
int row_sums[SIZE];

// Function to initialize matrix from file
void initialize_matrix() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open data.txt\n");
        exit(1);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &A[i][j]);
        }
    }
    fclose(file);
}

// Function to save transposed matrix to file
void save_matrix() {
    FILE *file = fopen("result.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open result.txt\n");
        exit(1);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", AT[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Function to print a matrix
void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Serial transpose function
void transpose_serial() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            AT[j][i] = A[i][j];
        }
    }
}

// Thread arguments for transpose
typedef struct {
    int i;
    int j;
    int thread_id;
} ThreadArgs;

// Worker function for multithreaded transpose
void *transpose_worker(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int i = args->i;
    int j = args->j;
    int thread_id = args->thread_id;

    AT[j][i] = A[i][j];
    printf("Thread %d calculated element A[%d][%d] -> AT[%d][%d]\n", thread_id, i, j, j, i);

    free(args);
    pthread_exit(NULL);
}

// Serial row sum function
void row_sum_serial() {
    for (int i = 0; i < SIZE; i++) {
        row_sums[i] = 0;
        for (int j = 0; j < SIZE; j++) {
            row_sums[i] += AT[i][j];
        }
    }
}

// Worker function for multithreaded row sum
void *row_sum_worker(void *arg) {
    int row = *(int *)arg;
    row_sums[row] = 0;
    for (int j = 0; j < SIZE; j++) {
        row_sums[row] += AT[row][j];
    }
    free(arg);
    pthread_exit(NULL);
}

// Timing function
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    struct timespec start, end;
    double serial_time, parallel_time;

    // Initialize matrix
    initialize_matrix();
    printf("Original Matrix A:\n");
    print_matrix(A);

    // Serial Transpose
    clock_gettime(CLOCK_MONOTONIC, &start);
    transpose_serial();
    clock_gettime(CLOCK_MONOTONIC, &end);
    serial_time = get_time_diff(start, end);
    printf("Serial Transposed Matrix AT:\n");
    print_matrix(AT);
    printf("Serial transpose time: %.9f seconds\n", serial_time);

    // Multithreaded Transpose
    pthread_t transpose_threads[SIZE * SIZE];
    clock_gettime(CLOCK_MONOTONIC, &start);
    int thread_idx = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
            args->i = i;
            args->j = j;
            args->thread_id = thread_idx;
            pthread_create(&transpose_threads[thread_idx], NULL, transpose_worker, args);
            thread_idx++;
        }
    }
    for (int i = 0; i < SIZE * SIZE; i++) {
        pthread_join(transpose_threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    parallel_time = get_time_diff(start, end);
    printf("Multithreaded Transposed Matrix AT:\n");
    print_matrix(AT);
    printf("Parallel transpose time: %.9f seconds\n", parallel_time);

    // Serial Row Sum
    clock_gettime(CLOCK_MONOTONIC, &start);
    row_sum_serial();
    clock_gettime(CLOCK_MONOTONIC, &end);
    serial_time = get_time_diff(start, end);
    printf("Row Sums (Serial): ");
    for (int i = 0; i < SIZE; i++) printf("%d ", row_sums[i]);
    printf("\nSerial row sum time: %.9f seconds\n", serial_time);

    // Multithreaded Row Sum
    pthread_t row_sum_threads[SIZE];
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < SIZE; i++) {
        int *row_arg = (int *)malloc(sizeof(int));
        *row_arg = i;
        pthread_create(&row_sum_threads[i], NULL, row_sum_worker, row_arg);
    }
    for (int i = 0; i < SIZE; i++) {
        pthread_join(row_sum_threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    parallel_time = get_time_diff(start, end);
    printf("Row Sums (Multithreaded): ");
    for (int i = 0; i < SIZE; i++) printf("%d ", row_sums[i]);
    printf("\nParallel row sum time: %.9f seconds\n", parallel_time);

    // Save result
    save_matrix();

    return 0;
}