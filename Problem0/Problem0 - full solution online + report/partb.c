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

void forkChildrenbfs (int nChildren) {
	int i;
	pid_t pid;

	f = fopen("file.txt", "r");
	int status;

	int num,check,sum=0;
	writeFile = fopen("Output Part B.txt", "w+");

	printf("\n [DAD] = %d\n", getpid());


	for (i = 1; i <= nChildren; i++) {
	   
	   	if(fscanf(f,"%d",&num) !=EOF){
			check = num;
			sum = sum+num;
		 }
		pid = fork();
		if (pid<0) {

			printf("Error Creating Child!\n");
			return ;
		}

		else if (pid>0){
			 wait(NULL);
			 return ;
		  }

		else if (pid == 0) {
			fprintf(writeFile,"Hi I am process: %d and my parent is: %d\n",getpid(), getppid());
			
			if(check<min){
				min = check;
			}
			if(check>max){
				max = check;
			}
		}
		

	}

fprintf(writeFile,"\nmax: %d\n", max);
fprintf(writeFile,"\nmin: %d\n",min );
fprintf(writeFile,"\nsum: %d\n",sum );

}

int main () {
	
	double msec = 0, trigger = 10; /* 10ms */
	clock_t before = clock();
	int x=0;

do{
	FILE *fi;
	fi = fopen("file.txt", "r");
	int nval = 0;
	int number;
	while(fscanf(fi,"%d",&number) != EOF){
		nval++;
	}
	printf("\nnmber of values in file: %d", nval);
	forkChildrenbfs (nval);

	x =1;
	}while ( x=0 );


 	clock_t difference = clock() - before;
  	msec = (double)(difference * 1000) / (double)CLOCKS_PER_SEC;
	//x=1;
	printf("Time taken %.4f seconds \n",
  	msec);


return 0;
	
}
