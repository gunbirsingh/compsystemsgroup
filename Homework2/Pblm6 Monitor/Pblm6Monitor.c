#include <stdio.h>

int n = 10;// file id number

int sumOfID = 0;

int numOfProcessWaiting = 0;

semaphore mutex = 1;

semaphore okToAccess = 0;

void get_access(int pid)

{

	sem_wait(mutex);

	while (sumOfID + pid > n)

	{

		numOfProcessWaiting++;

		sem_signal(mutex);

		sem_wait(okToAccess);

		sem_wait(mutex);

	}

	sumOfID += pid;

	sem_signal(mutex);

}

void release_access(int pid)

{

	int i;

	sem_wait(mutex);

	sumOfID -= pid;

	for (i = 0; i < numOfProcessWaiting; ++i) {

		sem_signal(okToAccess);

	}

	numOfProcessWaiting = 0;

	sem_signal(mutex);

}

void Monitor()

{

	int id;

	for (id = 0; id<20; id++)

	{

		get_access(id);

	}



	for (id = 0; id<20; id++)

	{

		release_access(id);

	}

}

int main()

{

	Monitor();

}