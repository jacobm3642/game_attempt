#include "algorithms.h"

#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_pass(int *array, int size, int i) {
    int left = 2 * i + 1;  
    int right = 2 * i + 2;  
    int largest = i;
    
    if (left < size && array[left] > array[largest]) {
        largest = left;
    }
    
    if (right < size && array[right] > array[largest]) {
        largest = right;
    }
    
    if (largest != i) {
        swap(&array[i], &array[largest]);
        heapify_pass(array, size, largest);
    }
}

void max_heapify(int *array, int size) {
    for (int i = 0; i < size+1; i++) {
        heapify_pass(array, size, size-i);
    }
}