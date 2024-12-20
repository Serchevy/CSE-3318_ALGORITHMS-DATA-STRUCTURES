#include "hw1_student.h"  // .h extenssion, not .c

// You do not need to change this print_1D function. It is included here for 
// your convenience, in case you need to call it while debugging.
void print_1D(int sz, int * arr){
	for(int k = 0; k< sz; k++){
		printf("%8d", arr[k]);
	}
	printf("\n");
}


/* dynamically allocates an array and populates it with all values from arr 
	that are strictly smaller than thresh. 
	Returns:
		- the pointer to the dynamically allocated array.
		- NULL if all elements of arr are greater or equal to thres. In this case it does not allocate any memory, and sets content of sz_res to 0.
*/
int* get_scores_below(int thresh, int sz_arr, int * arr, int* sz_res){
	// change code here to correct function implementation
	
	int num = 0;
	
	for(int i = 0; i < sz_arr; i++) { 
		if(arr[i] >= thresh) {
			num++;
			if(num == sz_arr) {
				*sz_res = 0;
				return NULL;
			}
		}
	}
	
	int new_sz = sz_arr - num;
	int indx = 0;
	int* ptr = malloc(sizeof(int) * new_sz);
	
	for(int i = 0; i < sz_arr; i++) {
		if(arr[i] < thresh) { 
			ptr[indx] = arr[i];
			indx++;
		} 
	}
	*sz_res = new_sz;
	return ptr;
}


void update_min_max(int num_rows, int num_cols, int* arr2D[], int* arr_min, int* arr_max){
	// write your code here
	
	int max_r, max_c, min_r, min_c;
	int max = arr2D[0][0];
	int min = arr2D[0][0];
	
	for(int i = 0; i<num_rows; i++) {
		for(int j = 0; j<num_cols; j++) {
			if(max<arr2D[i][j]) {
				max = arr2D[i][j];
				max_r = i;
				max_c = j;
			}
			
			if(min>arr2D[i][j]) {
				min = arr2D[i][j];
				min_r = i;
				min_c = j;
			}
		}
	}
	
	*arr_max = arr2D[max_r][max_c];
	*arr_min = arr2D[min_r][min_c];
}


// improve this code to print as required in the homework
void print_2D(int num_rows, int num_cols, int* arr2D[]){	
	int r,c;
	printf("\n   |");
	for(c = 0; c<num_cols; c++) {
		printf("%8d|", c);
	}
	printf("\n");
	for(c = -1; c<num_cols; c++) {
		printf("--------");
	}
	printf("-\n");
	for (r = 0; r<num_rows; r++){
		printf("%3d|", r);
		for(c=0; c<num_cols; c++){
			printf("%8d|", arr2D[r][c]);
		}
		printf("\n");
	}	
}


