#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "spell.h"

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as 
they are declared before the array in the parameter list. See: 
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

Worst case time complexity to compute the edit distance from T test words
 to D dictionary words where all words have length MAX_LEN: 
Student answer:  Theta(.....D*T*MAX_LEN.......)

Worst case to do an unsuccessful binary search in a dictionary with D words, when 
all dictionary words and the searched word have length MAX_LEN 
Student answer:  Theta(.....log(D)*MAX_LEN.......)
*/


/* You can write helper functions here */
void print_Table(int row, int col, char *v, char *h, int arr[row][col]);
int compare(const void *a, const void *b);
void spell_check_plus(char *file_out, char *testname, int N, char *dict[], int print);
int binary_search(char *dict[],int N, char *buffer, int *cmp_count, int print);
void similar_words(int N, char *dict[], char *buffer, int (*arr)[]);
/*
Parameters:
first_string - pointer to the first string (displayed vertical in the table)
second_string - pointer to the second string (displayed horizontal in the table)
print_table - If 1, the table with the calculations for the edit distance will be printed.
              If 0 (or any value other than 1)it will not print the table
(See sample run)
Return:  the value of the edit distance (e.g. 3)
*/
int edit_distance(char * first_string, char * second_string, int print_table) {
	int row = strlen(first_string) + 1;  
	int col = strlen(second_string) + 1; 

	int table[row][col]; 
	
	for(int i=0; i<row; i++) { // Special cases. y-indexes
		table[i][0] = i;
	}
	for(int j=0; j<col; j++) { // Special cases. x-indexes
		table[0][j] = j;
	}
	for(int k=1; k<row; k++) {
		char c1 = first_string[k-1]; // slowly build up 1st_str 

		for(int z=1; z<col; z++) {
			char c2 = second_string[z-1]; // slowly build up 2nd_str

			if(c1==c2) {
				table[k][z] = table[k-1][z-1]; // if equal curr cell = diagonal cell
			} else {
				int top, left, diag, minDist;

				top = table[k-1][z] + 1;       // upper cell
				left = table[k][z-1] + 1;      // left cell
				diag = table[k-1][z-1] + 1;    // diagonal cell
				minDist = top;                 // Lets asssume smalles is upper cell

				if(left < minDist) {     // cmp with left cell. If smaller, update minDist
					minDist = left;
				}
				if(diag < minDist) {     // then cmp with diag cell. if smaller, update minDist
					minDist = diag;
				}

				table[k][z] = minDist;    // assign minDist to curr cell 
			}
		}
	}
	if(print_table==1) {
		print_Table(row, col, first_string, second_string, table);
	}

	return table[row-1][col-1];  // replace this line with your code
}

// Messy/fancy table formatting
void print_Table(int row, int col, char *v, char *h, int arr[row][col]) {
	printf("\n");
	for(int k=0; k<=col; k++){ // Prints word on the horizontal after 2 empty cells
		if(k==0){
			printf("  |");
		} else if(k==1) {
			printf("   |");
		} else {
			printf("%3c|", h[k-2]);
		}
	}

	printf("\n"); // A lot of new lines around here...
	for(int k=0; k<(col*4)+3; k++) { // Prints a single bottom border
			printf("-");
	}
	printf("\n");
	
	for(int i=0; i<row; i++){
		if(i>0) {
			printf("%2c|", v[i-1]);  // Slowly prints word on the vertical after 2 empty cells
		} else {
			printf("  |");
		}
		for(int j=0; j<col; j++){ 
			printf("%3d|", arr[i][j]); // Prints one table row with the edit distances
		}
		printf("\n");
		for(int k=0; k<(col*4)+3; k++) { // Prints bottom borders
			printf("-");
		}
		printf("\n");
	}
	printf("\n");
}	  

/*
Parameters:
testname - string containing the name of the file with the paragraph to spell check, includes .txt e.g. "text1.txt" 
dictname - name of file with dictionary words. Includes .txt, e.g. "dsmall.txt"
printOn - If 1 it will print EXTRA debugging/tracing information (in addition to what it prints when 0):
			 dictionary as read from the file	
			 dictionary AFTER sorting in lexicographical order
			 for every word searched in the dictionary using BINARY SEARCH shows all the "probe" words and their indices indices
			 See sample run.
	      If 0 (or anything other than 1) does not print the above information, but still prints the number of probes.
		     See sample run.
*/
void spell_check(char * testname, char * dictname, int printOn){
	char file_out[100] = "out_";
	strcat(file_out, testname);  // create name of output file
	printf("Corrected output filename: %s\n", file_out);
	printf("\nLoading the dictionary file: %s\n", dictname);

	int N = 0;
	FILE *fp;
	fp = fopen(dictname, "r"); // Sanity check for dictionary file
		if (fp == NULL) {
			printf("\n  Dictionary file could not be opened.\n");
			return;
		}
	fscanf(fp, "%d", &N);  
	printf("\nDictionary has size: %d\n", N);
	if(printOn==1) { printf(" Original Dictionary\n"); }

	char *dict[N];
	char buff[100];
	for(int i=0; i<N; i++) {  //Process Dictionary into str array
		fscanf(fp, "%s", buff);
		dict[i] = malloc(100);
		strncpy(dict[i], buff, 100);
		if(printOn==1) { printf("%d. %s\n", i, dict[i]); }
	}

	qsort(dict, N, sizeof(char*), compare);  //Alphabetically sort dictionary
	if(printOn==1) {
		printf("\n Sorted dictionary (alphabetical order):\n");
		for (int i=0; i<N; i++) { printf("%d. %s\n", i, dict[i]); }
		printf("\n");
	}

	spell_check_plus(file_out, testname, N, dict, printOn); //Contine logic on another function. Lord forgive me for what I'm about to code...
	printf("\n");

	for(int i=0; i<N; i++){ //Free allocated space for dictionary
		free(dict[i]); 
	}
	fclose(fp);
}

int compare(const void *a, const void *b) { // Sorting logic. Took a bit of researching...
    const char **pa,**pb;

    pa = (const char **)a;
    pb = (const char **)b;
    return(strcmp(*pa,*pb));
}

void spell_check_plus(char *file_out, char *file, int N, char *dict[], int printOn){
	FILE *fp, *fp2;
	fp = fopen(file, "r"); // Open input file and do a sanity check
		if (fp == NULL) {
			printf("\n  Text file could not be opened.\n");
			return;
		}
	fp2 = fopen(file_out, "w"); // Open output file and do a sanity check
		if (fp == NULL) {
			printf("\n  Output text file could not be opened.\n");
			return;
		}

	int i = 0;               // counter of grabbed characters/indx of buffer
	int selection = 0;       // User's choice 
	char c;						 
	char buffer[100] = {0};   

	do { 
		c = fgetc(fp);  // if any of the chars from below is found enter separator mode
		if(c== ' ' || c== ',' || c== '.' || c== '?' || c=='!' || c == EOF) {
			int cmp_count = 0;         // counter of words from dictionary compared to a word from file, in binary search
			int arr[N/2];              // arr that will contain indxes of words with min edit distance in Dict, after calling similar_words
			int BSearch = 0;
			buffer[i] = '\0'; 
			
			if(i!=0) {BSearch = binary_search(dict, N, buffer, &cmp_count, printOn);} // Do Binary search on dict with the word

			if(i==0) {                          // In Separator mode, i is set to ZER0 at the end, so if another separator is found right away
				if(c!= EOF) { fputc(c, fp2); }  // just write it to the output file, as long as that separator is not EOF
			} else if(BSearch > -1) {
				printf("\n");
				printf("---> |%s| (words compared when searching %d)\n", buffer, cmp_count); // If BS FOUND an exact match, just writte it to output file
				printf("     - OK\n\n");													 // along with the separator in front of it, if said separator is not EOF
				fputs(buffer, fp2);
				if(c!= EOF) { fputc(c, fp2); }
			} else {
				printf("\n");
				printf("---> |%s| (words compared when searching: %d)\n", buffer, cmp_count); // If BS did NOT found an exact match, call simsilar_words
				printf("-1 - type correction\n");											  // (which will print the words in Dict with the min edit distance)
				printf(" 0 - leave the words as is (do not fix spelling)\n");                 // and ask user for a choice
				similar_words(N, dict, buffer, &arr);
				printf("Enter your choice (from the above options): ");
				scanf("%d", &selection);

				if(selection == -1) {                     // If -1, prompt user to input correct word and write it to output file along with the separator
					char str[100];
					printf("Enter correct word: ");
					scanf("%s", str);
					fputs(str, fp2);
					if(c!= EOF) { fputc(c, fp2); }
				} else if (selection == 0) {             // if 0, write word as it is to output file along with separator
					fputs(buffer, fp2);
					if(c!= EOF) { fputc(c, fp2); }
				} else {
					fputs(dict[arr[selection-1]], fp2);  // otherwise, get indx of word in dict (from arr of indexes with min edit ditance) and write that word to file
					if(c!= EOF) { fputc(c, fp2); }       
				}
			}
			memset(buffer, 0, 100); // clear buffer & go back to indx 0 on it
			i = 0;
		} else { // Build word mode!
			buffer[i] = c;
			i++;
		}
	} while(!feof(fp));

	fclose(fp);
	fclose(fp2);
}

int binary_search(char *dict[], int N, char *buf, int *cmp_count, int printOn) {
	char buffer[101];     
	strcpy(buffer, buf); 

	for(int i=0; buffer[i]; i++) {       // lower case copy of current word being process!
		buffer[i] = tolower(buffer[i]);  
	}

	if(printOn == 1) { printf("\nBinary Search for: %s\n", buffer); }

	int left = 0;
	int right = N-1;

	while(left <= right) {      // Regular old binary search
		int m = (left+right)/2;
		(*cmp_count)++;
		if((strcmp(buffer, dict[m]) == 0)) {
			return m;
		}

		if((strcmp(buffer, dict[m])) < 0){
			right = m-1;
		} else {
			left = m+1;
		}
		if(printOn == 1) { printf("[%d]. %s\n", m, dict[m]); }
	}
	if(printOn == 1) { printf("Not Found\n"); }
	return -1;
}

void similar_words(int N, char *dict[], char *buffer, int (*arr)[]) {
	int distances[N];      // arr that will hold the edit ditances for each word in dict
	int dist = 0;         
	int n = 0;             // indx of array that holds the indexes of words in dict with min edit distance

	for(int j=0; j<N; j++) {
		dist = edit_distance(dict[j], buffer, 0); // Compute edit ditances for every word in dict
		distances[j] = dist;
		//printf("[%d]. %d\n", j, distances[j]);
	}

	int min = distances[0];    // Find min distance
	for (int i=0; i<N; i++) {      
		if(distances[i] < min){    
			min = distances[i];   
    	}
	}
	printf("     Minimun distance: %d  (computed edit distances: %d)\n", min, N);
	printf("     Words that gave minimun distance:\n");

	for(int k=0; k<N; k++) {  // Put the dict indexes of words with min ditance inside arr
		if(distances[k] == min) { 
			printf(" %d - %s\n", n+1, dict[k]);
			(*arr)[n] = k;
			n++;
		}
	}
}