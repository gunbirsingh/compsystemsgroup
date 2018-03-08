#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


struct evaluate{
	int par;   // integer result for parent expression
	int chi;   // integer result for child expression
};
pid_t pid1, pid2;
void* shared_memory;

int main()
{
	int int1;
	char op1;
	int int2;
	char op2;
	int int3;
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
