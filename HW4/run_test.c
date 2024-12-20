// Servando Olvera

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "heap.h"

int main(void) {
    int N, P;
    char fname[50];
	struct heap_struct heapS;
	FILE *fp;

	printf("This program will create a max-heap and perform operations on it based on data from a file.\n");
	printf("\nEnter the filename: ");
	scanf("%s", fname);
	fp = fopen(fname, "r"); // Sanity check for file
		if (fp == NULL) {
			printf("\n  File could not be opened.\n");
			return 1;
		}
	
	fscanf(fp, "%d", &N);
		if(N <= 0) { // You never knwo...
			printf("\n  Array size appears to be zero or less than zero.\n");
			printf("  Math cannot math....\n"); 
			fclose(fp);
			return 1;
		}
	int *arr = (int*) calloc(N, sizeof(int)); 
	//int arr[N]; // I had a different logic before 
	for (int i = 0; i < N; i++) {
		fscanf(fp, "%d ", &arr[i]);
	}

	heapS = make_heap(N, arr); 
	print_heap(heapS);

    fscanf(fp, "%d", &P);
	for(int k=1; k<=P; k++) { //After scanning the number of operations, scan file x times. Each time store the read as string, and determine what to do next
		char buf[10];
		fscanf(fp, "%s", buf);
		printf("Operation number %d, string: %s\n", k, buf);

		if((strcmp(buf, "P") == 0) || (strcmp(buf, "p") == 0)) { // For peek
			printf("peek:    %6d\n", peek(heapS));
			print_heap(heapS);
		} else if(strcmp(buf, "*") == 0) { // for remove
			int val = poll(&heapS); 
			if(val == 0) { // Just in case heap is areldy empty and a remove was scaned
				printf("Empty heap. no remove performed.removed:      %d\n", val);
			} else {
				printf("removed: %6d\n", val);
			}	
			print_heap(heapS);
		} else { // if scan wasn't either P, p, or * then we have an int but you gotta change it from string to int
			int num = atoi(buf);
			printf("add:     %6d\n", num);
			add(&heapS, num);
			print_heap(heapS);
		}
	}

	destroy(&heapS); // Kill it with fire!

	fclose(fp);
    return 0;
}