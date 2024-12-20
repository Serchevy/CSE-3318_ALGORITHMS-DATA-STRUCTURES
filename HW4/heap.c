// Servando Olvera

#include <stdlib.h>
#include <stdio.h>

#include "heap.h"  

#define DEBUG 1

int indx_of_Max_Value(int* A, int p, int left, int right, int N);

// So what I got from the instructions is that we don't really touch this function. Only heap_calls uses it
struct heap_struct make_heap_empty(int cap){
    struct heap_struct temp;
    temp.items = (int*) calloc(cap, sizeof(int)); 
	temp.N = 0;
    temp.capacity = cap;
    return temp;
}

// Set *arr equal to heap.items, set N and capacity to N and go from there to make the MaxHeap
struct heap_struct make_heap(int N, int * arr) {
    printf("in function make_heap, in DEBUG MODE, printing array BEFORE it gets turned into a heap :\n");
    struct heap_struct heap;
    heap.items = arr;
    heap.N = N;
    heap.capacity = N;
    // heap = make_heap_empty(N);     // initially I thought you neede to make make_heap and make_heap_empty intract with one another
    // for (int i = 0; i < N; i++) {  // but that isn't the case... or so I think. Either way imma leave it here. If tested 
	// 	heap.items[i] = arr[i];       // the previous three lines would have to be commented out and so would the dynamic array in main
    //     heap.N++;                
	// }
    print_heap(heap);

    for(int p = (N/2)-1; p>=0; p--) { //Build MaxHeap
        printf("in function make_heap, in DEBUG MODE, printing array after sink_down at index %d.\n", p);
        sink_down(p, heap.N, heap.items);
        print_heap(heap);
    }

    return heap;
}

void destroy(struct heap_struct * heapP) { 
    if(heapP->items == NULL) {
        return;
    }
    free(heapP->items);
    heapP->N = 0;
    heapP->capacity = 0;
    heapP->items = NULL; // Just in case some funny business happen and fucntions gets called twice
}


void print_heap(struct heap_struct heapS) { // Fancy formating according to Professor
    printf("Heap:  size: %d, capacity: %d\n", heapS.N, heapS.capacity);
    printf("indexes:    ");
    for(int i=0; i<heapS.N; i++) {
        printf("%7d,", i);
    }
    printf("\nvalues:     ");
    for(int j=0; j<heapS.N; j++) {
        printf("%7d,", heapS.items[j]);
    }
    printf("\n\n");
}   


void swim_up(int idx, int * arr) { // Not sure if I implemented it correclty but here it is :)
    int temp = 0;

    // took me a while to make this work with indexe starting at 0
    while((idx >= 0) && (arr[idx] > arr[(idx-1)/2])) {
        temp = arr[idx];
        arr[idx] = arr[(idx-1)/2]; // Swapping logic
        arr[(idx-1)/2] = temp; 
        idx = (idx-1)/2;   
    }
}


void sink_down(int i, int N, int * arr) { // Same confident implementation as above
    int left, right, imv, temp;
    left = (2*i)+1;
    right = (2*i)+2;
    imv = indx_of_Max_Value(arr, i, left, right, N); 

    while((imv != i) && (imv<N)) {
        temp = arr[imv];
        arr[imv] = arr[i]; // Swapping logic
        arr[i] = temp;

        i = imv;
        left = (2*i)+1;
        right = (2*i)+2;
        imv = indx_of_Max_Value(arr, i, left, right, N); 
    }
}

void add(struct heap_struct * heapP, int new_item){ // Kinda messy but bare with me
    if(heapP->N == heapP->capacity) { // Sanity check to determine if resizing is neccessary
        printf("\nresizing...\n");
        heapP->capacity = (heapP->capacity) * 2; // Doubleling capacity
        heapP->items = realloc((heapP->items),sizeof(int)*(heapP->capacity));
    }

    heapP->items[heapP->N] = new_item; // indx N can now be used due to previous or current resizing. 
                                       // You knwo since N is the size, and an array has indexexs N-1 
                                       // but we're adding an item so we use index N.
    heapP->N = (heapP->N)+1; // Update curret size
    swim_up(heapP->N - 1, heapP->items);
}

int peek(struct heap_struct heapS){ // Well it just takes the higher value, which is at indx 0, and returns it!
    return heapS.items[0];
}


int poll(struct heap_struct * heapP){ // Kinda messy too
    if(heapP->N > 0) { // Sanity check in case current size is zero and a remove was requested
        int mx = heapP->items[0];
        heapP->items[0] = heapP->items[heapP->N - 1];
        heapP->N = heapP->N - 1;
        sink_down(0, heapP->N, heapP->items);
        return mx;
    }

    return 0;
}

// Straight out of the slides, with some tweaking to avoud errors!
int indx_of_Max_Value(int* A, int p, int left, int right, int N) {
    int imv = p;

    if ((left < N) && (A[left] > A[imv])) {
        imv = left; 
    }
    if ((right < N) && (A[right] > A[imv])) {
        imv = right;
    }
    return imv;
}