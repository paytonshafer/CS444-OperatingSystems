/* 
Operating Systems - CS444/544
Spring 2023
Payton Shafer
HW2: Producer-Consumer Problem
Program Description: This is a solution using pthreads and mutexes to solve the critical region access problem.
Last Modified: 2/17/2023
Code Review: https://youtu.be/l7Ywm86JVwM */

// Includes
#include <iostream> //standard I/O library
#include <queue> //A library that allows us to use a queue (for the buffer)
#include <pthread.h> //the pthread library

using namespace std; //The namespace I will use is the standard one

// Constants/Defines
#define MAX 100 //This is the amount of numbers that will be produced/consumes

// Global Variables
pthread_mutex_t mtx; //This is the mutex declaration
pthread_cond_t cCond, pCond; //These are the condition declarations, these are used for the signalling between producor and consumer
queue<int> buffer; //This is the shared buffer that the consumer and producer use

// Functions
//This is the function for the producer that will loop MAX times and each time will add something to the buffer
void *producer(void *arg){
    //The follwoing for loop will loop MAX times and each time will add an item to the buffer
    for(int i = 0; i <= MAX; i++){
        pthread_mutex_lock(&mtx); //This call locks the mutex to get exclusive access to buffer
        //This loop will check if the buffer isnt empty then waits for a wake-up(signal) call to it
        while (!(buffer.empty())) {
            pthread_cond_wait(&pCond, &mtx); //This waits to get a signal from pCond
        }
        buffer.push(i); //adds i to the buffer
        cout << "Producer produced: " << i << endl; //prints what the producer produced to standard output
        pthread_mutex_unlock(&mtx); //This unlocks the mutex and releases access to the buffer
        pthread_cond_signal(&cCond); //signals to consumer to wake up
    }
    return NULL; //Just so it returns something
}

//This is the function for the consumer that will loop MAX times and each time consumes (removes) something from the buffer
void *consumer(void *arg){
    //The follwoing for loop will loop MAX times and each time will remove an item to the buffer
    for(int i = 0; i <= MAX; i++){
        pthread_mutex_lock(&mtx); //This call locks the mutex to get exclusive access to buffer
        //This loop will check if the buffer is empty then waits for a wake-up(signal) call to it
        while (buffer.empty()) {
            pthread_cond_wait(&cCond, &mtx); //This waits to get a signal from cCond
        }
        int item = buffer.front(); //saves the value on the front of the buffer
        buffer.pop(); //removes the value in the front of the buffer (will be same as item)
        cout << "Consumer consumed: " << item << endl; //prints what the consumer consumed to standard output
        pthread_mutex_unlock(&mtx); //This unlocks the mutex and releases access to the buffer
        pthread_cond_signal(&pCond); //signals to producer to wake up
    }
    return NULL; //Just so it returns something
}

// Main
int main() {
    pthread_t pro, con; //intialization of producer and consumer thread, respectfully
    pthread_mutex_init(&mtx, NULL); //initialization of the mutex
    pthread_cond_init(&pCond, NULL); //initailize the producer condition variable
    pthread_cond_init(&cCond, NULL); //initailize the consumer condition variable

    pthread_create(&pro, NULL, producer, NULL); //creating and starting the producer thread running the producer function
    pthread_create(&con, NULL, consumer, NULL); //creating and starting the consumer thread running the consumer function

    //The folowing 2 join calls ensure that the two threads are syncronized and waits for them to finish their operations
    pthread_join(pro, NULL);
    pthread_join(con, NULL);

    pthread_mutex_destroy(&mtx); //destroys the mutex
    pthread_cond_destroy(&pCond); //destorys the producer condition variable
    pthread_cond_destroy(&cCond); //destorys the consumer condition variable
}