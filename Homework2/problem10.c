//Jeremy Kritz
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
int res = 0, num = 2, pd[2], arg[32];
pthread_t thread[32];
sem_t sem;

void h(int s)
{
    read(pd[0], &num, sizeof(num)); // puts pipe into num
    sem_post(&sem); // increment sem
}

void* f(void* var)
{
    int tmp = *(int*)var;
    res += tmp; //increments res by var, and then kills thread
    pthread_exit((void*)0);
}

int main()
{
    int i, k, p, pstatus;
    void* tstatus;

    sem_init(&sem, 0, 0); // sets up semaphore
    signal(SIGUSR1, h); // h puts num at whatever is in the pipe, prob p low
    pipe(pd); // sets up pipe

    for (i = 0; i < num; i++) { // num starts as 2
        p = fork(); // parent does nothing and jumps right back in

        if (p == 0) {
            sem_wait(&sem); // decr semaphore
            num++; // num++, so, uh, this seems bad, creates race bc for depends on it
            for (k = 0; k < num; k++)            {
                arg[k] = k;
                pthread_create(&thread[k], NULL, f, (void*)&arg[k]); // this creates a thread in f (increments res by k i think)
            }

            for (k = 0; k < num; k++) pthread_join(thread[k], &tstatus); // starts collecting threads
            if (num < 4) write(pd[1], &num, sizeof(num)); // for a low num, put num into the pipe

            exit(res); // exit, value of res is returned to the parent
        }
    }  // OK, so, child processes never reenter the for, only parents ever make it here

    write(pd[1], &num, sizeof(num)); // puts num into the pipe
    signal(SIGUSR1, SIG_IGN); //sigign = ignore
    kill(0, SIGUSR1); // this seems to be stopped by sig_ign

    for (i = 0; i < num; i++){
        wait(&pstatus); // 
        if (WIFEXITED(pstatus)) res += WEXITSTATUS(pstatus);
    }

    printf("Final result1: %d \n", res); // online ide says it reaches 9
    read(pd[0], &num, sizeof(num)); // trying to read num, where was num written
    printf("Final result2: %d \n", num); // it wont reach this in testing, it seems like 

    return 0;
}