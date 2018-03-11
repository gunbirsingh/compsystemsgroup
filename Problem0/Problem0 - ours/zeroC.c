#include <stdio.h>
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

    pid_t waitpid(pid_t);
    pid_t pid;

    pipe(fd);

    pid = fork();

    if (pid < 0)
    {
        perror("Error");
        exit(0);
    }

    else if (pid == 0) // child
    {

        fprintf(writeF, "Hi I'm  %d and my parent is %d\n", getpid(), getppid());
        close(fd[0]);
        binarySplit((lo + hi) / 2 + 1, hi, struc);


        write(fd[1], struc, sizeof(dataStruc));
        exit(0);
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
    dataStruc struc = {.min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0 };
    binarySplit(0, count - 1, &struc);

    fprintf(writeF, "minimum: %d\n", struc.min);
    fprintf(writeF, "maximum: %d\n", struc.max);
    fprintf(writeF, "sum: %d\n", struc.sum);

    fclose(writeF);

    return (0);
}