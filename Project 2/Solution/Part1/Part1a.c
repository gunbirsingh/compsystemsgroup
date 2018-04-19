// Project 2 - Part 1a

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>

FILE* writeF;

typedef struct dataStruc { int min, max, sum, count; }
dataStruc;

void binarySplit(int lo, int hi, dataStruc* struc)
{

	if (lo == hi) // the end
	{
		int curr, num;
		FILE* fp = fopen("data.txt", "r");

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

