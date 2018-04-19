// Project 2 - Part 1a

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>


void sighup(int);
void sigint(int);
void sigquit(int);

FILE *writeF;

typedef struct dataStruc {    // declare a struct w/ following variables
	int min;
	int max;
	int sum;
	int count;
}
dataStruc;

void binarySplit(int lo, int hi, dataStruc *struc)   // binarySplit function for 
{

	if (lo == hi) // all valid inputs have been read -- the end
	{
		int curr, num;
		FILE *fp = fopen("data.txt", "r");

		int cs = 0;
		while (fscanf(fp, "%d\n", &num) != EOF)
		{
			if (lo == cs++)
			{
				curr = num;
			}
		}

		if (curr < struc->min)
		{
			struc->min = curr;
		}
		if (curr > struc->max)
		{
			struc->max = curr;
		}

		struc->sum += curr;
		struc->count++;
		return;
	}

	int fd[2];

// edit below ---------------------------------------------------------------------
	pid_t waitpid(pid_t);
	pid_t pid;

	// edit below ------------------------------------
	signal(SIGHUP, sighup); /* set function calls */
	signal(SIGINT, sigint);
	signal(SIGQUIT, sigquit);
	// -----------------------------------------------

	pid = fork();

	if (pid < 0)
	{
		perror("Error");
		exit(1);   // unsuccessful termination
	}

	else if (pid == 0) // child
	{

		fprintf(writeF, "Hi I'm child (%d) and my parent is %d\n", getpid(), getppid());
		close(fd[0]);
		binarySplit((lo + hi) / 2 + 1, hi, struc);


		write(fd[1], struc, sizeof(dataStruc));
		exit(0);   // successful termination
	}
	else
	{
		close(fd[1]);
		binarySplit(lo, (lo + hi) / 2, struc);
		dataStruc new_variables;

		read(fd[0], &new_variables, sizeof(dataStruc));

		if (new_variables.min < struc->min)
		{
			struc->min = new_variables.min;
		}
		if (new_variables.max > struc->max)
		{
			struc->max = new_variables.max;
		}

		struc->sum += new_variables.sum;
		struc->count += new_variables.count;
		waitpid(pid);
	}
// ------------------------------------------------------------------------------------

}

int main()
{
	writeF = fopen("output.txt", "w+");  // create output file for reading & writing
	FILE *fp = fopen("data.txt", "r");   // open existing data.txt file for reading
	int count = 0;
	int nums;

	while (fscanf(fp, "%d\n", &nums) != EOF)   // while value from input file is not -1
	{
		count++;   // find total number of valid inputs
	}

	fclose(fp);    // close file
	dataStruc struc = { .min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0 };   // initialize structure
	binarySplit(0, count - 1, &struc);   // call binarySplit function w/ parameters

	fprintf(writeF, "minimum: %d\n", struc.min);   // print struc minimum
	fprintf(writeF, "maximum: %d\n", struc.max);   // print struc maximum
	fprintf(writeF, "sum: %d\n", struc.sum);       // print struc sum

	fclose(writeF);  // close file

	system("pause"); // pause system output
	return(0);   // exit program successfully
}

// edit below ------------------------------------
void sighup(int signo) {
	signal(SIGHUP, sighup); /* reset signal */
	printf("CHILD: I have received a SIGHUP\n");
}

void sigint(int signo) {
	signal(SIGINT, sigint); /* reset signal */
	printf("CHILD: I have received a SIGINT\n");
}

void sigquit(int signo) {
	printf("My DADDY has Killed me!!!\n");
	exit(0);
}
// -----------------------------------------------