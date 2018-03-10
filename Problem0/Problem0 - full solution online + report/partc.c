	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <limits.h>
	#include <time.h>

		int min =INT_MAX;
		int max =INT_MIN;

		FILE *f;
		FILE *writeFile; 

	void forkChildrendfs (int nChildren) {
		int i;
		pid_t pid;

		int fd[2];
		pipe(fd);

		f = fopen("file.txt", "r");
		writeFile = fopen("Output Part C.txt", "w+");

		int num,check,sum=0;
		printf("\n [DAD] = %d\n", getpid());

		for (i = 1; i <= nChildren; i++) {

			if(fscanf(f,"%d",&num) == 1)
				//check = num;
			pid = fork();
			if (pid == -1) {
				/* error handling */
				return;
			}
			if (pid == 0) {
				
				//sum = sum+num;
			 	check = num;
			 	write(fd[1], &check, sizeof(check));
				fprintf(writeFile, "I am a process: %d and my parent is: %d\n", getpid(), getppid());
				exit(0);
			}
			if(pid > 0){
				wait(NULL);	
				int id=0;
				read(fd[0], &id, sizeof(id));
				sum = sum + id;
				if(id>max)
					max = id;
				if(id<min)
					min = id;

			}
		}

	fprintf(writeFile, "\nmin: %d ",min );
	fprintf(writeFile, "\nmax: %d ",max );	
	fprintf(writeFile, "\nsum: %d ",sum );
	}

	int main () {
		double msec = 0, trigger = 1; /* 10ms */
	clock_t before = clock();
	int x=0;
	do {
	  FILE *fi;
		fi = fopen("file.txt", "r");
		int nval = 0;
		int number;
		while(fscanf(fi,"%d",&number) != EOF){
			nval++;
		}
		printf("\nnmber of values in file: %d", nval);
		forkChildrendfs (nval);
		
	  clock_t difference = clock() - before;
	  msec = difference * 1000 / CLOCKS_PER_SEC;
	 x=1;
	  
	} 
	while ( x=0 );
	fclose(f);

	printf("\nTime taken %.4f seconds\n", msec/1000);
	fclose(writeFile);
		
return 0;	
}
