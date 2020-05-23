#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 25000,50000,75000,100000,125000,150000 };

void fill_random(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void fill_increasing(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = i;
    }
}

void fill_decreasing(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = n - i;
    }
}

void fill_vshape(int *t, unsigned int n) {
    for (unsigned int i = 0; i < (n+1)/2; i++) {
        t[i] = 2 * (n - i);
        t[n-i-1] = 2 * (n - i) - 1;
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(int *t, unsigned int n) {
    for(int i = 0; i < n-1; i++){
        int min;
        min = i;
        for(int j = i+1; j < n; j++)
            if(t[j] < t[min])
                min = j;
        swap(&t[min], &t[i]);
    }
}

void insertion_sort(int *t, unsigned int n) {
    int key, j;
    for(int i = 1; i < n; i++){
        key = t[i];
        j = i - 1;
        while(j >= 0 && t[j] > key){
            t[j + 1] = t[j];
            j = j - 1;
        }
        t[j + 1] = key;
    }
}

void quick_sort_1(int *t,  int n) {
    quick_sort_recursive_1(t, 0, n-1);
}

void quick_sort_2(int *t,  int n) {
    quick_sort_recursive_2(t, 0, n-1);
}

void quick_sort_recursive_1(int *t,  int l,  int r) {
    if (l < r){
        int pi = quick_sort_split_1(t, l, r);
        quick_sort_recursive_1(t, l, pi - 1);
        quick_sort_recursive_1(t, pi + 1, r);
    }
}

void quick_sort_recursive_2(int *t,  int l,  int r) {
    if (l < r){
        int pi = quick_sort_split_2(t, l, r);
        quick_sort_recursive_2(t, l, pi - 1);
        quick_sort_recursive_2(t, pi + 1, r);
    }
}

int quick_sort_split_1(int *t,  int l,  int r) {
    int pivot = quick_sort_choose_pivot_1(l, r);
    int value = t[pivot];
    swap(&t[pivot], &t[r]);
    int i = (l - 1);
    for (int j = l; j <= r- 1; j++){
        if (t[j] < value){
            i++;
            swap(&t[i], &t[j]);
        }
    }
    swap(&t[i + 1], &t[r]);
    return (i + 1);
}

int quick_sort_split_2(int *t,  int l,  int r) {
    int pivot = quick_sort_choose_pivot_2(l, r);
    int value = t[pivot];
    swap(&t[pivot], &t[r]);
    int i = (l - 1);
    for (int j = l; j <= r- 1; j++){
        if (t[j] < value){
            i++;
            swap(&t[i], &t[j]);
        }
    }
    swap(&t[i + 1], &t[r]);
    return (i + 1);
}

int quick_sort_choose_pivot_1( int l,  int r) {
    return r;
}

int quick_sort_choose_pivot_2( int l,  int r) {
    return (rand() % (r - l + 1)) + l;
}

void heapify(int arr[], int n, int i){
   int largest = i;
   int l = 2*i + 1;
   int r = 2*i + 2;
   if (l < n && arr[l] > arr[largest])
     largest = l;
   if (r < n && arr[r] > arr[largest])
     largest = r;
   if (largest != i){
     swap(&arr[i], &arr[largest]);
     heapify(arr, n, largest);
   }
}

void heap_sort(int *t, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(t, n, i);
    for (int i=n-1; i>=0; i--){
        swap(&t[0], &t[i]);
        heapify(t, i, 0);
    }
}

void is_random(int *t, unsigned int n) {
    return;
}

void is_increasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int *t, unsigned int n) {
    int *begin = t;
    int *end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > *end);
        begin++;
        assert(*end > *begin);
        end--;
    }
}

void is_sorted(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int *, unsigned int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape };
void (*check_functions[])(int *, unsigned int) = { is_random, is_increasing, is_decreasing, is_vshape };
void (*sort_functions[])(int *, unsigned int) = { selection_sort, insertion_sort, quick_sort_1, quick_sort_2, heap_sort };

char *fill_names[] = { "Random", "Increasing", "Decreasing", "V-Shape" };
char *sort_names[] = { /*"SelectionSort", "InsertionSort", "QuickSortLast",*/ "QuickSortRandom", "HeapSort" };

int main() {
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int *, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, unsigned int) = fill_functions[j];
            void (*check)(int *, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int *t = malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                printf("%s\t%s\t%u\t%f\n", sort_names[i], fill_names[j], n, (double)(end - begin) / (double) CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    return 0;
}
