/* 
 * Updated 2/17/2022 - Alexandra Stefan
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "list.h"

/* 
compile with -g to collect debugging info needed for Valgrind ( -lm links the math library): 
gcc -g bucket_sort.c list.c -lm

run with Valgrind:
valgrind --leak-check=full ./a.out

On some systems, but not all, you can also use the --show-leak-kinds=all flag:
valgrind --leak-check=full --show-leak-kinds=all ./a.out

run without Valgrind:
./a.out
*/

void min_max(int arr[], int N, long* min, long* max);

// recommended helper functions:
// function to insert a new node in a sorted list.
void insert_sorted(nodePT *L, nodePT newP);
//  function to sort an array sing bucket sort
void bucket_sort(int arr[], int N);
void print_array(int arr[], int N);
int run1();

int main()
{	
	printf("This program will read a file name, load data for an array from there and print the sorted array.\n");
	printf("The array is sorted using bucket sort.\n");
	printf("This will be repeated as long as the user wants.\n");
	int option;
	do {
		run1();
		printf("\nDo you want to repeat? Enter 1 to repeat, or 0 to stop) ");
		char ch;
		scanf("%d%c",&option, &ch);  // ch is used to remove the Enter from the input buffer
 	} while (option == 1);

   return 0;
}

void print_array(int arr[], int N){
	int j;
	printf("\n array: ");
	for(j= 0; j<N; j++){
		printf("%5d,", arr[j]);
	}
	printf("\n");
}
//-------------------------------------------------------------

int run1() {
	char fname[50];
	int N;
	FILE *fp;

	printf("\n> Enter the filename: ");
	scanf("%s", fname);
	fp = fopen(fname, "r");
		if (fp == NULL) {
			printf("\n  File could not be opened.\n");
			return 1;
		}

	fscanf(fp, "%d", &N);
		if(N <= 0) {
			printf("\n  Array size appears to be zero or less than zero.\n");
			printf("  Math cannot math....\n"); 
			printf("  Try a different file.\n");
			fclose(fp);
			return 1;
		}

	int nums[N];
	for (int i = 0; i < N; i++) {
		fscanf(fp, "%d ", &nums[i]);
	}

	print_array(nums, N);
	bucket_sort(nums, N);

	fclose(fp);

	return 0;
}

void bucket_sort(int arr[], int N) {
	long min, max;
 
	nodePT buckets[N]; // Initialize buckets
	for(int i = 0; i<N; i++) {
		buckets[i] = NULL;
	}

	min_max(arr, N, &min, &max); // Get min and max

	for(int i = 0; i < N; i++ ) {
		int indx = floor(((arr[i]-min)*N)/(1+max-min)); // Calculate index 
		printf("arr[%d]=%10d, indx = %d\n", i, arr[i], indx);

		if(buckets[indx] == NULL) { // If buckets are empty create list, else add on to existing list while sorting it
			buckets[indx] = new_node(arr[i]);
		} else {
			nodePT temp = new_node(arr[i]);
			insert_sorted(&(buckets[indx]), temp);
		}
	}

	int n = 0;
	for(int i = 0; i<N; i++) { // Print buckets and Copy data from bucktes into original array
		printf("------ List at index %d :", i);
		print_list_horiz(buckets[i]);
		
		nodePT temp = buckets[i];
		while(temp != NULL) {
			arr[n] = temp->data;
			temp = temp->next;
			n++;
		}
	}
	print_array(arr, N);

	for(int i = 0; i<N; i++) { // Destroy buckets
		destroy_list(buckets[i]);
	}
}

// Find min and maxm, by updating varibales
void min_max(int arr[], int N, long* min, long* max) {
	*min = *max = arr[0];

    for(int i = 1; i < N; i++) {
		if(*min > arr[i])
		  *min = arr[i];   
		   if(*max < arr[i])
		    *max = arr[i];       
    }
	printf("\nBucket Sort: min = %ld, max = %ld, N = %d buckets\n", *min, *max, N);
}

//Complicated way to insert node into sorted list
void insert_sorted(nodePT *L, nodePT nd) {
	nodePT temp; 

	if(nd->data <=(*L)->data) {
		nd->next = *L;
		*L = nd;
	} else {
		temp = *L; 

		while(temp->next != NULL && (temp->next->data < nd->data)) {
			temp = temp->next;
		}
		nd->next = temp->next;
		temp->next = nd;
	}
} 