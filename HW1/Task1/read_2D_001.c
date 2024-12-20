/*
Covered:
- function signature before function definition and main() "at the top"
- populate array in function
- pass array as argument
- formatted printing with %7d
- reads data from a file in the format:
Rows Cols
r1c1 r1c2 ....
r2c1 r2c2 ....

compile:
gcc -g read_2D_001.c

run with user input
./a.out

run with input redirection
./a.out < test.txt

run with valgrind with user input:
valgrind --leak-check=full ./a.out

run with valgrind with input redirection
valgrind --leak-check=full ./a.out < test.txt
*/

#include <stdio.h>
#include <stdlib.h>

//#define MAX_ROWS 100
//#define MAX_COLUMNS 1000

void test_array_1();
void test_array_2();
void test_array_3();
int** test_array_3b(int* nrp, int* ncp);
void print_2D_1(int num_rows, int num_cols, int arr2D[][num_cols]);
void populate_array_1(int num_rows, int num_cols, int arr2D[][num_cols], FILE *fp);
void populate_2(int num_rows, int num_cols, int* arr2D[], FILE *fp);
void print_2(int num_rows, int num_cols, int* arr2D[]);

int main(){
	//test_array_1();
	//test_array_2();
	//test_array_3();
	int r, nr=0, nc=0;
	int** my_arr = test_array_3b(&nr, &nc);  // my_arr = 1b1b;

	// better moved in a separate function
	for(r = 0; r<nr; r++){
		free(my_arr[r]);
	}
	free(my_arr);

	return 0;
}

int** test_array_3b(int* nrp, int* ncp){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return NULL;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);

	//int* arr2[num_rows];
	int** arr2;
	arr2 = calloc(num_rows, sizeof(int*) );

	for(r = 0; r<num_rows; r++){
		arr2[r] = calloc( num_cols, sizeof(int) );
	}

	populate_2(num_rows, num_cols, arr2, fp);
	print_2(num_rows, num_cols, arr2);

	(*nrp) = num_rows;
	(*ncp) = num_cols;

	fclose(fp);

	return arr2;
}


void test_array_3(){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);

	//int* arr2[num_rows];
	int** arr2;
	arr2 = calloc(num_rows, sizeof(int*) );

	for(r = 0; r<num_rows; r++){
		arr2[r] = calloc( num_cols, sizeof(int) );
	}

	populate_2(num_rows, num_cols, arr2, fp);
	print_2(num_rows, num_cols, arr2);

	for(r = 0; r<num_rows; r++){
		free(arr2[r]);
	}

	free( arr2);

	fclose(fp);
}


//============================

// mix heap, stack
void test_array_2(){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);

	int* arr2[num_rows];
	for(r = 0; r<num_rows; r++){
		arr2[r] = calloc( num_cols, sizeof(int) );
	}

	populate_2(num_rows, num_cols, arr2, fp);
	print_2(num_rows, num_cols, arr2);

	for(r = 0; r<num_rows; r++){
		free(arr2[r]);
	}

	fclose(fp);
}

void populate_2(int num_rows, int num_cols, int* arr2D[], FILE *fp){
	for (int r = 0; r<num_rows; r++){
		for(int c=0; c<num_cols; c++){
			//scanf("%d", &n);
			//fscanf(fp, "%d", &n);	arr2[r][c]=n;
			fscanf(fp, "%d", &arr2D[r][c]);
		}
	}
}


void print_2(int num_rows, int num_cols, int* arr2D[]){
	int r,c;
	printf("\n");
	for (r = 0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			printf("%8d, ", arr2D[r][c]);
		}
		printf("\n");
	}

}



//=========================================================

void test_array_1(){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);

	int arr2[num_rows][num_cols];

	populate_array_1(num_rows, num_cols, arr2, fp);
	print_2D_1(num_rows, num_cols, arr2);

	//return arr2 ;
	fclose(fp);

}

void populate_array_1(int num_rows, int num_cols, int arr2D[][num_cols], FILE *fp){
	for (int r = 0; r<num_rows; r++){
		for(int c=0; c<num_cols; c++){
			//scanf("%d", &n);
			//fscanf(fp, "%d", &n);	arr2[r][c]=n;
			fscanf(fp, "%d", &arr2D[r][c]);
		}
	}
}

void print_2D_1(int num_rows, int num_cols, int arr2D[][num_cols]){
	int r,c;
	printf("\n");
	for (r = 0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			printf("%8d, ", arr2D[r][c]);
		}
		printf("\n");
	}

}

/*
void test_array_1(){
	char fname[101]; // max length for file name? 99 + \0
	int num_rows, num_cols, r,c;
	FILE *fp;

	printf("Enter the filename: ");
	scanf("%s", fname);
	fp =fopen(fname, "r");
	if (fp == NULL){
		printf("File could not be opened.\n");
		return;
	}
	fscanf(fp, "%d %d", &num_rows, &num_cols);

	int arr2[num_rows][num_cols];
	for (r = 0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			//scanf("%d", &n);
			//fscanf(fp, "%d", &n);	arr2[r][c]=n;
			fscanf(fp, "%d", &arr2[r][c]);
		}
	}

	printf("\n");
	for (r = 0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			printf("%8d, ", arr2[r][c]);
		}
		printf("\n");
	}
}
*/
