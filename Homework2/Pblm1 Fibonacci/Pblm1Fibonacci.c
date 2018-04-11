// Problem #1 Fibonacci
// Homework 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int fibonacci[100];  // global array for Fibonacci numbers allowing up to 100 indices


void *fibonacciMethod(void *userInput);  // Fibonacci values pulled from memory
void *fibonacciMethod(void *userInput){  // corresponding method
	int previous = 0; int current = 1; int fibonacciNo = 0;   // initialized variables 
	for (int i = 0; i < userInput; i++){
		fibonacci[i] = fibonacciNo;   // set value from integer and assign to appropriate index in Fibonacci array per iteration
		fibonacciNo = previous + current;  // performing Fibonacci operation
		previous = current;
		current = fibonacciNo;
	}
	pthread_exit(0);    // exit thread upon completion
}

int main(int argc, char *argv[]){   // 'argc' = # of arguments -&&- 'argv[]' contains arguments in string form
	pthread_attr_t fib_attributes;    // initialize Fibonacci thread attributes object
	int input = atoi(argv[1]);    // convert argument vector strings to integer number values

	if (argc != 2){   // if number of arguments is not 2, terminate program
		exit(0);
	}

	if (input < 0){    // if input from user is < 0, terminate program 
		printf("Enter positive integer value only.");
		exit(0);
	}
	else {
		pthread_attr_init(&fib_attributes);
		pthread_t fib_thread;    // initialize Fibonacci thread
		pthread_create(&fib_thread, &fib_attributes, fibonacciMethod, input);   // create a thread referencing 'void *fibonacciMethod(void *userInput)'
		pthread_join(fib_thread, NULL);    // wait for completion of thread execution
		printf("Fibonacci sequence:\n");   // output fibonacci sequence
		for (int i = 0; i < input; i++){
			printf("%d ", fibonacci[i]);   // print all array values 
		}
	}


	system("pause");    // suspend system screen upon program completion
	return 0;    // terminate program
}