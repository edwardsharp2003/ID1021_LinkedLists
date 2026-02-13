#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the linked list structures
typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

// Function to create a new linked list
linked *linked_create() {
    linked *new = (linked*)malloc(sizeof(linked));
    new->first = NULL;
    return new;
}

// Function to free the linked list
void linked_free(linked *lnk) {
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}

// Function to add a value to the start of the list
void linked_add(linked *lnk, int item) {
    cell *new = (cell*)malloc(sizeof(cell));
    new->value = item;
    new->tail = lnk->first;
    lnk->first = new;
}

// Function to initialize a linked list with n elements
linked *linked_init(int n) {
    linked *lnk = linked_create();
    for (int i = 0; i < n; i++) {
        linked_add(lnk, i);
    }
    return lnk;
}

// Function to append list b to the end of list a
void linked_append(linked *a, linked *b) {
    cell *curr = a->first;
    cell *prev = NULL;

    while (curr != NULL) {
        prev = curr;
        curr = curr->tail;
    }
    if (prev != NULL) {
        prev->tail = b->first;
    } else {
        a->first = b->first;
    }
    b->first = NULL; // Clear list b
}

// Utility function to get nanoseconds from clock
long long nano_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

// Benchmark function for varying the size of list a
void benchmark_append_vary_a(int max_size, int increment, int b_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "# SizeA Time(ns)\n");

    for (int size_a = increment; size_a <= max_size; size_a += increment) {
        linked *a = linked_init(size_a);
        linked *b = linked_init(b_size);

        long long start = nano_seconds();
        linked_append(a, b);
        long long end = nano_seconds();

        fprintf(file, "%d %lld\n", size_a, end - start);

        linked_free(a);
        linked_free(b);
    }

    fclose(file);
}

// Benchmark function for varying the size of list b
void benchmark_append_vary_b(int max_size, int increment, int a_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "# SizeB Time(ns)\n");

    for (int size_b = increment; size_b <= max_size; size_b += increment) {
        linked *a = linked_init(a_size);
        linked *b = linked_init(size_b);

        long long start = nano_seconds();
        linked_append(a, b);
        long long end = nano_seconds();

        fprintf(file, "%d %lld\n", size_b, end - start);

        linked_free(a);
        linked_free(b);
    }

    fclose(file);
}

int main() {
    // Configure benchmark parameters
    int max_size_a = 10000; // Maximum size of list a
    int max_size_b = 10000; // Maximum size of list b
    int increment = 500;    // Size increment for both lists
    int fixed_size_a = 1000; // Fixed size of list a for varying b
    int fixed_size_b = 1000; // Fixed size of list b for varying a

    // Generate data files
    benchmark_append_vary_a(max_size_a, increment, fixed_size_b, "append_a_results.dat");
    benchmark_append_vary_b(max_size_b, increment, fixed_size_a, "append_b_results.dat");

    printf("Benchmark data generated successfully!\n");
    return 0;
}