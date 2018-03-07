#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
    FILE *fp, *fp1;
    int i = 0, j;
    
    fp1 = fopen("output.txt", "a+");// open text1 in append mode
    
    int num[15];
    fp=fopen("data.txt","r");
    
    if (fp==NULL)
    {
        printf("Error opening file \n");
        return 0;
    }
    else
    {
        for (i=0; i<15; i++)
        {
            fscanf(fp, "%d \n" , &num[i]);
        }
    }
    
    fclose (fp);
    
    int fd [2];
    pipe(fd);
    pid_t pid;
    pid_t pidb;

    pid = fork ();
    pidb = fork ();
    
    if(pid>0)
    {
        if(pid==0)
        {
            fprintf(fp1, "\nHi I'm a child process %d and my parent is %d\n", getpid(), getppid()); //write data into text1 file
            int max = num[0];
            for(j=0; j<15; j++)
            {
                if(num[j] > max)
                {
                max = num[j];
                }
            }
        }
        else 
        {
            fprintf(fp1, "\nHi Im parent process %d", getpid());
        }
        if(pidb==0){
            int min = num[0];
            for(j=0; j<15; j++)
            {
                int total=0;
                total = total + num[j];
                if(num[j] < min)
                {
                    min = num[j];
                }
        }
    }

    else 
    {
        printf("Error!");
    }
    
    int max = num[0];
    int min = num[0];
    for(j=0; j<15; j++)
            {   
                int total=0;
                total = total + num[j];
                if(num[j] < min)
                {
                    min = num[j];
                }
                if(num[j] > max)
                {
                max = num[j];
                }
            }
    fprintf(fp1,"Max=%d\n", max);
    fprintf(fp1,"Min=%d\n", min);
    fprintf(fp1,"Sum=%d\n", total);
}


