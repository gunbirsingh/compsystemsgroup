// Pblem 6 - Monitor Implementation 
// HW 2

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>


// Variables
int n = 10, sumProcessIDs = 0, waitingProcesses = 0; // sum of relevant process IDs (accessing file) initially zero, must be less than n && zero initial waiting processes
semaphore mutex = 1;   // initialize semaphore mutex
semaphore getControl = 0;    // initialize semaphore getControl

// Main
int main(){
	monitor();   // call monitor() function
	system("pause");   // freeze output screen upon program completion
	return 0;
}

// Processes waiting for control
void get_control(int pid){
	sem_wait(mutex);   // semaphore lock operation on semaphore mutex (decrement mutex count)
	int total = sumProcessIDs + pid;  // sum of process IDs
	while (total > n){
		waitingProcesses++;
		sem_signal(mutex);    // increment mutex count
		sem_wait(getControl);   // lock operation (decrement count) on semaphore getControl
		sem_wait(mutex);   // again decrement mutex count
	}
	sumProcessIDs = sumProcessIDs + pid;
	sem_signal(mutex);  // increment mutex count 
}

// Transferring control
void release(int pid){
	sem_wait(mutex);   // lock semaphore mutex
	sumProcessIDs = sumProcessIDs - pid;
	for (int i = 0; i < waitingProcesses; ++i) {
		sem_signal(getControl);    // increment getControl semaphore
	}
	waitingProcesses = 0;   // no waiting processes
	sem_signal(mutex);    // increment mutex semaphore
}

// Monitor 
void monitor(){
	for (int accessID = 0; accessID<10; accessID++){
		get_control(accessID);     // if sum of all process IDs < n, coordinate access to file
	}
	for (int releaseID = 0; releaseID<10; releaseID++){
		release(releaseID);   // if sum of all process IDs < n, coordinate release of the file
	}
}
