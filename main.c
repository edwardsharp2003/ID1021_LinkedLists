#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>




// Define the `cell` and `linked` data structures
typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

// Allocate and initialize an empty linked list
linked *linked_create() {
    linked *new = (linked*)malloc(sizeof(linked));
    new->first = NULL;
    return new;
}

// Free the memory of the linked list and all its cells
void linked_free(linked *lnk) {
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}

// Add a new integer to the beginning of the linked list
void linked_add(linked *lnk, int item) {
    cell *new_cell = (cell*)malloc(sizeof(cell));
    new_cell->value = item;
    new_cell->tail = lnk->first;  // Point the new cell to the previous first cell
    lnk->first = new_cell;       // Update the first cell of the list
}

// Calculate the length of the linked list
int linked_length(linked *lnk) {
    int length = 0;
    cell *current = lnk->first;
    while (current != NULL) {
        length++;
        current = current->tail;
    }
    return length;
}

// Find if an integer exists in the linked list
bool linked_find(linked *lnk, int item) {
    cell *current = lnk->first;
    while (current != NULL) {
        if (current->value == item) {
            return true; // Return true if the item is found
        }
        current = current->tail;
    }
    return false; // Return false if the item is not found
}

// Remove an item from the linked list, if it exists
void linked_remove(linked *lnk, int item) {
    cell *current = lnk->first;
    cell *previous = NULL;

    while (current != NULL) {
        if (current->value == item) { // If the item is found
            if (previous == NULL) {   // If the item is the first cell
                lnk->first = current->tail;
            } else {                  // If the item is in the middle or end
                previous->tail = current->tail;
            }
            free(current);
            return; // Exit after removing the item
        }
        previous = current;
        current = current->tail;
    }
}

void linked_append(linked *a, linked *b) {
    if (a->first == NULL) { // If the first list is empty
        a->first = b->first; // Point the first list to the second list
    } else {
        // Traverse to the last cell of the first list
        cell *current = a->first;
        while (current->tail != NULL) {
            current = current->tail;
        }
        // Append the second list
        current->tail = b->first;
    }
    b->first = NULL; // Set the second list to be empty
}

#include <time.h> // For measuring time

linked *linked_init(int n) {
    linked *a = linked_create();
    for (int i = 0; i < n; i++) {
        linked_add(a, i); // Add elements to the list
    }
    return a;
}

// Benchmark appending a fixed size list `b` to varying size list `a`
void benchmark_append_varying_a(int fixed_size_b, int max_size_a) {
    // Initialize the fixed-size list `b`
    linked *b = linked_init(fixed_size_b);

    printf("Benchmark: Appending a fixed-size list b (%d elements) to varying-size lists a\n", fixed_size_b);
    for (int size_a = 1; size_a <= max_size_a; size_a *= 2) {
        // Initialize the list `a` with `size_a` elements
        linked *a = linked_init(size_a);

        clock_t start = clock();  // Start the timer
        linked_append(a, b);
        clock_t end = clock();    // End the timer

        printf("Size(a): %d, Time taken: %.6f seconds\n", size_a, (double)(end - start) / CLOCKS_PER_SEC);

        // Free the memory
        linked_free(a);
        b->first = NULL; // Reset b's first pointer to reuse it
    }

    linked_free(b);
}

int main() {
    // Create a linked list
    linked *list = linked_create();

    // Add elements to the list
    linked_add(list, 10);
    linked_add(list, 20);
    linked_add(list, 30);

    // Print the length of the list
    printf("Length of list: %d\n", linked_length(list));

    // Find elements in the list
    printf("Find 20 in list: %s\n", linked_find(list, 20) ? "Found" : "Not Found");
    printf("Find 40 in list: %s\n", linked_find(list, 40) ? "Found" : "Not Found");

    // Remove an element from the list
    printf("Removing 20 from list...\n");
    linked_remove(list, 20);
    printf("Length of list after removal: %d\n", linked_length(list));

    // Print the remaining elements in the list
    printf("Remaining list elements: ");
    cell *current = list->first;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->tail;
    }
    printf("\n");

    // Create and append another list
    linked *list2 = linked_create();
    linked_add(list2, 50);
    printf("Appending second list to the first list...\n");
    linked_append(list, list2);

    // Print all elements after appending
    printf("List after appending: ");
    current = list->first;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->tail;
    }
    printf("\n");

    // Free the memory
    linked_free(list);
    linked_free(list2);

    return 0;
}
