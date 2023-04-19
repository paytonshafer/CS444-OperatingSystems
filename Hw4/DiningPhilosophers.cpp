/* 
Operating Systems - CS444/544
Spring 2023
Payton Shafer
HW4: Dining Philosophers Problem
Program Description: This is a solution to the dining philosphers problem using threads and mutexes to prevent race conditions, I used the idea of Reasource Heirachy to solve this problem.
Last Modified: 4/19/2023
Code Review: https://youtu.be/j6fTAlVVUtI*/

/**********************************************************
compile with:  g++ DiningPhilosophers.cpp -std=c++11 -o dp
**********************************************************/

// Includes
#include <iostream> //standard io library for c++
#include <chrono> //time library
#include <mutex> //library for mutexes
#include <thread> //standard thread library for c++
#include <random> //library to generate randome numbers

using namespace std; //The namespace I will use is the standard one

// Global Variables
//The below array holds the status of each philospher where status[p-1] is the status of philospher p
//each philospher is given an initial status of 100%
int status[5] = {100, 100, 100, 100, 100}; 


// Functions
//The below function is a random number generator that returns an int between min and max
int myrand(int min, int max) {
  //this is initializing the seed for the rand number generator to take the current time
  //the use of the key work static is that it is only ran once
  static mt19937 rnd(time(nullptr));
  //We use uniform_int_distribution to generate a uniformly distributed integer between min and max
  //using the rnd generator as the source of randomness.
  return uniform_int_distribution<>(min,max)(rnd);
}

//The below function represents a philospher where ph is the philisopher number, ma and mb are the forks next to the philisopher,
//ma is always the fork with a lower id number than mb, mo is a mutex to ensure that output is properly nested, ms is a mutex for the status array
void philosopher(int ph, mutex& ma, mutex& mb, mutex& mo, mutex& ms) {
  //We create an infinite loop for the philisopher
  for (;;) {  // prevent thread from termination
    this_thread::sleep_for(chrono::milliseconds(200)); //let the thread sleep for 200 ms
    int duration = myrand(800, 1200); //genertae a random int from 800 to 1200 for how long the philospher will think for
    {//we lock the mutex mo for the block and it stays locked until the end of the block
      lock_guard<mutex> gmo(mo); //lock the output mutex
      cout<<ph<<" thinks "<<duration<<"ms\n"; //print that the philisopher thinks for x amount of time
    }// at the end of the block the mutex mo is unlocked
    this_thread::sleep_for(chrono::milliseconds(duration)); //have philispher thread sleep for x amount of time while 'sleeping'
    {//we lock the mutex mo for the block and it stays locked until the end of the block
      lock_guard<mutex> gmo(mo); //lock the output mutex
      cout<<"\t\t"<<ph<<" is hungry\n"; //print that the philospher is now hungry
    }// at the end of the block the mutex mo is unlocked
    lock_guard<mutex> gma(ma); //lock the first fork
    this_thread::sleep_for(chrono::milliseconds(400)); //sleep for 400 ms so that we give time for the second fork to unlock
    lock_guard<mutex> gmb(mb); //lock the second fork
    duration = myrand(800, 1200); //set a rand amount of time for the ph to eat for
    {//we lock the mutex mo for the block and it stays locked until the end of the block
      lock_guard<mutex> gmo(mo); //lock the output mutex
      cout<<"\t\t\t\t"<<ph<<" eats "<<duration<<"ms\n"; //print that the philospher eats for x amount of time
      {//we lock the mutex ms for the block and it stays locked until the end of the block
        lock_guard<mutex> gms(ms); //lock status array mutex
        status[ph-1] = 100; //set back to 100 since they are eating
      }// at the end of the block the mutex ms is unlocked  
    }// at the end of the block the mutex mo is unlocked
    this_thread::sleep_for(chrono::milliseconds(duration)); //have philispher thread sleep for x amount of time while 'eating'
  }
}

//The below function is a thread for the printing the status that prints the status of all the philosphers every 3 seconds, 
//takes the output mutex and the status array mutex
void show_status(mutex& mo, mutex& ms){
  for(;;){ //create an infinite loop so the thread runs forever
    {//we lock the mutex mo for the block and it stays locked until the end of the block
      lock_guard<mutex> gmo(mo); //lock the output mutex
      //below we print the status of each philospher
      {//we lock the mutex ms for the block and it stays locked until the end of the block
        lock_guard<mutex> gms(ms); //lock status array mutex
        cout << "*STATUS:  1: " << status[0] << ", 2: " << status[1] << ", 3: " << status[2] << ", 4: " << status[3] << ", 5: " << status[4] << "*\n";
      }// at the end of the block the mutex ms is unlocked
    }// at the end of the block the mutex mo is unlocked
    this_thread::sleep_for(chrono::milliseconds(3000)); //have this thread sleep for 3 seconds
  }
}

//the below function takes away 20% of the philosphers health every 1.4 second and takes the status array mutex
void lose_status(mutex& ms){
  this_thread::sleep_for(chrono::milliseconds(1000)); //give loss of health one second delay to give time for start up
  for(;;){ //create an infinite loop
    {//we lock the mutex ms for the block and it stays locked until the end of the block
      lock_guard<mutex> gms(ms);//lock status array mutex
      for(int i = 0; i<5; i++){ //loop through the status of each philospher
        status[i] -= 20; //take away 20% health
        if(status[i] <= 0){ //if the philospher has a status of 0% then the philospher is dead
          cout<<i+1<<" had died from starvation\n"; //say the philisopher has died
          exit(1); //exit the program bc someone died
        }
      }
    }// at the end of the block the mutex ms is unlocked
    this_thread::sleep_for(chrono::milliseconds(1400)); //sleep for 1.4 seconds
  }
}

// Main
int main() {
    cout<<"DINING PHILOSOPHERS"; //print out a title
    mutex m1, m2, m3, m4, m5;  // 5 forks are 5 mutexes
    mutex mo;  //mutex for proper output (so all the philosphers can wrote to std output with out getting jumbled)
    mutex ms; //mutex for the status array for the philosphers
    //5 philosophers are 5 threads, always put forks in order of least to greatest
    thread t1([&] {philosopher(1, m1, m2, mo, ms);}); //thread for ph 1, he gets fork1 and fork2
    thread t2([&] {philosopher(2, m2, m3, mo, ms);}); //thread for ph 2, he gets fork2 and fork3
    thread t3([&] {philosopher(3, m3, m4, mo, ms);}); //thread for ph 3, he gets fork3 and fork4
    thread t4([&] {philosopher(4, m4, m5, mo, ms);}); //thread for ph 4, he gets fork4 and fork5
    thread t5([&] {philosopher(5, m1, m5, mo, ms);}); //thread for ph 5, he gets fork1 and fork5
    thread stats([&] {show_status(mo, ms);}); //start thread for printing status
    thread decay([&] {lose_status(ms);}); //start thread for decaying philospher health
    // prevent threads from termination by calling .join() from all 7 threads created
    t1.join(); //ph 1
    t2.join(); //ph 2
    t3.join(); //ph 3
    t4.join(); //ph 4
    t5.join(); //ph 5
    stats.join(); //status printing thread
    decay.join(); //decay status thread
}