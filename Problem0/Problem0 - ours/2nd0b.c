# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <limits.h>
# include <time.h>

int min = INT_MAX;
int max = INT_MIN;

FILE* f;
FILE* output;

int splitOff(int numKids, int index){
    
    pid_t pid;
    int i;
    f = fopen("data.txt", "r");
    int status;

    int curr, sum = 0;
    output = fopen("output.txt", "w+");

    printf("Initial process = %d\n", getpid());

    int total = 0;
    for (i = 0; i < numKids; i++)
    {
        total++;
        if (fscanf(f, "%d", &curr) != EOF)
        {
            sum = sum + curr;
        }
        if (pid ==0)
        {
            pid = fork();
        }
        if (pid < 0)
        {

            printf("Error!\n");
            return  -1;

        }

        else if (pid > 0)
        {
            i = numKids;
            wait(NULL);
           return 1;
          
        }

        else if (pid == 0)
        {

            fprintf(output, "%d Hi I am process: %d and my parent is: %d\n", i, getpid(), getppid());

            if (curr < min)
            {
                min = curr;
            }
            if (curr > max)
            {
                max = curr;
            }
            total = total + 1;
        }


    }

    fprintf(output, "\nmax: %d\n", max);
    fprintf(output, "\nmin: %d\n", min);
    fprintf(output, "\nsum: %d\n", sum);
    exit(0);
    return 0;
}

int main()
{

    clock_t first = clock();
   
        FILE* fi;
        fi = fopen("data.txt", "r");
        int listSize = 0;
        int curr;
        while (fscanf(fi, "%d", &curr) != EOF)
        {
            listSize++;
        }
    int isEnd = 0;
    isEnd = splitOff(listSize, 0);
 
   


    clock_t timeElapsed = clock() - first;

    printf("Time: %.4f secs \n",msec);


    return 0;

}