#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
/* Structures to define the proc tree */
struct procInfo
{
    char procName; /* A, B, C, D */
    int numChild;
    char children[5];
    int return_Value;
};

struct procInfo proc[] = {
    { 'A', 2, {'B', 'C'}, 5},
    { 'B', 1, {'D'}, 9 },
    { 'C', 0, { }, 8 },
    { 'D', 0, { }, 12 },
};

char myId = 'A'; /* my proc id */
int Value;

void procfunction()
{
    int i, j;
    int n = sizeof(proc) / sizeof(struct procInfo);
    printf("Started proc %c, pid=%d\n", myId, getpid());
    for(i = 0; i < n; i++)
    {
        if(myId == proc[i].procName) 
            break;
    }
    if(i < n) 
    {
        Value = proc[i].return_Value;
        pid_t pids[5];
        for(j = 0; j < proc[i].numChild; j++)
        {
            pids[j] = fork();
            if(pids[j] < 0)
            {
                printf("Proc %c, pid=%d: fork failed\n", myId, getpid());
            }
            if(pids[j] == 0)
            {
                myId = proc[i].children[j];
                procfunction();
                return;
            }
            else
            {
                printf("Proc %c, pid=%d: Forked %c, pid=%d\n", myId, getpid(), proc[i].children[j], pids[j]);
            }
        }
        printf("Proc %c, pid=%d: Waiting for children to end\n", myId, getpid());
        for(j = 0; j < proc[i].numChild; j++)
        {
            int status;
            if(pids[j] > 0) 
            {
                waitpid(pids[j], &status, 0);
                printf("Proc %c, pid=%d: Child exited with status %d\n", myId, getpid(), WEXITSTATUS(status));
            }
        }
    }
    sleep(10);
    printf("Proc %c, pid=%d: ending proc\n", myId, getpid());
}
int main()
{
    procfunction();
    return Value;
}
