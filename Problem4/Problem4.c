#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>

struct evaluate{
	int int1;
	char op1;
	int int2;
	char op2;
	int int3;
};

int main()
{
	FILE *inputFile;
	inputFile = fopen("inputFile.txt", "r");
	if (inputFile == NULL){
		printf("Error! File empty.\n");
		return 0;
	}
	else {
		fscanf(inputFile, "%d %c %d %c %d ", int1, op1, int2, op2, int3);
	}
}
