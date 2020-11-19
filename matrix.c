#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX 4

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 



.
typedef void*(*Operations)(void*);

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void* computeSum(void* args) { // pass in the number of the ith thread
   int *coord = (int*) args;
   int row = (*coord)/MAX;
   int col = (*coord)%MAX; 
  
   matSumResult[row][col] = matA[row][col] + matB[row][col];
}


void* computeDiff(void* args) { // pass in the number of the ith thread
  int *coord = (int*) args;
  int row = (*coord)/MAX;
  int col = (*coord)%MAX; 
  
  matDiffResult[row][col] = matA[row][col] - matB[row][col];

}
void* computeProduct(void* args) { // pass in the number of the ith thread
  int *coord = (int*) args;
  int row = (*coord)/MAX;
  int col = (*coord)%MAX; 
  
  for(int pos = 0; pos<MAX; pos++){
    matProductResult[row][col] += matA[row][pos] * matB[pos][col];
  }

}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char *argv[]) {
    //int max;
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
   
     pthread_t threads[MAX*MAX];
  
    // 4. Create a thread for each cell of each matrix operation.
   
    
    Operations op[3];
    op[0] = &computeSum;
    op[1] = &computeDiff;
    op[2] = &computeProduct;
  
   
  for(int i = 0; i<3; i++){
      for (int j = 0; j< MAX*MAX; j++){
       int *index = (int*)malloc(sizeof(int));
       *index = j;
        
        pthread_create(&threads[*index], NULL, op[i], (void*)index);
      }
    }
    // 5. Wait for all threads to finish.
    

    for(int j = 0; j< MAX*MAX; j++){
      pthread_join(threads[j],NULL);    
    }
 
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}

