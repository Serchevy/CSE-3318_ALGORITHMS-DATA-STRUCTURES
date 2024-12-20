#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

// As you can tell I Used a lot of pointers :)
int num_lines(char *file);
void coursesArr(char arr[][30], char *file);
int adj_matrix(int N, int matrix[][N], char arr[][30], char *file);
void course_perLine(int N, int (*num)[N], char *file);
void print_matrix(int N, int matrix[][N]);
int topo(int N, int (*res)[], int matrix[][N]);
int DFS_visit(int *i, int N, int (*res)[], int G[][N], int u, char *color, int *pred);

int main() {
    char file_name[30];

    printf("\nThis program will read, from a file, a list of courses and their prerequisites\n");
    printf("Enter filename: "); 
    scanf("%s", file_name);

    int N = num_lines(file_name);                           //Open file and count number of lines which equals number of unique course

    if(N==-1) {
        printf("\nFailed to read from file. Program will terminate.\n\n");
        return 0;
    } else if(N==0) {
        printf("Number of vertices in built graph: N = %d\n", N);
        printf("\nThe file appears to be empty.\n\n");
        return 0;
    } else {
        char courses[N][30];
        int matrix[N][N];
        int empty;
        int indx = 1;

        printf("Number of vertices in built graph: N = %d\n", N);

        coursesArr(courses, file_name);                     // Ope file again and store the unique courses in a char* arr

        printf("Vertex - coursename correspondence\n");
        for(int i=0; i<N; i++) {
            printf("%d - %s\n", i, courses[i]);
        }

        printf("\nAdjecency matrix:\n");                    

        for(int i=0; i<N; i++) {                            // Set all values to ZER0 on 2D adj Matrix
            for(int j=0; j<N; j++) matrix[i][j] = 0;
        }

        empty = adj_matrix(N, matrix, courses, file_name);  // Compute Adj Matrix. Dereferencing some variables ofc

        if(empty==TRUE) {                                   //if adj_matrix returns true then courses don't have any prereqs, else they do
            printf("Looks like courses don't have any prerequisites.\n");
            printf("You may take the courses in any order you wish!\n\n");
        } else {
            int res[N];                                     // Arr that will hold reverse order in which to take courese
            if(topo(N, &res, matrix)==0) {                  // Call topological sorting. If ZERO is returned there is a cycle.
                printf("\nThere was at least one cycle. There is no possible ordering of the courses.\n\n");
                return 0;
            }

            printf("Order in which to take courses:\n");
            for(int i=N-1; i>=0; i--) {                     //Iterate thru resArr in reverse to get the right order in which to take courses    
                printf("%d. - %s ", indx, courses[res[i]]);
                for(int j=0; j<N; j++) {
                    if(strcmp(courses[res[i]], courses[j]) == 0) {
                        printf("(corresponds to graph vertex %d)\n", j);
                    }
                }
                indx++;
            }
            printf("\n");
        }
    }
    return 0;
}

int num_lines(char *file) {                                 //Get number of lines/unique courses
    int N = 0;
    char c;

    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
		printf("Could not open file %s. Exit.\n", file);
		return -1;
	}

    while(!feof(fp)) {
        c = fgetc(fp);
        if(c == '\n') {
            N++;
        }
    }
    fclose(fp);
    return N;
}

void coursesArr(char arr[][30], char *file) {               // Tokenize the 1st word in each line, to get the unique courses
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
		printf("\nERROR:: File could not be opened.\n");
		return;
	}

    char buffer[100];
    char *token;
    int i = 0;

    while(!feof(fp)) {                                      
        fgets(buffer, 100, fp);
        token = strtok(buffer, " \n");
        strcpy(arr[i], token);
        i++;
    }
    fclose(fp);
}

int adj_matrix(int N, int matrix [][N], char arr[][30], char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
		printf("\nERROR:: File could not be opened.\n");
		return -1;
	}

    char buffer[100];
    int num[N];
    int max = 0;
    int empty = TRUE;

    course_perLine(N, &num, file);                          //Store in an array the # of courses per line respectively 

    for(int k=0; k<N; k++) {
        if(num[k]>max) max = num[k];                        //Find the max # courses in a line from all lines
    }

    for(int i=0; i<N; i++) {
        int vertices[max];                                  //Initialize an array of size max to hold indx of conected vertices
        int x = 0;
        int goal = 0;                                       //indx, in arrOfCourses, of the goal that prerequisite courses must achive

        fgets(buffer, 100, fp);

        if(num[i] > 1) {                                    //If current line only has one course(no pre-reqs) skip it!
            char *token = strtok(buffer, " \n");            //Start tokenizing line

            while(token) {
                for(int k=0; k<N; k++){                     //Tokenize every course, Look for it on arrOfCourses, and store its idx in verticesArr
                    if(strcmp(arr[k], token) == 0) {
                        vertices[x] = k;
                        x++;
                    }     
                }
                token = strtok(NULL, " \n");
            }
            goal = vertices[0];                             //Set first course as the goal, since that's the main course and the rest are prereqs
        } 
        for(int j=1; j<x; j++) {
            matrix[vertices[j]][goal] = 1;                  //Iterate thru verticesArr. Set its value as the 1st argumetn in Matrxi and the 2nd is goal
        }
    }

    for(int i=0; i<N; i++){
        for(int k=0; k<N; k++){
            if(matrix[i][k]>0) empty = FALSE;               //if a 1 is found in matrix, at least one course has a prereq so set empty to false
        }
    }

    print_matrix(N, matrix);                                //Print Matrix
    fclose(fp);
    return empty;
}

void course_perLine(int N, int (*num)[N], char *file) {     //Store in an array the # of courses per line respectively 
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
		printf("\nERROR:: File could not be opened.\n");
		return;
	}

    char buffer[1000];

    for(int i=0; i<N; i++) {
        int count = 0;
        fgets(buffer, 1000, fp);
        char *token = strtok(buffer, " \n");
        while(token) {
            token = strtok(NULL, " \n");
            count++;
        }
        (*num)[i] = count;
    }

    fclose(fp);
    return;
}

void print_matrix(int N, int matrix[][N]) {                 //Convoluted mess to print formatted matrix 
    for(int i=0; i<=N; i++) {
        if(i==0) printf("    |");
        else printf("%4d", i-1);
    }
    printf("\n");
    for(int j=0; j<(N*4)+5; j++) {
        printf("-");
    }
    printf("\n");
    for(int k=0; k<N; k++) {
        printf("%4d|", k);
        for(int z=0; z<N; z++){
            printf("%4d", matrix[k][z]);
        }
        printf("\n");
    }
    printf("\n");
}

int topo(int N, int (*res)[], int G[][N]) {                 //Perform topological sorting on Adj Matrx according to code from Slides                     
    char color[N];                                          //Res is the result arr back in main
    int pred[N];
    int i = 0;

    for(int u=0; u<N; u++){
        color[u] = 'W';
        pred[u] = 0;
    }
    for(int u=0; u<N; u++) {
        if(color[u]=='W') {
            if(DFS_visit(&i, N, res, G, u, color, pred)==0){//pass down res and [i] --> (current indx of res)
                return 0;
            }
        }
    }
    return 1;
}

int DFS_visit(int *i, int N, int (*res)[], int G[][N], int u, char *color, int *pred) {
    color[u] = 'G';                                         //Same-ish code as in the slides
    for(int v=0; v<N; v++) {
        if(G[u][v] == 1) {
            if(color[v]=='W') {
                pred[v] = u;                                // This lines seems useless. I genuenly don't know how it shoul've been used....
                if(DFS_visit(i, N, res, G, v, color, pred) == 0) {
                    return 0;
                }
            }
            if(color[v]=='G') {
                return 0;
            }
        }
    }
    color[u] = 'B';
    (*res)[(*i)] = u;                                       //set u/v in the [i] indx of res. Dereferencing ofc!
    (*i)++;                                                 //incrment i. Dereferencing ofc!
    return 1;
}