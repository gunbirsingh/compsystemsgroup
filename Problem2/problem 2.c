#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct procInfo
{
    char procName; 
    int numChild;
    char *childrenName;
};

struct procInfo proc[] = {
    
};

char myId;
int Value;

int openfile(){
    int i;
    int x[15];
    FILE *data;
    data=fopen("data.txt","r");
    if (data==NULL){
        printf("Error opening file \n");
        return 0;
    }
    else{
        for (i=0; i<15; i++){
            fscanf(data, "%s %d %s %s %s\n" , &proc[i].procName, &proc[i].numChild, &proc[i].childrenName);
        }
    }
}

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
        myId = proc[i].procName;
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
                myId = proc[i].childrenName[j];
                procfunction();
                return;
            }
            else
            {
                printf("Proc %c, pid=%d: Forked %c, pid=%d\n", myId, getpid(), proc[i].childrenName[j], pids[j]);
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
    openfile();
    procfunction();
}
