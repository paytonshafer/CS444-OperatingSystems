/* 
Operating Systems - CS444/544
Spring 2023
Payton Shafer
HW1: Matrix Multlipication with pthreads
Program Description: This program will ask the user for the size of 2 matricies and fill them with
random numbers from 1 to 100. Then, using multithreading with pthreads, the matrix multlipication is
started with each thread representing a cell for the solution matrix and the thread does the dot product
of the corresponding row from matrix 1 and column from matrix 2. Then it prints the solution matrix
Last Modified: 2/6/2023
Code Review: https://youtu.be/nXbQRqkso3U */

//inlcudes
#include <iostream> //standard i/o library
#include <stdlib.h> //the standard library (for random numbers)
#include <ctime> //c time library (for seeding the random number generator)
#include <pthread.h> //the pthread library
using namespace std;

//global variables
int** A; //This is the declartion for matrix one
int** B; //This is the declartion for matrix two
int** C; //This is the declartion for the solution matrix

//This is a struct for the paramerter that will be passed to the dot product funtion
struct params {
    int i; //This is the row posision of the cell for the solution matrix
    int j; //This is the column posision of the cell for the solution matrix
    int n; //This is the size of how many components are in the vectors for the dot product
};

//functions
//The function below creates a matrix taking the size as input and returns the array
int** createMatrix(int rows, int cols){
    int** array = new int*[rows]; // This line creates a 2D array and allocates the memory for each row 
    //The loop goes through each row and creates an array of size cols (for the number of columns)
    for (int i = 0; i < rows; i++) {
        array[i] = new int[cols];
    }
    return array; //returns created array
}

/*This function loops through the 2D array and puts a random integer in each 'cell'
The function takes the array and the size as inputs*/
void fillMatrix(int** array, int rows, int cols){
    //The loop below loops through each position in the matrix to put a rand number there
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            array[i][j] = rand() % 100 + 1; //This line creates a rand number from 1 to 100
        }
    }
}

//This function prints out the array to standard output, it takes an array as input
void printMatrix(int** array, int rows, int cols){
    //The loop below loops through each position in the matrix to print it out
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << array[i][j] << " "; //prints the current cell to standard output
        }
        cout << endl; //call for a new line at the end of each row
    }
}

//This function computues the dot project for the given row of matrix one and the column of matrix two
void *dotProduct(void *arg){
    params *p = (params*)arg; //Creating an instance of params and typcasing (params*) onto args
    //The next 2 lines are storing the values from p into ints i,j and n
    int i = p->i; //This is the row posision of the cell for the solution matrix
    int j = p->j; //This is the column posision of the cell for the solution matrix
    int n = p->n; //This is the size of how many components are in the vectors for the dot product
    

    C[i][j] = 0; //Setting inital value of cell to 0
    //This for loop is to go through each compenent that will be computed for the dot product
    for(int k = 0; k < n; k++){
       C[i][j] += ((A[i][k]) * (B[k][j])); //This is adding the current part of the dot product to the total sum for the cell
    }
    pthread_exit(0); //This calls for the pthread to exit since it is done doing it's work
}


//main
int main(){
    //This is used to seed a pseudorandom number generator with the current time
    srand(static_cast<unsigned int>(time(nullptr)));
    //These values represent the size of the rows and columns of matrix one and two respectivly
    int row1, col1, row2, col2;

    cout << "\n\t\t\t\tWELCOME TO THE MATRIX MULTIPLIER!\n";
    //The do while is to read in the size of the matrix and then checks that it passes the condition for matrix multlipication
    do{
        //The following lines get the size of matrix one and matrix two as rows and columns
        cout << "Reminder: To do matrix multlipication the columns of matrix one must equal the rows of matrix two \n\n";
        cout << "Input the number of rows and columns in matrix one with a space between them: ";
        cin >> row1 >> col1;
        cout << "Input the number of rows and columns in matrix two with a space between them: ";
        cin >> row2 >> col2;

        /*This if checks if the input passed the condition for matrix multlipication, 
        (M1 cols = M2 rows) if yes the matricies are shown else they are told why and asked to do it again*/
        if(!(col1 == row2)) cout << "\nNumber of columns in matrix one does not equal the number of rows in matrix two\n\n";
    }while(!(col1 == row2));
    
    //The code below creates 3 matricies where A is matrix one, B is matrix two, and C is the solution matrix
    A = createMatrix(row1,col1);
    B = createMatrix(row2,col2);
    C = createMatrix(row1,col2);

    //These calls fill Matrix A and B with random integers 
    fillMatrix(A, row1, col1);
    fillMatrix(B, row2, col2);

    //print matrix A and B
    cout << "\nMatrix One: " << row1 << "x" << col1 << endl; //prints size of matrix one
    printMatrix(A, row1, col1); //print matrix A
    cout << endl << "Matrix Two: " << row2 << "x" << col2 << endl; //prints size of matrix two
    printMatrix(B, row2, col2); //print matrix B
    
    
    //This is the declaration of the thread array
    pthread_t threads[row1][col2];

    //These nested for loops go through each cell of the solution matrix and create a thread doing dotProduct
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            params *p = new params(); //creates a pointer to an instance of the params struct
            p->i = i; //This is the row posision of the cell for the solution matrix
            p->j = j; //This is the column posision of the cell for the solution matrix
            p->n = col1; //This is the size of how many components are in the vectors for the dot product

            pthread_create(&threads[i][j], NULL, dotProduct, (void*)p); //This creates the thread and has it do the dot product
        }
    }

    //These nested for loops are to make sure all the threads wait and make sure they are synchronized
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col1; j++){
            pthread_join(threads[i][j], NULL); //This makes sure the thread waits and joins with all others
        }
    }
    
    //The following code is to print out the solution matrix
    cout << "\nSolution Matrix: " << row1 << "x" << col2 << endl; //prints size of solution matrix
    printMatrix(C, row1, col2);
}