#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <signal.h>
#include <pthread.h>


FILE* writeF;
int sigpipe = 1;
typedef struct dataStruc { int lo, hi, min, max, sum, count; }
dataStruc;

pthread_t tid[2];
int counter;
pthread_mutex_t lock;




void binarySplit( dataStruc* struc)
{
	int low = struc->lo;
	int high = struc->hi;
	int minn = struc->min;
	int maxx = struc->max;
	int summ = struc->sum;
	int cnt = struc->count;

    if (low == high) // the end
    {
        int curr, num;
        FILE* fp = fopen("data.txt", "r");

        int cs = 0;
        while (fscanf(fp, "%d\n", &num) != EOF)
        {
            if (low == cs++)
            {
                curr = num;
            }
        }

        if (curr < minn)
        {
            struc->min = curr;
        }
        if (curr > maxx)
        {
            struc->max = curr;
        }

        struc->sum += curr;
        struc->count++;
        return;
    }

    int fd[2];

    pid_t waitpid(pid_t);
    pid_t pid;
	pipe(fd);
  /*  pipe(fd);

    pid = fork();

    if (pid < 0)
    {
        perror("Error");
        exit(0);
    }*/
	for (int j = 0; j < 2; j++) {



		if (j = 0) // child
		{

			fprintf(writeF, "Hi I'm  %d and my parent is %d\n", getpid(), getppid());
			close(fd[0]);
			dataStruc strucA = { .hi = high,.lo = (low + high) / 2,.min = minn,.max = maxx,.sum = summ,.count = cnt };
			pthread_create(&(tid[0]), NULL, *binarySplit, &strucA);
			//binarySplit((lo + hi) / 2 + 1, hi, struc);
		//kill(getppid(), 42);

		//printf(sigpipe);
			write(fd[1], struc, sizeof(dataStruc));
		//	exit(0);
		}
		else
			//pthread_create(&(tid[i]), NULL, &trythis, NULL);
		{
			close(fd[1]);
			dataStruc strucB = { .lo = low,.hi = (low + high) / 2,.min = minn,.max = maxx,.sum = summ,.count = cnt };
			pthread_create(&(tid[1]), NULL, *binarySplit, &strucB);
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
	}
}


int main()
{
    writeF = fopen("output.txt", "w+");
    FILE* fp = fopen("data.txt", "r");
    int count = 0;
    int nums;

    while (fscanf(fp, "%d\n", &nums) != EOF)
    {
        count++;
    }

    fclose(fp);
    dataStruc struc = {.lo = 0, .hi = count-1, .min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0 };
    binarySplit(&struc);

    fprintf(writeF, "minimum: %d\n", struc.min);
    fprintf(writeF, "maximum: %d\n", struc.max);
    fprintf(writeF, "sum: %d\n", struc.sum);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_mutex_destroy(&lock);

    fclose(writeF);

    return (0);
}
