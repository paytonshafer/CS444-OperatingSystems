# CS444-OperatingSystems
This is a collection of my homework solutions for CS444: Operating Systems at Clarkson University in spring 2023. Below I go into depth with what each
homework is and how to run the program. A code review for each homework can be found in this youtube playlist: https://youtube.com/playlist?list=PLMAexaDQkkbDOClG_DTMeQBDowGB1q8M1

## HW1: Matrix multiplication using pthreads
This homework required me to write a program that does matrix multlipication with pthreads. The user first enters the size of both matricies involved,
if the sizes are invalid for matix multiplication the user is asked to try again. Then both matricies are filled with random integers and the first 2 
matricies as well as their product is printed out. 
### HW1: Usage
To run this program first download the source code then compile with the first commands and run with
the second command.
```sh
g++ -o matmult MatrixMult.cpp
./matmult
```

## HW2: Producer-Consumer Problem
This is my solution to the Producer-Consumer Problem using pthreads and mutexes to solve the critical region access problem. When this is ran the producer and consumer will take turns producing something for the buffer and consuming something from the buffer, respectivly. To stop just press CONTROL-C.
### HW2: Usage
To run this program first download the source code then compile with the first commands and run with
the second command.
```sh
g++ -o prodcon ProducerConsumer.cpp
./prodcon
```

## HW3: Key-Logger
This is my solution for a key logger that runs on MacOS, I had a bit of trouble with this assignment because there is no easy way to log keys on MacOs. So this program has a function that is commented out on how you log keys on a Windows system. Below that is the function for how I chose to log keys on MacOS then the program is ran a daemon.

### HW3: Usage
To run this program first download the source code then compile with the first commands and run with
the second command.
```sh
g++ -o keylog KeyLogger.cpp
./keylog
```
Since this is running as a daemon to stop this from running first find the pid of the process then kill it with the kill command:
```sh
ps -A | grep keylog
```
This will return the pid for your daemon, then run this command with the pid found from the prevous command:
```sh
kill pid
```

## HW4: Dining Philospher's Problem
This is a solution to the dining philosphers problem using threads and mutexes to prevent race conditions, I used the idea of Reasource Heirachy to solve this problem. There are 5 philospher and 5 forks (these being the critical regions) the philosphers all start at 100% health and every 1.4 secconds that they do not eat they lose 20% health and when they eat their health is set back to 100%. There is also a status array that shows the status of each philospher's health which is printed out every 3 seconds.

### HW4: Usage
To run this program first download the source code then compile with the first commands and run with
the second command. To stop just press CONTROL-C.
```sh
 g++ DiningPhilosophers.cpp -std=c++11 -o dp
./dp
```
