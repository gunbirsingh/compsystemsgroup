#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int primenum(int input){
	int a;
	int b;
	long int numbers[input];
    long int primearray[input];

    /*fill the array with numbers*/
    for (a=0; a < input; a++){
        numbers[a]=a+2;
    }

    /*sieve algorithm to determine which numbers in the array are prime*/
    for (a=0; a < input; a++){
        if (numbers[a] != -1){
            for (b = 2 * numbers[a] - 2; b<input; b += numbers[a])
                numbers[b]=-1;
        }
    }

    /*move out prime numbers found to the primearray*/
    b = 0;
    for (a=0; a < input && b < primearray; a++)
        if (numbers[a] != -1)
            primearray[b++] = numbers[a];

    /*print the prime numbers that were found*/
    printf("\nAll prime numbers up to the inputted integer %d", input);
    printf(" are: \n");
    for (a=0; a < primearray; a++)
        printf("%dn",primearray[a]);
    system("PAUSE");
 
return 0;

}

int main () {

pthread_attr_t attr;
pthread_mutex_t mutex;

/* Initiate the conditions of our thread */
pthread_attr_init(&attr);
pthread_mutex_init(&mutex, NULL);

pthread_t compute_prime_num_thread;

printf("Enter a integer:\n");

int input;

scanf("%d",&input);

int *arg=input;

/* Create and start our new thread. */
pthread_mutex_lock(&mutex);
pthread_create(&compute_prime_num_thread, NULL, primenum, arg);
pthread_join(compute_prime_num_thread,NULL);
pthread_mutex_trylock(&mutex);

return 0;
} 

